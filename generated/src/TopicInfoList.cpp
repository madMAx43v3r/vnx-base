
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/TopicInfoList.hxx>
#include <vnx/TopicInfo.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 TopicInfoList::VNX_TYPE_HASH(0x6c3f27749539f1c2ull);
const vnx::Hash64 TopicInfoList::VNX_CODE_HASH(0x8bba10a77036debaull);

vnx::Hash64 TopicInfoList::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string TopicInfoList::get_type_name() const {
	return "vnx.TopicInfoList";
}

const vnx::TypeCode* TopicInfoList::get_type_code() const {
	return vnx::vnx_native_type_code_TopicInfoList;
}

std::shared_ptr<TopicInfoList> TopicInfoList::create() {
	return std::make_shared<TopicInfoList>();
}

std::shared_ptr<vnx::Value> TopicInfoList::clone() const {
	return std::make_shared<TopicInfoList>(*this);
}

void TopicInfoList::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void TopicInfoList::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void TopicInfoList::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_TopicInfoList;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, time);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, process);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, list);
	_visitor.type_end(*_type_code);
}

void TopicInfoList::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.TopicInfoList\"";
	_out << ", \"time\": "; vnx::write(_out, time);
	_out << ", \"process\": "; vnx::write(_out, process);
	_out << ", \"list\": "; vnx::write(_out, list);
	_out << "}";
}

void TopicInfoList::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object TopicInfoList::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.TopicInfoList";
	_object["time"] = time;
	_object["process"] = process;
	_object["list"] = list;
	return _object;
}

void TopicInfoList::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "list") {
			_entry.second.to(list);
		} else if(_entry.first == "process") {
			_entry.second.to(process);
		} else if(_entry.first == "time") {
			_entry.second.to(time);
		}
	}
}

vnx::Variant TopicInfoList::get_field(const std::string& _name) const {
	if(_name == "time") {
		return vnx::Variant(time);
	}
	if(_name == "process") {
		return vnx::Variant(process);
	}
	if(_name == "list") {
		return vnx::Variant(list);
	}
	return vnx::Variant();
}

void TopicInfoList::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "time") {
		_value.to(time);
	} else if(_name == "process") {
		_value.to(process);
	} else if(_name == "list") {
		_value.to(list);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const TopicInfoList& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, TopicInfoList& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* TopicInfoList::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> TopicInfoList::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.TopicInfoList";
	type_code->type_hash = vnx::Hash64(0x6c3f27749539f1c2ull);
	type_code->code_hash = vnx::Hash64(0x8bba10a77036debaull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->native_size = sizeof(::vnx::TopicInfoList);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<TopicInfoList>(); };
	type_code->depends.resize(1);
	type_code->depends[0] = ::vnx::TopicInfo::static_get_type_code();
	type_code->methods.resize(0);
	type_code->fields.resize(3);
	{
		auto& field = type_code->fields[0];
		field.data_size = 8;
		field.name = "time";
		field.code = {8};
	}
	{
		auto& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "process";
		field.code = {32};
	}
	{
		auto& field = type_code->fields[2];
		field.is_extended = true;
		field.name = "list";
		field.code = {12, 19, 0};
	}
	type_code->build();
	return type_code;
}

std::shared_ptr<vnx::Value> TopicInfoList::vnx_call_switch(std::shared_ptr<const vnx::Value> _method) {
	switch(_method->get_type_hash()) {
	}
	return nullptr;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::TopicInfoList& value, const TypeCode* type_code, const uint16_t* code) {
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
			vnx::read_value(_buf + _field->offset, value.time, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 1: vnx::read(in, value.process, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.list, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::TopicInfoList& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_TopicInfoList;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::TopicInfoList>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(8);
	vnx::write_value(_buf + 0, value.time);
	vnx::write(out, value.process, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.list, type_code, type_code->fields[2].code.data());
}

void read(std::istream& in, ::vnx::TopicInfoList& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::TopicInfoList& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::TopicInfoList& value) {
	value.accept(visitor);
}

} // vnx
