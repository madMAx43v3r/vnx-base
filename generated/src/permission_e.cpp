
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/permission_e.hxx>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 permission_e::VNX_TYPE_HASH(0xb22885dc987a7be3ull);
const vnx::Hash64 permission_e::VNX_CODE_HASH(0xe9a50aad8a6a9b98ull);

vnx::Hash64 permission_e::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string permission_e::get_type_name() const {
	return "vnx.permission_e";
}

const vnx::TypeCode* permission_e::get_type_code() const {
	return vnx::vnx_native_type_code_permission_e;
}

std::shared_ptr<permission_e> permission_e::create() {
	return std::make_shared<permission_e>();
}

std::shared_ptr<permission_e> permission_e::clone() const {
	return std::make_shared<permission_e>(*this);
}

void permission_e::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void permission_e::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

vnx::bool_t permission_e::is_valid() const {
	switch(value) {
		case CONST_REQUEST: return true;
		case HOST_SHUTDOWN: return true;
		case INTERNAL: return true;
		case LOCAL: return true;
		case PROTECTED_CONFIG: return true;
		case PROXY_EXPORT: return true;
		case PROXY_FORWARD: return true;
		case PROXY_IMPORT: return true;
		case PROXY_INTERNAL: return true;
		case PUBLISH: return true;
		case READ_CONFIG: return true;
		case REQUEST: return true;
		case RESTART: return true;
		case SELF_TEST: return true;
		case SHUTDOWN: return true;
		case START: return true;
		case STOP: return true;
		case TIME_SYNC: return true;
		case VIEW: return true;
		case WRITE_CONFIG: return true;
	}
	return false;
}

std::string permission_e::to_string() const {
	switch(value) {
		case CONST_REQUEST: return "\"CONST_REQUEST\"";
		case HOST_SHUTDOWN: return "\"HOST_SHUTDOWN\"";
		case INTERNAL: return "\"INTERNAL\"";
		case LOCAL: return "\"LOCAL\"";
		case PROTECTED_CONFIG: return "\"PROTECTED_CONFIG\"";
		case PROXY_EXPORT: return "\"PROXY_EXPORT\"";
		case PROXY_FORWARD: return "\"PROXY_FORWARD\"";
		case PROXY_IMPORT: return "\"PROXY_IMPORT\"";
		case PROXY_INTERNAL: return "\"PROXY_INTERNAL\"";
		case PUBLISH: return "\"PUBLISH\"";
		case READ_CONFIG: return "\"READ_CONFIG\"";
		case REQUEST: return "\"REQUEST\"";
		case RESTART: return "\"RESTART\"";
		case SELF_TEST: return "\"SELF_TEST\"";
		case SHUTDOWN: return "\"SHUTDOWN\"";
		case START: return "\"START\"";
		case STOP: return "\"STOP\"";
		case TIME_SYNC: return "\"TIME_SYNC\"";
		case VIEW: return "\"VIEW\"";
		case WRITE_CONFIG: return "\"WRITE_CONFIG\"";
	}
	return std::to_string(value);
}

std::string permission_e::to_string_value() const {
	switch(value) {
		case CONST_REQUEST: return "CONST_REQUEST";
		case HOST_SHUTDOWN: return "HOST_SHUTDOWN";
		case INTERNAL: return "INTERNAL";
		case LOCAL: return "LOCAL";
		case PROTECTED_CONFIG: return "PROTECTED_CONFIG";
		case PROXY_EXPORT: return "PROXY_EXPORT";
		case PROXY_FORWARD: return "PROXY_FORWARD";
		case PROXY_IMPORT: return "PROXY_IMPORT";
		case PROXY_INTERNAL: return "PROXY_INTERNAL";
		case PUBLISH: return "PUBLISH";
		case READ_CONFIG: return "READ_CONFIG";
		case REQUEST: return "REQUEST";
		case RESTART: return "RESTART";
		case SELF_TEST: return "SELF_TEST";
		case SHUTDOWN: return "SHUTDOWN";
		case START: return "START";
		case STOP: return "STOP";
		case TIME_SYNC: return "TIME_SYNC";
		case VIEW: return "VIEW";
		case WRITE_CONFIG: return "WRITE_CONFIG";
	}
	return std::to_string(value);
}

