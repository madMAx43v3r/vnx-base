
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/ProxyInterface_on_remote_connect.hxx>
#include <vnx/Hash64.hpp>
#include <vnx/ProxyInterface_on_remote_connect_return.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 ProxyInterface_on_remote_connect::VNX_TYPE_HASH(0x58d0fc825fa3d477ull);
const vnx::Hash64 ProxyInterface_on_remote_connect::VNX_CODE_HASH(0x35a35ba812e6b172ull);

vnx::Hash64 ProxyInterface_on_remote_connect::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string ProxyInterface_on_remote_connect::get_type_name() const {
	return "vnx.ProxyInterface.on_remote_connect";
}

const vnx::TypeCode* ProxyInterface_on_remote_connect::get_type_code() const {
	return vnx::vnx_native_type_code_ProxyInterface_on_remote_connect;
}

std::shared_ptr<ProxyInterface_on_remote_connect> ProxyInterface_on_remote_connect::create() {
	return std::make_shared<ProxyInterface_on_remote_connect>();
}

std::shared_ptr<vnx::Value> ProxyInterface_on_remote_connect::clone() const {
	return std::make_shared<ProxyInterface_on_remote_connect>(*this);
}

void ProxyInterface_on_remote_connect::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void ProxyInterface_on_remote_connect::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void ProxyInterface_on_remote_connect::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_ProxyInterface_on_remote_connect;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, process_id);
	_visitor.type_end(*_type_code);
}

void ProxyInterface_on_remote_connect::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.ProxyInterface.on_remote_connect\"";
	_out << ", \"process_id\": "; vnx::write(_out, process_id);
	_out << "}";
}

void ProxyInterface_on_remote_connect::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object ProxyInterface_on_remote_connect::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.ProxyInterface.on_remote_connect";
	_object["process_id"] = process_id;
	return _object;
}

void ProxyInterface_on_remote_connect::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "process_id") {
			_entry.second.to(process_id);
		}
	}
}

vnx::Variant ProxyInterface_on_remote_connect::get_field(const std::string& _name) const {
	if(_name == "process_id") {
		return vnx::Variant(process_id);
	}
	return vnx::Variant();
}

void ProxyInterface_on_remote_connect::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "process_id") {
		_value.to(process_id);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const ProxyInterface_on_remote_connect& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, ProxyInterface_on_remote_connect& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* ProxyInterface_on_remote_connect::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> ProxyInterface_on_remote_connect::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.ProxyInterface.on_remote_connect";
	type_code->type_hash = vnx::Hash64(0x58d0fc825fa3d477ull);
	type_code->code_hash = vnx::Hash64(0x35a35ba812e6b172ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->native_size = sizeof(::vnx::ProxyInterface_on_remote_connect);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<ProxyInterface_on_remote_connect>(); };
	type_code->return_type = ::vnx::ProxyInterface_on_remote_connect_return::static_get_type_code();
	type_code->fields.resize(1);
	{
		auto& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "process_id";
		field.code = {4};
	}
	type_code->permission = "vnx.permission_e.PROXY_INTERNAL";
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::ProxyInterface_on_remote_connect& value, const TypeCode* type_code, const uint16_t* code) {
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
			case 0: vnx::read(in, value.process_id, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::ProxyInterface_on_remote_connect& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_ProxyInterface_on_remote_connect;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::ProxyInterface_on_remote_connect>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.process_id, type_code, type_code->fields[0].code.data());
}

void read(std::istream& in, ::vnx::ProxyInterface_on_remote_connect& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::ProxyInterface_on_remote_connect& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::ProxyInterface_on_remote_connect& value) {
	value.accept(visitor);
}

} // vnx
