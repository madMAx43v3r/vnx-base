
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/JRPC_Proxy_select_service_return.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 JRPC_Proxy_select_service_return::VNX_TYPE_HASH(0xdc7089e6c94148f7ull);
const vnx::Hash64 JRPC_Proxy_select_service_return::VNX_CODE_HASH(0xc944b4916d461860ull);

vnx::Hash64 JRPC_Proxy_select_service_return::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string JRPC_Proxy_select_service_return::get_type_name() const {
	return "vnx.JRPC_Proxy.select_service.return";
}

const vnx::TypeCode* JRPC_Proxy_select_service_return::get_type_code() const {
	return vnx::vnx_native_type_code_JRPC_Proxy_select_service_return;
}

std::shared_ptr<JRPC_Proxy_select_service_return> JRPC_Proxy_select_service_return::create() {
	return std::make_shared<JRPC_Proxy_select_service_return>();
}

std::shared_ptr<vnx::Value> JRPC_Proxy_select_service_return::clone() const {
	return std::make_shared<JRPC_Proxy_select_service_return>(*this);
}

void JRPC_Proxy_select_service_return::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void JRPC_Proxy_select_service_return::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void JRPC_Proxy_select_service_return::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_JRPC_Proxy_select_service_return;
	_visitor.type_begin(*_type_code);
	_visitor.type_end(*_type_code);
}

void JRPC_Proxy_select_service_return::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.JRPC_Proxy.select_service.return\"";
	_out << "}";
}

void JRPC_Proxy_select_service_return::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object JRPC_Proxy_select_service_return::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.JRPC_Proxy.select_service.return";
	return _object;
}

void JRPC_Proxy_select_service_return::from_object(const vnx::Object& _object) {
}

vnx::Variant JRPC_Proxy_select_service_return::get_field(const std::string& _name) const {
	return vnx::Variant();
}

void JRPC_Proxy_select_service_return::set_field(const std::string& _name, const vnx::Variant& _value) {
}

/// \private
std::ostream& operator<<(std::ostream& _out, const JRPC_Proxy_select_service_return& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, JRPC_Proxy_select_service_return& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* JRPC_Proxy_select_service_return::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> JRPC_Proxy_select_service_return::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.JRPC_Proxy.select_service.return";
	type_code->type_hash = vnx::Hash64(0xdc7089e6c94148f7ull);
	type_code->code_hash = vnx::Hash64(0xc944b4916d461860ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_return = true;
	type_code->native_size = sizeof(::vnx::JRPC_Proxy_select_service_return);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<JRPC_Proxy_select_service_return>(); };
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::JRPC_Proxy_select_service_return& value, const TypeCode* type_code, const uint16_t* code) {
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

void write(TypeOutput& out, const ::vnx::JRPC_Proxy_select_service_return& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_JRPC_Proxy_select_service_return;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::JRPC_Proxy_select_service_return>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
}

void read(std::istream& in, ::vnx::JRPC_Proxy_select_service_return& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::JRPC_Proxy_select_service_return& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::JRPC_Proxy_select_service_return& value) {
	value.accept(visitor);
}

} // vnx
