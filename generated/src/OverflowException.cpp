
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/OverflowException.hxx>
#include <vnx/Exception.hxx>
#include <vnx/Hash64.hpp>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 OverflowException::VNX_TYPE_HASH(0xba120fc8b6c0e004ull);
const vnx::Hash64 OverflowException::VNX_CODE_HASH(0xd54e083b4364da6eull);

vnx::Hash64 OverflowException::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string OverflowException::get_type_name() const {
	return "vnx.OverflowException";
}

const vnx::TypeCode* OverflowException::get_type_code() const {
	return vnx::vnx_native_type_code_OverflowException;
}

std::shared_ptr<OverflowException> OverflowException::create() {
	return std::make_shared<OverflowException>();
}

std::shared_ptr<vnx::Value> OverflowException::clone() const {
	return std::make_shared<OverflowException>(*this);
}

void OverflowException::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void OverflowException::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void OverflowException::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_OverflowException;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, what);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, dst_mac);
	_visitor.type_end(*_type_code);
}

void OverflowException::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.OverflowException\"";
	_out << ", \"what\": "; vnx::write(_out, what);
	_out << ", \"dst_mac\": "; vnx::write(_out, dst_mac);
	_out << "}";
}

void OverflowException::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object OverflowException::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.OverflowException";
	_object["what"] = what;
	_object["dst_mac"] = dst_mac;
	return _object;
}

void OverflowException::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "dst_mac") {
			_entry.second.to(dst_mac);
		} else if(_entry.first == "what") {
			_entry.second.to(what);
		}
	}
}

vnx::Variant OverflowException::get_field(const std::string& _name) const {
	if(_name == "what") {
		return vnx::Variant(what);
	}
	if(_name == "dst_mac") {
		return vnx::Variant(dst_mac);
	}
	return vnx::Variant();
}

void OverflowException::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "what") {
		_value.to(what);
	} else if(_name == "dst_mac") {
		_value.to(dst_mac);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const OverflowException& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, OverflowException& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* OverflowException::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> OverflowException::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.OverflowException";
	type_code->type_hash = vnx::Hash64(0xba120fc8b6c0e004ull);
	type_code->code_hash = vnx::Hash64(0xd54e083b4364da6eull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->native_size = sizeof(::vnx::OverflowException);
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::Exception::static_get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<OverflowException>(); };
	type_code->fields.resize(2);
	{
		auto& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "what";
		field.code = {32};
	}
	{
		auto& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "dst_mac";
		field.code = {4};
	}
	type_code->build();
	return type_code;
}

std::shared_ptr<vnx::Value> OverflowException::vnx_call_switch(std::shared_ptr<const vnx::Value> _method) {
	switch(_method->get_type_hash()) {
	}
	return nullptr;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::OverflowException& value, const TypeCode* type_code, const uint16_t* code) {
	TypeInput::recursion_t tag(in);
	if(code) {
		switch(code[0]) {
			case CODE_OBJECT:
			case CODE_ALT_OBJECT: {
				Object tmp;
				vnx::read(in, tmp, type_code, code);
				value.from_object(tmp);
				return;
			}
			case CODE_DYNAMIC:
			case CODE_ALT_DYNAMIC:
				vnx::read_dynamic(in, value);
				return;
		}
	}
	if(!type_code) {
		vnx::skip(in, type_code, code);
		return;
	}
	if(code) {
		switch(code[0]) {
			case CODE_STRUCT: type_code = type_code->depends[code[1]]; break;
			case CODE_ALT_STRUCT: type_code = type_code->depends[vnx::flip_bytes(code[1])]; break;
			default: {
				vnx::skip(in, type_code, code);
				return;
			}
		}
	}
	in.read(type_code->total_field_size);
	if(type_code->is_matched) {
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.what, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.dst_mac, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::OverflowException& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_OverflowException;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::OverflowException>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.what, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.dst_mac, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::OverflowException& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::OverflowException& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::OverflowException& value) {
	value.accept(visitor);
}

} // vnx
