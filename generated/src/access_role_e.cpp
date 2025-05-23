
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/access_role_e.hxx>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 access_role_e::VNX_TYPE_HASH(0x915660c6100c6c6eull);
const vnx::Hash64 access_role_e::VNX_CODE_HASH(0xf736f225dc57335aull);

vnx::Hash64 access_role_e::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string access_role_e::get_type_name() const {
	return "vnx.access_role_e";
}

const vnx::TypeCode* access_role_e::get_type_code() const {
	return vnx::vnx_native_type_code_access_role_e;
}

std::shared_ptr<access_role_e> access_role_e::create() {
	return std::make_shared<access_role_e>();
}

std::shared_ptr<access_role_e> access_role_e::clone() const {
	return std::make_shared<access_role_e>(*this);
}

void access_role_e::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void access_role_e::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

vnx::bool_t access_role_e::is_valid() const {
	switch(value) {
		case ADMIN: return true;
		case DEFAULT: return true;
		case INSTALLER: return true;
		case OBSERVER: return true;
		case UNAUTHENTICATED: return true;
		case USER: return true;
		case VIEWER: return true;
	}
	return false;
}

std::string access_role_e::to_string() const {
	switch(value) {
		case ADMIN: return "\"ADMIN\"";
		case DEFAULT: return "\"DEFAULT\"";
		case INSTALLER: return "\"INSTALLER\"";
		case OBSERVER: return "\"OBSERVER\"";
		case UNAUTHENTICATED: return "\"UNAUTHENTICATED\"";
		case USER: return "\"USER\"";
		case VIEWER: return "\"VIEWER\"";
	}
	return std::to_string(value);
}

std::string access_role_e::to_string_value() const {
	switch(value) {
		case ADMIN: return "ADMIN";
		case DEFAULT: return "DEFAULT";
		case INSTALLER: return "INSTALLER";
		case OBSERVER: return "OBSERVER";
		case UNAUTHENTICATED: return "UNAUTHENTICATED";
		case USER: return "USER";
		case VIEWER: return "VIEWER";
	}
	return std::to_string(value);
}

std::string access_role_e::to_string_value_full() const {
	switch(value) {
		case ADMIN: return "vnx.access_role_e.ADMIN";
		case DEFAULT: return "vnx.access_role_e.DEFAULT";
		case INSTALLER: return "vnx.access_role_e.INSTALLER";
		case OBSERVER: return "vnx.access_role_e.OBSERVER";
		case UNAUTHENTICATED: return "vnx.access_role_e.UNAUTHENTICATED";
		case USER: return "vnx.access_role_e.USER";
		case VIEWER: return "vnx.access_role_e.VIEWER";
	}
	return std::to_string(value);
}

void access_role_e::from_string(const std::string& _str) {
	std::string _name;
	vnx::from_string(_str, _name);
	from_string_value(_name);
}

void access_role_e::from_string_value(const std::string& _name) {
	vnx::Variant var;
	vnx::from_string_value(_name, var);
	if(var.is_string()) {
		if(_name == "ADMIN") value = ADMIN;
		else if(_name == "DEFAULT") value = DEFAULT;
		else if(_name == "INSTALLER") value = INSTALLER;
		else if(_name == "OBSERVER") value = OBSERVER;
		else if(_name == "UNAUTHENTICATED") value = UNAUTHENTICATED;
		else if(_name == "USER") value = USER;
		else if(_name == "VIEWER") value = VIEWER;
		else value = enum_t(vnx::hash64(_name));
	} else {
		value = enum_t(std::stoul(_name.c_str(), nullptr, 0));
	}
}

void access_role_e::accept(vnx::Visitor& _visitor) const {
	std::string _name;
	switch(value) {
		case ADMIN: _name = "ADMIN"; break;
		case DEFAULT: _name = "DEFAULT"; break;
		case INSTALLER: _name = "INSTALLER"; break;
		case OBSERVER: _name = "OBSERVER"; break;
		case UNAUTHENTICATED: _name = "UNAUTHENTICATED"; break;
		case USER: _name = "USER"; break;
		case VIEWER: _name = "VIEWER"; break;
	}
	_visitor.enum_value(value, _name);
}

