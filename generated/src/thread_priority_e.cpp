
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/thread_priority_e.hxx>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 thread_priority_e::VNX_TYPE_HASH(0x2718d6bed61ee41cull);
const vnx::Hash64 thread_priority_e::VNX_CODE_HASH(0x57a7eaee60626dddull);

vnx::Hash64 thread_priority_e::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string thread_priority_e::get_type_name() const {
	return "vnx.thread_priority_e";
}

const vnx::TypeCode* thread_priority_e::get_type_code() const {
	return vnx::vnx_native_type_code_thread_priority_e;
}

std::shared_ptr<thread_priority_e> thread_priority_e::create() {
	return std::make_shared<thread_priority_e>();
}

std::shared_ptr<thread_priority_e> thread_priority_e::clone() const {
	return std::make_shared<thread_priority_e>(*this);
}

void thread_priority_e::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void thread_priority_e::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

vnx::bool_t thread_priority_e::is_valid() const {
	switch(value) {
		case HIGH: return true;
		case LOW: return true;
		case NORMAL: return true;
		case VERY_HIGH: return true;
		case VERY_LOW: return true;
	}
	return false;
}

std::string thread_priority_e::to_string() const {
	switch(value) {
		case HIGH: return "\"HIGH\"";
		case LOW: return "\"LOW\"";
		case NORMAL: return "\"NORMAL\"";
		case VERY_HIGH: return "\"VERY_HIGH\"";
		case VERY_LOW: return "\"VERY_LOW\"";
	}
	return std::to_string(value);
}

std::string thread_priority_e::to_string_value() const {
	switch(value) {
		case HIGH: return "HIGH";
		case LOW: return "LOW";
		case NORMAL: return "NORMAL";
		case VERY_HIGH: return "VERY_HIGH";
		case VERY_LOW: return "VERY_LOW";
	}
	return std::to_string(value);
}

std::string thread_priority_e::to_string_value_full() const {
	switch(value) {
		case HIGH: return "vnx.thread_priority_e.HIGH";
		case LOW: return "vnx.thread_priority_e.LOW";
		case NORMAL: return "vnx.thread_priority_e.NORMAL";
		case VERY_HIGH: return "vnx.thread_priority_e.VERY_HIGH";
		case VERY_LOW: return "vnx.thread_priority_e.VERY_LOW";
	}
	return std::to_string(value);
}

void thread_priority_e::from_string(const std::string& _str) {
	std::string _name;
	vnx::from_string(_str, _name);
	from_string_value(_name);
}

void thread_priority_e::from_string_value(const std::string& _name) {
	vnx::Variant var;
	vnx::from_string_value(_name, var);
	if(var.is_string()) {
		if(_name == "HIGH") value = HIGH;
		else if(_name == "LOW") value = LOW;
		else if(_name == "NORMAL") value = NORMAL;
		else if(_name == "VERY_HIGH") value = VERY_HIGH;
		else if(_name == "VERY_LOW") value = VERY_LOW;
		else value = enum_t(vnx::hash64(_name));
	} else {
		value = enum_t(std::stoul(_name.c_str(), nullptr, 0));
	}
}

void thread_priority_e::accept(vnx::Visitor& _visitor) const {
	std::string _name;
	switch(value) {
		case HIGH: _name = "HIGH"; break;
		case LOW: _name = "LOW"; break;
		case NORMAL: _name = "NORMAL"; break;
		case VERY_HIGH: _name = "VERY_HIGH"; break;
		case VERY_LOW: _name = "VERY_LOW"; break;
	}
	_visitor.enum_value(value, _name);
}

void thread_priority_e::write(std::ostream& _out) const {
	switch(value) {
		case HIGH: _out << "\"HIGH\""; break;
		case LOW: _out << "\"LOW\""; break;
		case NORMAL: _out << "\"NORMAL\""; break;
		case VERY_HIGH: _out << "\"VERY_HIGH\""; break;
		case VERY_LOW: _out << "\"VERY_LOW\""; break;
		default: _out << value;
	}
}

void thread_priority_e::read(std::istream& _in) {
	from_string_value(vnx::read(_in).to_string_value());
}

