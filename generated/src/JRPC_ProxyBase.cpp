
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/JRPC_ProxyBase.hxx>
#include <vnx/NoSuchMethod.hxx>
#include <vnx/BaseProxy.h>
#include <vnx/JRPC_Proxy_select_service.hxx>
#include <vnx/JRPC_Proxy_select_service_return.hxx>
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
#include <vnx/ProxyInterface_disable_export.hxx>
#include <vnx/ProxyInterface_disable_export_return.hxx>
#include <vnx/ProxyInterface_disable_forward.hxx>
#include <vnx/ProxyInterface_disable_forward_return.hxx>
#include <vnx/ProxyInterface_disable_import.hxx>
#include <vnx/ProxyInterface_disable_import_return.hxx>
#include <vnx/ProxyInterface_disable_tunnel.hxx>
#include <vnx/ProxyInterface_disable_tunnel_return.hxx>
#include <vnx/ProxyInterface_enable_export.hxx>
#include <vnx/ProxyInterface_enable_export_return.hxx>
#include <vnx/ProxyInterface_enable_forward.hxx>
#include <vnx/ProxyInterface_enable_forward_return.hxx>
#include <vnx/ProxyInterface_enable_import.hxx>
#include <vnx/ProxyInterface_enable_import_return.hxx>
#include <vnx/ProxyInterface_enable_tunnel.hxx>
#include <vnx/ProxyInterface_enable_tunnel_return.hxx>
#include <vnx/ProxyInterface_get_session.hxx>
#include <vnx/ProxyInterface_get_session_return.hxx>
#include <vnx/ProxyInterface_login.hxx>
#include <vnx/ProxyInterface_login_return.hxx>
#include <vnx/ProxyInterface_on_connect.hxx>
#include <vnx/ProxyInterface_on_connect_return.hxx>
#include <vnx/ProxyInterface_on_disconnect.hxx>
#include <vnx/ProxyInterface_on_disconnect_return.hxx>
#include <vnx/ProxyInterface_on_login.hxx>
#include <vnx/ProxyInterface_on_login_return.hxx>
#include <vnx/ProxyInterface_on_remote_connect.hxx>
#include <vnx/ProxyInterface_on_remote_connect_return.hxx>
#include <vnx/ProxyInterface_on_remote_login.hxx>
#include <vnx/ProxyInterface_on_remote_login_return.hxx>
#include <vnx/ProxyInterface_wait_on_connect.hxx>
#include <vnx/ProxyInterface_wait_on_connect_return.hxx>
#include <vnx/ProxyInterface_wait_on_disconnect.hxx>
#include <vnx/ProxyInterface_wait_on_disconnect_return.hxx>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 JRPC_ProxyBase::VNX_TYPE_HASH(0x8f49f08256b4e765ull);
const vnx::Hash64 JRPC_ProxyBase::VNX_CODE_HASH(0xbe898b8173d7608aull);

JRPC_ProxyBase::JRPC_ProxyBase(const std::string& _vnx_name)
	:	BaseProxy::BaseProxy(_vnx_name)
{
}

vnx::Hash64 JRPC_ProxyBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string JRPC_ProxyBase::get_type_name() const {
	return "vnx.JRPC_Proxy";
}

const vnx::TypeCode* JRPC_ProxyBase::get_type_code() const {
	return vnx::vnx_native_type_code_JRPC_ProxyBase;
}

