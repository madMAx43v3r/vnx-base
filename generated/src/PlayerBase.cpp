
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/PlayerBase.hxx>
#include <vnx/NoSuchMethod.hxx>
#include <vnx/Module.h>
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
#include <vnx/Player_get_info.hxx>
#include <vnx/Player_get_info_return.hxx>
#include <vnx/Player_pause.hxx>
#include <vnx/Player_pause_return.hxx>
#include <vnx/Player_play.hxx>
#include <vnx/Player_play_return.hxx>
#include <vnx/Player_seek_by_count.hxx>
#include <vnx/Player_seek_by_count_return.hxx>
#include <vnx/Player_seek_by_time.hxx>
#include <vnx/Player_seek_by_time_return.hxx>
#include <vnx/Player_seek_to_position.hxx>
#include <vnx/Player_seek_to_position_return.hxx>
#include <vnx/Player_seek_to_time.hxx>
#include <vnx/Player_seek_to_time_return.hxx>
#include <vnx/Player_set_speed.hxx>
#include <vnx/Player_set_speed_return.hxx>
#include <vnx/Player_stop.hxx>
#include <vnx/Player_stop_return.hxx>
#include <vnx/Player_toggle.hxx>
#include <vnx/Player_toggle_return.hxx>
#include <vnx/RecordHeader.hxx>
#include <vnx/TopicPtr.hpp>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 PlayerBase::VNX_TYPE_HASH(0x18af8c0a3c1bbcull);
const vnx::Hash64 PlayerBase::VNX_CODE_HASH(0x75fe456c4815e6b9ull);

PlayerBase::PlayerBase(const std::string& _vnx_name)
	:	Module::Module(_vnx_name)
{
	vnx::read_config(vnx_name + ".output_status", output_status);
	vnx::read_config(vnx_name + ".files", files);
	vnx::read_config(vnx_name + ".interval_ms", interval_ms);
	vnx::read_config(vnx_name + ".max_time_gap_ms", max_time_gap_ms);
	vnx::read_config(vnx_name + ".play_speed", play_speed);
	vnx::read_config(vnx_name + ".is_default_enable", is_default_enable);
	vnx::read_config(vnx_name + ".is_blocking", is_blocking);
	vnx::read_config(vnx_name + ".is_repeat", is_repeat);
	vnx::read_config(vnx_name + ".is_autostart", is_autostart);
	vnx::read_config(vnx_name + ".is_autoclose", is_autoclose);
	vnx::read_config(vnx_name + ".is_autoshutdown", is_autoshutdown);
	vnx::read_config(vnx_name + ".topic_map", topic_map);
}

vnx::Hash64 PlayerBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string PlayerBase::get_type_name() const {
	return "vnx.Player";
}

const vnx::TypeCode* PlayerBase::get_type_code() const {
	return vnx::vnx_native_type_code_PlayerBase;
}

void PlayerBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_PlayerBase;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, output_status);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, files);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, interval_ms);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, max_time_gap_ms);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, play_speed);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, is_default_enable);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, is_blocking);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, is_repeat);
	_visitor.type_field(_type_code->fields[8], 8); vnx::accept(_visitor, is_autostart);
	_visitor.type_field(_type_code->fields[9], 9); vnx::accept(_visitor, is_autoclose);
	_visitor.type_field(_type_code->fields[10], 10); vnx::accept(_visitor, is_autoshutdown);
	_visitor.type_field(_type_code->fields[11], 11); vnx::accept(_visitor, topic_map);
	_visitor.type_end(*_type_code);
}

void PlayerBase::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"output_status\": "; vnx::write(_out, output_status);
	_out << ", \"files\": "; vnx::write(_out, files);
	_out << ", \"interval_ms\": "; vnx::write(_out, interval_ms);
	_out << ", \"max_time_gap_ms\": "; vnx::write(_out, max_time_gap_ms);
	_out << ", \"play_speed\": "; vnx::write(_out, play_speed);
	_out << ", \"is_default_enable\": "; vnx::write(_out, is_default_enable);
	_out << ", \"is_blocking\": "; vnx::write(_out, is_blocking);
	_out << ", \"is_repeat\": "; vnx::write(_out, is_repeat);
	_out << ", \"is_autostart\": "; vnx::write(_out, is_autostart);
	_out << ", \"is_autoclose\": "; vnx::write(_out, is_autoclose);
	_out << ", \"is_autoshutdown\": "; vnx::write(_out, is_autoshutdown);
	_out << ", \"topic_map\": "; vnx::write(_out, topic_map);
	_out << "}";
}

