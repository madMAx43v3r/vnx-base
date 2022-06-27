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
#include <vnx/Output.hpp>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <cstdlib>

#ifdef _WIN32
#include <io.h>
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

 
namespace vnx {

void BasicOutputStream::write(const void* buf, size_t len) {
	const char* pos = (const char*)buf;
	while(len > 0) {
		ssize_t res = ::write(fd, pos, len);
		if(res <= 0) {
			throw std::runtime_error("write() failed with: " + std::string(strerror(errno)));
		}
		pos += res;
		len -= res;
	}
}

void FileOutputStream::write(const void* buf, size_t len) {
	size_t res = ::fwrite(buf, 1, len, file);
	if(res != len) {
		throw std::runtime_error("write() failed with: " + std::string(strerror(errno)));
	}
}

int64_t FileOutputStream::get_output_pos() const {
	const int64_t pos = vnx::ftell(file);
	if(pos < 0) {
		throw std::runtime_error("ftell() failed with: " + std::string(strerror(errno)));
	}
	return pos;
}

void VectorOutputStream::write(const void* buf, size_t len) {
	if(!vector) {
		throw std::runtime_error("vector == null");
	}
	const size_t offset = vector->size();
	vector->resize(offset + len);
	::memcpy(&(*vector)[offset], buf, len);
}

int64_t VectorOutputStream::get_output_pos() const {
	if(!vector) {
		throw std::runtime_error("vector == null");
	}
	return int64_t(vector->size());
}

void SocketOutputStream::write(const void* buf, size_t len) {
	const char* pos = (const char*)buf;
	while(len > 0) {
#ifdef _WIN32
		ssize_t res = ::send(fd, pos, len, 0);
#else
		ssize_t res = ::send(fd, pos, len, MSG_NOSIGNAL);
#endif
		if(res <= 0) {
			throw std::runtime_error("write() failed with: " + std::string(strerror(errno)));
		}
		pos += res;
		len -= res;
	}
}

bool TypeOutput::write_type_code(const TypeCode* type_code) {
	if(type_code && !disable_type_codes) {
		if(!type_code_map.count(type_code->code_hash)) {
			vnx::write(*this, *type_code);
			return true;
		}
	}
	return false;
}

void TypeOutput::clear() {
	reset();
	type_code_map.clear();
}

std::streamsize TextOutput::xsputn(const char_type* s, std::streamsize n) {
	try {
		buffer.write(s, n);
		return n;
	} catch(...) {
		return 0;
	}
}

std::streambuf::int_type TextOutput::overflow(int_type c) {
	try {
		if(c != traits_type::eof()) {
			*buffer.write(1) = c;
			return c;
		}
	} catch(...) {
		// fall through
	}
	return traits_type::eof();
}

int TextOutput::sync() {
	try {
		buffer.flush();
		return 0;
	} catch(...) {
		return -1;
	}
}


void write(TypeOutput& out, const Value& value, const TypeCode* type_code, const uint16_t* code) {
	value.write(out, type_code, code);
}

void write(TypeOutput& out, const std::nullptr_t& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		Object dummy;
		write(out, dummy, type_code, code);
	} else {
		write_null(out);
	}
}

void write(TypeOutput& out, std::shared_ptr<const Value> value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_NULL) {
		return;
	}
	if(value) {
		value->write(out, nullptr, code);		// type_code is null here since we want to allow polymorphy
	} else {
		write(out, nullptr, type_code, code);
	}
}

void write(TypeOutput& out, const Value& value) {
	write(out, value, nullptr, nullptr);
}

void write(TypeOutput& out, const Variant& value) {
	write(out, value, nullptr, nullptr);
}

void write(TypeOutput& out, const std::nullptr_t& value) {
	write(out, value, nullptr, nullptr);
}

void write(TypeOutput& out, std::shared_ptr<const Value> value) {
	write(out, value, nullptr, nullptr);
}

void write(TypeOutput& out, const std::string& string, const TypeCode* type_code, const uint16_t* code) {
	if(string.size() > VNX_MAX_SIZE) {
		throw std::invalid_argument("write(std::string): size > VNX_MAX_SIZE");
	}
	write(out, uint32_t(string.size()));
	out.write(string.c_str(), string.size());
}

