
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/Process_set_debug.hxx>
#include <vnx/Process_set_debug_return.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 Process_set_debug::VNX_TYPE_HASH(0xdc4916fdbd8d5616ull);
const vnx::Hash64 Process_set_debug::VNX_CODE_HASH(0x9be03d68228cdda9ull);

vnx::Hash64 Process_set_debug::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string Process_set_debug::get_type_name() const {
	return "vnx.Process.set_debug";
}

const vnx::TypeCode* Process_set_debug::get_type_code() const {
	return vnx::vnx_native_type_code_Process_set_debug;
}

std::shared_ptr<Process_set_debug> Process_set_debug::create() {
	return std::make_shared<Process_set_debug>();
}

std::shared_ptr<vnx::Value> Process_set_debug::clone() const {
	return std::make_shared<Process_set_debug>(*this);
}

void Process_set_debug::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void Process_set_debug::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void Process_set_debug::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_Process_set_debug;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, level);
	_visitor.type_end(*_type_code);
}

void Process_set_debug::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.Process.set_debug\"";
	_out << ", \"level\": "; vnx::write(_out, level);
	_out << "}";
}

void Process_set_debug::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object Process_set_debug::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.Process.set_debug";
	_object["level"] = level;
	return _object;
}

void Process_set_debug::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "level") {
			_entry.second.to(level);
		}
	}
}

vnx::Variant Process_set_debug::get_field(const std::string& _name) const {
	if(_name == "level") {
		return vnx::Variant(level);
	}
	return vnx::Variant();
}

void Process_set_debug::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "level") {
		_value.to(level);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const Process_set_debug& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, Process_set_debug& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* Process_set_debug::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> Process_set_debug::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.Process.set_debug";
	type_code->type_hash = vnx::Hash64(0xdc4916fdbd8d5616ull);
	type_code->code_hash = vnx::Hash64(0x9be03d68228cdda9ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->native_size = sizeof(::vnx::Process_set_debug);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<Process_set_debug>(); };
	type_code->return_type = ::vnx::Process_set_debug_return::static_get_type_code();
	type_code->fields.resize(1);
	{
		auto& field = type_code->fields[0];
		field.data_size = 4;
		field.name = "level";
		field.code = {7};
	}
	type_code->permission = "vnx.permission_e.LOCAL";
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::Process_set_debug& value, const TypeCode* type_code, const uint16_t* code) {
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
			vnx::read_value(_buf + _field->offset, value.level, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::Process_set_debug& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_Process_set_debug;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::Process_set_debug>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	auto* const _buf = out.write(4);
	vnx::write_value(_buf + 0, value.level);
}

void read(std::istream& in, ::vnx::Process_set_debug& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::Process_set_debug& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::Process_set_debug& value) {
	value.accept(visitor);
}

} // vnx