void PlayerBase::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object PlayerBase::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.Player";
	_object["output_status"] = output_status;
	_object["files"] = files;
	_object["interval_ms"] = interval_ms;
	_object["max_time_gap_ms"] = max_time_gap_ms;
	_object["play_speed"] = play_speed;
	_object["is_default_enable"] = is_default_enable;
	_object["is_blocking"] = is_blocking;
	_object["is_repeat"] = is_repeat;
	_object["is_autostart"] = is_autostart;
	_object["is_autoclose"] = is_autoclose;
	_object["is_autoshutdown"] = is_autoshutdown;
	_object["topic_map"] = topic_map;
	return _object;
}

void PlayerBase::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "files") {
			_entry.second.to(files);
		} else if(_entry.first == "interval_ms") {
			_entry.second.to(interval_ms);
		} else if(_entry.first == "is_autoclose") {
			_entry.second.to(is_autoclose);
		} else if(_entry.first == "is_autoshutdown") {
			_entry.second.to(is_autoshutdown);
		} else if(_entry.first == "is_autostart") {
			_entry.second.to(is_autostart);
		} else if(_entry.first == "is_blocking") {
			_entry.second.to(is_blocking);
		} else if(_entry.first == "is_default_enable") {
			_entry.second.to(is_default_enable);
		} else if(_entry.first == "is_repeat") {
			_entry.second.to(is_repeat);
		} else if(_entry.first == "max_time_gap_ms") {
			_entry.second.to(max_time_gap_ms);
		} else if(_entry.first == "output_status") {
			_entry.second.to(output_status);
		} else if(_entry.first == "play_speed") {
			_entry.second.to(play_speed);
		} else if(_entry.first == "topic_map") {
			_entry.second.to(topic_map);
		}
	}
}

vnx::Variant PlayerBase::get_field(const std::string& _name) const {
	if(_name == "output_status") {
		return vnx::Variant(output_status);
	}
	if(_name == "files") {
		return vnx::Variant(files);
	}
	if(_name == "interval_ms") {
		return vnx::Variant(interval_ms);
	}
	if(_name == "max_time_gap_ms") {
		return vnx::Variant(max_time_gap_ms);
	}
	if(_name == "play_speed") {
		return vnx::Variant(play_speed);
	}
	if(_name == "is_default_enable") {
		return vnx::Variant(is_default_enable);
	}
	if(_name == "is_blocking") {
		return vnx::Variant(is_blocking);
	}
	if(_name == "is_repeat") {
		return vnx::Variant(is_repeat);
	}
	if(_name == "is_autostart") {
		return vnx::Variant(is_autostart);
	}
	if(_name == "is_autoclose") {
		return vnx::Variant(is_autoclose);
	}
	if(_name == "is_autoshutdown") {
		return vnx::Variant(is_autoshutdown);
	}
	if(_name == "topic_map") {
		return vnx::Variant(topic_map);
	}
	return vnx::Variant();
}