void JRPC_ProxyBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_JRPC_ProxyBase;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, address);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, auto_login);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, import_list);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, export_list);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, forward_list);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, tunnel_list);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, import_map);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, export_map);
	_visitor.type_field(_type_code->fields[8], 8); vnx::accept(_visitor, receive_tunnel);
	_visitor.type_field(_type_code->fields[9], 9); vnx::accept(_visitor, request_tunnel);
	_visitor.type_field(_type_code->fields[10], 10); vnx::accept(_visitor, no_retry);
	_visitor.type_field(_type_code->fields[11], 11); vnx::accept(_visitor, auto_import);
	_visitor.type_field(_type_code->fields[12], 12); vnx::accept(_visitor, time_sync);
	_visitor.type_field(_type_code->fields[13], 13); vnx::accept(_visitor, allow_login);
	_visitor.type_field(_type_code->fields[14], 14); vnx::accept(_visitor, use_authentication);
	_visitor.type_field(_type_code->fields[15], 15); vnx::accept(_visitor, block_until_connect);
	_visitor.type_field(_type_code->fields[16], 16); vnx::accept(_visitor, block_until_reconnect);
	_visitor.type_field(_type_code->fields[17], 17); vnx::accept(_visitor, max_queue_ms);
	_visitor.type_field(_type_code->fields[18], 18); vnx::accept(_visitor, max_queue_size);
	_visitor.type_field(_type_code->fields[19], 19); vnx::accept(_visitor, max_hop_count);
	_visitor.type_field(_type_code->fields[20], 20); vnx::accept(_visitor, recv_buffer_size);
	_visitor.type_field(_type_code->fields[21], 21); vnx::accept(_visitor, send_buffer_size);
	_visitor.type_field(_type_code->fields[22], 22); vnx::accept(_visitor, heartbeat_ms);
	_visitor.type_field(_type_code->fields[23], 23); vnx::accept(_visitor, heartbeat_timeout);
	_visitor.type_field(_type_code->fields[24], 24); vnx::accept(_visitor, default_access);
	_visitor.type_end(*_type_code);
}

void JRPC_ProxyBase::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"address\": "; vnx::write(_out, address);
	_out << ", \"auto_login\": "; vnx::write(_out, auto_login);
	_out << ", \"import_list\": "; vnx::write(_out, import_list);
	_out << ", \"export_list\": "; vnx::write(_out, export_list);
	_out << ", \"forward_list\": "; vnx::write(_out, forward_list);
	_out << ", \"tunnel_list\": "; vnx::write(_out, tunnel_list);
	_out << ", \"import_map\": "; vnx::write(_out, import_map);
	_out << ", \"export_map\": "; vnx::write(_out, export_map);
	_out << ", \"receive_tunnel\": "; vnx::write(_out, receive_tunnel);
	_out << ", \"request_tunnel\": "; vnx::write(_out, request_tunnel);
	_out << ", \"no_retry\": "; vnx::write(_out, no_retry);
	_out << ", \"auto_import\": "; vnx::write(_out, auto_import);
	_out << ", \"time_sync\": "; vnx::write(_out, time_sync);
	_out << ", \"allow_login\": "; vnx::write(_out, allow_login);
	_out << ", \"use_authentication\": "; vnx::write(_out, use_authentication);
	_out << ", \"block_until_connect\": "; vnx::write(_out, block_until_connect);
	_out << ", \"block_until_reconnect\": "; vnx::write(_out, block_until_reconnect);
	_out << ", \"max_queue_ms\": "; vnx::write(_out, max_queue_ms);
	_out << ", \"max_queue_size\": "; vnx::write(_out, max_queue_size);
	_out << ", \"max_hop_count\": "; vnx::write(_out, max_hop_count);
	_out << ", \"recv_buffer_size\": "; vnx::write(_out, recv_buffer_size);
	_out << ", \"send_buffer_size\": "; vnx::write(_out, send_buffer_size);
	_out << ", \"heartbeat_ms\": "; vnx::write(_out, heartbeat_ms);
	_out << ", \"heartbeat_timeout\": "; vnx::write(_out, heartbeat_timeout);
	_out << ", \"default_access\": "; vnx::write(_out, default_access);
	_out << "}";
}

void JRPC_ProxyBase::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object JRPC_ProxyBase::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.JRPC_Proxy";
	_object["address"] = address;
	_object["auto_login"] = auto_login;
	_object["import_list"] = import_list;
	_object["export_list"] = export_list;
	_object["forward_list"] = forward_list;
	_object["tunnel_list"] = tunnel_list;
	_object["import_map"] = import_map;
	_object["export_map"] = export_map;
	_object["receive_tunnel"] = receive_tunnel;
	_object["request_tunnel"] = request_tunnel;
	_object["no_retry"] = no_retry;
	_object["auto_import"] = auto_import;
	_object["time_sync"] = time_sync;
	_object["allow_login"] = allow_login;
	_object["use_authentication"] = use_authentication;
	_object["block_until_connect"] = block_until_connect;
	_object["block_until_reconnect"] = block_until_reconnect;
	_object["max_queue_ms"] = max_queue_ms;
	_object["max_queue_size"] = max_queue_size;
	_object["max_hop_count"] = max_hop_count;
	_object["recv_buffer_size"] = recv_buffer_size;
	_object["send_buffer_size"] = send_buffer_size;
	_object["heartbeat_ms"] = heartbeat_ms;
	_object["heartbeat_timeout"] = heartbeat_timeout;
	_object["default_access"] = default_access;
	return _object;
}

