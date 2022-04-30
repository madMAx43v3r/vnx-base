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
#include <vnx/Binary.h>


namespace vnx {

const Hash64 Binary::VNX_TYPE_HASH(0x45d0ffa41b4b394eull);
const Hash64 Binary::VNX_CODE_HASH(0x16fa01b1210334edull);

Binary::Binary() {}

Binary::~Binary() {
	clear();
}

Hash64 Binary::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string Binary::get_type_name() const {
	return type_code ? type_code->name : "vnx.Binary";
}

const TypeCode* Binary::get_type_code() const {
	return type_code;
}

std::shared_ptr<Binary> Binary::create() {
	return std::make_shared<Binary>();
}

std::shared_ptr<Value> Binary::clone() const {
	auto copy = std::make_shared<Binary>();
	MemoryOutputStream stream(&copy->data);
	data.write(stream);
	return copy;
}

void Binary::read(TypeInput& in, const TypeCode* type_code, const uint16_t* code) {
	vnx::read(in, *this, type_code, code);
}

void Binary::write(TypeOutput& out, const TypeCode* type_code, const uint16_t* code) const {
	vnx::write(out, *this, type_code, code);
}

void Binary::write(std::ostream& out) const {
	DefaultPrinter printer(out);
	accept(printer);
}

void Binary::read(std::istream& in) {
	throw std::logic_error("Binary::read(std::istream&) not supported");
}

void Binary::accept(Visitor& visitor) const {
	BinaryInputStream stream(this);
	TypeInput in(&stream);
	vnx::accept(visitor, in, type_code);
}

Object Binary::to_object() const {
	ToObject visitor;
	accept(visitor);
	return visitor.get_object();
}

void Binary::from_object(const Object& object) {
	clear();
	if(auto value = object.to_value()) {
		type_code = value->get_type_code();
		if(type_code) {
			BinaryOutputStream stream(this);
			TypeOutput out(&stream);
			value->write(out, type_code, nullptr);
			out.flush();
		}
	}
}

Variant Binary::get_field(const std::string& name) const {
	return to_object()[name];
}

void Binary::set_field(const std::string& name, const Variant& value) {
	// not supported
}

void Binary::clear() {
	type_code = nullptr;
	data.clear();
}


void read(TypeInput& in, vnx::Binary& binary, const TypeCode* type_code, const uint16_t* code) {
	binary.clear();
	if(!type_code) {
		throw std::logic_error("read(vnx::Binary): !type_code");
	}
	binary.type_code = type_code;
	BinaryOutputStream stream(&binary);
	TypeOutput out(&stream);
	vnx::copy(in, &out, type_code, code);
	out.flush();
}

void write(TypeOutput& out, const vnx::Binary& binary, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, binary.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		if(!binary.type_code) {
			throw std::logic_error("write(vnx::Binary): !binary.type_code");
		}
		out.write_type_code(binary.type_code);
		vnx::write_class_header(out, binary.type_code);
	}
	binary.data.write(out);
}

void write(std::ostream& out, const vnx::Binary& binary) {
	binary.write(out);
}

void accept(Visitor& visitor, const vnx::Binary& binary) {
	binary.accept(visitor);
}


} // vnx
