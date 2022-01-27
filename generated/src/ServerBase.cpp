
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/ServerBase.hxx>
#include <vnx/NoSuchMethod.hxx>
#include <vnx/BaseServer.h>
#include <vnx/ModuleInterface_vnx_get_config.hxx>
#include <vnx/ModuleInterface_vnx_get_config_return.hxx>
#include <vnx/ModuleInterface_vnx_get_config_object.hxx>
#include <vnx/ModuleInterface_vnx_get_config_object_return.hxx>
#include <vnx/ModuleInterface_vnx_get_module_info.hxx>
#include <vnx/ModuleInterface_vnx_get_module_info_return.hxx>
#include <vnx/ModuleInterface_vnx_get_type_code.hxx>
#include <vnx/ModuleInterface_vnx_get_type_code_return.hxx>
#include <vnx/ModuleInterface_vnx_restart.hxx>
#include <vnx/ModuleInterface_vnx_restart_return.hxx>
#include <vnx/ModuleInterface_vnx_self_test.hxx>
#include <vnx/ModuleInterface_vnx_self_test_return.hxx>
#include <vnx/ModuleInterface_vnx_set_config.hxx>
#include <vnx/ModuleInterface_vnx_set_config_return.hxx>
#include <vnx/ModuleInterface_vnx_set_config_object.hxx>
#include <vnx/ModuleInterface_vnx_set_config_object_return.hxx>
#include <vnx/ModuleInterface_vnx_stop.hxx>
#include <vnx/ModuleInterface_vnx_stop_return.hxx>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 ServerBase::VNX_TYPE_HASH(0x150d7ad1f906225aull);
const vnx::Hash64 ServerBase::VNX_CODE_HASH(0x83b35d2aab671ff7ull);

ServerBase::ServerBase(const std::string& _vnx_name)
	:	BaseServer::BaseServer(_vnx_name)
{
}

vnx::Hash64 ServerBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string ServerBase::get_type_name() const {
	return "vnx.Server";
}

const vnx::TypeCode* ServerBase::get_type_code() const {
	return vnx::vnx_native_type_code_ServerBase;
}

void ServerBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_ServerBase;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, address);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, allow_login);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, use_authentication);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, export_list);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, max_queue_ms);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, max_queue_size);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, recv_buffer_size);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, send_buffer_size);
	_visitor.type_field(_type_code->fields[8], 8); vnx::accept(_visitor, default_access);
	_visitor.type_end(*_type_code);
}

void ServerBase::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"address\": "; vnx::write(_out, address);
	_out << ", \"allow_login\": "; vnx::write(_out, allow_login);
	_out << ", \"use_authentication\": "; vnx::write(_out, use_authentication);
	_out << ", \"export_list\": "; vnx::write(_out, export_list);
	_out << ", \"max_queue_ms\": "; vnx::write(_out, max_queue_ms);
	_out << ", \"max_queue_size\": "; vnx::write(_out, max_queue_size);
	_out << ", \"recv_buffer_size\": "; vnx::write(_out, recv_buffer_size);
	_out << ", \"send_buffer_size\": "; vnx::write(_out, send_buffer_size);
	_out << ", \"default_access\": "; vnx::write(_out, default_access);
	_out << "}";
}

void ServerBase::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object ServerBase::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.Server";
	_object["address"] = address;
	_object["allow_login"] = allow_login;
	_object["use_authentication"] = use_authentication;
	_object["export_list"] = export_list;
	_object["max_queue_ms"] = max_queue_ms;
	_object["max_queue_size"] = max_queue_size;
	_object["recv_buffer_size"] = recv_buffer_size;
	_object["send_buffer_size"] = send_buffer_size;
	_object["default_access"] = default_access;
	return _object;
}

void ServerBase::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "address") {
			_entry.second.to(address);
		} else if(_entry.first == "allow_login") {
			_entry.second.to(allow_login);
		} else if(_entry.first == "default_access") {
			_entry.second.to(default_access);
		} else if(_entry.first == "export_list") {
			_entry.second.to(export_list);
		} else if(_entry.first == "max_queue_ms") {
			_entry.second.to(max_queue_ms);
		} else if(_entry.first == "max_queue_size") {
			_entry.second.to(max_queue_size);
		} else if(_entry.first == "recv_buffer_size") {
			_entry.second.to(recv_buffer_size);
		} else if(_entry.first == "send_buffer_size") {
			_entry.second.to(send_buffer_size);
		} else if(_entry.first == "use_authentication") {
			_entry.second.to(use_authentication);
		}
	}
}

