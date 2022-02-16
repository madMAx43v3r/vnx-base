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
#define NOMINMAX
#include <windows.h>
#undef ERROR
#else
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


} // vnx
