
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/User.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 User::VNX_TYPE_HASH(0x500d208d47b945a2ull);
const vnx::Hash64 User::VNX_CODE_HASH(0x754433fe1dc703ecull);

vnx::Hash64 User::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string User::get_type_name() const {
	return "vnx.User";
}

const vnx::TypeCode* User::get_type_code() const {
	return vnx::vnx_native_type_code_User;
}

std::shared_ptr<User> User::create() {
	return std::make_shared<User>();
}

std::shared_ptr<vnx::Value> User::clone() const {
	return std::make_shared<User>(*this);
}

void User::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void User::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void User::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_User;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, name);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, hashed_password);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, access_roles);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, permissions);
	_visitor.type_end(*_type_code);
}

void User::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.User\"";
	_out << ", \"name\": "; vnx::write(_out, name);
	_out << ", \"hashed_password\": "; vnx::write(_out, hashed_password);
	_out << ", \"access_roles\": "; vnx::write(_out, access_roles);
	_out << ", \"permissions\": "; vnx::write(_out, permissions);
	_out << "}";
}

void User::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object User::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.User";
	_object["name"] = name;
	_object["hashed_password"] = hashed_password;
	_object["access_roles"] = access_roles;
	_object["permissions"] = permissions;
	return _object;
}

void User::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "access_roles") {
			_entry.second.to(access_roles);
		} else if(_entry.first == "hashed_password") {
			_entry.second.to(hashed_password);
		} else if(_entry.first == "name") {
			_entry.second.to(name);
		} else if(_entry.first == "permissions") {
			_entry.second.to(permissions);
		}
	}
}

vnx::Variant User::get_field(const std::string& _name) const {
	if(_name == "name") {
		return vnx::Variant(name);
	}
	if(_name == "hashed_password") {
		return vnx::Variant(hashed_password);
	}
	if(_name == "access_roles") {
		return vnx::Variant(access_roles);
	}
	if(_name == "permissions") {
		return vnx::Variant(permissions);
	}
	return vnx::Variant();
}

void User::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "name") {
		_value.to(name);
	} else if(_name == "hashed_password") {
		_value.to(hashed_password);
	} else if(_name == "access_roles") {
		_value.to(access_roles);
	} else if(_name == "permissions") {
		_value.to(permissions);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const User& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, User& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* User::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> User::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.User";
	type_code->type_hash = vnx::Hash64(0x500d208d47b945a2ull);
	type_code->code_hash = vnx::Hash64(0x754433fe1dc703ecull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->native_size = sizeof(::vnx::User);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<User>(); };
	type_code->fields.resize(4);
	{
		auto& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "name";
		field.code = {32};
	}
	{
		auto& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "hashed_password";
		field.code = {32};
	}
	{
		auto& field = type_code->fields[2];
		field.is_extended = true;
		field.name = "access_roles";
		field.code = {12, 32};
	}
	{
		auto& field = type_code->fields[3];
		field.is_extended = true;
		field.name = "permissions";
		field.code = {12, 32};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::User& value, const TypeCode* type_code, const uint16_t* code) {
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
			case 0: vnx::read(in, value.name, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.hashed_password, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.access_roles, type_code, _field->code.data()); break;
			case 3: vnx::read(in, value.permissions, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::User& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_User;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::User>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.name, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.hashed_password, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.access_roles, type_code, type_code->fields[2].code.data());
	vnx::write(out, value.permissions, type_code, type_code->fields[3].code.data());
}

void read(std::istream& in, ::vnx::User& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::User& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::User& value) {
	value.accept(visitor);
}

} // vnx