vnx::Variant ServerBase::get_field(const std::string& _name) const {
	if(_name == "address") {
		return vnx::Variant(address);
	}
	if(_name == "allow_login") {
		return vnx::Variant(allow_login);
	}
	if(_name == "use_authentication") {
		return vnx::Variant(use_authentication);
	}
	if(_name == "export_list") {
		return vnx::Variant(export_list);
	}
	if(_name == "max_queue_ms") {
		return vnx::Variant(max_queue_ms);
	}
	if(_name == "max_queue_size") {
		return vnx::Variant(max_queue_size);
	}
	if(_name == "recv_buffer_size") {
		return vnx::Variant(recv_buffer_size);
	}
	if(_name == "send_buffer_size") {
		return vnx::Variant(send_buffer_size);
	}
	if(_name == "default_access") {
		return vnx::Variant(default_access);
	}
	return vnx::Variant();
}

void ServerBase::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "address") {
		_value.to(address);
	} else if(_name == "allow_login") {
		_value.to(allow_login);
	} else if(_name == "use_authentication") {
		_value.to(use_authentication);
	} else if(_name == "export_list") {
		_value.to(export_list);
	} else if(_name == "max_queue_ms") {
		_value.to(max_queue_ms);
	} else if(_name == "max_queue_size") {
		_value.to(max_queue_size);
	} else if(_name == "recv_buffer_size") {
		_value.to(recv_buffer_size);
	} else if(_name == "send_buffer_size") {
		_value.to(send_buffer_size);
	} else if(_name == "default_access") {
		_value.to(default_access);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const ServerBase& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, ServerBase& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* ServerBase::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> ServerBase::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.Server";
	type_code->type_hash = vnx::Hash64(0x150d7ad1f906225aull);
	type_code->code_hash = vnx::Hash64(0x83b35d2aab671ff7ull);
	type_code->is_native = true;
	type_code->native_size = sizeof(::vnx::ServerBase);
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::BaseServerBase::static_get_type_code();
	type_code->methods.resize(9);
	type_code->methods[0] = ::vnx::ModuleInterface_vnx_get_config_object::static_get_type_code();
	type_code->methods[1] = ::vnx::ModuleInterface_vnx_get_config::static_get_type_code();
	type_code->methods[2] = ::vnx::ModuleInterface_vnx_set_config_object::static_get_type_code();
	type_code->methods[3] = ::vnx::ModuleInterface_vnx_set_config::static_get_type_code();
	type_code->methods[4] = ::vnx::ModuleInterface_vnx_get_type_code::static_get_type_code();
	type_code->methods[5] = ::vnx::ModuleInterface_vnx_get_module_info::static_get_type_code();
	type_code->methods[6] = ::vnx::ModuleInterface_vnx_restart::static_get_type_code();
	type_code->methods[7] = ::vnx::ModuleInterface_vnx_stop::static_get_type_code();
	type_code->methods[8] = ::vnx::ModuleInterface_vnx_self_test::static_get_type_code();
	type_code->fields.resize(9);
	{
		auto& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "address";
		field.code = {32};
	}
	{
		auto& field = type_code->fields[1];
		field.data_size = 1;
		field.name = "allow_login";
		field.value = vnx::to_string(true);
		field.code = {31};
	}
	{
		auto& field = type_code->fields[2];
		field.data_size = 1;
		field.name = "use_authentication";
		field.value = vnx::to_string(false);
		field.code = {31};
	}
	{
		auto& field = type_code->fields[3];
		field.is_extended = true;
		field.name = "export_list";
		field.code = {12, 12, 5};
	}
	{
		auto& field = type_code->fields[4];
		field.data_size = 4;
		field.name = "max_queue_ms";
		field.value = vnx::to_string(100);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[5];
		field.data_size = 4;
		field.name = "max_queue_size";
		field.value = vnx::to_string(1000);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[6];
		field.data_size = 4;
		field.name = "recv_buffer_size";
		field.value = vnx::to_string(0);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[7];
		field.data_size = 4;
		field.name = "send_buffer_size";
		field.value = vnx::to_string(131072);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[8];
		field.is_extended = true;
		field.name = "default_access";
		field.value = vnx::to_string("DEFAULT");
		field.code = {32};
	}
	type_code->build();
	return type_code;
}

