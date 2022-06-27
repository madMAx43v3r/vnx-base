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
#include <vnx/File.h>

#include <fcntl.h>


namespace vnx {

File::File()
	:	in(&stream_in),
		out(&stream_out)
{
}

File::File(const std::string& path_) : File() {
	path = path_;
}

File::~File() {
	close();
}

void File::open(const std::string& path_, const std::string& mode) {
	path = path_;
	open(mode);
}

void File::open(const std::string& mode) {
	close();
	p_file = ::fopen(path.c_str(), mode.c_str());
	if(!p_file) {
		throw std::runtime_error("fopen('" + path + "') failed with: " + std::string(::strerror(errno)));
	}
	stream_in.reset(p_file);
	stream_out.reset(p_file);
}

bool File::create() {
	if(exists()) {
		return false;
	}
	open("wb");
	close();
	return true;
}

uint16_t File::read_header() {
	while(true) {
		uint16_t code;
		vnx::read(in, code);
		if(code == CODE_NONE || code == CODE_MAGIC) {
			continue;
		} else {
			return code;
		}
	}
}

void File::write_header() {
	write(out, uint16_t(CODE_NONE));
	write(out, uint16_t(CODE_MAGIC));
}

void File::seek_begin() {
	out.flush();
	if(vnx::fseek(p_file, 0, SEEK_SET) < 0) {
		throw std::runtime_error("fseek('" + path + "') failed with: " + std::string(::strerror(errno)));
	}
	in.reset();
}

void File::seek_end() {
	out.flush();
	if(vnx::fseek(p_file, 0, SEEK_END) < 0) {
		throw std::runtime_error("fseek('" + path + "') failed with: " + std::string(::strerror(errno)));
	}
	in.reset();
}

void File::seek_to(int64_t pos) {
	out.flush();
	if(vnx::fseek(p_file, pos, SEEK_SET) < 0) {
		throw std::runtime_error("fseek('" + path + "') failed with: " + std::string(::strerror(errno)));
	}
	in.reset();
}

void File::fadvise(int advice, int64_t pos, int64_t length) {
#if _POSIX_C_SOURCE >= 200112L
	const int ret = ::posix_fadvise(::fileno(p_file), pos, std::max(length, int64_t(0)), advice);
	if(ret) {
		throw std::runtime_error("posix_fadvise('" + path + "') failed with: " + std::string(::strerror(ret)));
	}
#endif
}

std::string File::get_name(bool with_ext) const {
	std::string result = path;
	while(!result.empty() && (result.back() == '/' || result.back() == '\\')) {
		result.pop_back();
	}
	{
		const size_t pos = result.find_last_of("/\\");
		if(pos != std::string::npos) {
			result = result.substr(pos + 1);
		}
	}
	if(!with_ext) {
		const size_t pos = result.find_last_of('.');
		if(pos != std::string::npos) {
			result = result.substr(0, pos);
		}
	}
	return result;
}

std::string File::get_extension() const {
	const auto name = get_name();
	const auto pos = name.find_last_of('.');
	if(pos != std::string::npos) {
		return name.substr(pos);
	}
	return std::string();
}

int64_t File::last_write_time() const {
	vnx::stat info = {};
	if(get_stat(path.c_str(), info) == 0) {
		return info.st_mtime;
	}
	return 0;
}

int64_t File::file_size() const {
	vnx::stat info = {};
	if(get_stat(path.c_str(), info) == 0) {
		return info.st_size;
	}
	return 0;
}

bool File::exists() const {
	vnx::stat info = {};
	if(get_stat(path.c_str(), info) == 0) {
		return true;
	}
	return false;
}

bool File::is_directory() const {
	vnx::stat info = {};
	if(get_stat(path.c_str(), info) == 0) {
		return S_ISDIR(info.st_mode);
	}
	return false;
}

bool File::is_regular() const {
	vnx::stat info = {};
	if(get_stat(path.c_str(), info) == 0) {
		return S_ISREG(info.st_mode);
	}
	return false;
}

bool File::is_symlink() const {
#ifdef _WIN32
	return false;
#else
	vnx::stat info = {};
	if(get_stat(path.c_str(), info) == 0) {
		return S_ISLNK(info.st_mode);
	}
	return false;
#endif
}

void File::flush() {
	out.flush();
	if(::fflush(p_file)) {
		throw std::runtime_error("fflush('" + path + "') failed with: " + std::string(::strerror(errno)));
	}
}

void File::close() {
	if(p_file) {
		out.flush();
		if(::fclose(p_file)) {
			throw std::runtime_error("fclose('" + path + "') failed with: " + std::string(::strerror(errno)));
		}
		p_file = nullptr;
		stream_in.reset(nullptr);
		stream_out.reset(nullptr);
	}
}

void File::remove() {
	close();
	if(::remove(path.c_str())) {
		throw std::runtime_error("remove('" + path + "') failed with: " + std::string(::strerror(errno)));
	}
}


} // vnx
