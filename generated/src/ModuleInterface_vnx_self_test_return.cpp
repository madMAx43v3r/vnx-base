
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/ModuleInterface_vnx_self_test_return.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 ModuleInterface_vnx_self_test_return::VNX_TYPE_HASH(0x8fa3e6609b3347c3ull);
const vnx::Hash64 ModuleInterface_vnx_self_test_return::VNX_CODE_HASH(0xfd08c52703d6bf2full);

vnx::Hash64 ModuleInterface_vnx_self_test_return::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string ModuleInterface_vnx_self_test_return::get_type_name() const {
	return "vnx.ModuleInterface.vnx_self_test.return";
}

const vnx::TypeCode* ModuleInterface_vnx_self_test_return::get_type_code() const {
	return vnx::vnx_native_type_code_ModuleInterface_vnx_self_test_return;
}

std::shared_ptr<ModuleInterface_vnx_self_test_return> ModuleInterface_vnx_self_test_return::create() {
	return std::make_shared<ModuleInterface_vnx_self_test_return>();
}

std::shared_ptr<vnx::Value> ModuleInterface_vnx_self_test_return::clone() const {
	return std::make_shared<ModuleInterface_vnx_self_test_return>(*this);
}

void ModuleInterface_vnx_self_test_return::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void ModuleInterface_vnx_self_test_return::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void ModuleInterface_vnx_self_test_return::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_ModuleInterface_vnx_self_test_return;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, _ret_0);
	_visitor.type_end(*_type_code);
}

void ModuleInterface_vnx_self_test_return::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.ModuleInterface.vnx_self_test.return\"";
	_out << ", \"_ret_0\": "; vnx::write(_out, _ret_0);
	_out << "}";
}

void ModuleInterface_vnx_self_test_return::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object ModuleInterface_vnx_self_test_return::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.ModuleInterface.vnx_self_test.return";
	_object["_ret_0"] = _ret_0;
	return _object;
}

void ModuleInterface_vnx_self_test_return::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "_ret_0") {
			_entry.second.to(_ret_0);
		}
	}
}

vnx::Variant ModuleInterface_vnx_self_test_return::get_field(const std::string& _name) const {
	if(_name == "_ret_0") {
		return vnx::Variant(_ret_0);
	}
	return vnx::Variant();
}

void ModuleInterface_vnx_self_test_return::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "_ret_0") {
		_value.to(_ret_0);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const ModuleInterface_vnx_self_test_return& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, ModuleInterface_vnx_self_test_return& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* ModuleInterface_vnx_self_test_return::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> ModuleInterface_vnx_self_test_return::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.ModuleInterface.vnx_self_test.return";
	type_code->type_hash = vnx::Hash64(0x8fa3e6609b3347c3ull);
	type_code->code_hash = vnx::Hash64(0xfd08c52703d6bf2full);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_return = true;
	type_code->native_size = sizeof(::vnx::ModuleInterface_vnx_self_test_return);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<ModuleInterface_vnx_self_test_return>(); };
	type_code->fields.resize(1);
	{
		auto& field = type_code->fields[0];
		field.data_size = 1;
		field.name = "_ret_0";
		field.code = {31};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::ModuleInterface_vnx_self_test_return& value, const TypeCode* type_code, const uint16_t* code) {
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
			vnx::read_value(_buf + _field->offset, value._ret_0, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::ModuleInterface_vnx_self_test_return& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_ModuleInterface_vnx_self_test_return;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::ModuleInterface_vnx_self_test_return>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	auto* const _buf = out.write(1);
	vnx::write_value(_buf + 0, value._ret_0);
}

void read(std::istream& in, ::vnx::ModuleInterface_vnx_self_test_return& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::ModuleInterface_vnx_self_test_return& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::ModuleInterface_vnx_self_test_return& value) {
	value.accept(visitor);
}

} // vnx