void JRPC_ProxyBase::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "address") {
			_entry.second.to(address);
		} else if(_entry.first == "allow_login") {
			_entry.second.to(allow_login);
		} else if(_entry.first == "auto_import") {
			_entry.second.to(auto_import);
		} else if(_entry.first == "auto_login") {
			_entry.second.to(auto_login);
		} else if(_entry.first == "block_until_connect") {
			_entry.second.to(block_until_connect);
		} else if(_entry.first == "block_until_reconnect") {
			_entry.second.to(block_until_reconnect);
		} else if(_entry.first == "default_access") {
			_entry.second.to(default_access);
		} else if(_entry.first == "export_list") {
			_entry.second.to(export_list);
		} else if(_entry.first == "export_map") {
			_entry.second.to(export_map);
		} else if(_entry.first == "forward_list") {
			_entry.second.to(forward_list);
		} else if(_entry.first == "heartbeat_ms") {
			_entry.second.to(heartbeat_ms);
		} else if(_entry.first == "heartbeat_timeout") {
			_entry.second.to(heartbeat_timeout);
		} else if(_entry.first == "import_list") {
			_entry.second.to(import_list);
		} else if(_entry.first == "import_map") {
			_entry.second.to(import_map);
		} else if(_entry.first == "max_hop_count") {
			_entry.second.to(max_hop_count);
		} else if(_entry.first == "max_queue_ms") {
			_entry.second.to(max_queue_ms);
		} else if(_entry.first == "max_queue_size") {
			_entry.second.to(max_queue_size);
		} else if(_entry.first == "no_retry") {
			_entry.second.to(no_retry);
		} else if(_entry.first == "receive_tunnel") {
			_entry.second.to(receive_tunnel);
		} else if(_entry.first == "recv_buffer_size") {
			_entry.second.to(recv_buffer_size);
		} else if(_entry.first == "request_tunnel") {
			_entry.second.to(request_tunnel);
		} else if(_entry.first == "send_buffer_size") {
			_entry.second.to(send_buffer_size);
		} else if(_entry.first == "time_sync") {
			_entry.second.to(time_sync);
		} else if(_entry.first == "tunnel_list") {
			_entry.second.to(tunnel_list);
		} else if(_entry.first == "use_authentication") {
			_entry.second.to(use_authentication);
		}
	}
}

vnx::Variant JRPC_ProxyBase::get_field(const std::string& _name) const {
	if(_name == "address") {
		return vnx::Variant(address);
	}
	if(_name == "auto_login") {
		return vnx::Variant(auto_login);
	}
	if(_name == "import_list") {
		return vnx::Variant(import_list);
	}
	if(_name == "export_list") {
		return vnx::Variant(export_list);
	}
	if(_name == "forward_list") {
		return vnx::Variant(forward_list);
	}
	if(_name == "tunnel_list") {
		return vnx::Variant(tunnel_list);
	}
	if(_name == "import_map") {
		return vnx::Variant(import_map);
	}
	if(_name == "export_map") {
		return vnx::Variant(export_map);
	}
	if(_name == "receive_tunnel") {
		return vnx::Variant(receive_tunnel);
	}
	if(_name == "request_tunnel") {
		return vnx::Variant(request_tunnel);
	}
	if(_name == "no_retry") {
		return vnx::Variant(no_retry);
	}
	if(_name == "auto_import") {
		return vnx::Variant(auto_import);
	}
	if(_name == "time_sync") {
		return vnx::Variant(time_sync);
	}
	if(_name == "allow_login") {
		return vnx::Variant(allow_login);
	}
	if(_name == "use_authentication") {
		return vnx::Variant(use_authentication);
	}
	if(_name == "block_until_connect") {
		return vnx::Variant(block_until_connect);
	}
	if(_name == "block_until_reconnect") {
		return vnx::Variant(block_until_reconnect);
	}
	if(_name == "max_queue_ms") {
		return vnx::Variant(max_queue_ms);
	}
	if(_name == "max_queue_size") {
		return vnx::Variant(max_queue_size);
	}
	if(_name == "max_hop_count") {
		return vnx::Variant(max_hop_count);
	}
	if(_name == "recv_buffer_size") {
		return vnx::Variant(recv_buffer_size);
	}
	if(_name == "send_buffer_size") {
		return vnx::Variant(send_buffer_size);
	}
	if(_name == "heartbeat_ms") {
		return vnx::Variant(heartbeat_ms);
	}
	if(_name == "heartbeat_timeout") {
		return vnx::Variant(heartbeat_timeout);
	}
	if(_name == "default_access") {
		return vnx::Variant(default_access);
	}
	return vnx::Variant();
}

