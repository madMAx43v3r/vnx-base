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
#include <vnx/Visitor.h>
#include <vnx/ToStringValue.h>
#include <vnx/DefaultPrinter.h>


namespace vnx {

void Visitor::enum_value(uint32_t value, const std::string& name) {
	if(!name.empty()) {
		visit(name);
	} else {
		visit(value);
	}
}

void Visitor::map_begin(size_t size) {
	list_begin(size);
}

void Visitor::map_key(size_t index) {
	if(index) {
		list_end(2);
	}
	list_element(index);
	list_begin(2);
	list_element(0);
}

void Visitor::map_value(size_t index) {
	list_element(1);
}

void Visitor::map_end(size_t size) {
	if(size) {
		list_end(2);
	}
	list_end(size);
}

void Visitor::type_begin(size_t num_fields, const std::string* type_name) {
	map_begin(num_fields);
}

void Visitor::type_field(const std::string& field, size_t index) {
	map_key(index);
	visit(field);
	map_value(index);
}

void Visitor::type_end(size_t num_fields, const std::string* type_name) {
	map_end(num_fields);
}

void Visitor::type_begin(const TypeCode& type) {
	type_begin(type.fields.size(), &type.name);
}

void Visitor::type_field(const TypeField& field, size_t index) {
	type_field(field.name, index);
}

void Visitor::type_end(const TypeCode& type) {
	type_end(type.fields.size(), &type.name);
}


ToStringValue::ToStringValue(std::ostream& out_)
	:	DefaultPrinter(out_)
{
}

void ToStringValue::visit_null() {
	if(stack == 0) {
		// nothing
	} else {
		DefaultPrinter::visit_null();
	}
}

void ToStringValue::visit(const std::string& value) {
	if(stack == 0) {
		out << value;
	} else {
		DefaultPrinter::visit(value);
	}
}


DefaultPrinter::DefaultPrinter(std::ostream& out)
	:	out(out)
{
}

void DefaultPrinter::visit_null() { out << "null"; }
void DefaultPrinter::visit(const bool& value) { write(out, value); }
void DefaultPrinter::visit(const uint8_t& value) { write(out, value); }
void DefaultPrinter::visit(const uint16_t& value) { write(out, value); }
void DefaultPrinter::visit(const uint32_t& value) { write(out, value); }
void DefaultPrinter::visit(const uint64_t& value) { write(out, value); }
void DefaultPrinter::visit(const int8_t& value) { write(out, value); }
void DefaultPrinter::visit(const int16_t& value) { write(out, value); }
void DefaultPrinter::visit(const int32_t& value) { write(out, value); }
void DefaultPrinter::visit(const int64_t& value) { write(out, value); }
void DefaultPrinter::visit(const float32_t& value) { write(out, value); }
void DefaultPrinter::visit(const float64_t& value) { write(out, value); }
void DefaultPrinter::visit(const std::string& value) { write(out, value); }

void DefaultPrinter::list_begin(size_t size) {
	out << '[';
	stack++;
}

void DefaultPrinter::list_element(size_t index) {
	if(index > 0) {
		out << ", ";
	}
}

void DefaultPrinter::list_end(size_t size) {
	out << ']';
	stack--;
}

void DefaultPrinter::type_begin(size_t num_fields, const std::string* type_name) {
	out << '{';
	stack++;
	if(type_name) {
		type_field("__type", 0);
		visit(*type_name);
		if(num_fields) {
			out << ", ";
		}
	}
}

void DefaultPrinter::type_field(const std::string& field, size_t index) {
	if(index > 0) {
		out << ", ";
	}
	out << '"' << field << "\": ";
}

void DefaultPrinter::type_end(size_t num_fields, const std::string* type_name) {
	out << '}';
	stack--;
}


} // vnx