void ServerBase::vnx_handle_switch(std::shared_ptr<const vnx::Value> _value) {
	const auto* _type_code = _value->get_type_code();
	while(_type_code) {
		switch(_type_code->type_hash) {
			default:
				_type_code = _type_code->super;
		}
	}
	handle(std::static_pointer_cast<const vnx::Value>(_value));
}

std::shared_ptr<vnx::Value> ServerBase::vnx_call_switch(std::shared_ptr<const vnx::Value> _method, const vnx::request_id_t& _request_id) {
	switch(_method->get_type_hash()) {
		case 0x17f58f68bf83abc0ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_get_config_object>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_get_config_object_return::create();
			_return_value->_ret_0 = vnx_get_config_object();
			return _return_value;
		}
		case 0xbbc7f1a01044d294ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_get_config>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_get_config_return::create();
			_return_value->_ret_0 = vnx_get_config(_args->name);
			return _return_value;
		}
		case 0xca30f814f17f322full: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_set_config_object>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_set_config_object_return::create();
			vnx_set_config_object(_args->config);
			return _return_value;
		}
		case 0x362aac91373958b7ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_set_config>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_set_config_return::create();
			vnx_set_config(_args->name, _args->value);
			return _return_value;
		}
		case 0x305ec4d628960e5dull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_get_type_code>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_get_type_code_return::create();
			_return_value->_ret_0 = vnx_get_type_code();
			return _return_value;
		}
		case 0xf6d82bdf66d034a1ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_get_module_info>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_get_module_info_return::create();
			_return_value->_ret_0 = vnx_get_module_info();
			return _return_value;
		}
		case 0x9e95dc280cecca1bull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_restart>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_restart_return::create();
			vnx_restart();
			return _return_value;
		}
		case 0x7ab49ce3d1bfc0d2ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_stop>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_stop_return::create();
			vnx_stop();
			return _return_value;
		}
		case 0x6ce3775b41a42697ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_self_test>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_self_test_return::create();
			_return_value->_ret_0 = vnx_self_test();
			return _return_value;
		}
	}
	auto _ex = vnx::NoSuchMethod::create();
	_ex->dst_mac = vnx_request ? vnx_request->dst_mac : vnx::Hash64();
	_ex->method = _method->get_type_name();
	return _ex;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::ServerBase& value, const TypeCode* type_code, const uint16_t* code) {
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
	const char* const _buf = in.read(type_code->total_field_size);
	if(type_code->is_matched) {
		if(const auto* const _field = type_code->field_map[1]) {
			vnx::read_value(_buf + _field->offset, value.allow_login, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[2]) {
			vnx::read_value(_buf + _field->offset, value.use_authentication, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[4]) {
			vnx::read_value(_buf + _field->offset, value.max_queue_ms, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[5]) {
			vnx::read_value(_buf + _field->offset, value.max_queue_size, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[6]) {
			vnx::read_value(_buf + _field->offset, value.recv_buffer_size, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[7]) {
			vnx::read_value(_buf + _field->offset, value.send_buffer_size, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.address, type_code, _field->code.data()); break;
			case 3: vnx::read(in, value.export_list, type_code, _field->code.data()); break;
			case 8: vnx::read(in, value.default_access, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::ServerBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_ServerBase;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::ServerBase>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(18);
	vnx::write_value(_buf + 0, value.allow_login);
	vnx::write_value(_buf + 1, value.use_authentication);
	vnx::write_value(_buf + 2, value.max_queue_ms);
	vnx::write_value(_buf + 6, value.max_queue_size);
	vnx::write_value(_buf + 10, value.recv_buffer_size);
	vnx::write_value(_buf + 14, value.send_buffer_size);
	vnx::write(out, value.address, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.export_list, type_code, type_code->fields[3].code.data());
	vnx::write(out, value.default_access, type_code, type_code->fields[8].code.data());
}

void read(std::istream& in, ::vnx::ServerBase& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::ServerBase& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::ServerBase& value) {
	value.accept(visitor);
}

} // vnx
