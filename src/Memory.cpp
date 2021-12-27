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

Memory& Memory::operator=(const Memory& other) {
	clear();
	const chunk_t* chunk = other.front_;
	while(chunk) {
		::memcpy(add_chunk(chunk->size()), chunk->data(), chunk->size());
		chunk = chunk->next();
	}
	return *this;
}

bool Memory::operator==(const Memory& other) const {
	if(get_size() != other.get_size()) {
		return false;
	}
	MemoryInputStream A(this);
	MemoryInputStream B(&other);
	char buf_A[1024];
	char buf_B[1024];
	while(true) {
		const size_t len_A = A.read(buf_A, 1024);
		const size_t len_B = B.read(buf_B, 1024);
		if(len_A != len_B) {
			throw std::logic_error("Memory::operator==(): len_A != len_B");
		}
		if(::memcmp(buf_A, buf_B, len_A) != 0) {
			return false;
		}
		if(len_A != 1024) {
			break;
		}
	}
	return true;
}

bool Memory::operator!=(const Memory& other) const {
	return !(*this == other);
}

bool Memory::operator<(const Memory& other) const {
	MemoryInputStream A(this);
	MemoryInputStream B(&other);
	char buf_A[1024];
	char buf_B[1024];
	size_t size_A = 0;
	size_t size_B = 0;
	while(true) {
		const size_t len_A = A.read(buf_A, 1024);
		const size_t len_B = B.read(buf_B, 1024);
		if(int res = ::memcmp(buf_A, buf_B, len_A < len_B ? len_A : len_B)) {
			return res < 0;
		}
		size_A += len_A;
		size_B += len_B;
		if(len_A != 1024 || len_B != 1024) {
			break;
		}
	}
	return size_A < size_B;
}

bool Memory::operator>(const Memory& other) const {
	return other < *this;
}

Hash64 Memory::get_hash() const {
	CRC64 func;
	const chunk_t* chunk = front_;
	while(chunk) {
		func.update(chunk->data(), chunk->size());
		chunk = chunk->next();
	}
	return Hash64(func.get());
}

char* Memory::add_chunk(size_t num_bytes) {
	if(num_bytes == 0) {
		throw std::logic_error("add_chunk(): len == 0");
	}
	if(front_ == &first_) {
		chunk_t* tmp = new chunk_t();
		if(front_->is_short()) {
			tmp->alloc(front_->size(), false);
			::memcpy(tmp->data(), front_->data(), front_->size());
		} else {
			*tmp = *front_;
		}
		front_ = tmp;
		back_ = front_;
	}
	chunk_t* chunk;
	if(!front_) {
		first_ = chunk_t();
		chunk = &first_;
		front_ = chunk;
		chunk->alloc(num_bytes, true);
	} else {
		chunk = new chunk_t();
		back_->next_ = chunk;
		back_ = chunk;
		chunk->alloc(num_bytes, false);
	}
	return chunk->data();
}

size_t Memory::get_size() const {
	size_t size = 0;
	const chunk_t* chunk = front_;
	while(chunk) {
		size += chunk->size();
		chunk = chunk->next();
	}
	return size;
}

void Memory::write(OutputStream& out) const {
	const chunk_t* chunk = front_;
	while(chunk) {
		out.write(chunk->data(), chunk->size());
		chunk = chunk->next();
	}
}

void Memory::write(OutputBuffer& out) const {
	const chunk_t* chunk = front_;
	while(chunk) {
		out.write(chunk->data(), chunk->size());
		chunk = chunk->next();
	}
}

void Memory::clear() {
	chunk_t* chunk = front_;
	while(chunk) {
		chunk_t* tmp = chunk;
		chunk = chunk->next();
		tmp->free();
		if(tmp != &first_) {
			delete tmp;
		}
	}
	front_ = 0;
}

std::string Memory::as_string() const {
	std::string result;
	result.resize(get_size());
	size_t offset = 0;
	const Memory::chunk_t* chunk = front_;
	while(chunk) {
		::memcpy(&result[offset], chunk->data(), chunk->size());
		offset += chunk->size();
		chunk = chunk->next();
	}
	return result;
}


void read(TypeInput& in, Memory& value, const TypeCode* type_code, const uint16_t* code) {
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
		char* data = value.add_chunk(size);
		in.read(data, size);
	} else {
		value.clear();
	}
}

void write(TypeOutput& out, const Memory& value, const TypeCode* type_code, const uint16_t* code) {
	const size_t size = value.get_size();
	if(size > VNX_MAX_SIZE) {
		throw std::invalid_argument("write(vnx::Memory): size > VNX_MAX_SIZE");
	}
	write(out, uint32_t(size));
	const Memory::chunk_t* chunk = value.front();
	while(chunk) {
		out.write(chunk->data(), chunk->size());
		chunk = chunk->next();
	}
}

void read(std::istream& in, Memory& value) {
	value.clear();
	std::vector<uint8_t> tmp;
	read(in, tmp);
	char* data = value.add_chunk(tmp.size());
	::memcpy(data, tmp.data(), tmp.size());
}

void write(std::ostream& out, const Memory& value) {
	DefaultPrinter printer(out);
	accept(printer, value);
}

void accept(Visitor& visitor, const Memory& value) {
	const size_t size = value.get_size();
	visitor.list_begin(size);
	size_t index = 0;
	const Memory::chunk_t* chunk = value.front();
	while(chunk) {
		for(size_t i = 0; i < chunk->size(); ++i) {
			visitor.list_element(index++);
			visitor.visit(uint8_t(chunk->data()[i]));
		}
		chunk = chunk->next();
	}
	visitor.list_end(size);
}


} // vnx
