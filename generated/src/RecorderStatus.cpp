
// AUTO GENERATED by vnxcppcodegen

#include <vnx/package.hxx>
#include <vnx/RecorderStatus.hxx>
#include <vnx/RecordHeader.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {


const vnx::Hash64 RecorderStatus::VNX_TYPE_HASH(0xc79edae794af8704ull);
const vnx::Hash64 RecorderStatus::VNX_CODE_HASH(0xa5961e35abfc0caeull);

vnx::Hash64 RecorderStatus::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string RecorderStatus::get_type_name() const {
	return "vnx.RecorderStatus";
}

const vnx::TypeCode* RecorderStatus::get_type_code() const {
	return vnx::vnx_native_type_code_RecorderStatus;
}

std::shared_ptr<RecorderStatus> RecorderStatus::create() {
	return std::make_shared<RecorderStatus>();
}

std::shared_ptr<vnx::Value> RecorderStatus::clone() const {
	return std::make_shared<RecorderStatus>(*this);
}

void RecorderStatus::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void RecorderStatus::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void RecorderStatus::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::vnx_native_type_code_RecorderStatus;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, file_name);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, is_recording);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, bandwidth);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, sample_rate);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, loss_factor);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, header);
	_visitor.type_end(*_type_code);
}

void RecorderStatus::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.RecorderStatus\"";
	_out << ", \"file_name\": "; vnx::write(_out, file_name);
	_out << ", \"is_recording\": "; vnx::write(_out, is_recording);
	_out << ", \"bandwidth\": "; vnx::write(_out, bandwidth);
	_out << ", \"sample_rate\": "; vnx::write(_out, sample_rate);
	_out << ", \"loss_factor\": "; vnx::write(_out, loss_factor);
	_out << ", \"header\": "; vnx::write(_out, header);
	_out << "}";
}

void RecorderStatus::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object RecorderStatus::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.RecorderStatus";
	_object["file_name"] = file_name;
	_object["is_recording"] = is_recording;
	_object["bandwidth"] = bandwidth;
	_object["sample_rate"] = sample_rate;
	_object["loss_factor"] = loss_factor;
	_object["header"] = header;
	return _object;
}

void RecorderStatus::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "bandwidth") {
			_entry.second.to(bandwidth);
		} else if(_entry.first == "file_name") {
			_entry.second.to(file_name);
		} else if(_entry.first == "header") {
			_entry.second.to(header);
		} else if(_entry.first == "is_recording") {
			_entry.second.to(is_recording);
		} else if(_entry.first == "loss_factor") {
			_entry.second.to(loss_factor);
		} else if(_entry.first == "sample_rate") {
			_entry.second.to(sample_rate);
		}
	}
}

vnx::Variant RecorderStatus::get_field(const std::string& _name) const {
	if(_name == "file_name") {
		return vnx::Variant(file_name);
	}
	if(_name == "is_recording") {
		return vnx::Variant(is_recording);
	}
	if(_name == "bandwidth") {
		return vnx::Variant(bandwidth);
	}
	if(_name == "sample_rate") {
		return vnx::Variant(sample_rate);
	}
	if(_name == "loss_factor") {
		return vnx::Variant(loss_factor);
	}
	if(_name == "header") {
		return vnx::Variant(header);
	}
	return vnx::Variant();
}

void RecorderStatus::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "file_name") {
		_value.to(file_name);
	} else if(_name == "is_recording") {
		_value.to(is_recording);
	} else if(_name == "bandwidth") {
		_value.to(bandwidth);
	} else if(_name == "sample_rate") {
		_value.to(sample_rate);
	} else if(_name == "loss_factor") {
		_value.to(loss_factor);
	} else if(_name == "header") {
		_value.to(header);
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const RecorderStatus& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, RecorderStatus& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* RecorderStatus::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> RecorderStatus::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.RecorderStatus";
	type_code->type_hash = vnx::Hash64(0xc79edae794af8704ull);
	type_code->code_hash = vnx::Hash64(0xa5961e35abfc0caeull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->native_size = sizeof(::vnx::RecorderStatus);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<RecorderStatus>(); };
	type_code->depends.resize(1);
	type_code->depends[0] = ::vnx::RecordHeader::static_get_type_code();
	type_code->fields.resize(6);
	{
		auto& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "file_name";
		field.code = {32};
	}
	{
		auto& field = type_code->fields[1];
		field.data_size = 1;
		field.name = "is_recording";
		field.code = {31};
	}
	{
		auto& field = type_code->fields[2];
		field.data_size = 8;
		field.name = "bandwidth";
		field.code = {10};
	}
	{
		auto& field = type_code->fields[3];
		field.data_size = 8;
		field.name = "sample_rate";
		field.code = {10};
	}
	{
		auto& field = type_code->fields[4];
		field.data_size = 8;
		field.name = "loss_factor";
		field.code = {10};
	}
	{
		auto& field = type_code->fields[5];
		field.is_extended = true;
		field.name = "header";
		field.code = {19, 0};
	}
	type_code->build();
	return type_code;
}

std::shared_ptr<vnx::Value> RecorderStatus::vnx_call_switch(std::shared_ptr<const vnx::Value> _method) {
	switch(_method->get_type_hash()) {
	}
	return nullptr;
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::RecorderStatus& value, const TypeCode* type_code, const uint16_t* code) {
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
		if(const auto* const _field = type_code->field_map[1]) {
			vnx::read_value(_buf + _field->offset, value.is_recording, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[2]) {
			vnx::read_value(_buf + _field->offset, value.bandwidth, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[3]) {
			vnx::read_value(_buf + _field->offset, value.sample_rate, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[4]) {
			vnx::read_value(_buf + _field->offset, value.loss_factor, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.file_name, type_code, _field->code.data()); break;
			case 5: vnx::read(in, value.header, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::RecorderStatus& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::vnx_native_type_code_RecorderStatus;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::RecorderStatus>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	auto* const _buf = out.write(25);
	vnx::write_value(_buf + 0, value.is_recording);
	vnx::write_value(_buf + 1, value.bandwidth);
	vnx::write_value(_buf + 9, value.sample_rate);
	vnx::write_value(_buf + 17, value.loss_factor);
	vnx::write(out, value.file_name, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.header, type_code, type_code->fields[5].code.data());
}

void read(std::istream& in, ::vnx::RecorderStatus& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::RecorderStatus& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::RecorderStatus& value) {
	value.accept(visitor);
}

} // vnx
