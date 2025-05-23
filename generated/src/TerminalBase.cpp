
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/TerminalBase.hxx>
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
#include <vnx/Terminal_read_char.hxx>
#include <vnx/Terminal_read_char_return.hxx>
#include <vnx/Terminal_read_event.hxx>
#include <vnx/Terminal_read_event_return.hxx>
#include <vnx/terminal_event_e.hxx>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 TerminalBase::VNX_TYPE_HASH(0xa4ce75801916357bull);
const vnx::Hash64 TerminalBase::VNX_CODE_HASH(0x389596df788b267bull);

TerminalBase::TerminalBase(const std::string& _vnx_name)
	:	Module::Module(_vnx_name)
{
	vnx::read_config(vnx_name + ".max_list_size", max_list_size);
	vnx::read_config(vnx_name + ".prompt", prompt);
	vnx::read_config(vnx_name + ".htop_interval", htop_interval);
}

vnx::Hash64 TerminalBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string TerminalBase::get_type_name() const {
	return "vnx.Terminal";
}

const vnx::TypeCode* TerminalBase::get_type_code() const {
	return vnx::vnx_native_type_code_TerminalBase;
}

void TerminalBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_TerminalBase;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, max_list_size);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, prompt);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, htop_interval);
	_visitor.type_end(*_type_code);
}

void TerminalBase::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"max_list_size\": "; vnx::write(_out, max_list_size);
	_out << ", \"prompt\": "; vnx::write(_out, prompt);
	_out << ", \"htop_interval\": "; vnx::write(_out, htop_interval);
	_out << "}";
}

void TerminalBase::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object TerminalBase::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.Terminal";
	_object["max_list_size"] = max_list_size;
	_object["prompt"] = prompt;
	_object["htop_interval"] = htop_interval;
	return _object;
}

void TerminalBase::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "htop_interval") {
			_entry.second.to(htop_interval);
		} else if(_entry.first == "max_list_size") {
			_entry.second.to(max_list_size);
		} else if(_entry.first == "prompt") {
			_entry.second.to(prompt);
		}
	}
}

vnx::Variant TerminalBase::get_field(const std::string& _name) const {
	if(_name == "max_list_size") {
		return vnx::Variant(max_list_size);
	}
	if(_name == "prompt") {
		return vnx::Variant(prompt);
	}
	if(_name == "htop_interval") {
		return vnx::Variant(htop_interval);
	}
	return vnx::Variant();
}

void TerminalBase::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "max_list_size") {
		_value.to(max_list_size);
	} else if(_name == "prompt") {
		_value.to(prompt);
	} else if(_name == "htop_interval") {
		_value.to(htop_interval);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const TerminalBase& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, TerminalBase& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* TerminalBase::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> TerminalBase::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.Terminal";
	type_code->type_hash = vnx::Hash64(0xa4ce75801916357bull);
	type_code->code_hash = vnx::Hash64(0x389596df788b267bull);
	type_code->is_native = true;
	type_code->native_size = sizeof(::vnx::TerminalBase);
	type_code->methods.resize(11);
	type_code->methods[0] = ::vnx::ModuleInterface_vnx_get_config::static_get_type_code();
	type_code->methods[1] = ::vnx::ModuleInterface_vnx_get_config_object::static_get_type_code();
	type_code->methods[2] = ::vnx::ModuleInterface_vnx_get_module_info::static_get_type_code();
	type_code->methods[3] = ::vnx::ModuleInterface_vnx_get_type_code::static_get_type_code();
	type_code->methods[4] = ::vnx::ModuleInterface_vnx_restart::static_get_type_code();
	type_code->methods[5] = ::vnx::ModuleInterface_vnx_self_test::static_get_type_code();
	type_code->methods[6] = ::vnx::ModuleInterface_vnx_set_config::static_get_type_code();
	type_code->methods[7] = ::vnx::ModuleInterface_vnx_set_config_object::static_get_type_code();
	type_code->methods[8] = ::vnx::ModuleInterface_vnx_stop::static_get_type_code();
	type_code->methods[9] = ::vnx::Terminal_read_char::static_get_type_code();
	type_code->methods[10] = ::vnx::Terminal_read_event::static_get_type_code();
	type_code->fields.resize(3);
	{
		auto& field = type_code->fields[0];
		field.data_size = 4;
		field.name = "max_list_size";
		field.value = vnx::to_string(1000);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "prompt";
		field.value = vnx::to_string("> ");
		field.code = {32};
	}
	{
		auto& field = type_code->fields[2];
		field.data_size = 4;
		field.name = "htop_interval";
		field.value = vnx::to_string(3000);
		field.code = {7};
	}
	type_code->build();
	return type_code;
}

void TerminalBase::vnx_handle_switch(std::shared_ptr<const vnx::Value> _value) {
	const auto* _type_code = _value->get_type_code();
	while(_type_code) {
		switch(_type_code->type_hash) {
			default:
				_type_code = _type_code->super;
		}
	}
	handle(std::static_pointer_cast<const vnx::Value>(_value));
}

std::shared_ptr<vnx::Value> TerminalBase::vnx_call_switch(std::shared_ptr<const vnx::Value> _method, const vnx::request_id_t& _request_id) {
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
		case 0xbba4b21472683454ull: {
			auto _args = std::static_pointer_cast<const ::vnx::Terminal_read_char>(_method);
			auto _return_value = ::vnx::Terminal_read_char_return::create();
			read_char(_args->c);
			return _return_value;
		}
		case 0x5c01345c24f51c67ull: {
			auto _args = std::static_pointer_cast<const ::vnx::Terminal_read_event>(_method);
			auto _return_value = ::vnx::Terminal_read_event_return::create();
			read_event(_args->event);
			return _return_value;
		}
	}
	auto _ex = vnx::NoSuchMethod::create();
	_ex->method = _method->get_type_name();
	return _ex;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::TerminalBase& value, const TypeCode* type_code, const uint16_t* code) {
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
		if(const auto* const _field = type_code->field_map[0]) {
			vnx::read_value(_buf + _field->offset, value.max_list_size, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[2]) {
			vnx::read_value(_buf + _field->offset, value.htop_interval, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 1: vnx::read(in, value.prompt, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::TerminalBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_TerminalBase;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::TerminalBase>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	auto* const _buf = out.write(8);
	vnx::write_value(_buf + 0, value.max_list_size);
	vnx::write_value(_buf + 4, value.htop_interval);
	vnx::write(out, value.prompt, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::TerminalBase& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::TerminalBase& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::TerminalBase& value) {
	value.accept(visitor);
}

} // vnx