void JRPC_ProxyBase::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "address") {
		_value.to(address);
	} else if(_name == "auto_login") {
		_value.to(auto_login);
	} else if(_name == "import_list") {
		_value.to(import_list);
	} else if(_name == "export_list") {
		_value.to(export_list);
	} else if(_name == "forward_list") {
		_value.to(forward_list);
	} else if(_name == "tunnel_list") {
		_value.to(tunnel_list);
	} else if(_name == "import_map") {
		_value.to(import_map);
	} else if(_name == "export_map") {
		_value.to(export_map);
	} else if(_name == "receive_tunnel") {
		_value.to(receive_tunnel);
	} else if(_name == "request_tunnel") {
		_value.to(request_tunnel);
	} else if(_name == "no_retry") {
		_value.to(no_retry);
	} else if(_name == "auto_import") {
		_value.to(auto_import);
	} else if(_name == "time_sync") {
		_value.to(time_sync);
	} else if(_name == "allow_login") {
		_value.to(allow_login);
	} else if(_name == "use_authentication") {
		_value.to(use_authentication);
	} else if(_name == "block_until_connect") {
		_value.to(block_until_connect);
	} else if(_name == "block_until_reconnect") {
		_value.to(block_until_reconnect);
	} else if(_name == "max_queue_ms") {
		_value.to(max_queue_ms);
	} else if(_name == "max_queue_size") {
		_value.to(max_queue_size);
	} else if(_name == "max_hop_count") {
		_value.to(max_hop_count);
	} else if(_name == "recv_buffer_size") {
		_value.to(recv_buffer_size);
	} else if(_name == "send_buffer_size") {
		_value.to(send_buffer_size);
	} else if(_name == "heartbeat_ms") {
		_value.to(heartbeat_ms);
	} else if(_name == "heartbeat_timeout") {
		_value.to(heartbeat_timeout);
	} else if(_name == "default_access") {
		_value.to(default_access);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const JRPC_ProxyBase& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, JRPC_ProxyBase& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* JRPC_ProxyBase::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> JRPC_ProxyBase::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.JRPC_Proxy";
	type_code->type_hash = vnx::Hash64(0x8f49f08256b4e765ull);
	type_code->code_hash = vnx::Hash64(0xbe898b8173d7608aull);
	type_code->is_native = true;
	type_code->native_size = sizeof(::vnx::JRPC_ProxyBase);
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::BaseProxyBase::static_get_type_code();
	type_code->methods.resize(27);
	type_code->methods[0] = ::vnx::JRPC_Proxy_select_service::static_get_type_code();
	type_code->methods[1] = ::vnx::ModuleInterface_vnx_get_config::static_get_type_code();
	type_code->methods[2] = ::vnx::ModuleInterface_vnx_get_config_object::static_get_type_code();
	type_code->methods[3] = ::vnx::ModuleInterface_vnx_get_module_info::static_get_type_code();
	type_code->methods[4] = ::vnx::ModuleInterface_vnx_get_type_code::static_get_type_code();
	type_code->methods[5] = ::vnx::ModuleInterface_vnx_restart::static_get_type_code();
	type_code->methods[6] = ::vnx::ModuleInterface_vnx_self_test::static_get_type_code();
	type_code->methods[7] = ::vnx::ModuleInterface_vnx_set_config::static_get_type_code();
	type_code->methods[8] = ::vnx::ModuleInterface_vnx_set_config_object::static_get_type_code();
	type_code->methods[9] = ::vnx::ModuleInterface_vnx_stop::static_get_type_code();
	type_code->methods[10] = ::vnx::ProxyInterface_disable_export::static_get_type_code();
	type_code->methods[11] = ::vnx::ProxyInterface_disable_forward::static_get_type_code();
	type_code->methods[12] = ::vnx::ProxyInterface_disable_import::static_get_type_code();
	type_code->methods[13] = ::vnx::ProxyInterface_disable_tunnel::static_get_type_code();
	type_code->methods[14] = ::vnx::ProxyInterface_enable_export::static_get_type_code();
	type_code->methods[15] = ::vnx::ProxyInterface_enable_forward::static_get_type_code();
	type_code->methods[16] = ::vnx::ProxyInterface_enable_import::static_get_type_code();
	type_code->methods[17] = ::vnx::ProxyInterface_enable_tunnel::static_get_type_code();
	type_code->methods[18] = ::vnx::ProxyInterface_get_session::static_get_type_code();
	type_code->methods[19] = ::vnx::ProxyInterface_login::static_get_type_code();
	type_code->methods[20] = ::vnx::ProxyInterface_on_connect::static_get_type_code();
	type_code->methods[21] = ::vnx::ProxyInterface_on_disconnect::static_get_type_code();
	type_code->methods[22] = ::vnx::ProxyInterface_on_login::static_get_type_code();
	type_code->methods[23] = ::vnx::ProxyInterface_on_remote_connect::static_get_type_code();
	type_code->methods[24] = ::vnx::ProxyInterface_on_remote_login::static_get_type_code();
	type_code->methods[25] = ::vnx::ProxyInterface_wait_on_connect::static_get_type_code();
	type_code->methods[26] = ::vnx::ProxyInterface_wait_on_disconnect::static_get_type_code();
	type_code->fields.resize(25);
	{
		auto& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "address";
		field.code = {32};
	}
	{
		auto& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "auto_login";
		field.code = {23, 2, 4, 5, 32, 32};
	}
	{
		auto& field = type_code->fields[2];
		field.is_extended = true;
		field.name = "import_list";
		field.code = {12, 12, 5};
	}
	{
		auto& field = type_code->fields[3];
		field.is_extended = true;
		field.name = "export_list";
		field.code = {12, 12, 5};
	}
	{
		auto& field = type_code->fields[4];
		field.is_extended = true;
		field.name = "forward_list";
		field.code = {12, 32};
	}
	{
		auto& field = type_code->fields[5];
		field.is_extended = true;
		field.name = "tunnel_list";
		field.code = {12, 4};
	}
	{
		auto& field = type_code->fields[6];
		field.is_extended = true;
		field.name = "import_map";
		field.code = {13, 4, 12, 5, 12, 5};
	}
	{
		auto& field = type_code->fields[7];
		field.is_extended = true;
		field.name = "export_map";
		field.code = {13, 4, 12, 5, 12, 5};
	}
	{
		auto& field = type_code->fields[8];
		field.is_extended = true;
		field.name = "receive_tunnel";
		field.code = {4};
	}
	{
		auto& field = type_code->fields[9];
		field.is_extended = true;
		field.name = "request_tunnel";
		field.code = {4};
	}
	{
		auto& field = type_code->fields[10];
		field.data_size = 1;
		field.name = "no_retry";
		field.value = vnx::to_string(false);
		field.code = {31};
	}
	{
		auto& field = type_code->fields[11];
		field.data_size = 1;
		field.name = "auto_import";
		field.value = vnx::to_string(false);
		field.code = {31};
	}
	{
		auto& field = type_code->fields[12];
		field.data_size = 1;
		field.name = "time_sync";
		field.value = vnx::to_string(false);
		field.code = {31};
	}
	{
		auto& field = type_code->fields[13];
		field.data_size = 1;
		field.name = "allow_login";
		field.value = vnx::to_string(true);
		field.code = {31};
	}
	{
		auto& field = type_code->fields[14];
		field.data_size = 1;
		field.name = "use_authentication";
		field.value = vnx::to_string(false);
		field.code = {31};
	}
	{
		auto& field = type_code->fields[15];
		field.data_size = 1;
		field.name = "block_until_connect";
		field.value = vnx::to_string(true);
		field.code = {31};
	}
	{
		auto& field = type_code->fields[16];
		field.data_size = 1;
		field.name = "block_until_reconnect";
		field.value = vnx::to_string(false);
		field.code = {31};
	}
	{
		auto& field = type_code->fields[17];
		field.data_size = 4;
		field.name = "max_queue_ms";
		field.value = vnx::to_string(100);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[18];
		field.data_size = 4;
		field.name = "max_queue_size";
		field.value = vnx::to_string(1000);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[19];
		field.data_size = 4;
		field.name = "max_hop_count";
		field.value = vnx::to_string(10);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[20];
		field.data_size = 4;
		field.name = "recv_buffer_size";
		field.value = vnx::to_string(0);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[21];
		field.data_size = 4;
		field.name = "send_buffer_size";
		field.value = vnx::to_string(131072);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[22];
		field.data_size = 4;
		field.name = "heartbeat_ms";
		field.value = vnx::to_string(10000);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[23];
		field.data_size = 4;
		field.name = "heartbeat_timeout";
		field.value = vnx::to_string(3);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[24];
		field.is_extended = true;
		field.name = "default_access";
		field.value = vnx::to_string("DEFAULT");
		field.code = {32};
	}
	type_code->build();
	return type_code;
}

