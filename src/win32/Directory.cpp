/*
 * Directory.cpp
 *
 *  Created on: Jun 8, 2020
 *      Author: jaw
 */

#include <vnx/vnx.h>
#include <vnx/Directory.h>

#include <filesystem>


namespace vnx{

Directory::Directory() {
	// nothing
}

Directory::Directory(const std::string &path_) {
	set_path(path_);
}

Directory::~Directory() {
	// nothing to do
}

void Directory::set_path(const std::string& path_) {
	close();
	path = path_;
	while(path.size() > 1 && (path.back() == '/' || path.back() == '\\')) {
		path.pop_back();
	}
}

void Directory::open(const std::string &path_) {
	set_path(path_);
	open();
}

void Directory::open() const {
	close();
	// nothing to do
}

void Directory::create(const std::string &path_) {
	set_path(path_);
	create();
}

void Directory::create() {
	if (!path.empty()) {
		std::filesystem::create_directories(path);
	}
}

std::vector<std::shared_ptr<File>> Directory::files() const {
	std::vector<std::shared_ptr<File>> result;

	if (!std::filesystem::exists(path)) {
		throw std::runtime_error("Error opening directory '" + path + "'");
	}

	for (auto const& dir_entry : std::filesystem::directory_iterator(path)) {
		if (std::filesystem::is_regular_file(dir_entry)) {
			result.push_back(std::make_shared<File>(dir_entry.path().string()));
		}
	}

	return result;
}

std::vector<std::shared_ptr<Directory>> Directory::directories() const {
	std::vector<std::shared_ptr<Directory>> result;

	if (!std::filesystem::exists(path)) {
		throw std::runtime_error("Error opening directory '" + path + "'");
	}

	for (auto const& dir_entry : std::filesystem::directory_iterator(path)) {
		if (std::filesystem::is_directory(dir_entry)) {
			DWORD attributes = GetFileAttributes(dir_entry.path().string().c_str());
			if (!(attributes & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM))) {
				result.push_back(std::make_shared<Directory>(dir_entry.path().string()));
			}
		}
	}

	return result;
}

std::vector<std::shared_ptr<File>> Directory::all_files() const {
	std::vector<std::shared_ptr<File>> result;

	if (!std::filesystem::exists(path)) {
		throw std::runtime_error("Error opening directory '" + path + "'");
	}

	for (auto const& dir_entry : std::filesystem::directory_iterator(path)) {
        result.push_back(std::make_shared<File>(dir_entry.path().string()));
    }

	return result;
}

std::string Directory::get_path() const {
	std::string result = path;
	if(!result.empty()) {
		result += '\\';
	}
	return result;
}

std::string Directory::get_name() const {
	std::string result = path;
	const size_t pos = result.find_last_of("/\\");
	if(pos != std::string::npos && pos != 0) {
		result = result.substr(pos + 1);
	}
	return result;
}

void Directory::close() const {
	// nothing to do
}


} // vnx