void access_role_e::write(std::ostream& _out) const {
	switch(value) {
		case ADMIN: _out << "\"ADMIN\""; break;
		case DEFAULT: _out << "\"DEFAULT\""; break;
		case INSTALLER: _out << "\"INSTALLER\""; break;
		case OBSERVER: _out << "\"OBSERVER\""; break;
		case UNAUTHENTICATED: _out << "\"UNAUTHENTICATED\""; break;
		case USER: _out << "\"USER\""; break;
		case VIEWER: _out << "\"VIEWER\""; break;
		default: _out << value;
	}
}

void access_role_e::read(std::istream& _in) {
	from_string_value(vnx::read(_in).to_string_value());
}

vnx::Object access_role_e::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.access_role_e";
	_object["value"] = value;
	return _object;
}

void access_role_e::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "value") {
			_entry.second.to(value);
		}
	}
}

vnx::Variant access_role_e::get_field(const std::string& _name) const {
	if(_name == "value") {
		return vnx::Variant(value);
	}
	return vnx::Variant();
}

void access_role_e::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "value") {
		_value.to(value);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const access_role_e& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, access_role_e& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* access_role_e::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> access_role_e::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.access_role_e";
	type_code->type_hash = vnx::Hash64(0x915660c6100c6c6eull);
	type_code->code_hash = vnx::Hash64(0xf736f225dc57335aull);
	type_code->is_native = true;
	type_code->is_enum = true;
	type_code->native_size = sizeof(::vnx::access_role_e);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<vnx::Struct<access_role_e>>(); };
	type_code->fields.resize(1);
	{
		auto& field = type_code->fields[0];
		field.data_size = 4;
		field.name = "value";
		field.code = {3};
	}
	type_code->enum_map[2100538712] = "ADMIN";
	type_code->enum_map[490312487] = "DEFAULT";
	type_code->enum_map[455339260] = "INSTALLER";
	type_code->enum_map[1844813261] = "OBSERVER";
	type_code->enum_map[2279186489] = "UNAUTHENTICATED";
	type_code->enum_map[137129985] = "USER";
	type_code->enum_map[2077878907] = "VIEWER";
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::access_role_e& value, const TypeCode* type_code, const uint16_t* code) {
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
			value = ::vnx::access_role_e::enum_t(tmp.to<uint32_t>());
		} else {
			value = ::vnx::access_role_e();
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

void write(TypeOutput& out, const ::vnx::access_role_e& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_STRING) {
		vnx::write(out, value.to_string_value(), nullptr, code);
		return;
	}
	if(code && code[0] == CODE_UINT32) {
		vnx::write(out, value.value, nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_access_role_e;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::access_role_e>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	auto* const _buf = out.write(4);
	vnx::write_value(_buf + 0, value.value);
}

void read(std::istream& in, ::vnx::access_role_e& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::access_role_e& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::access_role_e& value) {
	value.accept(visitor);
}

void read(TypeInput& in, ::vnx::access_role_e::enum_t& value, const TypeCode* type_code, const uint16_t* code) {
	uint32_t tmp = 0;
	vnx::read(in, tmp, type_code, code);
	value = ::vnx::access_role_e::enum_t(tmp);
}

void write(TypeOutput& out, const ::vnx::access_role_e::enum_t& value, const TypeCode* type_code, const uint16_t* code) {
	vnx::write(out, uint32_t(value), type_code, code);
}

template<>
std::string to_string(const ::vnx::access_role_e& _value) {
	return _value.to_string();
}

template<>
std::string to_string_value(const ::vnx::access_role_e& _value) {
	return _value.to_string_value();
}

template<>
std::string to_string_value_full(const ::vnx::access_role_e& _value) {
	return _value.to_string_value_full();
}

template<>
std::string to_string(const ::vnx::access_role_e::enum_t& _value) {
	return ::vnx::access_role_e(_value).to_string();
}

template<>
std::string to_string_value(const ::vnx::access_role_e::enum_t& _value) {
	return ::vnx::access_role_e(_value).to_string_value();
}

template<>
std::string to_string_value_full(const ::vnx::access_role_e::enum_t& _value) {
	return ::vnx::access_role_e(_value).to_string_value_full();
}

bool is_equivalent<::vnx::access_role_e>::operator()(const uint16_t* code, const TypeCode* type_code) {
	if(code[0] != CODE_STRUCT || !type_code) {
		return false;
	}
	type_code = type_code->depends[code[1]];
	return type_code->type_hash == ::vnx::access_role_e::VNX_TYPE_HASH && type_code->is_equivalent;
}

} // vnx
