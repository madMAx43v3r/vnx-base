/*
 * ToObject.cpp
 *
 *  Created on: Dec 16, 2020
 *      Author: mad
 */

#include <vnx/vnx.h>
#include <vnx/ToObject.h>


namespace vnx {

ToObject::ToObject() {
	stack.emplace_back();
}

Object ToObject::get_object() const {
	if(!stack.empty()) {
		return stack.back().object;
	}
	return Object();
}

Variant ToObject::get_value() const {
	if(!stack.empty()) {
		return stack.back().value;
	}
	return Variant();
}


void ToObject::visit_null() {
	set_value(nullptr);
}

void ToObject::visit(const bool& value) {
	set_value(value);
}

void ToObject::visit(const uint8_t& value) {
	set_value(value);
}

void ToObject::visit(const uint16_t& value) {
	set_value(value);
}

void ToObject::visit(const uint32_t& value) {
	set_value(value);
}

void ToObject::visit(const uint64_t& value) {
	set_value(value);
}

void ToObject::visit(const int8_t& value) {
	set_value(value);
}

void ToObject::visit(const int16_t& value) {
	set_value(value);
}

void ToObject::visit(const int32_t& value) {
	set_value(value);
}

void ToObject::visit(const int64_t& value) {
	set_value(value);
}

void ToObject::visit(const float32_t& value) {
	set_value(value);
}

void ToObject::visit(const float64_t& value) {
	set_value(value);
}

void ToObject::visit(const std::string& value) {
	set_value(value);
}


void ToObject::list_begin(size_t size) {
	stack.emplace_back(size);
}

void ToObject::list_element(size_t index) {
	auto& entry = stack.back();
	if(index) {
		entry.array[index - 1] = entry.value;
	}
}

void ToObject::list_end(size_t size) {
	auto& entry = stack.back();
	if(size) {
		entry.array[size - 1] = entry.value;
	}
	Variant value(entry.array);
	stack.pop_back();
	stack.back().value = std::move(value);
}

void ToObject::type_begin(size_t num_fields, const std::string* type_name) {
	// nothing to do
}

void ToObject::type_field(const std::string& field, size_t index) {
	if(index) {
		const auto entry = std::move(stack.back());
		stack.pop_back();
		stack.back().object[entry.field] = entry.value;
	}
	stack.emplace_back(field);
}

void ToObject::type_end(size_t num_fields, const std::string* type_name) {
	if(num_fields) {
		const auto entry = std::move(stack.back());
		stack.pop_back();
		stack.back().object[entry.field] = entry.value;
	}
	auto& entry = stack.back();
	entry.value = entry.object;
}


template<typename T>
void ToObject::set_value(const T& value) {
	auto& entry = stack.back();
	entry.value = value;
}


} // vnx
