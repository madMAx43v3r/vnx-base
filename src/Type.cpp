 /*************************************************************************
 * 
 *  [2017] - [2018] Automy Inc. 
 *  All Rights Reserved.
 * 
 * NOTICE:  All information contained herein is, and remains
 * the property of Automy Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to Automy Incorporated
 * and its suppliers and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Automy Incorporated.
 */

#include <vnx/vnx.h>
#include <vnx/Type.h>
#include <vnx/Binary.h>

#include <mutex>


namespace vnx {

size_t get_value_size(const uint16_t* code, const TypeCode* type_code) {
	switch(code[0]) {
		case CODE_BOOL:
		case CODE_INT8:
		case CODE_UINT8:
		case CODE_ALT_BOOL:
		case CODE_ALT_INT8:
		case CODE_ALT_UINT8:
			return 1;
		case CODE_INT16: return 2;
		case CODE_INT32: return 4;
		case CODE_INT64: return 8;
		case CODE_UINT16: return 2;
		case CODE_UINT32: return 4;
		case CODE_UINT64: return 8;
		case CODE_FLOAT: return 4;
		case CODE_DOUBLE: return 8;
		case CODE_ARRAY:
			return code[1] * get_value_size(code + 2, type_code);
		case CODE_TUPLE: {
			size_t sum = 0;
			const int N = code[1];
			for(int i = 0; i < N; ++i) {
				if(const auto size = get_value_size(code + code[2 + i], type_code)) {
					sum += size;
				} else {
					return 0;
				}
			}
			return sum;
		}
		case CODE_STRUCT:
			if(type_code) {
				type_code = type_code->depends[code[1]];
				if(type_code->is_equivalent) {
					return type_code->total_field_size;
				}
			}
			break;
	}
	return 0;
}

const uint16_t* validate_code(const uint16_t* code, const TypeCode* type_code, const size_t size, const size_t pos) {
	if(!code || pos >= size) {
		throw std::logic_error("validate_code(): invalid code (unexpected end)");
	}
	switch(code[pos]) {
		case CODE_NULL:
		case CODE_BOOL:
		case CODE_UINT8:
		case CODE_UINT16:
		case CODE_UINT32:
		case CODE_UINT64:
		case CODE_INT8:
		case CODE_INT16:
		case CODE_INT32:
		case CODE_INT64:
		case CODE_FLOAT:
		case CODE_DOUBLE:
		case CODE_ANY:
		case CODE_DYNAMIC:
		case CODE_OBJECT:
		case CODE_TYPE_CODE:
		case CODE_STRING:
		case CODE_ALT_BOOL:
		case CODE_ALT_UINT8:
		case CODE_ALT_UINT16:
		case CODE_ALT_UINT32:
		case CODE_ALT_UINT64:
		case CODE_ALT_INT8:
		case CODE_ALT_INT16:
		case CODE_ALT_INT32:
		case CODE_ALT_INT64:
		case CODE_ALT_FLOAT:
		case CODE_ALT_DOUBLE:
		case CODE_ALT_ANY:
		case CODE_ALT_DYNAMIC:
		case CODE_ALT_OBJECT:
		case CODE_ALT_TYPE_CODE:
		case CODE_ALT_STRING:
			return code + pos;
		case CODE_ARRAY:
		case CODE_ALT_ARRAY:
			return validate_code(code, type_code, size, pos + 2);
		case CODE_LIST:
		case CODE_ALT_LIST:
			return validate_code(code, type_code, size, pos + 1);
		case CODE_MAP:
		case CODE_ALT_MAP: {
			if(pos + 3 >= size) {
				throw std::logic_error("validate_code(): invalid CODE_MAP (too short)");
			}
			validate_code(code, type_code, size, pos + 2);
			auto offset = code[pos + 1];
			if(code[pos] == CODE_ALT_MAP) {
				offset = flip_bytes(offset);
			}
			if(offset < 3) {
				throw std::logic_error("validate_code(): invalid CODE_MAP (code offset too small)");
			}
			return validate_code(code, type_code, size, pos + offset);
		}
		case CODE_STRUCT:
		case CODE_ALT_STRUCT: {
			if(!type_code) {
				throw std::logic_error("validate_code(): invalid CODE_STRUCT (!type_code)");
			}
			if(pos + 1 >= size) {
				throw std::logic_error("validate_code(): invalid CODE_STRUCT (too short)");
			}
			const auto index = (code[pos] == CODE_STRUCT ? code[pos + 1] : flip_bytes(code[pos + 1]));
			if(index >= type_code->depends.size()) {
				throw std::logic_error("validate_code(): invalid CODE_STRUCT (index out of bounds)");
			}
			return code + pos + 1;
		}
		case CODE_MATRIX:
		case CODE_ALT_MATRIX:
			if(pos + 2 >= size) {
				throw std::logic_error("validate_code(): invalid CODE_MATRIX (too short)");
			}
			return validate_code(code, type_code, size, pos + 2 + (code[pos] == CODE_MATRIX ? code[pos + 1] : flip_bytes(code[pos + 1])));
		case CODE_IMAGE:
		case CODE_ALT_IMAGE:
			if(pos + 2 >= size) {
				throw std::logic_error("validate_code(): invalid CODE_IMAGE (too short)");
			}
			return validate_code(code, type_code, size, pos + 2);
		case CODE_TUPLE:
		case CODE_ALT_TUPLE: {
			const size_t N = (code[pos] == CODE_TUPLE ? code[pos + 1] : flip_bytes(code[pos + 1]));
			const uint16_t* end = code + pos + 1;
			if(pos + 2 + N > size) {
				throw std::logic_error("validate_code(): invalid CODE_TUPLE (too short)");
			}
			for(size_t i = 0; i < N; ++i) {
				auto offset = code[pos + 2 + i];
				if(code[pos] == CODE_ALT_TUPLE) {
					offset = flip_bytes(offset);
				}
				if(offset < 2 + N) {
					throw std::logic_error("validate_code(): invalid CODE_TUPLE (code offset too small)");
				}
				end = validate_code(code, type_code, size, pos + offset);
			}
			return end;
		}
		case CODE_OPTIONAL:
		case CODE_ALT_OPTIONAL:
			return validate_code(code, type_code, size, pos + 1);
		default:
			throw std::logic_error("validate_code(): invalid code at [" + std::to_string(pos) + "]: " + std::to_string(code[pos]));
	}
}

std::string code_to_string(const uint16_t* code, const TypeCode* type_code) {
	switch(code[0]) {
		case CODE_NULL: return "null";
		case CODE_ALT_BOOL:
		case CODE_BOOL: return "bool";
		case CODE_ALT_UINT8:
		case CODE_UINT8: return "uchar";
		case CODE_ALT_UINT16:
		case CODE_UINT16: return "ushort";
		case CODE_ALT_UINT32:
		case CODE_UINT32: return "uint";
		case CODE_ALT_UINT64:
		case CODE_UINT64: return "ulong";
		case CODE_ALT_INT8:
		case CODE_INT8: return "char";
		case CODE_ALT_INT16:
		case CODE_INT16: return "short";
		case CODE_ALT_INT32:
		case CODE_INT32: return "int";
		case CODE_ALT_INT64:
		case CODE_INT64: return "long";
		case CODE_ALT_FLOAT:
		case CODE_FLOAT: return "float";
		case CODE_ALT_DOUBLE:
		case CODE_DOUBLE: return "double";
		case CODE_ALT_ANY:
		case CODE_ANY: return "vnx.Value*";
		case CODE_ALT_DYNAMIC:
		case CODE_DYNAMIC: return "vnx.Variant";
		case CODE_ALT_OBJECT:
		case CODE_OBJECT: return "vnx.Object";
		case CODE_ALT_TYPE_CODE:
		case CODE_TYPE_CODE: return "vnx.TypeCode";
		case CODE_ALT_STRING:
		case CODE_STRING: return "string";
		case CODE_ARRAY:
		case CODE_ALT_ARRAY:
			return code_to_string(code + 2, type_code) + "[" + std::to_string(code[1]) + "]";
		case CODE_LIST:
		case CODE_ALT_LIST:
			return "vector<" + code_to_string(code + 1, type_code) + ">";
		case CODE_MAP:
		case CODE_ALT_MAP:
			return "map<" + code_to_string(code + 2, type_code) + ", "
					+ code_to_string(code + (code[0] == CODE_MAP ? code[1] : flip_bytes(code[1])), type_code) + ">";
		case CODE_STRUCT:
		case CODE_ALT_STRUCT: {
			const auto index = (code[0] == CODE_STRUCT ? code[1] : flip_bytes(code[1]));
			if(type_code && index < type_code->depends.size()) {
				return type_code->depends[index]->name;
			}
			return "struct";
		}
		case CODE_MATRIX:
		case CODE_ALT_MATRIX: {
			const size_t N = (code[0] == CODE_MATRIX ? code[1] : flip_bytes(code[1]));
			std::string suffix;
			for(size_t i = 0; i < N; ++i) {
				const size_t size = (code[0] == CODE_MATRIX ? code[2 + i] : flip_bytes(code[2 + i]));
				suffix += ", " + std::to_string(size);
			}
			return "matrix<" + code_to_string(code + 2 + N, type_code) + suffix + ">";
		}
		case CODE_IMAGE:
		case CODE_ALT_IMAGE: {
			const size_t N = (code[0] == CODE_IMAGE ? code[1] : flip_bytes(code[1]));
			return "image<" + code_to_string(code + 2, type_code) + ", " + std::to_string(N) + ">";
		}
		case CODE_TUPLE:
		case CODE_ALT_TUPLE: {
			const size_t N = (code[0] == CODE_TUPLE ? code[1] : flip_bytes(code[1]));
			std::string result = (N == 2 ? "pair<" : "tuple<");
			for(size_t i = 0; i < N; ++i) {
				const auto offset = code[2 + i];
				result += (i ? ", " : "") + code_to_string(code + (code[0] == CODE_TUPLE ? offset : flip_bytes(offset)), type_code);
			}
			return result + ">";
		}
		case CODE_OPTIONAL:
		case CODE_ALT_OPTIONAL:
			return "optional<" + code_to_string(code + 1, type_code) + ">";
	}
	return "<unknown>";
}

std::string code_to_string(const std::vector<uint16_t>& code, const TypeCode* type_code) {
	try {
		validate_code(code.data(), type_code, code.size());
	} catch(...) {
		return "";
	}
	return code_to_string(code.data(), type_code);
}

bool is_equivalent_code(const uint16_t* code_lhs, const uint16_t* code_rhs) {
	switch(code_lhs[0]) {
		case CODE_BOOL:
			if(sizeof(vnx::bool_t) != 1) {
				break;
			}
			/* no break */
		case CODE_NULL:
		case CODE_UINT8:
		case CODE_UINT16:
		case CODE_UINT32:
		case CODE_UINT64:
		case CODE_INT8:
		case CODE_INT16:
		case CODE_INT32:
		case CODE_INT64:
		case CODE_FLOAT:
		case CODE_DOUBLE:
			if(code_rhs[0] == code_lhs[0]) {
				return true;
			}
	}
	switch(code_lhs[0]) {
		case CODE_BOOL:
			if(sizeof(vnx::bool_t) != 1) {
				break;
			}
			/* no break */
		case CODE_INT8:
		case CODE_UINT8:
		case CODE_ALT_BOOL:
		case CODE_ALT_INT8:
		case CODE_ALT_UINT8:
			switch(code_rhs[0]) {
				case CODE_BOOL:
					if(sizeof(vnx::bool_t) != 1) {
						break;
					}
					/* no break */
				case CODE_INT8:
				case CODE_UINT8:
				case CODE_ALT_BOOL:
				case CODE_ALT_INT8:
				case CODE_ALT_UINT8:
					return true;
			}
	}
	switch(code_lhs[0]) {
		case CODE_INT16:
		case CODE_UINT16:
			switch(code_rhs[0]) {
				case CODE_INT16:
				case CODE_UINT16:
					return true;
			}
			break;
		case CODE_INT32:
		case CODE_UINT32:
			switch(code_rhs[0]) {
				case CODE_INT32:
				case CODE_UINT32:
					return true;
			}
			break;
		case CODE_INT64:
		case CODE_UINT64:
			switch(code_rhs[0]) {
				case CODE_INT64:
				case CODE_UINT64:
					return true;
			}
			break;
		case CODE_ARRAY:
			switch(code_rhs[0]) {
				case CODE_ARRAY:
					if(code_rhs[1] == code_lhs[1]) {
						return is_equivalent_code(code_lhs + 2, code_rhs + 2);
					}
					break;
			}
			break;
		case CODE_TUPLE:
			switch(code_rhs[0]) {
				case CODE_TUPLE:
					if(code_rhs[1] == code_lhs[1]) {
						const int N = code_lhs[1];
						for(int i = 0; i < N; ++i) {
							if(!is_equivalent_code(code_lhs + code_lhs[2 + i], code_rhs + code_rhs[2 + i])) {
								return false;
							}
						}
						return true;
					}
					break;
			}
			break;
	}
	return false;
}

void TypeField::compile(const TypeCode* type_code) {
	validate_code(code.data(), type_code, code.size());
	if(!is_extended) {
		const auto size = get_value_size(code.data(), type_code);
		if(data_size == 0) {
			// backwards compatibility to where data_size wasn't serialized
			data_size = size;
		} else if(data_size < size) {
			throw std::logic_error("compile(): invalid data_size");
		}
		if(data_size == 0) {
			throw std::logic_error("compile(): invalid primitive field");
		}
	}
}

void TypeField::accept(Visitor& visitor, const TypeCode* type_code) const {
	static const int num_fields = 7;
	visitor.type_begin(num_fields);
	int i = 0;
	visitor.type_field("type", i++);
	visitor.visit(code_to_string(code, type_code));
	visitor.type_field("name", i++);
	visitor.visit(name);
	visitor.type_field("value", i++);
	visitor.visit(value);
	visitor.type_field("code", i++);
	vnx::accept(visitor, code);
	visitor.type_field("version", i++);
	visitor.visit(version);
	visitor.type_field("data_size", i++);
	visitor.visit(data_size);
	visitor.type_field("is_extended", i++);
	visitor.visit(is_extended);
	visitor.type_end(num_fields);
	if(i != num_fields) {
		throw std::logic_error("i != num_fields");
	}
}

void TypeField::from_object(const Object& object) {
	object["version"].to(version);
	object["name"].to(name);
	object["value"].to(value);
	object["code"].to(code);
	object["data_size"].to(data_size);
	object["is_extended"].to(is_extended);
}


TypeCode::TypeCode() {
	create_value = []() -> std::shared_ptr<Value> { return std::make_shared<Binary>(); };
}

void TypeCode::compile() {
	size_t index = 0;
	size_t offset = 0;
	for(TypeField& field : fields) {
		field.index = index++;
		field.compile(this);
		if(field.is_extended) {
			ext_fields.push_back(&field);
		} else {
			if(field.data_size == 0) {
				throw std::logic_error("compile(): invalid primitive size");
			}
			field.offset = offset;
			offset += field.data_size;
		}
	}
	total_field_size = offset;
	
	if(version < 2) {
		is_enum = !enum_map.empty() && fields.size() == 1 && fields[0].name == "value" && fields[0].code[0] == CODE_UINT32;
		is_method = return_type != nullptr;
	}
	
	inv_enum_map.clear();
	for(const auto& entry : enum_map) {
		inv_enum_map[entry.second] = entry.first;
	}

	if(is_method) {
		const auto pos = name.find_last_of('.');
		if(pos != std::string::npos) {
			method_name = name.substr(pos + 1);
		} else {
			method_name = name;
		}
	}
	if(!parents.empty()) {
		super = parents.front();
	}
}

template<typename V>
void link_field_map(std::map<std::string, V>& map, const std::map<std::string, std::string>& alias_map) {
	for(const auto& entry : alias_map) {
		if(!map.count(entry.first) && map.count(entry.second)) {
			map[entry.first] = map[entry.second];
		}
		if(!map.count(entry.second) && map.count(entry.first)) {
			map[entry.second] = map[entry.first];
		}
	}
}

void TypeCode::match(const TypeCode* native_) {
	if(!native_->is_native) {
		throw std::logic_error("match(): native !is_native");
	}
	{
		std::map<std::string, TypeField*> map;
		for(TypeField& field : fields) {
			if(!field.is_extended) {
				map[field.name] = &field;
			}
		}
		link_field_map(map, alias_map);
		link_field_map(map, native_->alias_map);
		field_map.clear();
		field_map.resize(native_->fields.size());
		for(const TypeField& field : native_->fields) {
			field_map[field.index] = map[field.name];
		}
	}
	{
		std::map<std::string, int32_t> map;
		for(const TypeField* field : native_->ext_fields) {
			map[field->name] = int32_t(field->index);
		}
		link_field_map(map, alias_map);
		link_field_map(map, native_->alias_map);
		for(TypeField* field : ext_fields) {
			auto iter = map.find(field->name);
			if(iter != map.end()) {
				field->native_index = iter->second;
			} else {
				field->native_index = -1;
			}
		}
	}
	is_equivalent = !native_->is_class && ext_fields.empty() && native_->ext_fields.empty();
	if(fields.size() == native_->fields.size()) {
		for(size_t i = 0; i < fields.size(); ++i) {
			if(fields[i].name != native_->fields[i].name) {
				is_equivalent = false;
			}
			if(!vnx::is_equivalent_code(fields[i].code.data(), native_->fields[i].code.data())) {
				is_equivalent = false;
			}
		}
	} else {
		is_equivalent = false;
	}
	if(total_field_size != native_->native_size) {
		is_equivalent = false;
	}
	native = native_;
	create_value = native_->create_value;
	is_matched = true;
}

void TypeCode::link() {
	const TypeCode* native_ = nullptr;
	if(is_native) {
		native_ = this;
	} else {
		native_ = get_type_code(type_hash);
	}
	if(native_) {
		match(native_);
	}
}

void TypeCode::build() {
	compile();
	link();
}

bool TypeCode::instanceof(Hash64 type_hash_) const {
	if(type_hash_ == type_hash) {
		return true;
	}
	for(const TypeCode* parent : parents) {
		if(parent->type_hash == type_hash_) {
			return true;
		}
	}
	return false;
}

std::string TypeCode::get_method_name() const {
	return method_name;
}

const TypeCode* TypeCode::get_field_type_code(const std::string& name) const {
	for(const auto& field : fields) {
		if(field.name == name) {
			switch(field.code[0]) {
				case CODE_STRUCT: return depends[field.code[1]];
			}
		}
	}
	return nullptr;
}

void TypeCode::accept(Visitor& visitor) const {
	static const int num_fields = 20;
	visitor.type_begin(num_fields);
	int i = 0;
	visitor.type_field("version", i++);
	visitor.visit(version);
	visitor.type_field("type_hash", i++);
	visitor.visit(type_hash);
	visitor.type_field("code_hash", i++);
	visitor.visit(code_hash);
	visitor.type_field("parents", i++);
	visitor.list_begin(parents.size());
	for(size_t k = 0; k < parents.size(); ++k) {
		visitor.list_element(k);
		parents[k]->accept(visitor);
	}
	visitor.list_end(parents.size());
	visitor.type_field("depends", i++);
	visitor.list_begin(depends.size());
	for(size_t k = 0; k < depends.size(); ++k) {
		visitor.list_element(k);
		depends[k]->accept(visitor);
	}
	visitor.list_end(depends.size());
	visitor.type_field("return_type", i++);
	if(return_type) {
		return_type->accept(visitor);
	} else {
		visitor.visit_null();
	}
	visitor.type_field("returns", i++);
	if(return_type) {
		if(return_type->fields.empty()) {
			visitor.visit(std::string("void"));
		} else if(return_type->fields.size() == 1) {
			visitor.visit(code_to_string(return_type->fields[0].code, this));
		} else {
			return_type->accept(visitor);
		}
	} else {
		visitor.visit_null();
	}
	visitor.type_field("name", i++);
	visitor.visit(name);
	visitor.type_field("fields", i++);
	visitor.list_begin(fields.size());
	for(size_t k = 0; k < fields.size(); ++k) {
		visitor.list_element(k);
		fields[k].accept(visitor, this);
	}
	visitor.list_end(fields.size());
	visitor.type_field("static_fields", i++);
	visitor.list_begin(static_fields.size());
	for(size_t k = 0; k < static_fields.size(); ++k) {
		visitor.list_element(k);
		static_fields[k].accept(visitor, this);
	}
	visitor.list_end(static_fields.size());
	visitor.type_field("methods", i++);
	visitor.list_begin(methods.size());
	for(size_t k = 0; k < methods.size(); ++k) {
		visitor.list_element(k);
		methods[k]->accept(visitor);
	}
	visitor.list_end(methods.size());
	visitor.type_field("enum_map", i++);
	vnx::accept(visitor, enum_map);
	visitor.type_field("alias_map", i++);
	vnx::accept(visitor, alias_map);
	visitor.type_field("is_enum", i++);
	visitor.visit(is_enum);
	visitor.type_field("is_class", i++);
	visitor.visit(is_class);
	visitor.type_field("is_method", i++);
	visitor.visit(is_method);
	visitor.type_field("is_return", i++);
	visitor.visit(is_return);
	visitor.type_field("is_const", i++);
	visitor.visit(is_const);
	visitor.type_field("is_async", i++);
	visitor.visit(is_async);
	visitor.type_field("permission", i++);
	visitor.visit(permission);
	visitor.type_end(num_fields);
	if(i != num_fields) {
		throw std::logic_error("i != num_fields");
	}
}

void TypeCode::print(std::ostream& out) const {
	if(is_method) {
		if(return_type) {
			return_type->print(out);
		} else {
			out << "<invalid>";
		}
		out << " " << name << " (";
		int i = 0;
		for(const auto& field : fields) {
			out << (i++ ? ", " : "") << code_to_string(field.code, this) << " " << field.name;
		}
		out << ")" << (is_const ? " const" : "") << (is_async ? " async" : "") << std::endl;
	} else if(is_return) {
		if(fields.empty()) {
			out << "void";
		} else if(fields.size() == 1) {
			out << code_to_string(fields[0].code, this);
		}
	} else if(is_enum) {
		out << "enum " << name << " {";
		int i = 0;
		for(const auto& entry : enum_map) {
			out << entry.second << (i++ ? "," : "") << std::endl;
		}
		out << "}" << std::endl;
	} else {
		out << (is_class ? "class " : "struct ") << name << " {" << std::endl;
		for(const auto& field : fields) {
			out << code_to_string(field.code, this) << " " << field.name;
			if(!field.value.empty()) {
				out << " = " << field.value;
			}
			out << std::endl;
		}
		for(auto* method : methods) {
			method->print(out);
		}
		out << "}" << std::endl;
	}
}

Object TypeCode::to_object() const {
	ToObject visitor;
	accept(visitor);
	return visitor.get_object();
}

void TypeCode::from_object(const Object& object) {
	{
		std::vector<std::shared_ptr<TypeCode>> parents_;
		object["parents"].to(parents_);
		for(auto type_code : parents_) {
			if(type_code) {
				parents.push_back(vnx::register_type_code(type_code->code_hash, type_code));
			}
		}
	}
	{
		std::vector<std::shared_ptr<TypeCode>> depends_;
		object["depends"].to(depends_);
		for(auto type_code : depends_) {
			if(type_code) {
				depends.push_back(vnx::register_type_code(type_code->code_hash, type_code));
			}
		}
	}
	{
		std::vector<std::shared_ptr<TypeCode>> methods_;
		object["methods"].to(methods_);
		for(auto type_code : methods_) {
			if(type_code) {
				methods.push_back(vnx::register_type_code(type_code->code_hash, type_code));
			}
		}
	}
	{
		std::shared_ptr<TypeCode> return_type_;
		object["return_type"].to(return_type_);
		if(return_type_) {
			return_type = vnx::register_type_code(return_type_->code_hash, return_type_);
		}
	}
	object["version"].to(version);
	object["type_hash"].to(type_hash);
	object["code_hash"].to(code_hash);
	object["name"].to(name);
	object["fields"].to(fields);
	object["static_fields"].to(static_fields);
	object["enum_map"].to(enum_map);
	object["alias_map"].to(alias_map);
	object["permission"].to(permission);
	object["is_enum"].to(is_enum);
	object["is_class"].to(is_class);
	object["is_method"].to(is_method);
	object["is_return"].to(is_return);
	object["is_const"].to(is_const);
	object["is_async"].to(is_async);
	build();
}


void convert_code(std::vector<uint16_t>& code, const uint16_t version) {
	if(version < 2) {
		for(size_t k = 0; k < code.size(); ++k) {
			// convert old CODE_CLASS to CODE_STRUCT
			if(code[k] == 15 && (k == 0 || (code[k - 1] != CODE_ARRAY && code[k - 1] != CODE_MAP && code[k - 1] != 15))) {
				code[k] = CODE_STRUCT;
			}
		}
	}
}

void read(TypeInput& in, TypeField& field, const TypeCode* type_code, const uint16_t* code) {
	if(!code) {
		throw std::logic_error("!code");
	}
	switch(code[0]) {
		case CODE_OBJECT:
		case CODE_ALT_OBJECT: {
			Object tmp;
			vnx::read(in, tmp, type_code, code);
			field.from_object(tmp);
			return;
		}
		case CODE_DYNAMIC:
		case CODE_ALT_DYNAMIC:
			vnx::read_dynamic(in, field);
			return;
		case CODE_STRUCT:
		case CODE_ALT_STRUCT:
			break;
		default:
			vnx::skip(in, type_code, code);
			return;
	}
	uint16_t version = 0;
	read(in, version);
	
	bool is_alt = false;
	if(version > 0xFF) {
		is_alt = true;
		version = flip_bytes(version);
	}
	
	if(version < 1 || version > 2) {
		throw std::runtime_error("read(vnx::TypeField): invalid version");
	}
	field.version = version;
	
	uint16_t i = 0;
	uint16_t N = 4;
	if(version >= 2) {
		read(in, N);
		if(is_alt) {
			N = flip_bytes(N);
		}
	}
	if(is_alt) {
		{
			const uint16_t code[] = {CODE_ALT_BOOL};
			read(in, field.is_extended, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_ALT_STRING};
			read(in, field.name, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_ALT_LIST, CODE_ALT_UINT16};
			read(in, field.code, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_ALT_STRING};
			read(in, field.value, nullptr, code); i++;
		}
	} else {
		{
			const uint16_t code[] = {CODE_BOOL};
			read(in, field.is_extended, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_STRING};
			read(in, field.name, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_LIST, CODE_UINT16};
			read(in, field.code, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_STRING};
			read(in, field.value, nullptr, code); i++;
		}
	}
	if(N >= 5) {
		read_dynamic(in, field.data_size); i++;
	}
	for(; i < N; ++i) {
		if(is_alt) {
			const uint16_t code = CODE_ALT_DYNAMIC;
			vnx::skip(in, nullptr, &code);
		} else {
			const uint16_t code = CODE_DYNAMIC;
			vnx::skip(in, nullptr, &code);
		}
	}
	convert_code(field.code, version);
}

void write(TypeOutput& out, const TypeField& field, const TypeCode* type_code, const uint16_t* code) {
	write(out, uint16_t(2));				// version
	write(out, uint16_t(5));				// number of fields
	write(out, field.is_extended);
	write(out, field.name);
	{
		const uint16_t code[] = {CODE_LIST, CODE_UINT16};
		write(out, field.code, nullptr, code);
	}
	write(out, field.value);
	write_dynamic(out, field.data_size);
}

void read(TypeInput& in, TypeCode& value, const TypeCode* type_code, const uint16_t* code) {
	if(!code) {
		throw std::logic_error("!code");
	}
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
		case CODE_TYPE_CODE:
		case CODE_ALT_TYPE_CODE:
			break;
		default:
			vnx::skip(in, type_code, code);
			return;
	}
	uint16_t version = 0;
	while(true) {
		read(in, version);
		if(version == CODE_TYPE_CODE || version == CODE_ALT_TYPE_CODE) {
			read_type_code(in, &version);
		} else {
			break;
		}
	}
	
