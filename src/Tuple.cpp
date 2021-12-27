/*
 * Tuple.cpp
 *
 *  Created on: Dec 15, 2020
 *      Author: mad
 */

#include <vnx/Tuple.hxx>


namespace vnx {

Variant Tuple::get_field_by_index(const uint32_t& index) const {
	if(index < fields.size()) {
		return fields[index];
	}
	return Variant();
}

void Tuple::set_field_by_index(const uint32_t& index, const Variant& value) {
	if(index >= fields.size()) {
		fields.resize(index + 1);
	}
	fields[index] = value;
}

uint32_t Tuple::get_num_fields() const {
	return fields.size();
}


} // vnx
