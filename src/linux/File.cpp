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
		throw std::runtime_error("flock('" + path + "') failed with: " + std::string(::strerror(errno)));
	}
}


} // mmx
