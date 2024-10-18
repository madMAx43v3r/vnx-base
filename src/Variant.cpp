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
#include <vnx/Variant.hpp>

#include <cctype>


namespace vnx {

const Variant Variant::empty_instance;

Variant& Variant::assign(const char* str) {
	clear();
	VectorOutputStream stream(&data);
	TypeOutput out(&stream);
	vnx::write_dynamic(out, std::string(str));
	out.flush();
	return *this;
}

Variant& Variant::assign(const char* str, size_t len) {
	clear();
	VectorOutputStream stream(&data);
	TypeOutput out(&stream);
	vnx::write_dynamic_list<int8_t>(out, (const int8_t*)str, len);
	out.flush();
	return *this;
}

const uint16_t* Variant::get_code() const {
	uint16_t dummy;
	return get_code(dummy);
}

const uint16_t* Variant::get_code(uint16_t& code_size) const {
	if(data.size() >= 2 * sizeof(uint16_t)) {
		const auto* code = (const uint16_t*)data.data();
		code_size = code[0];
		return code + 1;
	}
	code_size = 0;
	return nullptr;
}

bool Variant::is_null() const {
	const uint16_t* code = get_code();
	return !code || code[0] == CODE_NULL;
}

bool Variant::is_bool() const {
	const uint16_t* code = get_code();
	if(code) {
		switch(code[0]) {
			case CODE_BOOL:
			case CODE_ALT_BOOL:
				return true;
		}
	}
	return false;
}

bool Variant::is_ulong() const {
	const uint16_t* code = get_code();
	if(code) {
		switch(code[0]) {
			case CODE_UINT8:
			case CODE_UINT16:
			case CODE_UINT32:
			case CODE_UINT64:
			case CODE_ALT_UINT8:
			case CODE_ALT_UINT16:
			case CODE_ALT_UINT32:
			case CODE_ALT_UINT64:
				return true;
		}
	}
	return false;
}

bool Variant::is_long() const {
	const uint16_t* code = get_code();
	if(code) {
		switch(code[0]) {
			case CODE_UINT8:
			case CODE_UINT16:
			case CODE_UINT32:
			case CODE_INT8:
			case CODE_INT16:
			case CODE_INT32:
			case CODE_INT64:
			case CODE_ALT_UINT8:
			case CODE_ALT_UINT16:
			case CODE_ALT_UINT32:
			case CODE_ALT_INT8:
			case CODE_ALT_INT16:
			case CODE_ALT_INT32:
			case CODE_ALT_INT64:
				return true;
			case CODE_UINT64:
			case CODE_ALT_UINT64:
				return (to<uint64_t>() >> 63) == 0;
		}
	}
	return false;
}

bool Variant::is_double() const {
	const uint16_t* code = get_code();
	if(code) {
		switch(code[0]) {
			case CODE_FLOAT:
			case CODE_DOUBLE:
			case CODE_ALT_FLOAT:
			case CODE_ALT_DOUBLE:
				return true;
		}
	}
	return false;
}

bool Variant::is_integral() const {
	const uint16_t* code = get_code();
	if(code) {
		switch(code[0]) {
			case CODE_BOOL:
			case CODE_UINT8:
			case CODE_UINT16:
			case CODE_UINT32:
			case CODE_UINT64:
			case CODE_INT8:
			case CODE_INT16:
			case CODE_INT32:
			case CODE_INT64:
			case CODE_FLOAT:
			case CODE_DOUBLE:
			case CODE_ALT_BOOL:
			case CODE_ALT_UINT8:
			case CODE_ALT_UINT16:
			case CODE_ALT_UINT32:
			case CODE_ALT_UINT64:
			case CODE_ALT_INT8:
			case CODE_ALT_INT16:
			case CODE_ALT_INT32:
			case CODE_ALT_INT64:
			case CODE_ALT_FLOAT:
			case CODE_ALT_DOUBLE:
				return true;
		}
	}
	return false;
}

bool Variant::is_string() const {
	const uint16_t* code = get_code();
	if(code) {
		switch(code[0]) {
			case CODE_LIST:
				return code[1] == CODE_INT8;
			case CODE_ALT_LIST:
				return code[1] == CODE_ALT_INT8;
			case CODE_STRING:
			case CODE_ALT_STRING:
				return true;
		}
	}
	return false;
}

bool Variant::is_array() const {
	const uint16_t* code = get_code();
	if(code) {
		switch(code[0]) {
			case CODE_ARRAY:
			case CODE_LIST:
			case CODE_MATRIX:
			case CODE_ALT_ARRAY:
			case CODE_ALT_LIST:
			case CODE_ALT_MATRIX:
				return true;
		}
	}
	return false;
}

bool Variant::is_map() const {
	const uint16_t* code = get_code();
	if(code) {
		switch(code[0]) {
			case CODE_MAP:
			case CODE_ALT_MAP:
				return true;
		}
	}
	return false;
}

bool Variant::is_object() const {
	const uint16_t* code = get_code();
	if(code) {
		switch(code[0]) {
			case CODE_OBJECT:
			case CODE_ALT_OBJECT:
				return true;
		}
	}
	return false;
}

bool Variant::operator==(const Variant& other) const {
	uint16_t code_size = 0;
	uint16_t code_size_ = 0;
	const uint16_t* code = get_code(code_size);
	const uint16_t* code_ = other.get_code(code_size_);
	if(!code && !code_) {
		return true;
	}
	if(!code || !code_) {
		return false;
	}
	VectorInputStream stream(&data);
	VectorInputStream stream_(&other.data);
	stream.set_input_pos((code_size + 1) * sizeof(uint16_t));
	stream_.set_input_pos((code_size_ + 1) * sizeof(uint16_t));
	TypeInput in(&stream);
	TypeInput in_(&stream_);
	return compare(in, in_, code, code_) == 0;
}

bool Variant::operator!=(const Variant& other) const {
	return !(*this == other);
}

bool Variant::operator<(const Variant& other) const {
	uint16_t code_size = 0;
	uint16_t code_size_ = 0;
	const uint16_t* code = get_code(code_size);
	const uint16_t* code_ = other.get_code(code_size_);
	if(!code && !code_) {
		return false;
	}
	if(!code && code_) {
		return true;
	}
	if(code && !code_) {
		return false;
	}
	VectorInputStream stream(&data);
	VectorInputStream stream_(&other.data);
	stream.set_input_pos((code_size + 1) * sizeof(uint16_t));
	stream_.set_input_pos((code_size_ + 1) * sizeof(uint16_t));
	TypeInput in(&stream);
	TypeInput in_(&stream_);
	return compare(in, in_, code, code_) < 0;
}

bool Variant::operator>(const Variant& other) const {
	return other < *this;
}

bool Variant::operator<=(const Variant& other) const {
	uint16_t code_size = 0;
	uint16_t code_size_ = 0;
	const uint16_t* code = get_code(code_size);
	const uint16_t* code_ = other.get_code(code_size_);
	if(!code && !code_) {
		return true;
	}
	if(!code && code_) {
		return true;
	}
	if(code && !code_) {
		return false;
	}
	VectorInputStream stream(&data);
	VectorInputStream stream_(&other.data);
	stream.set_input_pos((code_size + 1) * sizeof(uint16_t));
	stream_.set_input_pos((code_size_ + 1) * sizeof(uint16_t));
	TypeInput in(&stream);
	TypeInput in_(&stream_);
	return compare(in, in_, code, code_) <= 0;
}

bool Variant::operator>=(const Variant& other) const {
	return other <= *this;
}

Hash64 Variant::get_hash() const {
	uint16_t code_size = 0;
	const uint16_t* code = get_code(code_size);
	if(!code) {
		return Hash64();
	}
	VectorInputStream stream(&data);
	stream.set_input_pos((code_size + 1) * sizeof(uint16_t));
	TypeInput in(&stream);
	return calc_hash(in, code);
}


void read(TypeInput& in, Variant& value, const TypeCode* type_code, const uint16_t* code) {
	if(code) {
		switch(code[0]) {
			case CODE_DYNAMIC:
			case CODE_ALT_DYNAMIC: {
				// if already dynamic, just do a 1-to-1 copy
				value.clear();
				VectorOutputStream stream(&value.data);
				TypeOutput out(&stream);
				copy(in, &out, type_code, code);
				out.flush();
				return;
			}
		}
	}
	ToObject visitor;
	accept(visitor, in, type_code, code);
	value = visitor.get_value();
}

void write(TypeOutput& out, const Variant& value, const TypeCode* type_code, const uint16_t* code) {
	if(!code) {
		write(out, (uint16_t)CODE_DYNAMIC);
	}
	if(value.empty()) {
		write_dynamic_null(out);
	} else {
		out.write(value.data.data(), value.data.size());
	}
}

void read(std::istream& in, Variant& value) {
	if(auto json = read_json(in)) {
		value = json->to_variant();
	} else {
		value = Variant();
	}
}

void write(std::ostream& out, const Variant& value) {
	DefaultPrinter printer(out);
	accept(printer, value);
}

void accept(Visitor& visitor, const Variant& value) {
	if(value.empty()) {
		visitor.visit_null();
	} else {
		VectorInputStream stream(&value.data);
		TypeInput in(&stream);
		const uint16_t code = CODE_DYNAMIC;
		accept(visitor, in, nullptr, &code);
	}
}

template<>
void from_string(const std::string& str, Variant& value) {
	std::istringstream stream;
	stream.str(str);
	if(auto json = read_json(stream, true)) {
		value = json->to_variant();
	} else {
		value.clear();
	}
}


} // vnx
