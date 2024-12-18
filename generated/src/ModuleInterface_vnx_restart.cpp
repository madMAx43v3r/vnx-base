
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/ModuleInterface_vnx_restart.hxx>
#include <vnx/ModuleInterface_vnx_restart_return.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 ModuleInterface_vnx_restart::VNX_TYPE_HASH(0x9e95dc280cecca1bull);
const vnx::Hash64 ModuleInterface_vnx_restart::VNX_CODE_HASH(0x8aa7620c408cb5baull);

vnx::Hash64 ModuleInterface_vnx_restart::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string ModuleInterface_vnx_restart::get_type_name() const {
	return "vnx.ModuleInterface.vnx_restart";
}

const vnx::TypeCode* ModuleInterface_vnx_restart::get_type_code() const {
	return vnx::vnx_native_type_code_ModuleInterface_vnx_restart;
}

std::shared_ptr<ModuleInterface_vnx_restart> ModuleInterface_vnx_restart::create() {
	return std::make_shared<ModuleInterface_vnx_restart>();
}

std::shared_ptr<vnx::Value> ModuleInterface_vnx_restart::clone() const {
	return std::make_shared<ModuleInterface_vnx_restart>(*this);
}

void ModuleInterface_vnx_restart::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void ModuleInterface_vnx_restart::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void ModuleInterface_vnx_restart::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_ModuleInterface_vnx_restart;
	_visitor.type_begin(*_type_code);
	_visitor.type_end(*_type_code);
}

void ModuleInterface_vnx_restart::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.ModuleInterface.vnx_restart\"";
	_out << "}";
}

void ModuleInterface_vnx_restart::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object ModuleInterface_vnx_restart::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.ModuleInterface.vnx_restart";
	return _object;
}

void ModuleInterface_vnx_restart::from_object(const vnx::Object& _object) {
}

vnx::Variant ModuleInterface_vnx_restart::get_field(const std::string& _name) const {
	return vnx::Variant();
}

void ModuleInterface_vnx_restart::set_field(const std::string& _name, const vnx::Variant& _value) {
}

/// \private
std::ostream& operator<<(std::ostream& _out, const ModuleInterface_vnx_restart& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, ModuleInterface_vnx_restart& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* ModuleInterface_vnx_restart::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> ModuleInterface_vnx_restart::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.ModuleInterface.vnx_restart";
	type_code->type_hash = vnx::Hash64(0x9e95dc280cecca1bull);
	type_code->code_hash = vnx::Hash64(0x8aa7620c408cb5baull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->native_size = sizeof(::vnx::ModuleInterface_vnx_restart);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<ModuleInterface_vnx_restart>(); };
	type_code->return_type = ::vnx::ModuleInterface_vnx_restart_return::static_get_type_code();
	type_code->permission = "vnx.permission_e.RESTART";
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::ModuleInterface_vnx_restart& value, const TypeCode* type_code, const uint16_t* code) {
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

void write(TypeOutput& out, const ::vnx::ModuleInterface_vnx_restart& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_ModuleInterface_vnx_restart;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::ModuleInterface_vnx_restart>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
}

void read(std::istream& in, ::vnx::ModuleInterface_vnx_restart& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::ModuleInterface_vnx_restart& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::ModuleInterface_vnx_restart& value) {
	value.accept(visitor);
}

} // vnx
