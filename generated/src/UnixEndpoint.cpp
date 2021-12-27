
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/UnixEndpoint.hxx>
#include <vnx/Endpoint.hxx>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 UnixEndpoint::VNX_TYPE_HASH(0x8007f4ae5d6628e5ull);
const vnx::Hash64 UnixEndpoint::VNX_CODE_HASH(0x800f837d5f7f8e53ull);

vnx::Hash64 UnixEndpoint::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string UnixEndpoint::get_type_name() const {
	return "vnx.UnixEndpoint";
}

const vnx::TypeCode* UnixEndpoint::get_type_code() const {
	return vnx::vnx_native_type_code_UnixEndpoint;
}

std::shared_ptr<UnixEndpoint> UnixEndpoint::create() {
	return std::make_shared<UnixEndpoint>();
}

std::shared_ptr<vnx::Value> UnixEndpoint::clone() const {
	return std::make_shared<UnixEndpoint>(*this);
}

void UnixEndpoint::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void UnixEndpoint::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void UnixEndpoint::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_UnixEndpoint;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, send_buffer_size);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, receive_buffer_size);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, listen_queue_size);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, non_blocking);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, domain_path);
	_visitor.type_end(*_type_code);
}

void UnixEndpoint::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.UnixEndpoint\"";
	_out << ", \"send_buffer_size\": "; vnx::write(_out, send_buffer_size);
	_out << ", \"receive_buffer_size\": "; vnx::write(_out, receive_buffer_size);
	_out << ", \"listen_queue_size\": "; vnx::write(_out, listen_queue_size);
	_out << ", \"non_blocking\": "; vnx::write(_out, non_blocking);
	_out << ", \"domain_path\": "; vnx::write(_out, domain_path);
	_out << "}";
}

void UnixEndpoint::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object UnixEndpoint::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.UnixEndpoint";
	_object["send_buffer_size"] = send_buffer_size;
	_object["receive_buffer_size"] = receive_buffer_size;
	_object["listen_queue_size"] = listen_queue_size;
	_object["non_blocking"] = non_blocking;
	_object["domain_path"] = domain_path;
	return _object;
}

void UnixEndpoint::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "domain_path") {
			_entry.second.to(domain_path);
		} else if(_entry.first == "listen_queue_size") {
			_entry.second.to(listen_queue_size);
		} else if(_entry.first == "non_blocking") {
			_entry.second.to(non_blocking);
		} else if(_entry.first == "receive_buffer_size") {
			_entry.second.to(receive_buffer_size);
		} else if(_entry.first == "send_buffer_size") {
			_entry.second.to(send_buffer_size);
		}
	}
}

vnx::Variant UnixEndpoint::get_field(const std::string& _name) const {
	if(_name == "send_buffer_size") {
		return vnx::Variant(send_buffer_size);
	}
	if(_name == "receive_buffer_size") {
		return vnx::Variant(receive_buffer_size);
	}
	if(_name == "listen_queue_size") {
		return vnx::Variant(listen_queue_size);
	}
	if(_name == "non_blocking") {
		return vnx::Variant(non_blocking);
	}
	if(_name == "domain_path") {
		return vnx::Variant(domain_path);
	}
	return vnx::Variant();
}

void UnixEndpoint::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "send_buffer_size") {
		_value.to(send_buffer_size);
	} else if(_name == "receive_buffer_size") {
		_value.to(receive_buffer_size);
	} else if(_name == "listen_queue_size") {
		_value.to(listen_queue_size);
	} else if(_name == "non_blocking") {
		_value.to(non_blocking);
	} else if(_name == "domain_path") {
		_value.to(domain_path);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const UnixEndpoint& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, UnixEndpoint& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* UnixEndpoint::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> UnixEndpoint::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.UnixEndpoint";
	type_code->type_hash = vnx::Hash64(0x8007f4ae5d6628e5ull);
	type_code->code_hash = vnx::Hash64(0x800f837d5f7f8e53ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->native_size = sizeof(::vnx::UnixEndpoint);
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::Endpoint::static_get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<UnixEndpoint>(); };
	type_code->fields.resize(5);
	{
		auto& field = type_code->fields[0];
		field.data_size = 4;
		field.name = "send_buffer_size";
		field.value = vnx::to_string(0);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[1];
		field.data_size = 4;
		field.name = "receive_buffer_size";
		field.value = vnx::to_string(0);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[2];
		field.data_size = 4;
		field.name = "listen_queue_size";
		field.value = vnx::to_string(10);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[3];
		field.data_size = 1;
		field.name = "non_blocking";
		field.value = vnx::to_string(false);
		field.code = {31};
	}
	{
		auto& field = type_code->fields[4];
		field.is_extended = true;
		field.name = "domain_path";
		field.code = {32};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::UnixEndpoint& value, const TypeCode* type_code, const uint16_t* code) {
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
			vnx::read_value(_buf + _field->offset, value.send_buffer_size, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[1]) {
			vnx::read_value(_buf + _field->offset, value.receive_buffer_size, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[2]) {
			vnx::read_value(_buf + _field->offset, value.listen_queue_size, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[3]) {
			vnx::read_value(_buf + _field->offset, value.non_blocking, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 4: vnx::read(in, value.domain_path, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::UnixEndpoint& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_UnixEndpoint;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::UnixEndpoint>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(13);
	vnx::write_value(_buf + 0, value.send_buffer_size);
	vnx::write_value(_buf + 4, value.receive_buffer_size);
	vnx::write_value(_buf + 8, value.listen_queue_size);
	vnx::write_value(_buf + 12, value.non_blocking);
	vnx::write(out, value.domain_path, type_code, type_code->fields[4].code.data());
}

void read(std::istream& in, ::vnx::UnixEndpoint& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::UnixEndpoint& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::UnixEndpoint& value) {
	value.accept(visitor);
}

} // vnx
