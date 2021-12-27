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
#include <vnx/Object.hpp>


namespace vnx {

const uint16_t Object::CODE[] = {CODE_MAP, 3, CODE_STRING, CODE_DYNAMIC};
const uint16_t Object::ALT_CODE[] = {CODE_ALT_MAP, 0x300, CODE_ALT_STRING, CODE_ALT_DYNAMIC};

Object::Object(std::initializer_list<std::pair<const std::string, Variant>> entries)
	:	field(entries)
{
}

Object& Object::operator=(std::initializer_list<std::pair<const std::string, Variant>> entries) {
	field.clear();
	return insert(entries);
}

Object& Object::insert(std::initializer_list<std::pair<const std::string, Variant>> entries) {
	field.insert(entries);
	return *this;
}

std::string Object::get_type_name() const {
	const auto type = (*this)["__type"];
	if(!type.empty()) {
		return type.to_string_value();
	}
	return std::string();
}

const TypeCode* Object::get_type_code() const {
	return vnx::get_type_code(get_type_name());
}

std::shared_ptr<Value> Object::to_value(const std::map<std::string, std::string>& type_map) const {
	std::shared_ptr<Value> value;
	auto type_name = get_type_name();
	{
		const auto iter = type_map.find(type_name);
		if(iter != type_map.end()) {
			type_name = iter->second;
		}
	}
	const auto* type_code = vnx::get_type_code(type_name);
	if(type_code) {
		value = type_code->create_value();
		if(value) {
			value->from_object(*this);
		}
	}
	return value;
}

std::ostream& operator<<(std::ostream& out, const Object& value) {
	vnx::write(out, value);
	return out;
}

std::istream& operator>>(std::istream& in, Object& value) {
	vnx::read(in, value);
	return in;
}

void read(TypeInput& in, Object& value, const TypeCode* type_code, const uint16_t* code) {
	switch(code[0]) {
		case CODE_OBJECT:
			read(in, value.field, type_code, Object::CODE);
			break;
		case CODE_ALT_OBJECT:
			read(in, value.field, type_code, Object::ALT_CODE);
			break;
		case CODE_DYNAMIC:
		case CODE_ALT_DYNAMIC: {
			read_dynamic(in, value);
			return;
		}
		default:
			skip(in, type_code, code);
	}
}

void write(TypeOutput& out, const Object& value, const TypeCode* type_code, const uint16_t* code) {
	if(!code || code[0] == CODE_ANY) {
		write(out, uint16_t(CODE_OBJECT));
	}
	write(out, value.field, type_code, Object::CODE);
}

void read(std::istream& in, Object& value) {
	if(auto object = std::dynamic_pointer_cast<JSON_Object>(read_json(in))) {
		value = object->to_object();
	} else {
		value = Object();
	}
}

void write(std::ostream& out, const Object& value) {
	DefaultPrinter printer(out);
	accept(printer, value);
}

void accept(Visitor& visitor, const Object& value) {
	visitor.type_begin(value.field.size());
	size_t i = 0;
	for(const auto& field : value.field) {
		visitor.type_field(field.first, i++);
		accept(visitor, field.second);
	}
	visitor.type_end(value.field.size());
}


} // vnx
