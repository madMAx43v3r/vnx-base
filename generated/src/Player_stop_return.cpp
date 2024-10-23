
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/Player_stop_return.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 Player_stop_return::VNX_TYPE_HASH(0xe722f78b3a61d58full);
const vnx::Hash64 Player_stop_return::VNX_CODE_HASH(0xe4b9171a47c752cull);

vnx::Hash64 Player_stop_return::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string Player_stop_return::get_type_name() const {
	return "vnx.Player.stop.return";
}

const vnx::TypeCode* Player_stop_return::get_type_code() const {
	return vnx::vnx_native_type_code_Player_stop_return;
}

std::shared_ptr<Player_stop_return> Player_stop_return::create() {
	return std::make_shared<Player_stop_return>();
}

std::shared_ptr<vnx::Value> Player_stop_return::clone() const {
	return std::make_shared<Player_stop_return>(*this);
}

void Player_stop_return::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void Player_stop_return::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void Player_stop_return::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_Player_stop_return;
	_visitor.type_begin(*_type_code);
	_visitor.type_end(*_type_code);
}

void Player_stop_return::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.Player.stop.return\"";
	_out << "}";
}

void Player_stop_return::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object Player_stop_return::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.Player.stop.return";
	return _object;
}

void Player_stop_return::from_object(const vnx::Object& _object) {
}

vnx::Variant Player_stop_return::get_field(const std::string& _name) const {
	return vnx::Variant();
}

void Player_stop_return::set_field(const std::string& _name, const vnx::Variant& _value) {
}

/// \private
std::ostream& operator<<(std::ostream& _out, const Player_stop_return& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, Player_stop_return& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* Player_stop_return::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> Player_stop_return::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.Player.stop.return";
	type_code->type_hash = vnx::Hash64(0xe722f78b3a61d58full);
	type_code->code_hash = vnx::Hash64(0xe4b9171a47c752cull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_return = true;
	type_code->native_size = sizeof(::vnx::Player_stop_return);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<Player_stop_return>(); };
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::Player_stop_return& value, const TypeCode* type_code, const uint16_t* code) {
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
	in.read(type_code->total_field_size);
	if(type_code->is_matched) {
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::Player_stop_return& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_Player_stop_return;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::Player_stop_return>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
}

void read(std::istream& in, ::vnx::Player_stop_return& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::Player_stop_return& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::Player_stop_return& value) {
	value.accept(visitor);
}

} // vnx