void PlayerBase::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "output_status") {
		_value.to(output_status);
	} else if(_name == "files") {
		_value.to(files);
	} else if(_name == "interval_ms") {
		_value.to(interval_ms);
	} else if(_name == "max_time_gap_ms") {
		_value.to(max_time_gap_ms);
	} else if(_name == "play_speed") {
		_value.to(play_speed);
	} else if(_name == "is_default_enable") {
		_value.to(is_default_enable);
	} else if(_name == "is_blocking") {
		_value.to(is_blocking);
	} else if(_name == "is_repeat") {
		_value.to(is_repeat);
	} else if(_name == "is_autostart") {
		_value.to(is_autostart);
	} else if(_name == "is_autoclose") {
		_value.to(is_autoclose);
	} else if(_name == "is_autoshutdown") {
		_value.to(is_autoshutdown);
	} else if(_name == "topic_map") {
		_value.to(topic_map);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const PlayerBase& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, PlayerBase& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* PlayerBase::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> PlayerBase::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.Player";
	type_code->type_hash = vnx::Hash64(0x18af8c0a3c1bbcull);
	type_code->code_hash = vnx::Hash64(0x75fe456c4815e6b9ull);
	type_code->is_native = true;
	type_code->native_size = sizeof(::vnx::PlayerBase);
	type_code->methods.resize(19);
	type_code->methods[0] = ::vnx::ModuleInterface_vnx_get_config::static_get_type_code();
	type_code->methods[1] = ::vnx::ModuleInterface_vnx_get_config_object::static_get_type_code();
	type_code->methods[2] = ::vnx::ModuleInterface_vnx_get_module_info::static_get_type_code();
	type_code->methods[3] = ::vnx::ModuleInterface_vnx_get_type_code::static_get_type_code();
	type_code->methods[4] = ::vnx::ModuleInterface_vnx_restart::static_get_type_code();
	type_code->methods[5] = ::vnx::ModuleInterface_vnx_self_test::static_get_type_code();
	type_code->methods[6] = ::vnx::ModuleInterface_vnx_set_config::static_get_type_code();
	type_code->methods[7] = ::vnx::ModuleInterface_vnx_set_config_object::static_get_type_code();
	type_code->methods[8] = ::vnx::ModuleInterface_vnx_stop::static_get_type_code();
	type_code->methods[9] = ::vnx::Player_get_info::static_get_type_code();
	type_code->methods[10] = ::vnx::Player_pause::static_get_type_code();
	type_code->methods[11] = ::vnx::Player_play::static_get_type_code();
	type_code->methods[12] = ::vnx::Player_seek_by_count::static_get_type_code();
	type_code->methods[13] = ::vnx::Player_seek_by_time::static_get_type_code();
	type_code->methods[14] = ::vnx::Player_seek_to_position::static_get_type_code();
	type_code->methods[15] = ::vnx::Player_seek_to_time::static_get_type_code();
	type_code->methods[16] = ::vnx::Player_set_speed::static_get_type_code();
	type_code->methods[17] = ::vnx::Player_stop::static_get_type_code();
	type_code->methods[18] = ::vnx::Player_toggle::static_get_type_code();
	type_code->fields.resize(12);
	{
		auto& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "output_status";
		field.value = vnx::to_string("vnx.player_status");
		field.code = {12, 5};
	}
	{
		auto& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "files";
		field.code = {12, 32};
	}
	{
		auto& field = type_code->fields[2];
		field.data_size = 4;
		field.name = "interval_ms";
		field.value = vnx::to_string(100);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[3];
		field.data_size = 4;
		field.name = "max_time_gap_ms";
		field.value = vnx::to_string(5000);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[4];
		field.data_size = 8;
		field.name = "play_speed";
		field.value = vnx::to_string(1);
		field.code = {10};
	}
	{
		auto& field = type_code->fields[5];
		field.data_size = 1;
		field.name = "is_default_enable";
		field.value = vnx::to_string(true);
		field.code = {31};
	}
	{
		auto& field = type_code->fields[6];
		field.data_size = 1;
		field.name = "is_blocking";
		field.value = vnx::to_string(false);
		field.code = {31};
	}
	{
		auto& field = type_code->fields[7];
		field.data_size = 1;
		field.name = "is_repeat";
		field.value = vnx::to_string(false);
		field.code = {31};
	}
	{
		auto& field = type_code->fields[8];
		field.data_size = 1;
		field.name = "is_autostart";
		field.value = vnx::to_string(false);
		field.code = {31};
	}
	{
		auto& field = type_code->fields[9];
		field.data_size = 1;
		field.name = "is_autoclose";
		field.value = vnx::to_string(false);
		field.code = {31};
	}
	{
		auto& field = type_code->fields[10];
		field.data_size = 1;
		field.name = "is_autoshutdown";
		field.value = vnx::to_string(false);
		field.code = {31};
	}
	{
		auto& field = type_code->fields[11];
		field.is_extended = true;
		field.name = "topic_map";
		field.code = {13, 4, 12, 5, 12, 5};
	}
	type_code->build();
	return type_code;
}

void PlayerBase::vnx_handle_switch(std::shared_ptr<const vnx::Value> _value) {
	const auto* _type_code = _value->get_type_code();
	while(_type_code) {
		switch(_type_code->type_hash) {
			default:
				_type_code = _type_code->super;
		}
	}
	handle(std::static_pointer_cast<const vnx::Value>(_value));
}

