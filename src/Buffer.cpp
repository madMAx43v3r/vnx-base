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
#include <vnx/Buffer.hpp>
 

namespace vnx {

Buffer::Buffer(const Memory& other) {
	*this = other;
}

Buffer& Buffer::operator=(const Memory& other) {
	resize(other.get_size());
	size_t offset = 0;
	const auto* chunk = other.front();
	while(chunk) {
		::memcpy(data(offset), chunk->data, chunk->size);
		offset += chunk->size;
		chunk = chunk->next;
	}
	return *this;
}

void Buffer::reserve(size_t new_capacity) {
	if(new_capacity > capacity_) {
		void* new_data = ::malloc(new_capacity);
		if(data_) {
			::memcpy(new_data, data_, size_);
			::free(data_);
		}
		data_ = new_data;
		capacity_ = new_capacity;
	}
}

void Buffer::resize(size_t new_size) {
	if(new_size > capacity_) {
		reserve(std::max(new_size, capacity_ * 2));
	}
	size_ = new_size;
}

void Buffer::read_file(const std::string& file_path) {
	::FILE* p_file = ::fopen(file_path.c_str(), "rb");
	if(p_file) {
		const auto offset = size();
		vnx::fseek(p_file, 0, SEEK_END);
		const auto num_bytes = vnx::ftell(p_file);
		resize(offset + num_bytes);
		vnx::fseek(p_file, 0, SEEK_SET);
		const auto num_read = ::fread(data(offset), 1, num_bytes, p_file);
		resize(offset + num_read);
		::fclose(p_file);
	}
}


size_t BufferInputStream::read(void* buf, size_t len) {
	if(data->size() > pos) {
		const size_t left = data->size() - pos;
		const size_t num_bytes = left < len ? left : len;
		::memcpy(buf, data->data(pos), num_bytes);
		pos += num_bytes;
		return num_bytes;
	}
	return 0;
}


void read(TypeInput& in, Buffer& value, const TypeCode* type_code, const uint16_t* code) {
	uint32_t size = 0;
	switch(code[0]) {
		case CODE_LIST:
		case CODE_ALT_LIST:
			switch(code[1]) {
				case CODE_INT8:
				case CODE_UINT8:
				case CODE_ALT_INT8:
				case CODE_ALT_UINT8:
					read(in, size);
			}
			if(code[0] == CODE_ALT_LIST) {
				size = flip_bytes(size);
			}
			break;
		default:
			skip(in, type_code, code);
	}
	if(size) {
		value.resize(size);
		in.read((char*)value.data(), size);
	} else {
		value.clear();
	}
}

void write(TypeOutput& out, const Buffer& value, const TypeCode* type_code, const uint16_t* code) {
	if(value.size() > VNX_MAX_SIZE) {
		throw std::invalid_argument("write(vnx::Buffer): size > VNX_MAX_SIZE");
	}
	write(out, (uint32_t)value.size());
	out.write(value.data(), value.size());
}

void read(std::istream& in, Buffer& value) {
	std::vector<uint8_t> data;
	read(in, data);
	value.resize(data.size());
	::memcpy(value.data(), data.data(), data.size());
}

void write(std::ostream& out, const Buffer& value) {
	DefaultPrinter printer(out);
	accept(printer, value);
}

void accept(Visitor& visitor, const Buffer& value) {
	std::vector<uint8_t> data(value.size());
	::memcpy(data.data(), value.data(), data.size());
	accept(visitor, data);
}


} // vnx
