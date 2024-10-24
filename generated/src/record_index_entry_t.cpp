
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/record_index_entry_t.hxx>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 record_index_entry_t::VNX_TYPE_HASH(0x84d2685378679269ull);
const vnx::Hash64 record_index_entry_t::VNX_CODE_HASH(0xfcb6a09b87b96bc2ull);

vnx::Hash64 record_index_entry_t::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string record_index_entry_t::get_type_name() const {
	return "vnx.record_index_entry_t";
}

const vnx::TypeCode* record_index_entry_t::get_type_code() const {
	return vnx::vnx_native_type_code_record_index_entry_t;
}

std::shared_ptr<record_index_entry_t> record_index_entry_t::create() {
	return std::make_shared<record_index_entry_t>();
}

std::shared_ptr<record_index_entry_t> record_index_entry_t::clone() const {
	return std::make_shared<record_index_entry_t>(*this);
}

void record_index_entry_t::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void record_index_entry_t::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void record_index_entry_t::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_record_index_entry_t;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, pos);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, time);
	_visitor.type_end(*_type_code);
}

void record_index_entry_t::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"pos\": "; vnx::write(_out, pos);
	_out << ", \"time\": "; vnx::write(_out, time);
	_out << "}";
}

void record_index_entry_t::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object record_index_entry_t::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.record_index_entry_t";
	_object["pos"] = pos;
	_object["time"] = time;
	return _object;
}

void record_index_entry_t::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "pos") {
			_entry.second.to(pos);
		} else if(_entry.first == "time") {
			_entry.second.to(time);
		}
	}
}

vnx::Variant record_index_entry_t::get_field(const std::string& _name) const {
	if(_name == "pos") {
		return vnx::Variant(pos);
	}
	if(_name == "time") {
		return vnx::Variant(time);
	}
	return vnx::Variant();
}

void record_index_entry_t::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "pos") {
		_value.to(pos);
	} else if(_name == "time") {
		_value.to(time);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const record_index_entry_t& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, record_index_entry_t& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* record_index_entry_t::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> record_index_entry_t::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.record_index_entry_t";
	type_code->type_hash = vnx::Hash64(0x84d2685378679269ull);
	type_code->code_hash = vnx::Hash64(0xfcb6a09b87b96bc2ull);
	type_code->is_native = true;
	type_code->native_size = sizeof(::vnx::record_index_entry_t);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<vnx::Struct<record_index_entry_t>>(); };
	type_code->fields.resize(2);
	{
		auto& field = type_code->fields[0];
		field.data_size = 8;
		field.name = "pos";
		field.value = vnx::to_string(-1);
		field.code = {8};
	}
	{
		auto& field = type_code->fields[1];
		field.data_size = 8;
		field.name = "time";
		field.code = {8};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::record_index_entry_t& value, const TypeCode* type_code, const uint16_t* code) {
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
			vnx::read_value(_buf + _field->offset, value.pos, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[1]) {
			vnx::read_value(_buf + _field->offset, value.time, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::record_index_entry_t& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_record_index_entry_t;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::record_index_entry_t>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	auto* const _buf = out.write(16);
	vnx::write_value(_buf + 0, value.pos);
	vnx::write_value(_buf + 8, value.time);
}

void read(std::istream& in, ::vnx::record_index_entry_t& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::record_index_entry_t& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::record_index_entry_t& value) {
	value.accept(visitor);
}

bool is_equivalent<::vnx::record_index_entry_t>::operator()(const uint16_t* code, const TypeCode* type_code) {
	if(code[0] != CODE_STRUCT || !type_code) {
		return false;
	}
	type_code = type_code->depends[code[1]];
	return type_code->type_hash == ::vnx::record_index_entry_t::VNX_TYPE_HASH && type_code->is_equivalent;
}

} // vnx
