/*
 * PrettyPrinter.cpp
 *
 *  Created on: Dec 16, 2020
 *      Author: mad
 */

#include <vnx/vnx.h>
#include <vnx/PrettyPrinter.h>


namespace vnx {

PrettyPrinter::PrettyPrinter(std::ostream& out)
	:	out(&out)
{
	binary_hexstr = true;
}

void PrettyPrinter::visit_null() { if(out) *out << "null"; }
void PrettyPrinter::visit(const bool& value) { if(out) write(*out, value); }
void PrettyPrinter::visit(const uint8_t& value) { if(out) write(*out, value); }
void PrettyPrinter::visit(const uint16_t& value) { if(out) write(*out, value); }
void PrettyPrinter::visit(const uint32_t& value) { if(out) write(*out, value); }
void PrettyPrinter::visit(const uint64_t& value) { if(out) write(*out, value); }
void PrettyPrinter::visit(const int8_t& value) { if(out) write(*out, value); }
void PrettyPrinter::visit(const int16_t& value) { if(out) write(*out, value); }
void PrettyPrinter::visit(const int32_t& value) { if(out) write(*out, value); }
void PrettyPrinter::visit(const int64_t& value) { if(out) write(*out, value); }
void PrettyPrinter::visit(const float32_t& value) { if(out) write(*out, value); }
void PrettyPrinter::visit(const float64_t& value) { if(out) write(*out, value); }

void PrettyPrinter::visit(const std::string& value) {
	if(out) write(*out, value);
}

void PrettyPrinter::list_begin(size_t size) {
	stack++;
	if(out) *out << '[';
}

void PrettyPrinter::list_element(size_t index) {
	if(index > 0) {
		if(out) *out << ", ";
	}
	if(out && max_size && index == max_size) {
		*out << "...";
		tmp = out;
		out = 0;
		resume = stack;
	}
}

void PrettyPrinter::list_end(size_t size) {
	if(tmp && stack == resume) {
		out = tmp;
		tmp = 0;
		resume = 0;
	}
	stack--;
	if(out) *out << ']';
}

void PrettyPrinter::type_begin(size_t num_fields, const std::string* type_name) {
	stack++;
	if(out) *out << '{';
	if(type_name) {
		type_field("__type", 0);
		visit(*type_name);
		if(num_fields) {
			if(out) { *out << ','; }
		}
	}
}

void PrettyPrinter::type_field(const std::string& field, size_t index) {
	if(out) {
		if(index > 0) {
			*out << ',';
		}
		*out << std::endl;
		add_indentation();
		*out << '"' << field << "\": ";
	}
}

void PrettyPrinter::type_end(size_t num_fields, const std::string* type_name) {
	stack--;
	if(out) {
		if(num_fields > 0 || type_name) {
			*out << std::endl;
			add_indentation();
		}
		*out << '}';
	}
}

void PrettyPrinter::add_indentation() {
	if(out) {
		for(size_t i = 0; i < stack; ++i) {
			*out << "  ";
		}
	}
}


} // vnx
