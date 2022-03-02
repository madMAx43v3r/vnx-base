
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/Process_pause_log.hxx>
#include <vnx/Process_pause_log_return.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 Process_pause_log::VNX_TYPE_HASH(0xab840945a4069720ull);
const vnx::Hash64 Process_pause_log::VNX_CODE_HASH(0x551303281dd2595aull);

vnx::Hash64 Process_pause_log::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string Process_pause_log::get_type_name() const {
	return "vnx.Process.pause_log";
}

const vnx::TypeCode* Process_pause_log::get_type_code() const {
	return vnx::vnx_native_type_code_Process_pause_log;
}

std::shared_ptr<Process_pause_log> Process_pause_log::create() {
	return std::make_shared<Process_pause_log>();
}

std::shared_ptr<vnx::Value> Process_pause_log::clone() const {
	return std::make_shared<Process_pause_log>(*this);
}

void Process_pause_log::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void Process_pause_log::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void Process_pause_log::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_Process_pause_log;
	_visitor.type_begin(*_type_code);
	_visitor.type_end(*_type_code);
}

void Process_pause_log::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.Process.pause_log\"";
	_out << "}";
}

void Process_pause_log::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object Process_pause_log::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.Process.pause_log";
	return _object;
}

void Process_pause_log::from_object(const vnx::Object& _object) {
}

vnx::Variant Process_pause_log::get_field(const std::string& _name) const {
	return vnx::Variant();
}

void Process_pause_log::set_field(const std::string& _name, const vnx::Variant& _value) {
}

/// \private
std::ostream& operator<<(std::ostream& _out, const Process_pause_log& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, Process_pause_log& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* Process_pause_log::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> Process_pause_log::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.Process.pause_log";
	type_code->type_hash = vnx::Hash64(0xab840945a4069720ull);
	type_code->code_hash = vnx::Hash64(0x551303281dd2595aull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->native_size = sizeof(::vnx::Process_pause_log);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<Process_pause_log>(); };
	type_code->methods.resize(0);
	type_code->return_type = ::vnx::Process_pause_log_return::static_get_type_code();
	type_code->permission = "vnx.permission_e.LOCAL";
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::Process_pause_log& value, const TypeCode* type_code, const uint16_t* code) {
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

void write(TypeOutput& out, const ::vnx::Process_pause_log& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_Process_pause_log;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::Process_pause_log>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
}

void read(std::istream& in, ::vnx::Process_pause_log& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::Process_pause_log& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::Process_pause_log& value) {
	value.accept(visitor);
}

} // vnx
