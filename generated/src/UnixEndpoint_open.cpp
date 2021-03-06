
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/UnixEndpoint_open.hxx>
#include <vnx/UnixEndpoint_open_return.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 UnixEndpoint_open::VNX_TYPE_HASH(0x98f15b94f93fe16ull);
const vnx::Hash64 UnixEndpoint_open::VNX_CODE_HASH(0xa1c08f79e8cc82a2ull);

vnx::Hash64 UnixEndpoint_open::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string UnixEndpoint_open::get_type_name() const {
	return "vnx.UnixEndpoint.open";
}

const vnx::TypeCode* UnixEndpoint_open::get_type_code() const {
	return vnx::vnx_native_type_code_UnixEndpoint_open;
}

std::shared_ptr<UnixEndpoint_open> UnixEndpoint_open::create() {
	return std::make_shared<UnixEndpoint_open>();
}

std::shared_ptr<vnx::Value> UnixEndpoint_open::clone() const {
	return std::make_shared<UnixEndpoint_open>(*this);
}

void UnixEndpoint_open::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void UnixEndpoint_open::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void UnixEndpoint_open::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_UnixEndpoint_open;
	_visitor.type_begin(*_type_code);
	_visitor.type_end(*_type_code);
}

void UnixEndpoint_open::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.UnixEndpoint.open\"";
	_out << "}";
}

void UnixEndpoint_open::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object UnixEndpoint_open::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.UnixEndpoint.open";
	return _object;
}

void UnixEndpoint_open::from_object(const vnx::Object& _object) {
}

vnx::Variant UnixEndpoint_open::get_field(const std::string& _name) const {
	return vnx::Variant();
}

void UnixEndpoint_open::set_field(const std::string& _name, const vnx::Variant& _value) {
}

/// \private
std::ostream& operator<<(std::ostream& _out, const UnixEndpoint_open& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, UnixEndpoint_open& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* UnixEndpoint_open::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> UnixEndpoint_open::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.UnixEndpoint.open";
	type_code->type_hash = vnx::Hash64(0x98f15b94f93fe16ull);
	type_code->code_hash = vnx::Hash64(0xa1c08f79e8cc82a2ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->native_size = sizeof(::vnx::UnixEndpoint_open);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<UnixEndpoint_open>(); };
	type_code->is_const = true;
	type_code->return_type = ::vnx::UnixEndpoint_open_return::static_get_type_code();
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::UnixEndpoint_open& value, const TypeCode* type_code, const uint16_t* code) {
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

void write(TypeOutput& out, const ::vnx::UnixEndpoint_open& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_UnixEndpoint_open;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::UnixEndpoint_open>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
}

void read(std::istream& in, ::vnx::UnixEndpoint_open& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::UnixEndpoint_open& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::UnixEndpoint_open& value) {
	value.accept(visitor);
}

} // vnx
