/*
 * Directory.cpp
 *
 *  Created on: Jun 8, 2020
 *      Author: jaw
 */

#include <vnx/vnx.h>
#include <vnx/Directory.h>
#include <windows.h>


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

void Directory::open() {
	close();
	// nothing to do
}

void Directory::create(const std::string &path_) {
	set_path(path_);
	create();
}

void Directory::create() {
	size_t position = 0;
	while(position < path.length()){
		position = path.find_first_of("/\\", position);
		if(position == std::string::npos) position = path.length();
		std::string parent = path.substr(0, position);
		if(!CreateDirectoryA(parent.c_str(), NULL)) {
			const auto err = GetLastError();
			if(err != ERROR_ALREADY_EXISTS) {
				throw std::runtime_error("CreateDirectoryA() failed with: code " + std::to_string(err));
			}
		}
		position++;
	}
}

std::vector<std::shared_ptr<File>> Directory::files() const {
	std::vector<std::shared_ptr<File>> result;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile((path + "/*").c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE){
		throw std::runtime_error("Error opening directory '" + path + "'");
	}

	do{
		std::string name = FindFileData.cFileName;
		if(name == "." || name == "..") continue;
		bool isDir = ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
		bool isFile = !isDir; //((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) != 0);

		if(isFile){
			result.push_back(std::make_shared<File>(get_path() + name));
		}
	}while(FindNextFile(hFind, &FindFileData) != 0);

	FindClose(hFind);
	return result;
}

std::vector<std::shared_ptr<Directory>> Directory::directories() const {
	std::vector<std::shared_ptr<Directory>> result;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile((path + "/*").c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE){
		throw std::runtime_error("Error opening directory '" + path + "'");
	}

	do{
		std::string name = FindFileData.cFileName;
		if(name == "." || name == "..") continue;
		bool isDir = ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);

		if(isDir){
			result.push_back(std::make_shared<Directory>(get_path() + name));
		}
	}while(FindNextFile(hFind, &FindFileData) != 0);

	FindClose(hFind);
	return result;
}

std::vector<std::shared_ptr<File>> Directory::all_files() const {
	std::vector<std::shared_ptr<File>> result;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile((path + "/*").c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE){
		throw std::runtime_error("Error opening directory '" + path + "'");
	}

	do{
		std::string name = FindFileData.cFileName;
		result.push_back(std::make_shared<File>(get_path() + name));
	}while(FindNextFile(hFind, &FindFileData) != 0);

	FindClose(hFind);
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

void Directory::close() {
	// nothing to do
}


} // vnx