	bool is_alt = false;
	if(version > 0xFF) {
		is_alt = true;
		version = flip_bytes(version);
	}
	
	if(version < 1 || version > 2) {
		throw std::runtime_error("read(vnx::TypeCode): invalid version");
	}
	value.version = version;
	
	uint16_t i = 0;
	uint16_t N = 10;
	if(version >= 2) {
		read(in, N);
		if(is_alt) {
			N = flip_bytes(N);
		}
	}
	std::vector<Hash64> parents;
	std::vector<Hash64> depends;
	std::vector<Hash64> methods;
	Hash64 return_hash;
	read(in, value.type_hash); i++;
	read(in, value.code_hash); i++;
	read(in, return_hash); i++;
	if(is_alt) {
		value.type_hash = flip_bytes(value.type_hash);
		value.code_hash = flip_bytes(value.code_hash);
		return_hash = flip_bytes(return_hash);
		{
			const uint16_t code[] = {CODE_ALT_LIST, CODE_ALT_UINT64};
			read(in, parents, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_ALT_LIST, CODE_ALT_UINT64};
			read(in, depends, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_ALT_STRING};
			read(in, value.name, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_ALT_LIST, CODE_ALT_STRUCT, 0};
			read(in, value.fields, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_ALT_LIST, CODE_ALT_STRUCT, 0};
			read(in, value.static_fields, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_ALT_LIST, CODE_ALT_UINT64};
			read(in, methods, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_ALT_MAP, 0x300, CODE_ALT_UINT32, CODE_ALT_STRING};
			read(in, value.enum_map, nullptr, code); i++;
		}
	} else {
		{
			const uint16_t code[] = {CODE_LIST, CODE_UINT64};
			read(in, parents, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_LIST, CODE_UINT64};
			read(in, depends, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_STRING};
			read(in, value.name, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_LIST, CODE_STRUCT, 0};
			read(in, value.fields, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_LIST, CODE_STRUCT, 0};
			read(in, value.static_fields, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_LIST, CODE_UINT64};
			read(in, methods, nullptr, code); i++;
		}
		{
			const uint16_t code[] = {CODE_MAP, 3, CODE_UINT32, CODE_STRING};
			read(in, value.enum_map, nullptr, code); i++;
		}
	}
	if(N >= 11) {
		read_dynamic(in, value.alias_map); i++;
	}
	if(N >= 12) {
		std::array<bool, 4> tmp = {};
		read_dynamic(in, tmp); i++;
		value.is_enum = tmp[0];
		value.is_class = tmp[1];
		value.is_method = tmp[2];
		value.is_return = tmp[3];
	}
	if(N >= 13) {
		read_dynamic(in, value.permission); i++;
	}
	for(; i < N; ++i) {
		if(is_alt) {
			const uint16_t code = CODE_ALT_DYNAMIC;
			vnx::skip(in, nullptr, &code);
		} else {
			const uint16_t code = CODE_DYNAMIC;
			vnx::skip(in, nullptr, &code);
		}
	}
	for(Hash64 code_hash : parents) {
		value.parents.push_back(in.get_type_code(code_hash));
	}
	for(Hash64 code_hash : depends) {
		value.depends.push_back(in.get_type_code(code_hash));
	}
	for(Hash64 code_hash : methods) {
		value.methods.push_back(in.get_type_code(code_hash));
	}
	if(return_hash) {
		value.return_type = in.get_type_code(return_hash);
	}
}