vnx::Object thread_priority_e::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.thread_priority_e";
	_object["value"] = value;
	return _object;
}

void thread_priority_e::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "value") {
			_entry.second.to(value);
		}
	}
}

vnx::Variant thread_priority_e::get_field(const std::string& _name) const {
	if(_name == "value") {
		return vnx::Variant(value);
	}
	return vnx::Variant();
}

void thread_priority_e::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "value") {
		_value.to(value);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const thread_priority_e& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, thread_priority_e& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* thread_priority_e::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> thread_priority_e::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.thread_priority_e";
	type_code->type_hash = vnx::Hash64(0x2718d6bed61ee41cull);
	type_code->code_hash = vnx::Hash64(0x57a7eaee60626dddull);
	type_code->is_native = true;
	type_code->is_enum = true;
	type_code->native_size = sizeof(::vnx::thread_priority_e);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<vnx::Struct<thread_priority_e>>(); };
	type_code->fields.resize(1);
	{
		auto& field = type_code->fields[0];
		field.data_size = 4;
		field.name = "value";
		field.code = {3};
	}
	type_code->enum_map[3488385431] = "HIGH";
	type_code->enum_map[289856195] = "LOW";
	type_code->enum_map[658629154] = "NORMAL";
	type_code->enum_map[934099048] = "VERY_HIGH";
	type_code->enum_map[253285142] = "VERY_LOW";
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::thread_priority_e& value, const TypeCode* type_code, const uint16_t* code) {
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
		Variant tmp;
		vnx::read(in, tmp, type_code, code);
		if(tmp.is_string()) {
			vnx::from_string(tmp.to_string(), value);
		} else if(tmp.is_ulong()) {
			value = ::vnx::thread_priority_e::enum_t(tmp.to<uint32_t>());
		} else {
			value = ::vnx::thread_priority_e();
		}
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
			vnx::read_value(_buf + _field->offset, value.value, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::thread_priority_e& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_STRING) {
		vnx::write(out, value.to_string_value(), nullptr, code);
		return;
	}
	if(code && code[0] == CODE_UINT32) {
		vnx::write(out, value.value, nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_thread_priority_e;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::thread_priority_e>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	auto* const _buf = out.write(4);
	vnx::write_value(_buf + 0, value.value);
}

void read(std::istream& in, ::vnx::thread_priority_e& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::thread_priority_e& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::thread_priority_e& value) {
	value.accept(visitor);
}

void read(TypeInput& in, ::vnx::thread_priority_e::enum_t& value, const TypeCode* type_code, const uint16_t* code) {
	uint32_t tmp = 0;
	vnx::read(in, tmp, type_code, code);
	value = ::vnx::thread_priority_e::enum_t(tmp);
}

void write(TypeOutput& out, const ::vnx::thread_priority_e::enum_t& value, const TypeCode* type_code, const uint16_t* code) {
	vnx::write(out, uint32_t(value), type_code, code);
}

template<>
std::string to_string(const ::vnx::thread_priority_e& _value) {
	return _value.to_string();
}

template<>
std::string to_string_value(const ::vnx::thread_priority_e& _value) {
	return _value.to_string_value();
}

template<>
std::string to_string_value_full(const ::vnx::thread_priority_e& _value) {
	return _value.to_string_value_full();
}

template<>
std::string to_string(const ::vnx::thread_priority_e::enum_t& _value) {
	return ::vnx::thread_priority_e(_value).to_string();
}

template<>
std::string to_string_value(const ::vnx::thread_priority_e::enum_t& _value) {
	return ::vnx::thread_priority_e(_value).to_string_value();
}

template<>
std::string to_string_value_full(const ::vnx::thread_priority_e::enum_t& _value) {
	return ::vnx::thread_priority_e(_value).to_string_value_full();
}

bool is_equivalent<::vnx::thread_priority_e>::operator()(const uint16_t* code, const TypeCode* type_code) {
	if(code[0] != CODE_STRUCT || !type_code) {
		return false;
	}
	type_code = type_code->depends[code[1]];
	return type_code->type_hash == ::vnx::thread_priority_e::VNX_TYPE_HASH && type_code->is_equivalent;
}

} // vnx
