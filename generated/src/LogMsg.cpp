
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/LogMsg.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {

const int32_t LogMsg::ERROR;
const int32_t LogMsg::WARN;
const int32_t LogMsg::INFO;
const int32_t LogMsg::DEBUG;

const vnx::Hash64 LogMsg::VNX_TYPE_HASH(0x2a13f6d072f9b852ull);
const vnx::Hash64 LogMsg::VNX_CODE_HASH(0x37ed6487b6c1ea8ull);

vnx::Hash64 LogMsg::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string LogMsg::get_type_name() const {
	return "vnx.LogMsg";
}

const vnx::TypeCode* LogMsg::get_type_code() const {
	return vnx::vnx_native_type_code_LogMsg;
}

std::shared_ptr<LogMsg> LogMsg::create() {
	return std::make_shared<LogMsg>();
}

std::shared_ptr<vnx::Value> LogMsg::clone() const {
	return std::make_shared<LogMsg>(*this);
}

void LogMsg::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void LogMsg::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void LogMsg::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_LogMsg;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, time);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, level);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, display_level);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, process);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, module);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, message);
	_visitor.type_end(*_type_code);
}

void LogMsg::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.LogMsg\"";
	_out << ", \"time\": "; vnx::write(_out, time);
	_out << ", \"level\": "; vnx::write(_out, level);
	_out << ", \"display_level\": "; vnx::write(_out, display_level);
	_out << ", \"process\": "; vnx::write(_out, process);
	_out << ", \"module\": "; vnx::write(_out, module);
	_out << ", \"message\": "; vnx::write(_out, message);
	_out << "}";
}

void LogMsg::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object LogMsg::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.LogMsg";
	_object["time"] = time;
	_object["level"] = level;
	_object["display_level"] = display_level;
	_object["process"] = process;
	_object["module"] = module;
	_object["message"] = message;
	return _object;
}

void LogMsg::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "display_level") {
			_entry.second.to(display_level);
		} else if(_entry.first == "level") {
			_entry.second.to(level);
		} else if(_entry.first == "message") {
			_entry.second.to(message);
		} else if(_entry.first == "module") {
			_entry.second.to(module);
		} else if(_entry.first == "process") {
			_entry.second.to(process);
		} else if(_entry.first == "time") {
			_entry.second.to(time);
		}
	}
}

vnx::Variant LogMsg::get_field(const std::string& _name) const {
	if(_name == "time") {
		return vnx::Variant(time);
	}
	if(_name == "level") {
		return vnx::Variant(level);
	}
	if(_name == "display_level") {
		return vnx::Variant(display_level);
	}
	if(_name == "process") {
		return vnx::Variant(process);
	}
	if(_name == "module") {
		return vnx::Variant(module);
	}
	if(_name == "message") {
		return vnx::Variant(message);
	}
	return vnx::Variant();
}

void LogMsg::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "time") {
		_value.to(time);
	} else if(_name == "level") {
		_value.to(level);
	} else if(_name == "display_level") {
		_value.to(display_level);
	} else if(_name == "process") {
		_value.to(process);
	} else if(_name == "module") {
		_value.to(module);
	} else if(_name == "message") {
		_value.to(message);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const LogMsg& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, LogMsg& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* LogMsg::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> LogMsg::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.LogMsg";
	type_code->type_hash = vnx::Hash64(0x2a13f6d072f9b852ull);
	type_code->code_hash = vnx::Hash64(0x37ed6487b6c1ea8ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->native_size = sizeof(::vnx::LogMsg);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<LogMsg>(); };
	type_code->fields.resize(6);
	{
		auto& field = type_code->fields[0];
		field.data_size = 8;
		field.name = "time";
		field.code = {8};
	}
	{
		auto& field = type_code->fields[1];
		field.data_size = 4;
		field.name = "level";
		field.code = {7};
	}
	{
		auto& field = type_code->fields[2];
		field.data_size = 4;
		field.name = "display_level";
		field.value = vnx::to_string(3);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[3];
		field.is_extended = true;
		field.name = "process";
		field.code = {32};
	}
	{
		auto& field = type_code->fields[4];
		field.is_extended = true;
		field.name = "module";
		field.code = {32};
	}
	{
		auto& field = type_code->fields[5];
		field.is_extended = true;
		field.name = "message";
		field.code = {32};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::LogMsg& value, const TypeCode* type_code, const uint16_t* code) {
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
		if(const auto* const _field = type_code->field_map[0]) {
			vnx::read_value(_buf + _field->offset, value.time, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[1]) {
			vnx::read_value(_buf + _field->offset, value.level, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[2]) {
			vnx::read_value(_buf + _field->offset, value.display_level, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 3: vnx::read(in, value.process, type_code, _field->code.data()); break;
			case 4: vnx::read(in, value.module, type_code, _field->code.data()); break;
			case 5: vnx::read(in, value.message, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::LogMsg& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_LogMsg;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::LogMsg>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(16);
	vnx::write_value(_buf + 0, value.time);
	vnx::write_value(_buf + 8, value.level);
	vnx::write_value(_buf + 12, value.display_level);
	vnx::write(out, value.process, type_code, type_code->fields[3].code.data());
	vnx::write(out, value.module, type_code, type_code->fields[4].code.data());
	vnx::write(out, value.message, type_code, type_code->fields[5].code.data());
}

void read(std::istream& in, ::vnx::LogMsg& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::LogMsg& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::LogMsg& value) {
	value.accept(visitor);
}

} // vnx