void write_dynamic(TypeOutput& out, const bool& value) {
	char* buf = out.write(4 + 1);
	write_value(buf, (uint16_t)1);
	write_value(buf + 2, (uint16_t)CODE_BOOL);
	write_value(buf + 4, (uint8_t)value);
}

void write_dynamic(TypeOutput& out, const uint8_t& value) {
	char* buf = out.write(4 + 1);
	write_value(buf, (uint16_t)1);
	write_value(buf + 2, (uint16_t)CODE_UINT8);
	write_value(buf + 4, value);
}

void write_dynamic(TypeOutput& out, const uint16_t& value) {
	char* buf = out.write(4 + 2);
	write_value(buf, (uint16_t)1);
	write_value(buf + 2, (uint16_t)CODE_UINT16);
	write_value(buf + 4, value);
}

void write_dynamic(TypeOutput& out, const uint32_t& value) {
	char* buf = out.write(4 + 4);
	write_value(buf, (uint16_t)1);
	write_value(buf + 2, (uint16_t)CODE_UINT32);
	write_value(buf + 4, value);
}

void write_dynamic(TypeOutput& out, const uint64_t& value) {
	char* buf = out.write(4 + 8);
	write_value(buf, (uint16_t)1);
	write_value(buf + 2, (uint16_t)CODE_UINT64);
	write_value(buf + 4, value);
}

void write_dynamic(TypeOutput& out, const char& value) {
	char* buf = out.write(4 + 1);
	write_value(buf, (uint16_t)1);
	write_value(buf + 2, (uint16_t)CODE_INT8);
	write_value(buf + 4, value);
}

void write_dynamic(TypeOutput& out, const int8_t& value) {
	char* buf = out.write(4 + 1);
	write_value(buf, (uint16_t)1);
	write_value(buf + 2, (uint16_t)CODE_INT8);
	write_value(buf + 4, value);
}

void write_dynamic(TypeOutput& out, const int16_t& value) {
	char* buf = out.write(4 + 2);
	write_value(buf, (uint16_t)1);
	write_value(buf + 2, (uint16_t)CODE_INT16);
	write_value(buf + 4, value);
}

void write_dynamic(TypeOutput& out, const int32_t& value) {
	char* buf = out.write(4 + 4);
	write_value(buf, (uint16_t)1);
	write_value(buf + 2, (uint16_t)CODE_INT32);
	write_value(buf + 4, value);
}

void write_dynamic(TypeOutput& out, const int64_t& value) {
	char* buf = out.write(4 + 8);
	write_value(buf, (uint16_t)1);
	write_value(buf + 2, (uint16_t)CODE_INT64);
	write_value(buf + 4, value);
}

void write_dynamic(TypeOutput& out, const float32_t& value) {
	char* buf = out.write(4 + 4);
	write_value(buf, (uint16_t)1);
	write_value(buf + 2, (uint16_t)CODE_FLOAT);
	write_value(buf + 4, value);
}

void write_dynamic(TypeOutput& out, const float64_t& value) {
	char* buf = out.write(4 + 8);
	write_value(buf, (uint16_t)1);
	write_value(buf + 2, (uint16_t)CODE_DOUBLE);
	write_value(buf + 4, value);
}

#ifdef __APPLE__
void write_dynamic(TypeOutput& out, const size_t& value) {
	if(sizeof(value) == 4) {
		write_dynamic(out, uint32_t(value));
	}
	if(sizeof(value) == 8) {
		write_dynamic(out, uint64_t(value));
	}
}

void write_dynamic(TypeOutput& out, const ssize_t& value) {
	if(sizeof(value) == 4) {
		write_dynamic(out, int32_t(value));
	}
	if(sizeof(value) == 8) {
		write_dynamic(out, int64_t(value));
	}
}
#endif

void write_dynamic_null(TypeOutput& out) {
	char* buf = out.write(4);
	write_value(buf, (uint16_t)1);
	write_value(buf + 2, (uint16_t)CODE_NULL);
}

void write_padding(TypeOutput& out, size_t size) {
	if(size < 6) {
		throw std::runtime_error("write_padding(): size < 6");
	} else if(size > VNX_MAX_SIZE) {
		throw std::runtime_error("write_padding(): size > VNX_MAX_SIZE");
	}
	size -= 6;
	write(out, uint16_t(CODE_PADDING));
	write(out, uint32_t(size));
	char buf[1024] = {};
	while(size > 0) {
		size_t len = sizeof(buf);
		if(len > size) {
			len = size;
		}
		out.write(buf, len);
		size -= len;
	}
}