std::shared_ptr<vnx::Value> PlayerBase::vnx_call_switch(std::shared_ptr<const vnx::Value> _method, const vnx::request_id_t& _request_id) {
	switch(_method->get_type_hash()) {
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
		case 0x1ae86b011006dadull: {
			auto _args = std::static_pointer_cast<const ::vnx::Player_get_info>(_method);
			auto _return_value = ::vnx::Player_get_info_return::create();
			_return_value->_ret_0 = get_info();
			return _return_value;
		}
		case 0x439f7eeb5c70c3ffull: {
			auto _args = std::static_pointer_cast<const ::vnx::Player_pause>(_method);
			auto _return_value = ::vnx::Player_pause_return::create();
			pause();
			return _return_value;
		}
		case 0x49f930aaa55662c1ull: {
			auto _args = std::static_pointer_cast<const ::vnx::Player_play>(_method);
			auto _return_value = ::vnx::Player_play_return::create();
			play();
			return _return_value;
		}
		case 0x8a3e8bcc0f1a9b93ull: {
			auto _args = std::static_pointer_cast<const ::vnx::Player_seek_by_count>(_method);
			auto _return_value = ::vnx::Player_seek_by_count_return::create();
			seek_by_count(_args->delta_count);
			return _return_value;
		}
		case 0xd00881ef749ec2b8ull: {
			auto _args = std::static_pointer_cast<const ::vnx::Player_seek_by_time>(_method);
			auto _return_value = ::vnx::Player_seek_by_time_return::create();
			seek_by_time(_args->delta_us);
			return _return_value;
		}
		case 0xaef7c805f3f91f23ull: {
			auto _args = std::static_pointer_cast<const ::vnx::Player_seek_to_position>(_method);
			auto _return_value = ::vnx::Player_seek_to_position_return::create();
			seek_to_position(_args->position);
			return _return_value;
		}
		case 0x31e33676e70bdfaull: {
			auto _args = std::static_pointer_cast<const ::vnx::Player_seek_to_time>(_method);
			auto _return_value = ::vnx::Player_seek_to_time_return::create();
			seek_to_time(_args->time_us);
			return _return_value;
		}
		case 0xadab13bfccf029a7ull: {
			auto _args = std::static_pointer_cast<const ::vnx::Player_set_speed>(_method);
			auto _return_value = ::vnx::Player_set_speed_return::create();
			set_speed(_args->speed);
			return _return_value;
		}
		case 0xec400dd621046187ull: {
			auto _args = std::static_pointer_cast<const ::vnx::Player_stop>(_method);
			auto _return_value = ::vnx::Player_stop_return::create();
			stop();
			return _return_value;
		}
		case 0xaac4c063def8098eull: {
			auto _args = std::static_pointer_cast<const ::vnx::Player_toggle>(_method);
			auto _return_value = ::vnx::Player_toggle_return::create();
			toggle();
			return _return_value;
		}
	}
	auto _ex = vnx::NoSuchMethod::create();
	_ex->method = _method->get_type_name();
	return _ex;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::PlayerBase& value, const TypeCode* type_code, const uint16_t* code) {
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
		if(const auto* const _field = type_code->field_map[2]) {
			vnx::read_value(_buf + _field->offset, value.interval_ms, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[3]) {
			vnx::read_value(_buf + _field->offset, value.max_time_gap_ms, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[4]) {
			vnx::read_value(_buf + _field->offset, value.play_speed, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[5]) {
			vnx::read_value(_buf + _field->offset, value.is_default_enable, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[6]) {
			vnx::read_value(_buf + _field->offset, value.is_blocking, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[7]) {
			vnx::read_value(_buf + _field->offset, value.is_repeat, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[8]) {
			vnx::read_value(_buf + _field->offset, value.is_autostart, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[9]) {
			vnx::read_value(_buf + _field->offset, value.is_autoclose, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[10]) {
			vnx::read_value(_buf + _field->offset, value.is_autoshutdown, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.output_status, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.files, type_code, _field->code.data()); break;
			case 11: vnx::read(in, value.topic_map, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::PlayerBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_PlayerBase;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::PlayerBase>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	auto* const _buf = out.write(22);
	vnx::write_value(_buf + 0, value.interval_ms);
	vnx::write_value(_buf + 4, value.max_time_gap_ms);
	vnx::write_value(_buf + 8, value.play_speed);
	vnx::write_value(_buf + 16, value.is_default_enable);
	vnx::write_value(_buf + 17, value.is_blocking);
	vnx::write_value(_buf + 18, value.is_repeat);
	vnx::write_value(_buf + 19, value.is_autostart);
	vnx::write_value(_buf + 20, value.is_autoclose);
	vnx::write_value(_buf + 21, value.is_autoshutdown);
	vnx::write(out, value.output_status, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.files, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.topic_map, type_code, type_code->fields[11].code.data());
}

void read(std::istream& in, ::vnx::PlayerBase& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::PlayerBase& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::PlayerBase& value) {
	value.accept(visitor);
}

} // vnx
