/*
 * File.cpp
 *
 *  Created on: Jun 28, 2022
 *      Author: mad
 */

#include <vnx/File.h>

#include <sys/file.h>


namespace vnx {

void File::lock_exclusive() {
	if(::flock(::fileno(p_file), LOCK_EX | LOCK_NB)) {
		throw std::runtime_error("flock('" + path + "', LOCK_EX) failed with: " + std::string(::strerror(errno)));
	}
	is_locked = true;
}

void File::unlock() {
	if(!is_locked) {
		return;
	}
	if(::flock(::fileno(p_file), LOCK_UN | LOCK_NB)) {
		throw std::runtime_error("flock('" + path + "', LOCK_UN) failed with: " + std::string(::strerror(errno)));
	}
	is_locked = false;
}


} // mmx