std::string permission_e::to_string_value_full() const {
	switch(value) {
		case CONST_REQUEST: return "vnx.permission_e.CONST_REQUEST";
		case HOST_SHUTDOWN: return "vnx.permission_e.HOST_SHUTDOWN";
		case INTERNAL: return "vnx.permission_e.INTERNAL";
		case LOCAL: return "vnx.permission_e.LOCAL";
		case PROTECTED_CONFIG: return "vnx.permission_e.PROTECTED_CONFIG";
		case PROXY_EXPORT: return "vnx.permission_e.PROXY_EXPORT";
		case PROXY_FORWARD: return "vnx.permission_e.PROXY_FORWARD";
		case PROXY_IMPORT: return "vnx.permission_e.PROXY_IMPORT";
		case PROXY_INTERNAL: return "vnx.permission_e.PROXY_INTERNAL";
		case PUBLISH: return "vnx.permission_e.PUBLISH";
		case READ_CONFIG: return "vnx.permission_e.READ_CONFIG";
		case REQUEST: return "vnx.permission_e.REQUEST";
		case RESTART: return "vnx.permission_e.RESTART";
		case SELF_TEST: return "vnx.permission_e.SELF_TEST";
		case SHUTDOWN: return "vnx.permission_e.SHUTDOWN";
		case START: return "vnx.permission_e.START";
		case STOP: return "vnx.permission_e.STOP";
		case TIME_SYNC: return "vnx.permission_e.TIME_SYNC";
		case VIEW: return "vnx.permission_e.VIEW";
		case WRITE_CONFIG: return "vnx.permission_e.WRITE_CONFIG";
	}
	return std::to_string(value);
}

void permission_e::from_string(const std::string& _str) {
	std::string _name;
	vnx::from_string(_str, _name);
	from_string_value(_name);
}

void permission_e::from_string_value(const std::string& _name) {
	vnx::Variant var;
	vnx::from_string_value(_name, var);
	if(var.is_string()) {
		if(_name == "CONST_REQUEST") value = CONST_REQUEST;
		else if(_name == "HOST_SHUTDOWN") value = HOST_SHUTDOWN;
		else if(_name == "INTERNAL") value = INTERNAL;
		else if(_name == "LOCAL") value = LOCAL;
		else if(_name == "PROTECTED_CONFIG") value = PROTECTED_CONFIG;
		else if(_name == "PROXY_EXPORT") value = PROXY_EXPORT;
		else if(_name == "PROXY_FORWARD") value = PROXY_FORWARD;
		else if(_name == "PROXY_IMPORT") value = PROXY_IMPORT;
		else if(_name == "PROXY_INTERNAL") value = PROXY_INTERNAL;
		else if(_name == "PUBLISH") value = PUBLISH;
		else if(_name == "READ_CONFIG") value = READ_CONFIG;
		else if(_name == "REQUEST") value = REQUEST;
		else if(_name == "RESTART") value = RESTART;
		else if(_name == "SELF_TEST") value = SELF_TEST;
		else if(_name == "SHUTDOWN") value = SHUTDOWN;
		else if(_name == "START") value = START;
		else if(_name == "STOP") value = STOP;
		else if(_name == "TIME_SYNC") value = TIME_SYNC;
		else if(_name == "VIEW") value = VIEW;
		else if(_name == "WRITE_CONFIG") value = WRITE_CONFIG;
		else value = enum_t(vnx::hash64(_name));
	} else {
		value = enum_t(std::stoul(_name.c_str(), nullptr, 0));
	}
}

