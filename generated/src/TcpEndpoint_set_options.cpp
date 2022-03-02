
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/TcpEndpoint_set_options.hxx>
#include <vnx/TcpEndpoint_set_options_return.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 TcpEndpoint_set_options::VNX_TYPE_HASH(0x8458afe05a08ddffull);
const vnx::Hash64 TcpEndpoint_set_options::VNX_CODE_HASH(0x370bab13792b1030ull);

vnx::Hash64 TcpEndpoint_set_options::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string TcpEndpoint_set_options::get_type_name() const {
	return "vnx.TcpEndpoint.set_options";
}

const vnx::TypeCode* TcpEndpoint_set_options::get_type_code() const {
	return vnx::vnx_native_type_code_TcpEndpoint_set_options;
}

std::shared_ptr<TcpEndpoint_set_options> TcpEndpoint_set_options::create() {
	return std::make_shared<TcpEndpoint_set_options>();
}

std::shared_ptr<vnx::Value> TcpEndpoint_set_options::clone() const {
	return std::make_shared<TcpEndpoint_set_options>(*this);
}

void TcpEndpoint_set_options::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void TcpEndpoint_set_options::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void TcpEndpoint_set_options::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_TcpEndpoint_set_options;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, socket);
	_visitor.type_end(*_type_code);
}

void TcpEndpoint_set_options::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.TcpEndpoint.set_options\"";
	_out << ", \"socket\": "; vnx::write(_out, socket);
	_out << "}";
}

void TcpEndpoint_set_options::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object TcpEndpoint_set_options::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.TcpEndpoint.set_options";
	_object["socket"] = socket;
	return _object;
}

void TcpEndpoint_set_options::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "socket") {
			_entry.second.to(socket);
		}
	}
}

vnx::Variant TcpEndpoint_set_options::get_field(const std::string& _name) const {
	if(_name == "socket") {
		return vnx::Variant(socket);
	}
	return vnx::Variant();
}

void TcpEndpoint_set_options::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "socket") {
		_value.to(socket);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const TcpEndpoint_set_options& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, TcpEndpoint_set_options& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* TcpEndpoint_set_options::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> TcpEndpoint_set_options::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.TcpEndpoint.set_options";
	type_code->type_hash = vnx::Hash64(0x8458afe05a08ddffull);
	type_code->code_hash = vnx::Hash64(0x370bab13792b1030ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->native_size = sizeof(::vnx::TcpEndpoint_set_options);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<TcpEndpoint_set_options>(); };
	type_code->methods.resize(0);
	type_code->is_const = true;
	type_code->return_type = ::vnx::TcpEndpoint_set_options_return::static_get_type_code();
	type_code->fields.resize(1);
	{
		auto& field = type_code->fields[0];
		field.data_size = 4;
		field.name = "socket";
		field.code = {7};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::TcpEndpoint_set_options& value, const TypeCode* type_code, const uint16_t* code) {
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
			vnx::read_value(_buf + _field->offset, value.socket, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::TcpEndpoint_set_options& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_TcpEndpoint_set_options;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::TcpEndpoint_set_options>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(4);
	vnx::write_value(_buf + 0, value.socket);
}

void read(std::istream& in, ::vnx::TcpEndpoint_set_options& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::TcpEndpoint_set_options& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::TcpEndpoint_set_options& value) {
	value.accept(visitor);
}

} // vnx
