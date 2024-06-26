 /*************************************************************************
 * 
 *  [2017] - [2018] Automy Inc. 
 *  All Rights Reserved.
 * 
 * NOTICE:  All information contained herein is, and remains
 * the property of Automy Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to Automy Incorporated
 * and its suppliers and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Automy Incorporated.
 */

#include <vnx/vnx.h>
#include <vnx/Util.h>
#include <vnx/ProcessClient.hxx>

#include <mutex>
#include <random>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#undef ERROR
#else
#include <unistd.h>
#include <limits.h>
#include <termios.h>
#endif

#ifdef _MSC_VER
void usleep(int64_t usec)
{
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}
#endif


namespace vnx {

std::string string_subs(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();	// handles case where 'to' is a substring of 'from'
    }
    return str;
}

std::vector<std::string> string_split(const std::string& str, char sep, bool clean) {
	std::vector<std::string> res;
	std::string tmp;
	for(char c : str) {
		if(c == sep) {
			if(!tmp.empty() || !clean) res.push_back(tmp);
			tmp.clear();
		} else {
			tmp.push_back(c);
		}
	}
	if(!tmp.empty() || !clean) res.push_back(tmp);
	return res;
}

bool string_iequals(const std::string& L, const std::string& R) {
    if(R.size() != L.size()) {
        return false;
    }
    for(size_t i = 0; i < L.size(); ++i) {
        if(std::tolower(L[i]) != std::tolower(R[i])) {
            return false;
        }
    }
    return true;
}

uint64_t rand64() {
	static std::mutex mutex;
	static bool is_init = false;
	static std::mt19937_64 generator;
	std::lock_guard<std::mutex> lock(mutex);
	if(!is_init) {
		generator.seed(get_wall_time_nanos());
		is_init = true;
	}
	return generator();
}

std::string to_hex_string(const void* data, const size_t length, bool big_endian, bool lower_case) {
	static const char map_lower[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
	static const char map_upper[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	const char* map = lower_case ? map_lower : map_upper;
	std::string str;
	str.resize(length * 2);
	for(size_t i = 0; i < length; ++i) {
		if(big_endian) {
			str[(length - i - 1) * 2] = map[((const uint8_t*)data)[i] >> 4];
			str[(length - i - 1) * 2 + 1] = map[((const uint8_t*)data)[i] & 0x0F];
		} else {
			str[i * 2] = map[((const uint8_t*)data)[i] >> 4];
			str[i * 2 + 1] = map[((const uint8_t*)data)[i] & 0x0F];
		}
	}
	return str;
}

template<typename T>
T hex_char_to_int(const char c) {
	if(c >= '0' && c <= '9') {
		return c - '0';
	}
	if(c >= 'a' && c <= 'f') {
		return 10 + (c - 'a');
	}
	if(c >= 'A' && c <= 'F') {
		return 10 + (c - 'A');
	}
	throw std::logic_error("invalid hex char: " + std::string{c});
}

std::vector<uint8_t> from_hex_string(const std::string& str) {
	size_t off = 0;
	if(str.substr(0, 2) == "0x") {
		off = 2;
	}
	const auto length = str.size() - off;
	if(length % 2) {
		throw std::logic_error("from_hex_string(): length not multiple of two: " + std::to_string(length));
	}
	std::vector<uint8_t> out(length / 2);
	for(size_t i = 0; i < out.size(); ++i) {
		out[i] = hex_char_to_int<uint8_t>(str[off + i * 2]) * 16 + hex_char_to_int<uint8_t>(str[off + i * 2 + 1]);
	}
	return out;
}

std::string input_password(const std::string &prompt){
#ifdef _WIN32
	HANDLE console = GetStdHandle(STD_INPUT_HANDLE);
	DWORD saved_attributes = 0;
	GetConsoleMode(console, &saved_attributes);
	SetConsoleMode(console, saved_attributes & ~ENABLE_ECHO_INPUT);
#else
	termios saved_attributes;
	tcgetattr(0, &saved_attributes);

	termios tmp = saved_attributes;
	tmp.c_lflag &= ~ECHO;
	tmp.c_cc[VMIN] = 1;
	tmp.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &tmp);
#endif

	std::string result;
	vnx::ProcessClient proc("vnx.process");
	proc.pause_log();

	std::cout << prompt;
	std::getline(std::cin, result);
	std::cout << std::endl;

	proc.resume_log();

#ifdef _WIN32
	SetConsoleMode(console, saved_attributes);
#else
	tcsetattr(0, TCSANOW, &saved_attributes);
#endif
	return result;
}

std::string get_host_name() {
#ifdef _WIN32
	TCHAR infoBuf[1024] = {};
	DWORD bufCharCount = sizeof(infoBuf);
	if(!GetComputerName(infoBuf, &bufCharCount)) {
		throw std::runtime_error("GetComputerName() failed");
	}
	return std::string(infoBuf, bufCharCount);
#else
	char hostname[HOST_NAME_MAX + 1] = {};
	if(::gethostname(hostname, HOST_NAME_MAX)) {
		throw std::runtime_error("gethostname() failed");
	}
	return std::string(hostname);
#endif
}

#ifdef _WIN32
class SetTerminate
{
	public:
		SetTerminate()
		{
			std::set_terminate([]() {
				try {
					if(auto ex = std::current_exception()) {
						std::rethrow_exception(ex);
					}
				}
				catch (const std::exception& e) {
					std::cerr << "Caught exception: " << e.what() << std::endl;
				}
				catch (...) {
					std::cerr << "Unhandled exception of unknown type" << std::endl;
				}

				std::abort();
			});
		}
};

SetTerminate setTerminate;
#endif

} // vnx
