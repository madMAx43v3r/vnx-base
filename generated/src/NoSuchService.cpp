
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/NoSuchService.hxx>
#include <vnx/Exception.hxx>
#include <vnx/Hash64.hpp>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 NoSuchService::VNX_TYPE_HASH(0x2e35fc01cf168b79ull);
const vnx::Hash64 NoSuchService::VNX_CODE_HASH(0x76a2fa845aacc576ull);

vnx::Hash64 NoSuchService::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string NoSuchService::get_type_name() const {
	return "vnx.NoSuchService";
}

const vnx::TypeCode* NoSuchService::get_type_code() const {
	return vnx::vnx_native_type_code_NoSuchService;
}

std::shared_ptr<NoSuchService> NoSuchService::create() {
	return std::make_shared<NoSuchService>();
}

std::shared_ptr<vnx::Value> NoSuchService::clone() const {
	return std::make_shared<NoSuchService>(*this);
}

void NoSuchService::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void NoSuchService::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void NoSuchService::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_NoSuchService;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, what);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, dst_mac);
	_visitor.type_end(*_type_code);
}

void NoSuchService::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.NoSuchService\"";
	_out << ", \"what\": "; vnx::write(_out, what);
	_out << ", \"dst_mac\": "; vnx::write(_out, dst_mac);
	_out << "}";
}

void NoSuchService::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object NoSuchService::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.NoSuchService";
	_object["what"] = what;
	_object["dst_mac"] = dst_mac;
	return _object;
}

void NoSuchService::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "dst_mac") {
			_entry.second.to(dst_mac);
		} else if(_entry.first == "what") {
			_entry.second.to(what);
		}
	}
}

vnx::Variant NoSuchService::get_field(const std::string& _name) const {
	if(_name == "what") {
		return vnx::Variant(what);
	}
	if(_name == "dst_mac") {
		return vnx::Variant(dst_mac);
	}
	return vnx::Variant();
}

void NoSuchService::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "what") {
		_value.to(what);
	} else if(_name == "dst_mac") {
		_value.to(dst_mac);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const NoSuchService& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, NoSuchService& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* NoSuchService::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> NoSuchService::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.NoSuchService";
	type_code->type_hash = vnx::Hash64(0x2e35fc01cf168b79ull);
	type_code->code_hash = vnx::Hash64(0x76a2fa845aacc576ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->native_size = sizeof(::vnx::NoSuchService);
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::Exception::static_get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<NoSuchService>(); };
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

std::shared_ptr<vnx::Value> NoSuchService::vnx_call_switch(std::shared_ptr<const vnx::Value> _method) {
	switch(_method->get_type_hash()) {
	}
	return nullptr;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::NoSuchService& value, const TypeCode* type_code, const uint16_t* code) {
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

void write(TypeOutput& out, const ::vnx::NoSuchService& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_NoSuchService;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::NoSuchService>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.what, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.dst_mac, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::NoSuchService& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::NoSuchService& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::NoSuchService& value) {
	value.accept(visitor);
}

} // vnx