void permission_e::accept(vnx::Visitor& _visitor) const {
	std::string _name;
	switch(value) {
		case CONST_REQUEST: _name = "CONST_REQUEST"; break;
		case HOST_SHUTDOWN: _name = "HOST_SHUTDOWN"; break;
		case INTERNAL: _name = "INTERNAL"; break;
		case LOCAL: _name = "LOCAL"; break;
		case PROTECTED_CONFIG: _name = "PROTECTED_CONFIG"; break;
		case PROXY_EXPORT: _name = "PROXY_EXPORT"; break;
		case PROXY_FORWARD: _name = "PROXY_FORWARD"; break;
		case PROXY_IMPORT: _name = "PROXY_IMPORT"; break;
		case PROXY_INTERNAL: _name = "PROXY_INTERNAL"; break;
		case PUBLISH: _name = "PUBLISH"; break;
		case READ_CONFIG: _name = "READ_CONFIG"; break;
		case REQUEST: _name = "REQUEST"; break;
		case RESTART: _name = "RESTART"; break;
		case SELF_TEST: _name = "SELF_TEST"; break;
		case SHUTDOWN: _name = "SHUTDOWN"; break;
		case START: _name = "START"; break;
		case STOP: _name = "STOP"; break;
		case TIME_SYNC: _name = "TIME_SYNC"; break;
		case VIEW: _name = "VIEW"; break;
		case WRITE_CONFIG: _name = "WRITE_CONFIG"; break;
	}
	_visitor.enum_value(value, _name);
}

void permission_e::write(std::ostream& _out) const {
	switch(value) {
		case CONST_REQUEST: _out << "\"CONST_REQUEST\""; break;
		case HOST_SHUTDOWN: _out << "\"HOST_SHUTDOWN\""; break;
		case INTERNAL: _out << "\"INTERNAL\""; break;
		case LOCAL: _out << "\"LOCAL\""; break;
		case PROTECTED_CONFIG: _out << "\"PROTECTED_CONFIG\""; break;
		case PROXY_EXPORT: _out << "\"PROXY_EXPORT\""; break;
		case PROXY_FORWARD: _out << "\"PROXY_FORWARD\""; break;
		case PROXY_IMPORT: _out << "\"PROXY_IMPORT\""; break;
		case PROXY_INTERNAL: _out << "\"PROXY_INTERNAL\""; break;
		case PUBLISH: _out << "\"PUBLISH\""; break;
		case READ_CONFIG: _out << "\"READ_CONFIG\""; break;
		case REQUEST: _out << "\"REQUEST\""; break;
		case RESTART: _out << "\"RESTART\""; break;
		case SELF_TEST: _out << "\"SELF_TEST\""; break;
		case SHUTDOWN: _out << "\"SHUTDOWN\""; break;
		case START: _out << "\"START\""; break;
		case STOP: _out << "\"STOP\""; break;
		case TIME_SYNC: _out << "\"TIME_SYNC\""; break;
		case VIEW: _out << "\"VIEW\""; break;
		case WRITE_CONFIG: _out << "\"WRITE_CONFIG\""; break;
		default: _out << value;
	}
}

void permission_e::read(std::istream& _in) {
	from_string_value(vnx::read(_in).to_string_value());
}

vnx::Object permission_e::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.permission_e";
	_object["value"] = value;
	return _object;
}

void permission_e::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "value") {
			_entry.second.to(value);
		}
	}
}

vnx::Variant permission_e::get_field(const std::string& _name) const {
	if(_name == "value") {
		return vnx::Variant(value);
	}
	return vnx::Variant();
}

