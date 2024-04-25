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
#include <vnx/JSON.h>
#include <vnx/Input.hpp>
#include <vnx/Generic.hxx>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <fstream>

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif


// Taken from https://stackoverflow.com/questions/42492843/using-readfile-in-c-code
#ifdef _WIN32
ssize_t pread(int fd, void *buf, size_t count, uint64_t offset){
	long unsigned int read_bytes = 0;
	OVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(OVERLAPPED));

	overlapped.OffsetHigh = (uint32_t)((offset & 0xFFFFFFFF00000000LL) >> 32);
	overlapped.Offset = (uint32_t)(offset & 0xFFFFFFFFLL);

	HANDLE file = (HANDLE)_get_osfhandle(fd);
	SetLastError(0);
	bool RF = ReadFile(file, buf, count, &read_bytes, &overlapped);

	// For some reason it errors when it hits end of file so we don't want to check that
	if ((RF == 0) && GetLastError() != ERROR_HANDLE_EOF) {
		errno = GetLastError();
		// printf ("Error reading file : %d\n", GetLastError());
		return -1;
	}

	return read_bytes;
}
#endif


namespace vnx {

size_t BasicInputStream::read(void* buf, size_t len) {
	const auto res = ::read(fd, buf, len);
	if(res < 0) {
		throw std::runtime_error("read() failed with: " + std::string(strerror(errno)));
	}
	return size_t(res);
}

size_t FileInputStream::read(void* buf, size_t len) {
	if(!file) {
		return 0;
	}
	return ::fread(buf, 1, len, file);
}

int64_t FileInputStream::get_input_pos() const {
	if(!file) {
		return 0;
	}
	const auto pos = vnx::ftell(file);
	if(pos < 0) {
		throw std::runtime_error("ftell() failed with: " + std::string(strerror(errno)));
	}
	return int64_t(pos);
}

size_t FileSectionInputStream::read(void* buf, size_t len) {
	if(length >= 0) {
		const auto left = length - pos;
		if(len > size_t(left)) {
			len = left;
		}
	}
	if(buffer_size) {
		len = std::min(len, buffer_size);
		buffer_size <<= 2;
	}
	const auto num_bytes = ::pread(fd, buf, len, offset + pos);
	if(num_bytes < 0) {
		throw std::runtime_error("pread() failed with: " + std::string(strerror(errno)));
	}
	pos += num_bytes;
	return num_bytes;
}

int64_t FileSectionInputStream::get_input_pos() const {
	return int64_t(offset + pos);
}

void FileSectionInputStream::reset(::FILE* file_, int64_t offset_, int64_t length_, size_t buffer_size_) {
	if(file_) {
		fd = ::fileno(file_);
		offset = offset_;
		length = length_;
	} else {
		fd = -1;
		offset = 0;
		length = 0;
	}
	buffer_size = buffer_size_;
	pos = 0;
}

size_t PointerInputStream::read(void* buf, size_t len) {
	if(!data) {
		throw std::logic_error("data == null");
	}
	const size_t left = size - pos;
	if(len > left) {
		len = left;
	}
	::memcpy(buf, data + pos, len);
	pos += len;
	return len;
}

size_t VectorInputStream::read(void* buf, size_t len) {
	if(!vector) {
		throw std::logic_error("vector == null");
	}
	const size_t left = vector->size() - pos;
	if(len > left) {
		len = left;
	}
	::memcpy(buf, vector->data() + pos, len);
	pos += len;
	return len;
}

size_t SocketInputStream::read(void* buf, size_t len) {
	const auto res = ::recv(socket, (char*)buf, len, 0);
	if(res < 0) {
		throw std::runtime_error("recv() failed with: " + std::string(strerror(errno)));
	}
	return size_t(res);
}

const TypeCode* TypeInput::get_type_code(Hash64 code_hash) const {
	auto iter = type_code_map.find(code_hash);
	if(iter != type_code_map.end()) {
		return iter->second;
	}
	const TypeCode* type_code = vnx::get_type_code(code_hash);
	if(!type_code) {
		// this should never happen
		throw std::runtime_error("get_type_code(): unknown code hash");
	}
	return type_code;
}

void TypeInput::clear() {
	reset();
	type_code_map.clear();
}

std::streamsize TextInput::showmanyc() {
	return buffer.get_num_avail();
}

std::streamsize TextInput::xsgetn(char_type* s, std::streamsize n) {
	try {
		buffer.read(s, n);
		return n;
	} catch(...) {
		return 0;
	}
}

TextInput::int_type TextInput::underflow() {
	if(gptr() < egptr()) {
		return *gptr();
	}
	try {
		if(const auto num_avail = buffer.fetch(1)) {
			setg((char_type*)&buffer[0], (char_type*)&buffer[0], (char_type*)&buffer[num_avail]);
			return *buffer.read(num_avail);
		}
	} catch(...) {
		// fall through
	}
	return traits_type::eof();
}


std::shared_ptr<Value> read(TypeInput& in) {
	while(true) {
		uint16_t code;
		read(in, code);
		switch(code) {
			case CODE_NULL:
				return nullptr;
			case CODE_TYPE:
			case CODE_ALT_TYPE: {
				Hash64 code_hash;
				read(in, code_hash);
				if(code == CODE_ALT_TYPE) {
					code_hash = flip_bytes(code_hash);
				}
				const TypeCode* type_code = in.get_type_code(code_hash);
				std::shared_ptr<Value> value = type_code->create_value();
				if(value) {
					value->read(in, type_code, nullptr);
				} else {
					skip(in, type_code, nullptr);
				}
				return value;
			}
			case CODE_DYNAMIC:
			case CODE_ALT_DYNAMIC: {
				std::shared_ptr<Generic> value = Generic::create();
				vnx::read(in, value->data, nullptr, &code);
				return value;
			}
			case CODE_OBJECT:
			case CODE_ALT_OBJECT: {
				std::shared_ptr<Object> value = Object::create();
				value->read(in, nullptr, &code);
				return value;
			}
			case CODE_TYPE_CODE:
			case CODE_ALT_TYPE_CODE:
				read_type_code(in, &code);
				break;
			case CODE_PADDING:
			case CODE_ALT_PADDING:
				skip(in, nullptr, &code);
				break;
			case CODE_ANY:
			case CODE_NONE:
			case CODE_MAGIC:
			case CODE_ALT_ANY:
			case CODE_ALT_MAGIC:
				break;
			default:
				throw std::runtime_error("read(): invalid code");
		}
	}
	return nullptr;
}

void read(TypeInput& in, Variant& value) {
	read(in, value, nullptr, nullptr);
}

void skip(TypeInput& in) {
	copy(in, nullptr);
}

void skip(TypeInput& in, const TypeCode* type_code, const uint16_t* code) {
	copy(in, nullptr, type_code, code);
}

void copy_bytes(TypeInput& in, TypeOutput* out, size_t num_bytes) {
	uint8_t buffer[VNX_BUFFER_SIZE];
	while(num_bytes) {
		const size_t len = num_bytes > VNX_BUFFER_SIZE ? VNX_BUFFER_SIZE : num_bytes;
		uint8_t* p_out = nullptr;
		if(out) {
			p_out = out->write(len);
		} else {
			p_out = buffer;
		}
		in.read(p_out, len);
		num_bytes -= len;
	}
}

void copy(TypeInput& in, TypeOutput* out) {
	copy(in, out, nullptr, nullptr);
}

void copy(TypeInput& in, TypeOutput* out, const TypeCode* type_code, const uint16_t* code) {
	if(code) {
		switch(code[0]) {
			case CODE_NULL:
				return;
			case CODE_BOOL:
			case CODE_INT8:
			case CODE_UINT8:
			case CODE_ALT_BOOL:
			case CODE_ALT_INT8:
			case CODE_ALT_UINT8: {
				const auto* src = in.read(1);
				if(out) {
					out->write(src, 1);
				}
				return;
			}
			case CODE_INT16:
			case CODE_UINT16:
			case CODE_ALT_INT16:
			case CODE_ALT_UINT16:{
				const auto* src = in.read(2);
				if(out) {
					out->write(src, 2);
				}
				return;
			}
			case CODE_INT32:
			case CODE_UINT32:
			case CODE_FLOAT:
			case CODE_ALT_INT32:
			case CODE_ALT_UINT32:
			case CODE_ALT_FLOAT: {
				const auto* src = in.read(4);
				if(out) {
					out->write(src, 4);
				}
				return;
			}
			case CODE_INT64:
			case CODE_UINT64:
			case CODE_DOUBLE:
			case CODE_ALT_INT64:
			case CODE_ALT_UINT64:
			case CODE_ALT_DOUBLE: {
				const auto* src = in.read(8);
				if(out) {
					out->write(src, 8);
				}
				return;
			}
			case CODE_STRING:
			case CODE_ALT_STRING: {
				uint32_t size;
				read(in, size);
				if(out) {
					write(*out, size);
				}
				if(code[0] == CODE_ALT_STRING) {
					size = flip_bytes(size);
				}
				copy_bytes(in, out, size);
				return;
			}
			case CODE_ARRAY:
			case CODE_ALT_ARRAY: {
				uint16_t size = code[1];
				if(code[0] == CODE_ALT_ARRAY) {
					size = flip_bytes(size);
				}
				const uint16_t* value_code = code + 2;
				const size_t value_size = get_value_size(value_code, type_code);
				if(value_size) {
					const auto* src = in.read(size * value_size);
					if(out) {
						::memcpy(out->write(size * value_size), src, size * value_size);
					}
				} else {
					for(size_t i = 0; i < size; ++i) {
						copy(in, out, type_code, value_code);
					}
				}
				return;
			}
			case CODE_LIST:
			case CODE_ALT_LIST: {
				uint32_t size;
				read(in, size);
				if(out) {
					write(*out, size);
				}
				if(code[0] == CODE_ALT_LIST) {
					size = flip_bytes(size);
				}
				const uint16_t* value_code = code + 1;
				const auto value_size = get_value_size(value_code, type_code);
				if(value_size) {
					copy_bytes(in, out, size * value_size);
				} else {
					for(size_t i = 0; i < size; ++i) {
						copy(in, out, type_code, value_code);
					}
				}
				return;
			}
			case CODE_MAP:
			case CODE_ALT_MAP: {
				uint32_t size;
				read(in, size);
				if(out) {
					write(*out, size);
				}
				const uint16_t* key_code = code + 2;
				const uint16_t* value_code;
				if(code[0] == CODE_MAP) {
					value_code = code + code[1];
				} else {
					size = flip_bytes(size);
					value_code = code + flip_bytes(code[1]);
				}
				for(size_t i = 0; i < size; ++i) {
					copy(in, out, type_code, key_code);
					copy(in, out, type_code, value_code);
				}
				return;
			}
			case CODE_TYPE_CODE:
			case CODE_ALT_TYPE_CODE: {
				TypeCode tmp;
				read(in, tmp, type_code, code);
				if(out) {
					write(*out, tmp, type_code, code);
				}
				return;
			}
			case CODE_ANY:
			case CODE_ALT_ANY: {
				std::shared_ptr<Value> value = read(in);
				if(out) {
					write(*out, value);
				}
				return;
			}
			case CODE_DYNAMIC:
			case CODE_ALT_DYNAMIC: {
				uint16_t dyn_code[VNX_MAX_BYTE_CODE_SIZE];
				const uint16_t code_size = read_byte_code(in, dyn_code);
				if(out) {
					write_byte_code(*out, dyn_code, code_size);
				}
				copy(in, out, type_code, dyn_code);
				return;
			}
			case CODE_PADDING:
			case CODE_ALT_PADDING: {
				uint32_t size;
				read(in, size);
				if(out) {
					write(*out, size);
				}
				if(code[0] == CODE_ALT_PADDING) {
					size = flip_bytes(size);
				}
				copy_bytes(in, out, size);
				return;
			}
			case CODE_MATRIX:
			case CODE_ALT_MATRIX: {
				std::vector<size_t> dims;
				const auto total_size = read_matrix_size(dims, code);
				const uint16_t* value_code = code + 2 + dims.size();
				const auto value_size = get_value_size(value_code, type_code);
				if(value_size) {
					const auto* src = in.read(total_size * value_size);
					if(out) {
						::memcpy(out->write(total_size * value_size), src, total_size * value_size);
					}
				} else {
					for(size_t i = 0; i < total_size; ++i) {
						copy(in, out, type_code, value_code);
					}
				}
				return;
			}
			case CODE_IMAGE:
			case CODE_ALT_IMAGE: {
				std::vector<size_t> dims;
				const auto total_size = read_image_size(in, dims, code);
				if(out) {
					for(auto dim : dims) {
						write(*out, uint32_t(dim));
					}
				}
				const uint16_t* value_code = code + 2;
				const auto value_size = get_value_size(value_code, type_code);
				if(value_size) {
					copy_bytes(in, out, total_size * value_size);
				} else {
					for(size_t i = 0; i < total_size; ++i) {
						copy(in, out, type_code, value_code);
					}
				}
				return;
			}
			case CODE_TUPLE: {
				const size_t N = code[1];
				for(size_t i = 0; i < N; ++i) {
					copy(in, out, type_code, code + code[2 + i]);
				}
				return;
			}
			case CODE_ALT_TUPLE: {
				const size_t N = flip_bytes(code[1]);
				for(size_t i = 0; i < N; ++i) {
					copy(in, out, type_code, code + flip_bytes(code[2 + i]));
				}
				return;
			}
			case CODE_OBJECT: {
				copy(in, out, type_code, Object::CODE);
				return;
			}
			case CODE_ALT_OBJECT: {
				copy(in, out, type_code, Object::ALT_CODE);
				return;
			}
			case CODE_OPTIONAL:
			case CODE_ALT_OPTIONAL: {
				bool flag = false;
				read(in, flag);
				if(out) {
					write(*out, flag);
				}
				if(flag) {
					copy(in, out, type_code, code + 1);
				}
				return;
			}
		}
	}
	if(!type_code) {
		if(code && code[0] != CODE_ANY && code[0] != CODE_ALT_ANY) {
			throw std::logic_error("copy(): expected CODE_ANY");
		}
		const uint16_t code = CODE_ANY;
		copy(in, out, nullptr, &code);
		return;
	}
	if(code) {
		if(code[0] != CODE_STRUCT && code[0] != CODE_ALT_STRUCT) {
			throw std::logic_error("copy(): expected CODE_STRUCT");
		}
		type_code = type_code->depends[code[1]];
	}
	const auto* src = in.read(type_code->total_field_size);
	if(out) {
		auto* dst = out->write(type_code->total_field_size);
		::memcpy(dst, src, type_code->total_field_size);
	}
	for(const TypeField* field : type_code->ext_fields) {
		copy(in, out, type_code, field->code.data());
	}
}


size_t read_matrix_size(std::vector<size_t>& dims, const uint16_t* code) {
	switch(code[0]) {
		case CODE_MATRIX:
			dims.resize(code[1]);
			for(size_t i = 0; i < dims.size(); ++i) {
				dims[i] = code[2 + i];
			}
			break;
		case CODE_ALT_MATRIX:
			dims.resize(flip_bytes(code[1]));
			for(size_t i = 0; i < dims.size(); ++i) {
				dims[i] = flip_bytes(code[2 + i]);
			}
			break;
		default:
			throw std::logic_error("read_matrix_size(): invalid code");
	}
	if(dims.empty()) {
		throw std::logic_error("CODE_MATRIX: N = 0");
	}
	size_t total_size = 1;
	for(auto dim : dims) {
		total_size *= dim;
	}
	return total_size;
}

size_t read_image_size(TypeInput& in, std::vector<size_t>& size, const uint16_t* code) {
	size_t N = 0;
	switch(code[0]) {
		case CODE_IMAGE: N = code[1]; break;
		case CODE_ALT_IMAGE: N = flip_bytes(code[1]); break;
		default:
			throw std::logic_error("read_image_size(): invalid code");
	}
	if(N == 0) {
		throw std::logic_error("CODE_IMAGE: N = 0");
	}
	size.resize(N);

	size_t total_size = 1;
	const auto* buf = in.read(4 * N);
	for(size_t i = 0; i < N; ++i) {
		uint32_t size_ = 0;
		read_value(buf + 4 * i, size_);
		if(code[0] == CODE_ALT_IMAGE) {
			size_ = flip_bytes(size_);
		}
		size[i] = size_;
		total_size *= size_;
	}
	return total_size;
}

void read(TypeInput& in, std::string& string, const TypeCode* type_code, const uint16_t* code) {
	switch(code[0]) {
		case CODE_LIST:
		case CODE_ALT_LIST:
			if(code[1] != CODE_INT8 && code[1] != CODE_ALT_INT8) {
				break;
			}
			/* no break */
		case CODE_STRING:
		case CODE_ALT_STRING: {
			uint32_t size;
			read(in, size);
			switch(code[0]) {
				case CODE_ALT_LIST:
				case CODE_ALT_STRING:
					size = flip_bytes(size);
			}
			if(in.safe_read) {
				for(uint32_t i = 0; i < size;) {
					const auto chunk_size = std::min<uint32_t>(size - i, VNX_BUFFER_SIZE);
					string.resize(i + chunk_size);
					in.read(&string[i], chunk_size);		// since C++11 this is guaranteed to be valid
					i += chunk_size;
				}
			} else {
				string.resize(size);
				in.read(&string[0], size);		// since C++11 this is guaranteed to be valid
			}
			return;
		}
		case CODE_OPTIONAL:
		case CODE_ALT_OPTIONAL: {
			bool flag = false;
			vnx::read(in, flag);
			if(flag) {
				read(in, string, type_code, code + 1);
			} else {
				string.clear();
			}
			return;
		}
	}
	string = to_string_value(in, type_code, code);
}

template<>
void read(TypeInput& in, std::vector<bool>& vector, const TypeCode* type_code, const uint16_t* code) {
	std::vector<uint8_t> tmp;
	vnx::read(in, tmp, type_code, code);
	vector.resize(tmp.size());
	for(size_t i = 0; i < tmp.size(); ++i) {
		vector[i] = tmp[i];
	}
}

uint16_t read_byte_code(TypeInput& in, uint16_t* code) {
	uint16_t code_size;
	read(in, code_size);
	if(code_size > 0xFF) {
		code_size = flip_bytes(code_size);
	}
	if(code_size == 0) {
		throw std::runtime_error("read_byte_code(): dynamic code size == 0");
	}
	if(code_size > VNX_MAX_BYTE_CODE_SIZE) {
		throw std::runtime_error("read_byte_code(): dynamic code size > VNX_MAX_BYTE_CODE_SIZE");
	}
	in.read(code, code_size * sizeof(uint16_t));
	validate_code(code, nullptr, code_size);
	return code_size;
}

const TypeCode* read_type_code(TypeInput& in, const uint16_t* code) {
	std::shared_ptr<TypeCode> type_code = std::make_shared<TypeCode>();
	vnx::read(in, *type_code, nullptr, code);
	type_code->build();
	const TypeCode* out = vnx::register_type_code(type_code);
	in.type_code_map[type_code->code_hash] = out;
	return out;
}

void read(std::istream& in, std::string& value) {
	value = read(in).to_string_value();
}

Variant read(std::istream& in) {
	if(auto value = read_json(in)) {
		return value->to_variant();
	}
	throw std::underflow_error("EOF");
}

template<>
void from_string_value<std::string>(const std::string& str, std::string& value) {
	value = str;
}

std::shared_ptr<Value> read_from_file(const std::string& file_name) {
	File file(file_name);
	std::shared_ptr<Value> value;
	try {
		if(file.get_extension() == ".json") {
			std::ifstream stream(file_name);
			auto object = Object::create();
			vnx::read(stream, *object);
			value = object->to_value();
			if(!value) {
				value = object;
			}
		} else {
			file.open("rb");
			value = vnx::read(file.in);
		}
	} catch(...) {
		// ignore errors
	}
	return value;
}


} // vnx
