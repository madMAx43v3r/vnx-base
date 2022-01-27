
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/JRPC_Notification.hxx>
#include <vnx/Value.h>
#include <vnx/Variant.hpp>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 JRPC_Notification::VNX_TYPE_HASH(0xace27e598a6b5256ull);
const vnx::Hash64 JRPC_Notification::VNX_CODE_HASH(0xbd56b9dfe5f535dcull);

vnx::Hash64 JRPC_Notification::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string JRPC_Notification::get_type_name() const {
	return "vnx.JRPC_Notification";
}

const vnx::TypeCode* JRPC_Notification::get_type_code() const {
	return vnx::vnx_native_type_code_JRPC_Notification;
}

std::shared_ptr<JRPC_Notification> JRPC_Notification::create() {
	return std::make_shared<JRPC_Notification>();
}

std::shared_ptr<vnx::Value> JRPC_Notification::clone() const {
	return std::make_shared<JRPC_Notification>(*this);
}

void JRPC_Notification::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void JRPC_Notification::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void JRPC_Notification::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_JRPC_Notification;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, jsonrpc);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, method);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, params);
	_visitor.type_end(*_type_code);
}

void JRPC_Notification::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.JRPC_Notification\"";
	_out << ", \"jsonrpc\": "; vnx::write(_out, jsonrpc);
	_out << ", \"method\": "; vnx::write(_out, method);
	_out << ", \"params\": "; vnx::write(_out, params);
	_out << "}";
}

void JRPC_Notification::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object JRPC_Notification::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.JRPC_Notification";
	_object["jsonrpc"] = jsonrpc;
	_object["method"] = method;
	_object["params"] = params;
	return _object;
}

void JRPC_Notification::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "jsonrpc") {
			_entry.second.to(jsonrpc);
		} else if(_entry.first == "method") {
			_entry.second.to(method);
		} else if(_entry.first == "params") {
			_entry.second.to(params);
		}
	}
}

vnx::Variant JRPC_Notification::get_field(const std::string& _name) const {
	if(_name == "jsonrpc") {
		return vnx::Variant(jsonrpc);
	}
	if(_name == "method") {
		return vnx::Variant(method);
	}
	if(_name == "params") {
		return vnx::Variant(params);
	}
	return vnx::Variant();
}

void JRPC_Notification::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "jsonrpc") {
		_value.to(jsonrpc);
	} else if(_name == "method") {
		_value.to(method);
	} else if(_name == "params") {
		_value.to(params);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const JRPC_Notification& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, JRPC_Notification& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* JRPC_Notification::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> JRPC_Notification::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.JRPC_Notification";
	type_code->type_hash = vnx::Hash64(0xace27e598a6b5256ull);
	type_code->code_hash = vnx::Hash64(0xbd56b9dfe5f535dcull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->native_size = sizeof(::vnx::JRPC_Notification);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<JRPC_Notification>(); };
	type_code->fields.resize(3);
	{
		auto& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "jsonrpc";
		field.code = {32};
	}
	{
		auto& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "method";
		field.code = {32};
	}
	{
		auto& field = type_code->fields[2];
		field.is_extended = true;
		field.name = "params";
		field.code = {17};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::JRPC_Notification& value, const TypeCode* type_code, const uint16_t* code) {
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
			case 0: vnx::read(in, value.jsonrpc, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.method, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.params, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::JRPC_Notification& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_JRPC_Notification;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::JRPC_Notification>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.jsonrpc, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.method, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.params, type_code, type_code->fields[2].code.data());
}

void read(std::istream& in, ::vnx::JRPC_Notification& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::JRPC_Notification& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::JRPC_Notification& value) {
	value.accept(visitor);
}

} // vnx