void permission_e::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "value") {
		_value.to(value);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const permission_e& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, permission_e& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* permission_e::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> permission_e::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.permission_e";
	type_code->type_hash = vnx::Hash64(0xb22885dc987a7be3ull);
	type_code->code_hash = vnx::Hash64(0xe9a50aad8a6a9b98ull);
	type_code->is_native = true;
	type_code->is_enum = true;
	type_code->native_size = sizeof(::vnx::permission_e);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<vnx::Struct<permission_e>>(); };
	type_code->fields.resize(1);
	{
		auto& field = type_code->fields[0];
		field.data_size = 4;
		field.name = "value";
		field.code = {3};
	}
	type_code->enum_map[1076482563] = "CONST_REQUEST";
	type_code->enum_map[1124463389] = "HOST_SHUTDOWN";
	type_code->enum_map[1861063121] = "INTERNAL";
	type_code->enum_map[3393177819] = "LOCAL";
	type_code->enum_map[2082950745] = "PROTECTED_CONFIG";
	type_code->enum_map[790344897] = "PROXY_EXPORT";
	type_code->enum_map[1111657009] = "PROXY_FORWARD";
	type_code->enum_map[191875899] = "PROXY_IMPORT";
	type_code->enum_map[3742832289] = "PROXY_INTERNAL";
	type_code->enum_map[1338854341] = "PUBLISH";
	type_code->enum_map[3203297939] = "READ_CONFIG";
	type_code->enum_map[3218098657] = "REQUEST";
	type_code->enum_map[3809341088] = "RESTART";
	type_code->enum_map[3038921719] = "SELF_TEST";
	type_code->enum_map[1988203803] = "SHUTDOWN";
	type_code->enum_map[4082106084] = "START";
	type_code->enum_map[883186718] = "STOP";
	type_code->enum_map[3725394100] = "TIME_SYNC";
	type_code->enum_map[3695368648] = "VIEW";
	type_code->enum_map[212794352] = "WRITE_CONFIG";
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::permission_e& value, const TypeCode* type_code, const uint16_t* code) {
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
		Variant tmp;
		vnx::read(in, tmp, type_code, code);
		if(tmp.is_string()) {
			vnx::from_string(tmp.to_string(), value);
		} else if(tmp.is_ulong()) {
			value = ::vnx::permission_e::enum_t(tmp.to<uint32_t>());
		} else {
			value = ::vnx::permission_e();
		}
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
			vnx::read_value(_buf + _field->offset, value.value, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::permission_e& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_STRING) {
		vnx::write(out, value.to_string_value(), nullptr, code);
		return;
	}
	if(code && code[0] == CODE_UINT32) {
		vnx::write(out, value.value, nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_permission_e;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::permission_e>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	auto* const _buf = out.write(4);
	vnx::write_value(_buf + 0, value.value);
}

void read(std::istream& in, ::vnx::permission_e& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::permission_e& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::permission_e& value) {
	value.accept(visitor);
}

void read(TypeInput& in, ::vnx::permission_e::enum_t& value, const TypeCode* type_code, const uint16_t* code) {
	uint32_t tmp = 0;
	vnx::read(in, tmp, type_code, code);
	value = ::vnx::permission_e::enum_t(tmp);
}

void write(TypeOutput& out, const ::vnx::permission_e::enum_t& value, const TypeCode* type_code, const uint16_t* code) {
	vnx::write(out, uint32_t(value), type_code, code);
}

template<>
std::string to_string(const ::vnx::permission_e& _value) {
	return _value.to_string();
}

template<>
std::string to_string_value(const ::vnx::permission_e& _value) {
	return _value.to_string_value();
}

template<>
std::string to_string_value_full(const ::vnx::permission_e& _value) {
	return _value.to_string_value_full();
}

template<>
std::string to_string(const ::vnx::permission_e::enum_t& _value) {
	return ::vnx::permission_e(_value).to_string();
}

template<>
std::string to_string_value(const ::vnx::permission_e::enum_t& _value) {
	return ::vnx::permission_e(_value).to_string_value();
}

template<>
std::string to_string_value_full(const ::vnx::permission_e::enum_t& _value) {
	return ::vnx::permission_e(_value).to_string_value_full();
}

bool is_equivalent<::vnx::permission_e>::operator()(const uint16_t* code, const TypeCode* type_code) {
	if(code[0] != CODE_STRUCT || !type_code) {
		return false;
	}
	type_code = type_code->depends[code[1]];
	return type_code->type_hash == ::vnx::permission_e::VNX_TYPE_HASH && type_code->is_equivalent;
}

} // vnx
