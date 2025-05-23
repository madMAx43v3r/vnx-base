
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/Player_seek_by_count.hxx>
#include <vnx/Player_seek_by_count_return.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 Player_seek_by_count::VNX_TYPE_HASH(0x8a3e8bcc0f1a9b93ull);
const vnx::Hash64 Player_seek_by_count::VNX_CODE_HASH(0x86efca5624406aa7ull);

vnx::Hash64 Player_seek_by_count::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string Player_seek_by_count::get_type_name() const {
	return "vnx.Player.seek_by_count";
}

const vnx::TypeCode* Player_seek_by_count::get_type_code() const {
	return vnx::vnx_native_type_code_Player_seek_by_count;
}

std::shared_ptr<Player_seek_by_count> Player_seek_by_count::create() {
	return std::make_shared<Player_seek_by_count>();
}

std::shared_ptr<vnx::Value> Player_seek_by_count::clone() const {
	return std::make_shared<Player_seek_by_count>(*this);
}

void Player_seek_by_count::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void Player_seek_by_count::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void Player_seek_by_count::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_Player_seek_by_count;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, delta_count);
	_visitor.type_end(*_type_code);
}

void Player_seek_by_count::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.Player.seek_by_count\"";
	_out << ", \"delta_count\": "; vnx::write(_out, delta_count);
	_out << "}";
}

void Player_seek_by_count::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object Player_seek_by_count::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.Player.seek_by_count";
	_object["delta_count"] = delta_count;
	return _object;
}

void Player_seek_by_count::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "delta_count") {
			_entry.second.to(delta_count);
		}
	}
}

vnx::Variant Player_seek_by_count::get_field(const std::string& _name) const {
	if(_name == "delta_count") {
		return vnx::Variant(delta_count);
	}
	return vnx::Variant();
}

void Player_seek_by_count::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "delta_count") {
		_value.to(delta_count);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const Player_seek_by_count& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, Player_seek_by_count& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* Player_seek_by_count::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> Player_seek_by_count::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.Player.seek_by_count";
	type_code->type_hash = vnx::Hash64(0x8a3e8bcc0f1a9b93ull);
	type_code->code_hash = vnx::Hash64(0x86efca5624406aa7ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->native_size = sizeof(::vnx::Player_seek_by_count);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<Player_seek_by_count>(); };
	type_code->return_type = ::vnx::Player_seek_by_count_return::static_get_type_code();
	type_code->fields.resize(1);
	{
		auto& field = type_code->fields[0];
		field.data_size = 8;
		field.name = "delta_count";
		field.code = {8};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::Player_seek_by_count& value, const TypeCode* type_code, const uint16_t* code) {
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
			vnx::read_value(_buf + _field->offset, value.delta_count, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::Player_seek_by_count& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_Player_seek_by_count;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::Player_seek_by_count>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	auto* const _buf = out.write(8);
	vnx::write_value(_buf + 0, value.delta_count);
}

void read(std::istream& in, ::vnx::Player_seek_by_count& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::Player_seek_by_count& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::Player_seek_by_count& value) {
	value.accept(visitor);
}

} // vnx
