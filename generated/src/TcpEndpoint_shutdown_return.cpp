
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/TcpEndpoint_shutdown_return.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 TcpEndpoint_shutdown_return::VNX_TYPE_HASH(0x4cd7688b73900f6eull);
const vnx::Hash64 TcpEndpoint_shutdown_return::VNX_CODE_HASH(0x8c0fdd1ba290ee72ull);

vnx::Hash64 TcpEndpoint_shutdown_return::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string TcpEndpoint_shutdown_return::get_type_name() const {
	return "vnx.TcpEndpoint.shutdown.return";
}

const vnx::TypeCode* TcpEndpoint_shutdown_return::get_type_code() const {
	return vnx::vnx_native_type_code_TcpEndpoint_shutdown_return;
}

std::shared_ptr<TcpEndpoint_shutdown_return> TcpEndpoint_shutdown_return::create() {
	return std::make_shared<TcpEndpoint_shutdown_return>();
}

std::shared_ptr<vnx::Value> TcpEndpoint_shutdown_return::clone() const {
	return std::make_shared<TcpEndpoint_shutdown_return>(*this);
}

void TcpEndpoint_shutdown_return::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void TcpEndpoint_shutdown_return::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void TcpEndpoint_shutdown_return::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_TcpEndpoint_shutdown_return;
	_visitor.type_begin(*_type_code);
	_visitor.type_end(*_type_code);
}

void TcpEndpoint_shutdown_return::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.TcpEndpoint.shutdown.return\"";
	_out << "}";
}

void TcpEndpoint_shutdown_return::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object TcpEndpoint_shutdown_return::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.TcpEndpoint.shutdown.return";
	return _object;
}

void TcpEndpoint_shutdown_return::from_object(const vnx::Object& _object) {
}

vnx::Variant TcpEndpoint_shutdown_return::get_field(const std::string& _name) const {
	return vnx::Variant();
}

void TcpEndpoint_shutdown_return::set_field(const std::string& _name, const vnx::Variant& _value) {
}

/// \private
std::ostream& operator<<(std::ostream& _out, const TcpEndpoint_shutdown_return& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, TcpEndpoint_shutdown_return& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* TcpEndpoint_shutdown_return::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> TcpEndpoint_shutdown_return::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.TcpEndpoint.shutdown.return";
	type_code->type_hash = vnx::Hash64(0x4cd7688b73900f6eull);
	type_code->code_hash = vnx::Hash64(0x8c0fdd1ba290ee72ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_return = true;
	type_code->native_size = sizeof(::vnx::TcpEndpoint_shutdown_return);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<TcpEndpoint_shutdown_return>(); };
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::TcpEndpoint_shutdown_return& value, const TypeCode* type_code, const uint16_t* code) {
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

void write(TypeOutput& out, const ::vnx::TcpEndpoint_shutdown_return& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_TcpEndpoint_shutdown_return;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::TcpEndpoint_shutdown_return>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
}

void read(std::istream& in, ::vnx::TcpEndpoint_shutdown_return& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::TcpEndpoint_shutdown_return& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::TcpEndpoint_shutdown_return& value) {
	value.accept(visitor);
}

} // vnx
