/*
 * Util.cpp
 *
 *  Created on: Apr 7, 2024
 *      Author: mad
 */

#include <vnx/Util.h>
#include <vnx/File.h>

#include <fstream>


namespace vnx {

void secure_random_bytes(void* data, const size_t length)
{
	vnx::File file("/dev/random");
	file.open("rb");
	file.in.read(data, length);
}


} // vnx
