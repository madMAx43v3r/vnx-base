
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/Generic.hxx>
#include <vnx/Generic_from_data_return.hxx>
#include <vnx/Value.h>
#include <vnx/Variant.hpp>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 Generic::VNX_TYPE_HASH(0x18b78df58e8b9cfeull);
const vnx::Hash64 Generic::VNX_CODE_HASH(0xb9891f2cb0adf041ull);

vnx::Hash64 Generic::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string Generic::get_type_name() const {
	return "vnx.Generic";
}

const vnx::TypeCode* Generic::get_type_code() const {
	return vnx::vnx_native_type_code_Generic;
}

std::shared_ptr<Generic> Generic::create() {
	return std::make_shared<Generic>();
}

std::shared_ptr<vnx::Value> Generic::clone() const {
	return std::make_shared<Generic>(*this);
}

void Generic::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void Generic::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void Generic::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_Generic;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, data);
	_visitor.type_end(*_type_code);
}

void Generic::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.Generic\"";
	_out << ", \"data\": "; vnx::write(_out, data);
	_out << "}";
}

void Generic::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object Generic::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.Generic";
	_object["data"] = data;
	return _object;
}

void Generic::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "data") {
			_entry.second.to(data);
		}
	}
}

vnx::Variant Generic::get_field(const std::string& _name) const {
	if(_name == "data") {
		return vnx::Variant(data);
	}
	return vnx::Variant();
}

void Generic::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "data") {
		_value.to(data);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const Generic& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, Generic& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* Generic::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> Generic::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.Generic";
	type_code->type_hash = vnx::Hash64(0x18b78df58e8b9cfeull);
	type_code->code_hash = vnx::Hash64(0xb9891f2cb0adf041ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->native_size = sizeof(::vnx::Generic);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<Generic>(); };
	type_code->methods.resize(0);
	type_code->fields.resize(1);
	{
		auto& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "data";
		field.code = {17};
	}
	type_code->build();
	return type_code;
}

std::shared_ptr<vnx::Value> Generic::vnx_call_switch(std::shared_ptr<const vnx::Value> _method) {
	switch(_method->get_type_hash()) {
	}
	return nullptr;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::Generic& value, const TypeCode* type_code, const uint16_t* code) {
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
			case 0: vnx::read(in, value.data, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::Generic& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_Generic;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::Generic>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.data, type_code, type_code->fields[0].code.data());
}

void read(std::istream& in, ::vnx::Generic& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::Generic& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::Generic& value) {
	value.accept(visitor);
}

} // vnx