void write(TypeOutput& out, const TypeCode& value, const TypeCode* type_code, const uint16_t* code) {
	if(code) {
		switch(code[0]) {
			case CODE_OBJECT:
				write(out, value.to_object(), type_code, code);
				return;
			case CODE_TYPE_CODE:
				break;
			default:
				throw std::logic_error("write(vnx::TypeCode): invalid code");
		}
	}
	std::vector<Hash64> parents;
	std::vector<Hash64> depends;
	std::vector<Hash64> methods;
	Hash64 return_hash;
	for(const TypeCode* code : value.parents) {
		out.write_type_code(code);
		parents.push_back(code->code_hash);
	}
	for(const TypeCode* code : value.depends) {
		out.write_type_code(code);
		depends.push_back(code->code_hash);
	}
	for(const TypeCode* code : value.methods) {
		out.write_type_code(code);
		methods.push_back(code->code_hash);
	}
	if(value.return_type) {
		out.write_type_code(value.return_type);
		return_hash = value.return_type->code_hash;
	}
	out.type_code_map[value.code_hash] = out.get_output_pos();
	if(!code) {
		write(out, uint16_t(CODE_TYPE_CODE));
	}
	write(out, uint16_t(2));						// version (14 is reserved, special meaning)
	write(out, uint16_t(13));						// number of fields
	write(out, value.type_hash);
	write(out, value.code_hash);
	write(out, return_hash);
	{
		const uint16_t code[] = {CODE_LIST, CODE_UINT64};
		write(out, parents, nullptr, code);
		write(out, depends, nullptr, code);
	}
	write(out, value.name);
	{
		const uint16_t code[] = {CODE_LIST, CODE_STRUCT, 0};
		write(out, value.fields, nullptr, code);
		write(out, value.static_fields, nullptr, code);
	}
	{
		const uint16_t code[] = {CODE_LIST, CODE_UINT64};
		write(out, methods, nullptr, code);
	}
	{
		const uint16_t code[] = {CODE_MAP, 3, 4, CODE_UINT32, CODE_STRING};
		write(out, value.enum_map, nullptr, code);
	}
	write_dynamic(out, value.alias_map);
	write_dynamic(out, std::array<bool, 4>{value.is_enum, value.is_class, value.is_method, value.is_return});
	write_dynamic(out, value.permission);
}

