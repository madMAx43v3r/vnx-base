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
#include <vnx/Directory.h>

#include <sys/stat.h>


namespace vnx {

Directory::Directory() {
	// nothing
}

Directory::Directory(const std::string& path_) {
	set_path(path_);
}

Directory::~Directory() {
	close();
}

void Directory::set_path(const std::string& path_) {
	close();
	path = path_;
	while(path.size() > 1 && path.back() == '/') {
		path.pop_back();
	}
}

void Directory::open(const std::string& path_) {
	set_path(path_);
	open();
}

void Directory::open() const {
	close();
	p_dir = ::opendir(path.c_str());
	if(!p_dir) {
		throw std::runtime_error("opendir('" + path + "') failed with: " + std::string(::strerror(errno)));
	}
}

void Directory::create(const std::string &path_) {
	set_path(path_);
	create();
}

void Directory::create() {
	size_t position = 0;
	while(position < path.length()) {
		position = path.find('/', position);
		if(position == std::string::npos) {
			position = path.length();
		}
		const auto parent = path.substr(0, position);
		if(!parent.empty()) {
			if(::mkdir(parent.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) {
				if(errno != EEXIST) {
					throw std::runtime_error("mkdir() failed with: " + std::string(::strerror(errno)));
				}
			}
		}
		position++;
	}
}

std::vector<std::shared_ptr<File>> Directory::files() const {
	std::vector<std::shared_ptr<File>> result;
	open();
	while(true) {
		::dirent* entry = ::readdir(p_dir);
		if(!entry) {
			break;
		}
		if(entry->d_type == DT_REG) {
			result.push_back(std::make_shared<File>(get_path() + entry->d_name));
		}
	}
	return result;
}

std::vector<std::shared_ptr<Directory>> Directory::directories() const {
	std::vector<std::shared_ptr<Directory>> result;
	open();
	while(true) {
		::dirent* entry = ::readdir(p_dir);
		if(!entry) {
			break;
		}
		if(entry->d_type == DT_DIR) {
			const std::string name(entry->d_name);
			if(name != "." && name != "..") {
				result.push_back(std::make_shared<Directory>(get_path() + name));
			}
		}
	}
	return result;
}

std::vector<std::shared_ptr<File>> Directory::all_files() const {
	std::vector<std::shared_ptr<File>> result;
	open();
	while(true) {
		::dirent* entry = ::readdir(p_dir);
		if(!entry) {
			break;
		}
		result.push_back(std::make_shared<File>(get_path() + entry->d_name));
	}
	return result;
}

std::string Directory::get_path() const {
	std::string result = path;
	if(!result.empty()) {
		result += '/';
	}
	return result;
}

std::string Directory::get_name() const {
	std::string result = path;
	const size_t pos = result.find_last_of('/');
	if(pos != std::string::npos && pos != 0) {
		result = result.substr(pos + 1);
	}
	return result;
}

void Directory::close() const {
	if(p_dir) {
		if(::closedir(p_dir) != 0) {
			throw std::runtime_error("closedir('" + path + "') failed with: " + std::string(::strerror(errno)));
		}
		p_dir = nullptr;
	}
}

	
} // vnx
