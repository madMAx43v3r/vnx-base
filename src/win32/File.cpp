/*
 * File.cpp
 *
 *  Created on: Jun 28, 2022
 *      Author: mad
 */

#include <vnx/File.h>

#include <fileapi.h>


namespace vnx {

void File::lock_exclusive() {
	OVERLAPPED overlapvar = {};
	HANDLE file = (HANDLE)_get_osfhandle(::fileno(p_file));
	if(!LockFileEx(file, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, MAXDWORD, MAXDWORD, &overlapvar)) {
		throw std::runtime_error("LockFileEx('" + path + "') failed with: " + std::string(::strerror(GetLastError())));
	}
	is_locked = true;
}

void File::unlock() {
	if(!is_locked) {
		return;
	}
	OVERLAPPED overlapvar = {};
	HANDLE file = (HANDLE)_get_osfhandle(::fileno(p_file));
	if(!UnlockFileEx(file, 0, MAXDWORD, MAXDWORD, &overlapvar)) {
		throw std::runtime_error("UnlockFileEx('" + path + "') failed with: " + std::string(::strerror(GetLastError())));
	}
	is_locked = false;
}


} // mmx