void JRPC_ProxyBase::vnx_handle_switch(std::shared_ptr<const vnx::Value> _value) {
	const auto* _type_code = _value->get_type_code();
	while(_type_code) {
		switch(_type_code->type_hash) {
			default:
				_type_code = _type_code->super;
		}
	}
	handle(std::static_pointer_cast<const vnx::Value>(_value));
}

std::shared_ptr<vnx::Value> JRPC_ProxyBase::vnx_call_switch(std::shared_ptr<const vnx::Value> _method, const vnx::request_id_t& _request_id) {
	switch(_method->get_type_hash()) {
		case 0x46f370b62d703bf3ull: {
			auto _args = std::static_pointer_cast<const ::vnx::JRPC_Proxy_select_service>(_method);
			auto _return_value = ::vnx::JRPC_Proxy_select_service_return::create();
			select_service(_args->service_name);
			return _return_value;
		}
		case 0xbbc7f1a01044d294ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_get_config>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_get_config_return::create();
			_return_value->_ret_0 = vnx_get_config(_args->name);
			return _return_value;
		}
		case 0x17f58f68bf83abc0ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_get_config_object>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_get_config_object_return::create();
			_return_value->_ret_0 = vnx_get_config_object();
			return _return_value;
		}
		case 0xf6d82bdf66d034a1ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_get_module_info>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_get_module_info_return::create();
			_return_value->_ret_0 = vnx_get_module_info();
			return _return_value;
		}
		case 0x305ec4d628960e5dull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_get_type_code>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_get_type_code_return::create();
			_return_value->_ret_0 = vnx_get_type_code();
			return _return_value;
		}
		case 0x9e95dc280cecca1bull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_restart>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_restart_return::create();
			vnx_restart();
			return _return_value;
		}
		case 0x6ce3775b41a42697ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_self_test>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_self_test_return::create();
			_return_value->_ret_0 = vnx_self_test();
			return _return_value;
		}
		case 0x362aac91373958b7ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_set_config>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_set_config_return::create();
			vnx_set_config(_args->name, _args->value);
			return _return_value;
		}
		case 0xca30f814f17f322full: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_set_config_object>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_set_config_object_return::create();
			vnx_set_config_object(_args->config);
			return _return_value;
		}
		case 0x7ab49ce3d1bfc0d2ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_stop>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_stop_return::create();
			vnx_stop();
			return _return_value;
		}
		case 0x255ec29037ffaac4ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ProxyInterface_disable_export>(_method);
			auto _return_value = ::vnx::ProxyInterface_disable_export_return::create();
			disable_export(_args->topic_name);
			return _return_value;
		}
		case 0x4829f4ced514a3e1ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ProxyInterface_disable_forward>(_method);
			auto _return_value = ::vnx::ProxyInterface_disable_forward_return::create();
			disable_forward(_args->service_name);
			return _return_value;
		}
		case 0xa465d5b138bd53eull: {
			auto _args = std::static_pointer_cast<const ::vnx::ProxyInterface_disable_import>(_method);
			auto _return_value = ::vnx::ProxyInterface_disable_import_return::create();
			disable_import(_args->topic_name);
			return _return_value;
		}
		case 0xb9a7abaf9d00b54bull: {
			auto _args = std::static_pointer_cast<const ::vnx::ProxyInterface_disable_tunnel>(_method);
			auto _return_value = ::vnx::ProxyInterface_disable_tunnel_return::create();
			disable_tunnel(_args->tunnel_addr);
			return _return_value;
		}
		case 0xbca6b569c4b73ff7ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ProxyInterface_enable_export>(_method);
			auto _return_value = ::vnx::ProxyInterface_enable_export_return::create();
			enable_export(_args->topic_name);
			return _return_value;
		}
		case 0x89b695187455be9bull: {
			auto _args = std::static_pointer_cast<const ::vnx::ProxyInterface_enable_forward>(_method);
			auto _return_value = ::vnx::ProxyInterface_enable_forward_return::create();
			enable_forward(_args->service_name, _args->max_queue_ms, _args->max_queue_size);
			return _return_value;
		}
		case 0x93be2aa2e0c3400dull: {
			auto _args = std::static_pointer_cast<const ::vnx::ProxyInterface_enable_import>(_method);
			auto _return_value = ::vnx::ProxyInterface_enable_import_return::create();
			enable_import(_args->topic_name);
			return _return_value;
		}
		case 0x205fdc566e482078ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ProxyInterface_enable_tunnel>(_method);
			auto _return_value = ::vnx::ProxyInterface_enable_tunnel_return::create();
			enable_tunnel(_args->tunnel_addr, _args->max_queue_ms, _args->max_queue_size);
			return _return_value;
		}
		case 0x60cf186ca96171f6ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ProxyInterface_get_session>(_method);
			auto _return_value = ::vnx::ProxyInterface_get_session_return::create();
			_return_value->_ret_0 = get_session();
			return _return_value;
		}
		case 0xe1cd627631f1b2f1ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ProxyInterface_login>(_method);
			login_async(_args->name, _args->password, _request_id);
			return nullptr;
		}
		case 0x62d6b28adfabac18ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ProxyInterface_on_connect>(_method);
			auto _return_value = ::vnx::ProxyInterface_on_connect_return::create();
			on_connect();
			return _return_value;
		}
		case 0x9f7411664fb7ff6ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ProxyInterface_on_disconnect>(_method);
			auto _return_value = ::vnx::ProxyInterface_on_disconnect_return::create();
			on_disconnect();
			return _return_value;
		}
		case 0x5df07f9c3a42bf5aull: {
			auto _args = std::static_pointer_cast<const ::vnx::ProxyInterface_on_login>(_method);
			auto _return_value = ::vnx::ProxyInterface_on_login_return::create();
			on_login(_args->name, _args->password);
			return _return_value;
		}
		case 0x58d0fc825fa3d477ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ProxyInterface_on_remote_connect>(_method);
			auto _return_value = ::vnx::ProxyInterface_on_remote_connect_return::create();
			on_remote_connect(_args->process_id);
			return _return_value;
		}
		case 0xfdef02ccd8b73be4ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ProxyInterface_on_remote_login>(_method);
			auto _return_value = ::vnx::ProxyInterface_on_remote_login_return::create();
			on_remote_login(_args->remote_session);
			return _return_value;
		}
		case 0x21e57eee5d93712eull: {
			auto _args = std::static_pointer_cast<const ::vnx::ProxyInterface_wait_on_connect>(_method);
			wait_on_connect_async(_request_id);
			return nullptr;
		}
		case 0xa313fafa981a7560ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ProxyInterface_wait_on_disconnect>(_method);
			wait_on_disconnect_async(_request_id);
			return nullptr;
		}
	}
	auto _ex = vnx::NoSuchMethod::create();
	_ex->method = _method->get_type_name();
	return _ex;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::JRPC_ProxyBase& value, const TypeCode* type_code, const uint16_t* code) {
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
	const auto* const _buf = in.read(type_code->total_field_size);
	if(type_code->is_matched) {
		if(const auto* const _field = type_code->field_map[10]) {
			vnx::read_value(_buf + _field->offset, value.no_retry, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[11]) {
			vnx::read_value(_buf + _field->offset, value.auto_import, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[12]) {
			vnx::read_value(_buf + _field->offset, value.time_sync, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[13]) {
			vnx::read_value(_buf + _field->offset, value.allow_login, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[14]) {
			vnx::read_value(_buf + _field->offset, value.use_authentication, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[15]) {
			vnx::read_value(_buf + _field->offset, value.block_until_connect, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[16]) {
			vnx::read_value(_buf + _field->offset, value.block_until_reconnect, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[17]) {
			vnx::read_value(_buf + _field->offset, value.max_queue_ms, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[18]) {
			vnx::read_value(_buf + _field->offset, value.max_queue_size, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[19]) {
			vnx::read_value(_buf + _field->offset, value.max_hop_count, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[20]) {
			vnx::read_value(_buf + _field->offset, value.recv_buffer_size, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[21]) {
			vnx::read_value(_buf + _field->offset, value.send_buffer_size, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[22]) {
			vnx::read_value(_buf + _field->offset, value.heartbeat_ms, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[23]) {
			vnx::read_value(_buf + _field->offset, value.heartbeat_timeout, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.address, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.auto_login, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.import_list, type_code, _field->code.data()); break;
			case 3: vnx::read(in, value.export_list, type_code, _field->code.data()); break;
			case 4: vnx::read(in, value.forward_list, type_code, _field->code.data()); break;
			case 5: vnx::read(in, value.tunnel_list, type_code, _field->code.data()); break;
			case 6: vnx::read(in, value.import_map, type_code, _field->code.data()); break;
			case 7: vnx::read(in, value.export_map, type_code, _field->code.data()); break;
			case 8: vnx::read(in, value.receive_tunnel, type_code, _field->code.data()); break;
			case 9: vnx::read(in, value.request_tunnel, type_code, _field->code.data()); break;
			case 24: vnx::read(in, value.default_access, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::JRPC_ProxyBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_JRPC_ProxyBase;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::JRPC_ProxyBase>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	auto* const _buf = out.write(35);
	vnx::write_value(_buf + 0, value.no_retry);
	vnx::write_value(_buf + 1, value.auto_import);
	vnx::write_value(_buf + 2, value.time_sync);
	vnx::write_value(_buf + 3, value.allow_login);
	vnx::write_value(_buf + 4, value.use_authentication);
	vnx::write_value(_buf + 5, value.block_until_connect);
	vnx::write_value(_buf + 6, value.block_until_reconnect);
	vnx::write_value(_buf + 7, value.max_queue_ms);
	vnx::write_value(_buf + 11, value.max_queue_size);
	vnx::write_value(_buf + 15, value.max_hop_count);
	vnx::write_value(_buf + 19, value.recv_buffer_size);
	vnx::write_value(_buf + 23, value.send_buffer_size);
	vnx::write_value(_buf + 27, value.heartbeat_ms);
	vnx::write_value(_buf + 31, value.heartbeat_timeout);
	vnx::write(out, value.address, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.auto_login, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.import_list, type_code, type_code->fields[2].code.data());
	vnx::write(out, value.export_list, type_code, type_code->fields[3].code.data());
	vnx::write(out, value.forward_list, type_code, type_code->fields[4].code.data());
	vnx::write(out, value.tunnel_list, type_code, type_code->fields[5].code.data());
	vnx::write(out, value.import_map, type_code, type_code->fields[6].code.data());
	vnx::write(out, value.export_map, type_code, type_code->fields[7].code.data());
	vnx::write(out, value.receive_tunnel, type_code, type_code->fields[8].code.data());
	vnx::write(out, value.request_tunnel, type_code, type_code->fields[9].code.data());
	vnx::write(out, value.default_access, type_code, type_code->fields[24].code.data());
}

void read(std::istream& in, ::vnx::JRPC_ProxyBase& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::JRPC_ProxyBase& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::JRPC_ProxyBase& value) {
	value.accept(visitor);
}

} // vnx