void write(TypeOutput& out, const TypeCode& value) {
	write(out, value, nullptr, nullptr);
}

void read(std::istream& in, TypeCode& value) {
	if(auto json = vnx::read_json(in)) {
		value.from_object(json->to_object());
	}
}

void write(std::ostream& out, const TypeCode& value) {
	DefaultPrinter printer(out);
	vnx::accept(printer, value);
}

void accept(Visitor& visitor, const TypeCode& value) {
	value.accept(visitor);
}


class GlobalTypeRegistry {
public:
	std::shared_ptr<TypeCode> register_type_code(Hash64 hash, std::shared_ptr<TypeCode> type_code) {
		if(!type_code) {
			throw std::logic_error("type_code == 0");
		}
		std::lock_guard<std::mutex> lock(mutex);
		std::shared_ptr<TypeCode>& p_type_code = type_map[hash];
		if(!p_type_code) {
			p_type_code = type_code;
		}
		return p_type_code;
	}
	
	std::shared_ptr<TypeCode> get_type_code(Hash64 hash) {
		std::lock_guard<std::mutex> lock(mutex);
		auto iter = type_map.find(hash);
		if(iter != type_map.end()) {
			return iter->second;
		}
		return nullptr;
	}
	
	std::vector<const TypeCode*> get_all_type_codes() {
		std::lock_guard<std::mutex> lock(mutex);
		std::vector<const TypeCode*> all;
		for(const auto& entry : type_map) {
			all.push_back(entry.second.get());
		}
		return all;
	}
	
private:
	std::mutex mutex;
	std::unordered_map<uint64_t, std::shared_ptr<TypeCode>> type_map;
	
};

static GlobalTypeRegistry& get_global_type_registry() {
	static GlobalTypeRegistry instance;
	return instance;
}

const TypeCode* register_type_code(std::shared_ptr<TypeCode> type_code) {
	if(type_code->is_native) {
		register_type_code(type_code->type_hash, type_code);
	}
	return register_type_code(type_code->code_hash, type_code);
}

const TypeCode* register_type_code(Hash64 hash, std::shared_ptr<TypeCode> type_code) {
	return get_global_type_registry().register_type_code(hash, type_code).get();
}

const TypeCode* register_type_alias(Hash64 type_hash, Hash64 new_type_hash) {
	std::shared_ptr<TypeCode> type_code = get_global_type_registry().get_type_code(type_hash);
	if(type_code) {
		return register_type_code(new_type_hash, type_code);
	}
	return nullptr;
}

const TypeCode* get_type_code(Hash64 hash) {
	return get_global_type_registry().get_type_code(hash).get();
}

const TypeCode* get_type_code(const std::string& type_name) {
	return get_type_code(Hash64(type_name));
}

std::vector<const TypeCode*> get_all_type_codes() {
	return get_global_type_registry().get_all_type_codes();
}


} // vnx
