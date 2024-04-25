
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/Sample.hxx>
#include <vnx/Hash64.hpp>
#include <vnx/Message.hxx>
#include <vnx/TopicPtr.hpp>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 Sample::VNX_TYPE_HASH(0x82bffc4ec21d2408ull);
const vnx::Hash64 Sample::VNX_CODE_HASH(0x83bca399538b39fdull);

vnx::Hash64 Sample::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string Sample::get_type_name() const {
	return "vnx.Sample";
}

const vnx::TypeCode* Sample::get_type_code() const {
	return vnx::vnx_native_type_code_Sample;
}

std::shared_ptr<Sample> Sample::create() {
	return std::make_shared<Sample>();
}

std::shared_ptr<vnx::Value> Sample::clone() const {
	return std::make_shared<Sample>(*this);
}

void Sample::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void Sample::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void Sample::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_Sample;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, stream);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, flags);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, hop_count);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, src_mac);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, seq_num);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, recv_time);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, topic);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, value);
	_visitor.type_end(*_type_code);
}

void Sample::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.Sample\"";
	_out << ", \"stream\": "; vnx::write(_out, stream);
	_out << ", \"flags\": "; vnx::write(_out, flags);
	_out << ", \"hop_count\": "; vnx::write(_out, hop_count);
	_out << ", \"src_mac\": "; vnx::write(_out, src_mac);
	_out << ", \"seq_num\": "; vnx::write(_out, seq_num);
	_out << ", \"recv_time\": "; vnx::write(_out, recv_time);
	_out << ", \"topic\": "; vnx::write(_out, topic);
	_out << ", \"value\": "; vnx::write(_out, value);
	_out << "}";
}

void Sample::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object Sample::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.Sample";
	_object["stream"] = stream;
	_object["flags"] = flags;
	_object["hop_count"] = hop_count;
	_object["src_mac"] = src_mac;
	_object["seq_num"] = seq_num;
	_object["recv_time"] = recv_time;
	_object["topic"] = topic;
	_object["value"] = value;
	return _object;
}

void Sample::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "flags") {
			_entry.second.to(flags);
		} else if(_entry.first == "hop_count") {
			_entry.second.to(hop_count);
		} else if(_entry.first == "recv_time") {
			_entry.second.to(recv_time);
		} else if(_entry.first == "seq_num") {
			_entry.second.to(seq_num);
		} else if(_entry.first == "src_mac") {
			_entry.second.to(src_mac);
		} else if(_entry.first == "stream") {
			_entry.second.to(stream);
		} else if(_entry.first == "topic") {
			_entry.second.to(topic);
		} else if(_entry.first == "value") {
			_entry.second.to(value);
		}
	}
}

vnx::Variant Sample::get_field(const std::string& _name) const {
	if(_name == "stream") {
		return vnx::Variant(stream);
	}
	if(_name == "flags") {
		return vnx::Variant(flags);
	}
	if(_name == "hop_count") {
		return vnx::Variant(hop_count);
	}
	if(_name == "src_mac") {
		return vnx::Variant(src_mac);
	}
	if(_name == "seq_num") {
		return vnx::Variant(seq_num);
	}
	if(_name == "recv_time") {
		return vnx::Variant(recv_time);
	}
	if(_name == "topic") {
		return vnx::Variant(topic);
	}
	if(_name == "value") {
		return vnx::Variant(value);
	}
	return vnx::Variant();
}

void Sample::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "stream") {
		_value.to(stream);
	} else if(_name == "flags") {
		_value.to(flags);
	} else if(_name == "hop_count") {
		_value.to(hop_count);
	} else if(_name == "src_mac") {
		_value.to(src_mac);
	} else if(_name == "seq_num") {
		_value.to(seq_num);
	} else if(_name == "recv_time") {
		_value.to(recv_time);
	} else if(_name == "topic") {
		_value.to(topic);
	} else if(_name == "value") {
		_value.to(value);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const Sample& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, Sample& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* Sample::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> Sample::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.Sample";
	type_code->type_hash = vnx::Hash64(0x82bffc4ec21d2408ull);
	type_code->code_hash = vnx::Hash64(0x83bca399538b39fdull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->native_size = sizeof(::vnx::Sample);
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::Message::static_get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<Sample>(); };
	type_code->fields.resize(8);
	{
		auto& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "stream";
		field.code = {4};
	}
	{
		auto& field = type_code->fields[1];
		field.data_size = 2;
		field.name = "flags";
		field.code = {2};
	}
	{
		auto& field = type_code->fields[2];
		field.data_size = 2;
		field.name = "hop_count";
		field.code = {2};
	}
	{
		auto& field = type_code->fields[3];
		field.is_extended = true;
		field.name = "src_mac";
		field.code = {4};
	}
	{
		auto& field = type_code->fields[4];
		field.data_size = 8;
		field.name = "seq_num";
		field.code = {4};
	}
	{
		auto& field = type_code->fields[5];
		field.data_size = 8;
		field.name = "recv_time";
		field.code = {8};
	}
	{
		auto& field = type_code->fields[6];
		field.is_extended = true;
		field.name = "topic";
		field.code = {12, 5};
	}
	{
		auto& field = type_code->fields[7];
		field.is_extended = true;
		field.name = "value";
		field.code = {16};
	}
	type_code->build();
	return type_code;
}

std::shared_ptr<vnx::Value> Sample::vnx_call_switch(std::shared_ptr<const vnx::Value> _method) {
	switch(_method->get_type_hash()) {
	}
	return nullptr;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::Sample& value, const TypeCode* type_code, const uint16_t* code) {
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
		if(const auto* const _field = type_code->field_map[1]) {
			vnx::read_value(_buf + _field->offset, value.flags, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[2]) {
			vnx::read_value(_buf + _field->offset, value.hop_count, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[4]) {
			vnx::read_value(_buf + _field->offset, value.seq_num, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[5]) {
			vnx::read_value(_buf + _field->offset, value.recv_time, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.stream, type_code, _field->code.data()); break;
			case 3: vnx::read(in, value.src_mac, type_code, _field->code.data()); break;
			case 6: vnx::read(in, value.topic, type_code, _field->code.data()); break;
			case 7: vnx::read(in, value.value, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::Sample& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_Sample;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::Sample>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	auto* const _buf = out.write(20);
	vnx::write_value(_buf + 0, value.flags);
	vnx::write_value(_buf + 2, value.hop_count);
	vnx::write_value(_buf + 4, value.seq_num);
	vnx::write_value(_buf + 12, value.recv_time);
	vnx::write(out, value.stream, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.src_mac, type_code, type_code->fields[3].code.data());
	vnx::write(out, value.topic, type_code, type_code->fields[6].code.data());
	vnx::write(out, value.value, type_code, type_code->fields[7].code.data());
}

void read(std::istream& in, ::vnx::Sample& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::Sample& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::Sample& value) {
	value.accept(visitor);
}

} // vnx
