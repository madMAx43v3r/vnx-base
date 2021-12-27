
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/Player_pause.hxx>
#include <vnx/Player_pause_return.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 Player_pause::VNX_TYPE_HASH(0x439f7eeb5c70c3ffull);
const vnx::Hash64 Player_pause::VNX_CODE_HASH(0xacdd7a8d451001b8ull);

vnx::Hash64 Player_pause::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string Player_pause::get_type_name() const {
	return "vnx.Player.pause";
}

const vnx::TypeCode* Player_pause::get_type_code() const {
	return vnx::vnx_native_type_code_Player_pause;
}

std::shared_ptr<Player_pause> Player_pause::create() {
	return std::make_shared<Player_pause>();
}

std::shared_ptr<vnx::Value> Player_pause::clone() const {
	return std::make_shared<Player_pause>(*this);
}

void Player_pause::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void Player_pause::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void Player_pause::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_Player_pause;
	_visitor.type_begin(*_type_code);
	_visitor.type_end(*_type_code);
}

void Player_pause::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.Player.pause\"";
	_out << "}";
}

void Player_pause::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object Player_pause::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.Player.pause";
	return _object;
}

void Player_pause::from_object(const vnx::Object& _object) {
}

vnx::Variant Player_pause::get_field(const std::string& _name) const {
	return vnx::Variant();
}

void Player_pause::set_field(const std::string& _name, const vnx::Variant& _value) {
	throw std::logic_error("no such field: '" + _name + "'");
}

/// \private
std::ostream& operator<<(std::ostream& _out, const Player_pause& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, Player_pause& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* Player_pause::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> Player_pause::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.Player.pause";
	type_code->type_hash = vnx::Hash64(0x439f7eeb5c70c3ffull);
	type_code->code_hash = vnx::Hash64(0xacdd7a8d451001b8ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->native_size = sizeof(::vnx::Player_pause);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<Player_pause>(); };
	type_code->return_type = ::vnx::Player_pause_return::static_get_type_code();
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::Player_pause& value, const TypeCode* type_code, const uint16_t* code) {
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
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::Player_pause& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_Player_pause;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::Player_pause>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
}

void read(std::istream& in, ::vnx::Player_pause& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::Player_pause& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::Player_pause& value) {
	value.accept(visitor);
}

} // vnx