void write_byte_code(TypeOutput& out, const uint16_t* code) {
	const uint16_t* end = validate_code(code);
	write_byte_code(out, code, 1 + (end - code));
}

void write_byte_code(TypeOutput& out, const uint16_t* code, size_t code_size) {
	if(code_size > VNX_MAX_BYTE_CODE_SIZE) {
		throw std::runtime_error("write_byte_code(): code_size > VNX_MAX_BYTE_CODE_SIZE");
	}
	write(out, uint16_t(code_size));
	out.write(code, code_size * sizeof(uint16_t));
}

void write_class_header(TypeOutput& out, const TypeCode* type_code) {
	char* const buf = out.write(10);
	write_value(buf, uint16_t(CODE_TYPE));
	write_value(buf + 2, uint64_t(type_code->code_hash));
}

template<typename T>
void write_float(std::ostream& out, const T& value) {
	if(std::isnan(value) || std::isinf(value)) {
		vnx::write(out, nullptr);
	} else {
		out << std::to_string(value);
	}
}

void write(std::ostream& out, const float32_t& value) {
	write_float<float32_t>(out, value);
}

void write(std::ostream& out, const float64_t& value) {
	write_float<float64_t>(out, value);
}

void write(std::ostream& out, const std::string& value) {
	out << '"';
	for(const char c : value) {
		switch(c) {
			case '\t':
				out << "\\t";
				continue;
			case '\r':
				out << "\\r";
				continue;
			case '\n':
				out << "\\n";
				continue;
			case '\b':
				out << "\\b";
				continue;
			case '\f':
				out << "\\f";
				continue;
			case '"':
			case '\\':
				out << '\\';
				break;
		}
		out << c;
	}
	out << '"';
}

void write(std::ostream& out, const vnx::Value& value) {
	value.write(out);
}

void write(std::ostream& out, const std::nullptr_t& value) {
	out << "null";
}

template<>
void write(TypeOutput& out, const std::vector<bool>& vector, const TypeCode* type_code, const uint16_t* code) {
	std::vector<uint8_t> tmp(vector.size());
	for(size_t i = 0; i < vector.size(); ++i) {
		tmp[i] = vector[i];
	}
	write(out, tmp, type_code, code);
}

template<>
void write(std::ostream& out, const std::vector<bool>& vector) {
	out << '[';
	for(size_t i = 0; i < vector.size(); ++i) {
		if(i > 0) {
			out << ", ";
		}
		out << (vector[i] ? "true" : "false");
	}
	out << ']';
}

std::string to_string(TypeInput& in) {
	return to_string(in, nullptr, nullptr);
}

std::string to_string(TypeInput& in, const TypeCode* type_code, const uint16_t* code) {
	std::ostringstream stream;
	DefaultPrinter printer(stream);
	accept(printer, in, type_code, code);
	return stream.str();
}

std::string to_string_value(TypeInput& in) {
	return to_string_value(in, nullptr, nullptr);
}

std::string to_string_value(TypeInput& in, const TypeCode* type_code, const uint16_t* code) {
	std::ostringstream stream;
	ToStringValue visitor(stream);
	accept(visitor, in, type_code, code);
	return stream.str();
}

void write_to_file(const std::string& file_name, const Value& value)
{
	if(file_name.empty()) {
		return;
	}
	size_t slash = file_name.find_last_of("/\\");
	if(slash != std::string::npos){
		std::string dir_name = file_name.substr(0, slash);
		Directory dir(dir_name);
		dir.create();
	}
	const std::string tmp_file_name = file_name + ".tmp";
	{
		File file;
		file.open(tmp_file_name, "wb");
		file.write_header();
		write(file.out, value);
		file.close();
	}
#ifdef _WIN32
	std::remove(file_name.c_str());
#endif
	if(::rename(tmp_file_name.c_str(), file_name.c_str())) {
		throw std::runtime_error("rename('" + tmp_file_name + "', '" + file_name + "') failed with: " + std::string(strerror(errno)));
	}
}

void write_to_file(const std::string& file_name, std::shared_ptr<const Value> value)
{
	if(value) {
		write_to_file(file_name, *value);
	}
}


} // vnx
