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
#include <vnx/Memory.hpp>

#include <cstdlib>
#include <cstring>


namespace vnx {

Hash64 Memory::get_hash() const {
	CRC64 func;
	const chunk_t* chunk = front_;
	while(chunk) {
		func.update(chunk->data, chunk->size);
		chunk = chunk->next;
	}
	return Hash64(func.get());
}

char* Memory::add_chunk(size_t num_bytes) {
	if(num_bytes == 0) {
		throw std::logic_error("add_chunk(): num_bytes == 0");
	}
	auto chunk = new chunk_t(num_bytes);
	if(!front_) {
		front_ = chunk;
		back_ = chunk;
	} else {
		back_->next = chunk;
		back_ = chunk;
	}
	return chunk->data;
}

size_t Memory::get_size() const {
	size_t size = 0;
	const chunk_t* chunk = front_;
	while(chunk) {
		size += chunk->size;
		chunk = chunk->next;
	}
	return size;
}

void Memory::write(OutputStream& out) const {
	const chunk_t* chunk = front_;
	while(chunk) {
		out.write(chunk->data, chunk->size);
		chunk = chunk->next;
	}
}

void Memory::write(OutputBuffer& out) const {
	const chunk_t* chunk = front_;
	while(chunk) {
		out.write(chunk->data, chunk->size);
		chunk = chunk->next;
	}
}

void Memory::clear() {
	auto chunk = front_;
	while(chunk) {
		auto tmp = chunk;
		chunk = chunk->next;
		delete tmp;
	}
	front_ = nullptr;
	back_ = nullptr;
}

std::string Memory::as_string() const {
	std::string result;
	result.resize(get_size());
	size_t offset = 0;
	const chunk_t* chunk = front_;
	while(chunk) {
		::memcpy(&result[offset], chunk->data, chunk->size);
		offset += chunk->size;
		chunk = chunk->next;
	}
	return result;
}


} // vnx
