
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/Process_get_topic_info.hxx>
#include <vnx/Process_get_topic_info_return.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 Process_get_topic_info::VNX_TYPE_HASH(0xa4d23f22700c78c4ull);
const vnx::Hash64 Process_get_topic_info::VNX_CODE_HASH(0xe78a828d4d6e4d85ull);

vnx::Hash64 Process_get_topic_info::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string Process_get_topic_info::get_type_name() const {
	return "vnx.Process.get_topic_info";
}

const vnx::TypeCode* Process_get_topic_info::get_type_code() const {
	return vnx::vnx_native_type_code_Process_get_topic_info;
}

std::shared_ptr<Process_get_topic_info> Process_get_topic_info::create() {
	return std::make_shared<Process_get_topic_info>();
}

std::shared_ptr<vnx::Value> Process_get_topic_info::clone() const {
	return std::make_shared<Process_get_topic_info>(*this);
}

void Process_get_topic_info::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void Process_get_topic_info::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void Process_get_topic_info::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_Process_get_topic_info;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, include_domains);
	_visitor.type_end(*_type_code);
}

void Process_get_topic_info::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.Process.get_topic_info\"";
	_out << ", \"include_domains\": "; vnx::write(_out, include_domains);
	_out << "}";
}

void Process_get_topic_info::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object Process_get_topic_info::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.Process.get_topic_info";
	_object["include_domains"] = include_domains;
	return _object;
}

void Process_get_topic_info::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "include_domains") {
			_entry.second.to(include_domains);
		}
	}
}

vnx::Variant Process_get_topic_info::get_field(const std::string& _name) const {
	if(_name == "include_domains") {
		return vnx::Variant(include_domains);
	}
	return vnx::Variant();
}

void Process_get_topic_info::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "include_domains") {
		_value.to(include_domains);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const Process_get_topic_info& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, Process_get_topic_info& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* Process_get_topic_info::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> Process_get_topic_info::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.Process.get_topic_info";
	type_code->type_hash = vnx::Hash64(0xa4d23f22700c78c4ull);
	type_code->code_hash = vnx::Hash64(0xe78a828d4d6e4d85ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->native_size = sizeof(::vnx::Process_get_topic_info);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<Process_get_topic_info>(); };
	type_code->is_const = true;
	type_code->return_type = ::vnx::Process_get_topic_info_return::static_get_type_code();
	type_code->fields.resize(1);
	{
		auto& field = type_code->fields[0];
		field.data_size = 1;
		field.name = "include_domains";
		field.code = {31};
	}
	type_code->permission = "vnx.permission_e.VIEW";
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::Process_get_topic_info& value, const TypeCode* type_code, const uint16_t* code) {
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
	const char* const _buf = in.read(type_code->total_field_size);
	if(type_code->is_matched) {
		if(const auto* const _field = type_code->field_map[0]) {
			vnx::read_value(_buf + _field->offset, value.include_domains, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::Process_get_topic_info& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_Process_get_topic_info;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::Process_get_topic_info>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(1);
	vnx::write_value(_buf + 0, value.include_domains);
}

void read(std::istream& in, ::vnx::Process_get_topic_info& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::Process_get_topic_info& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::Process_get_topic_info& value) {
	value.accept(visitor);
}

} // vnx
