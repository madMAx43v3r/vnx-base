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
#include <vnx/Binary.h>

#include <sstream>


namespace vnx {

std::ostream& operator<<(std::ostream& _out, const Value& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, Value& _value) {
	_value.read(_in);
	return _in;
}

Variant Value::get_field_by_index(const uint32_t& index) const {
	if(auto* type_code = get_type_code()) {
		if(index < type_code->fields.size()) {
			return get_field(type_code->fields[index].name);
		}
	}
	return Variant();
}

void Value::set_field_by_index(const uint32_t& index, const Variant& value) {
	if(auto* type_code = get_type_code()) {
		if(index < type_code->fields.size()) {
			return set_field(type_code->fields[index].name, value);
		}
	}
}

uint32_t Value::get_num_fields() const {
	if(auto* type_code = get_type_code()) {
		return type_code->fields.size();
	}
	return 0;
}

bool Value::is_void() const {
	return get_num_fields() == 0;
}

std::string Value::to_string() const {
	std::ostringstream stream;
	write(stream);
	return stream.str();
}

void Value::from_string(const std::string& str) {
	std::istringstream stream;
	stream.str(str);
	read(stream);
}

std::shared_ptr<Value> create(Hash64 hash) {
	const TypeCode* type_code = vnx::get_type_code(hash);
	if(type_code) {
		return type_code->create_value();
	}
	return 0;
}

std::shared_ptr<Value> create(const std::string& type_name) {
	return create(Hash64(type_name));
}

void read(std::istream& in, Value& value) {
	value.read(in);
}

void accept(Visitor& visitor, const Value& value) {
	value.accept(visitor);
}

void accept(Visitor& visitor, const std::nullptr_t& value) {
	visitor.visit_null();
}

template<typename T>
void accept_integral(Visitor& visitor, TypeInput& in, const TypeCode* type_code, const uint16_t* code, const TypeField* field, const char* p_buf) {
	T v = 0;
	if(!p_buf || !field || field->is_extended) {
		read(in, v, type_code, code);
	} else {
		read_value(p_buf, v, code);
	}
	visitor.visit(v);
}

static
void accept(Visitor& visitor, TypeInput& in, const TypeCode* type_code, const uint16_t* code, const TypeField* field, const char* p_buf) {
	if(code) {
		switch(code[0]) {
			case CODE_NULL:
				visitor.visit_null();
				return;
			case CODE_BOOL:
			case CODE_ALT_BOOL:
				accept_integral<bool>(visitor, in, type_code, code, field, p_buf);
				return;
			case CODE_UINT8:
			case CODE_ALT_UINT8:
				accept_integral<uint8_t>(visitor, in, type_code, code, field, p_buf);
				return;
			case CODE_UINT16:
			case CODE_ALT_UINT16:
				accept_integral<uint16_t>(visitor, in, type_code, code, field, p_buf);
				return;
			case CODE_UINT32:
			case CODE_ALT_UINT32:
				accept_integral<uint32_t>(visitor, in, type_code, code, field, p_buf);
				return;
			case CODE_UINT64:
			case CODE_ALT_UINT64:
				accept_integral<uint64_t>(visitor, in, type_code, code, field, p_buf);
				return;
			case CODE_INT8:
			case CODE_ALT_INT8:
				accept_integral<int8_t>(visitor, in, type_code, code, field, p_buf);
				return;
			case CODE_INT16:
			case CODE_ALT_INT16:
				accept_integral<int16_t>(visitor, in, type_code, code, field, p_buf);
				return;
			case CODE_INT32:
			case CODE_ALT_INT32:
				accept_integral<int32_t>(visitor, in, type_code, code, field, p_buf);
				return;
			case CODE_INT64:
			case CODE_ALT_INT64:
				accept_integral<int64_t>(visitor, in, type_code, code, field, p_buf);
				return;
			case CODE_FLOAT:
			case CODE_ALT_FLOAT:
				accept_integral<float32_t>(visitor, in, type_code, code, field, p_buf);
				return;
			case CODE_DOUBLE:
			case CODE_ALT_DOUBLE:
				accept_integral<float64_t>(visitor, in, type_code, code, field, p_buf);
				return;
			case CODE_STRING:
			case CODE_ALT_STRING: {
				std::string str;
				read(in, str, type_code, code);
				visitor.visit(str);
				return;
			}
			case CODE_ARRAY:
			case CODE_ALT_ARRAY: {
				uint16_t size = code[1];
				if(code[0] == CODE_ALT_ARRAY) {
					size = flip_bytes(size);
				}
				const uint16_t* value_code = code + 2;
				const size_t value_size = get_value_size(value_code, type_code);
				visitor.list_begin(size);
				for(size_t i = 0; i < size; ++i) {
					visitor.list_element(i);
					accept(visitor, in, type_code, value_code, field, p_buf ? p_buf + i * value_size : nullptr);
				}
				visitor.list_end(size);
				return;
			}
			case CODE_LIST:
			case CODE_ALT_LIST: {
				switch(code[1]) {
					case CODE_INT8:
					case CODE_ALT_INT8: {
						std::string str;
						read(in, str, type_code, code);
						visitor.visit(str);
						break;
					}
					default: {
						uint32_t size;
						read(in, size);
						if(code[0] == CODE_ALT_LIST) {
							size = flip_bytes(size);
						}
						visitor.list_begin(size);
						for(uint32_t i = 0; i < size; ++i) {
							visitor.list_element(i);
							accept(visitor, in, type_code, code + 1, field, nullptr);
						}
						visitor.list_end(size);
					}
				}
				return;
			}
			case CODE_MAP:
			case CODE_ALT_MAP: {
				uint32_t size;
				read(in, size);
				const uint16_t* key_code = code + 2;
				const uint16_t* value_code;
				if(code[0] == CODE_MAP) {
					value_code = code + code[1];
				} else {
					size = flip_bytes(size);
					value_code = code + flip_bytes(code[1]);
				}
				visitor.list_begin(size);
				for(uint32_t i = 0; i < size; ++i) {
					visitor.list_element(i);
					visitor.list_begin(2);
					visitor.list_element(0);
					accept(visitor, in, type_code, key_code, field, nullptr);
					visitor.list_element(1);
					accept(visitor, in, type_code, value_code, field, nullptr);
					visitor.list_end(2);
				}
				visitor.list_end(size);
				return;
			}
			case CODE_TYPE_CODE:
			case CODE_ALT_TYPE_CODE: {
				TypeCode tmp;
				vnx::read(in, tmp, type_code, code);
				vnx::accept(visitor, tmp);
				return;
			}
			case CODE_ANY:
			case CODE_ALT_ANY: {
				std::shared_ptr<Value> value = read(in);
				accept(visitor, value);
				return;
			}
			case CODE_DYNAMIC:
			case CODE_ALT_DYNAMIC: {
				uint16_t dyn_code[VNX_MAX_BYTE_CODE_SIZE];
				read_byte_code(in, dyn_code);
				accept(visitor, in, type_code, dyn_code, field, nullptr);
				return;
			}
			case CODE_PADDING:
			case CODE_ALT_PADDING: {
				throw std::logic_error("accept(): invalid code");
			}
			case CODE_MATRIX:
			case CODE_ALT_MATRIX: {
				const size_t N = (code[0] == CODE_MATRIX ? code[1] : flip_bytes(code[1]));
				size_t total_size = 1;
				bool is_array = true;
				for(size_t i = 0; i < N; ++i) {
					const size_t size = (code[0] == CODE_MATRIX ? code[2 + i] : flip_bytes(code[2 + i]));
					total_size *= size;
					if(i > 0 && size != 1) {
						is_array = false;
					}
				}
				const uint16_t* value_code = code + 2 + N;
				const size_t value_size = get_value_size(value_code, type_code);
				if(is_array) {
					visitor.list_begin(total_size);
					for(size_t i = 0; i < total_size; ++i) {
						visitor.list_element(i);
						accept(visitor, in, type_code, value_code, field, p_buf ? p_buf + i * value_size : nullptr);
					}
					visitor.list_end(total_size);
				} else {
					visitor.type_begin(2);
					visitor.type_field("size", 0);
					visitor.list_begin(N);
					for(size_t i = 0; i < N; ++i) {
						visitor.list_element(i);
						visitor.visit((code[0] == CODE_MATRIX ? code[2 + i] : flip_bytes(code[2 + i])));
					}
					visitor.list_end(N);
					visitor.type_field("data", 1);
					visitor.list_begin(total_size);
					for(size_t i = 0; i < total_size; ++i) {
						visitor.list_element(i);
						accept(visitor, in, type_code, value_code, field, p_buf ? p_buf + i * value_size : nullptr);
					}
					visitor.list_end(total_size);
					visitor.type_end(2);
				}
				return;
			}
			case CODE_IMAGE:
			case CODE_ALT_IMAGE: {
				const size_t N = (code[0] == CODE_IMAGE ? code[1] : flip_bytes(code[1]));
				size_t total_size = 1;
				std::vector<uint32_t> size(N);
				{
					const char* buf = in.read(4 * N);
					for(size_t i = 0; i < N; ++i) {
						read_value(buf + 4 * i, size[i]);
						if(code[0] == CODE_IMAGE) {
							total_size *= size[i];
						} else {
							total_size *= flip_bytes(size[i]);
						}
					}
				}
				visitor.type_begin(2);
				visitor.type_field("size", 0);
				visitor.list_begin(N);
				for(size_t i = 0; i < N; ++i) {
					visitor.list_element(i);
					visitor.visit(size[i]);
				}
				visitor.list_end(N);
				visitor.type_field("data", 1);
				const uint16_t* value_code = code + 2;
				visitor.list_begin(total_size);
				for(size_t i = 0; i < total_size; ++i) {
					visitor.list_element(i);
					accept(visitor, in, type_code, value_code, field, nullptr);
				}
				visitor.list_end(total_size);
				visitor.type_end(2);
				return;
			}
			case CODE_TUPLE:
			case CODE_ALT_TUPLE: {
				const size_t N = (code[0] == CODE_TUPLE ? code[1] : flip_bytes(code[1]));
				visitor.list_begin(N);
				for(size_t i = 0; i < N; ++i) {
					visitor.list_element(i);
					if(code[0] == CODE_TUPLE) {
						accept(visitor, in, type_code, code + code[2 + i], field, nullptr);
					} else {
						accept(visitor, in, type_code, code + flip_bytes(code[2 + i]), field, nullptr);
					}
				}
				visitor.list_end(N);
				return;
			}
			case CODE_OBJECT:
			case CODE_ALT_OBJECT: {
				Object tmp;
				read(in, tmp, type_code, code);
				accept(visitor, tmp);
				return;
			}
			case CODE_OPTIONAL:
			case CODE_ALT_OPTIONAL: {
				bool flag = false;
				read(in, flag);
				if(flag) {
					accept(visitor, in, type_code, code + 1, field, nullptr);
				} else {
					visitor.visit_null();
				}
				return;
			}
		}
	}
	if(!type_code) {
		if(code && code[0] != CODE_ANY && code[0] != CODE_ALT_ANY) {
			throw std::logic_error("accept(): expected CODE_ANY");
		}
		const uint16_t code = CODE_ANY;
		accept(visitor, in, nullptr, &code);
		return;
	}
	if(code) {
		if(code[0] != CODE_STRUCT && code[0] != CODE_ALT_STRUCT) {
			throw std::runtime_error("accept(): expected CODE_STRUCT");
		}
		type_code = type_code->depends[code[1]];
	}
	
	std::vector<char> buf(type_code->total_field_size);
	{
		const char* data = in.read(type_code->total_field_size);
		::memcpy(buf.data(), data, type_code->total_field_size);
	}
	if(type_code->is_enum) {
		uint32_t value = 0;
		if(type_code->field_map.size() > 0) {
			if(const auto* const _field = type_code->field_map[0]) {
				vnx::read_value(buf.data() + _field->offset, value, _field->code.data());
			}
		}
		auto iter = type_code->enum_map.find(value);
		if(iter != type_code->enum_map.end()) {
			visitor.enum_value(value, iter->second);
		} else {
			visitor.enum_value(value, "");
		}
	} else {
		visitor.type_begin(*type_code);
		size_t i = 0;
		for(const TypeField& field_ : type_code->fields) {
			visitor.type_field(field_, i++);
			accept(visitor, in, type_code, field_.code.data(), &field_, &buf[field_.offset]);
		}
		visitor.type_end(*type_code);
	}
}

void accept(Visitor& visitor, TypeInput& in, const TypeCode* type_code, const uint16_t* code) {
	accept(visitor, in, type_code, code, nullptr, nullptr);
}

template<typename T>
int compare_value(T A, T B) {
	if(A < B) {
		return -1;
	}
	if(A > B) {
		return 1;
	}
	return 0;
}

template<typename T>
int compare_integral(TypeInput& in_, const T A, const uint16_t* code_, bool is_float, bool is_signed) {
	switch(code_[0]) {
		case CODE_BOOL:
		case CODE_UINT8:
		case CODE_UINT16:
		case CODE_UINT32:
		case CODE_UINT64:
		case CODE_ALT_BOOL:
		case CODE_ALT_UINT8:
		case CODE_ALT_UINT16:
		case CODE_ALT_UINT32:
		case CODE_ALT_UINT64: {
			uint64_t B;
			vnx::read(in_, B, nullptr, code_);
			if(is_float) {
				return compare_value(double(A), double(B));
			}
			if(is_signed) {
				return compare_value(int64_t(A), int64_t(B));
			}
			return compare_value(uint64_t(A), B);
		}
		case CODE_INT8:
		case CODE_INT16:
		case CODE_INT32:
		case CODE_INT64:
		case CODE_ALT_INT8:
		case CODE_ALT_INT16:
		case CODE_ALT_INT32:
		case CODE_ALT_INT64: {
			int64_t B;
			vnx::read(in_, B, nullptr, code_);
			if(is_float) {
				return compare_value(double(A), double(B));
			}
			return compare_value(int64_t(A), B);
		}
		case CODE_FLOAT:
		case CODE_DOUBLE:
		case CODE_ALT_FLOAT:
		case CODE_ALT_DOUBLE: {
			double B;
			vnx::read(in_, B, nullptr, code_);
			return compare_value(double(A), B);
		}
		case CODE_OPTIONAL:
		case CODE_ALT_OPTIONAL: {
			bool flag = false;
			vnx::read(in_, flag);
			if(flag) {
				return compare_integral(in_, A, code_ + 1, is_float, is_signed);
			}
			return 1;
		}
	}
	return -1;
}

template<typename T>
int compare_integral_array(TypeInput& in, TypeInput& in_, size_t size, size_t size_, bool do_flip) {
	if(size != size_) {
		return compare_value(size, size_);
	}
	for(size_t i = 0; i < size; ++i) {
		T A, B;
		vnx::read(in, A);
		vnx::read(in_, B);
		if(do_flip) {
			A = vnx::flip_bytes(A);
			B = vnx::flip_bytes(B);
		}
		if(const int res = compare_value(A, B)) {
			return res;
		}
	}
	return 0;
}

static
int compare_array(TypeInput& in, TypeInput& in_, size_t size, size_t size_, const uint16_t* code, const uint16_t* code_) {
	if(code[0] == code_[0]) {
		switch(code[0]) {
			case CODE_BOOL:
			case CODE_ALT_BOOL:
				return compare_integral_array<bool>(in, in_, size, size_, false);
			case CODE_UINT8:
			case CODE_ALT_UINT8:
				return compare_integral_array<uint8_t>(in, in_, size, size_, false);
			case CODE_UINT16:
				return compare_integral_array<uint16_t>(in, in_, size, size_, false);
			case CODE_ALT_UINT16:
				return compare_integral_array<uint16_t>(in, in_, size, size_, true);
			case CODE_UINT32:
				return compare_integral_array<uint32_t>(in, in_, size, size_, false);
			case CODE_ALT_UINT32:
				return compare_integral_array<uint32_t>(in, in_, size, size_, true);
			case CODE_UINT64:
				return compare_integral_array<uint64_t>(in, in_, size, size_, false);
			case CODE_ALT_UINT64:
				return compare_integral_array<uint64_t>(in, in_, size, size_, true);
			case CODE_INT8:
			case CODE_ALT_INT8:
				return compare_integral_array<int8_t>(in, in_, size, size_, false);
			case CODE_INT16:
				return compare_integral_array<int16_t>(in, in_, size, size_, false);
			case CODE_ALT_INT16:
				return compare_integral_array<int16_t>(in, in_, size, size_, true);
			case CODE_INT32:
				return compare_integral_array<int32_t>(in, in_, size, size_, false);
			case CODE_ALT_INT32:
				return compare_integral_array<int32_t>(in, in_, size, size_, true);
			case CODE_INT64:
				return compare_integral_array<int64_t>(in, in_, size, size_, false);
			case CODE_ALT_INT64:
				return compare_integral_array<int64_t>(in, in_, size, size_, true);
			case CODE_FLOAT:
				return compare_integral_array<float32_t>(in, in_, size, size_, false);
			case CODE_ALT_FLOAT:
				return compare_integral_array<float32_t>(in, in_, size, size_, true);
			case CODE_DOUBLE:
				return compare_integral_array<float64_t>(in, in_, size, size_, false);
			case CODE_ALT_DOUBLE:
				return compare_integral_array<float64_t>(in, in_, size, size_, true);
		}
	}
	if(size != size_) {
		return compare_value(size, size_);
	}
	for(size_t i = 0; i < size; ++i) {
		if(const int res = compare(in, in_, code, code_)) {
			return res;
		}
	}
	return 0;
}

int compare(TypeInput& in, TypeInput& in_, const uint16_t* code, const uint16_t* code_) {
	switch(code_[0]) {
		case CODE_DYNAMIC:
		case CODE_ALT_DYNAMIC: {
			uint16_t dyn_code_[VNX_MAX_BYTE_CODE_SIZE];
			vnx::read_byte_code(in_, dyn_code_);
			return compare(in, in_, code, dyn_code_);
		}
		case CODE_OPTIONAL:
		case CODE_ALT_OPTIONAL: {
			bool flag = false;
			vnx::read(in_, flag);
			if(flag) {
				return compare(in, in_, code, code_ + 1);
			}
			const uint16_t null = CODE_NULL;
			return compare(in, in_, code, &null);
		}
	}
	switch(code[0]) {
		case CODE_BOOL:
		case CODE_UINT8:
		case CODE_UINT16:
		case CODE_UINT32:
		case CODE_UINT64:
		case CODE_ALT_BOOL:
		case CODE_ALT_UINT8:
		case CODE_ALT_UINT16:
		case CODE_ALT_UINT32:
		case CODE_ALT_UINT64: {
			uint64_t A;
			vnx::read(in, A, 0, code);
			return compare_integral(in_, A, code_, false, false);
		}
		case CODE_INT8:
		case CODE_INT16:
		case CODE_INT32:
		case CODE_INT64:
		case CODE_ALT_INT8:
		case CODE_ALT_INT16:
		case CODE_ALT_INT32:
		case CODE_ALT_INT64: {
			int64_t A;
			vnx::read(in, A, 0, code);
			return compare_integral(in_, A, code_, false, true);
		}
		case CODE_FLOAT:
		case CODE_DOUBLE:
		case CODE_ALT_FLOAT:
		case CODE_ALT_DOUBLE: {
			double A;
			vnx::read(in, A, 0, code);
			return compare_integral(in_, A, code_, true, false);
		}
		case CODE_STRING:
		case CODE_ALT_STRING: {
			std::string A;
			std::string B;
			vnx::read(in, A, nullptr, code);
			vnx::read(in_, B, nullptr, code_);
			return A.compare(B);
		}
		case CODE_ARRAY:
		case CODE_ALT_ARRAY:
			switch(code_[0]) {
				case CODE_ARRAY:
				case CODE_ALT_ARRAY: {
					uint16_t size = code[1];
					uint16_t size_ = code_[1];
					if(code[0] == CODE_ALT_ARRAY) {
						size = flip_bytes(size);
					}
					if(code_[0] == CODE_ALT_ARRAY) {
						size_ = flip_bytes(size_);
					}
					return compare_array(in, in_, size, size_, code + 2, code_ + 2);
				}
			}
			break;
		case CODE_LIST:
		case CODE_ALT_LIST:
			switch(code_[0]) {
				case CODE_LIST:
				case CODE_ALT_LIST: {
					uint32_t size = 0;
					uint32_t size_ = 0;
					vnx::read(in, size);
					vnx::read(in_, size_);
					if(code[0] == CODE_ALT_LIST) {
						size = flip_bytes(size);
					}
					if(code_[0] == CODE_ALT_LIST) {
						size_ = flip_bytes(size_);
					}
					return compare_array(in, in_, size, size_, code + 1, code_ + 1);
				}
				case CODE_STRING:
				case CODE_ALT_STRING: {
					std::string A;
					std::string B;
					vnx::read(in, A, nullptr, code);
					vnx::read(in_, B, nullptr, code_);
					return A.compare(B);
				}
			}
			break;
		case CODE_MAP:
		case CODE_ALT_MAP:
			switch(code_[0]) {
				case CODE_MAP:
				case CODE_ALT_MAP: {
					uint32_t size = 0;
					uint32_t size_ = 0;
					vnx::read(in, size);
					vnx::read(in_, size_);
					if(code[0] == CODE_ALT_MAP) {
						size = flip_bytes(size);
					}
					if(code_[0] == CODE_ALT_MAP) {
						size_ = flip_bytes(size_);
					}
					const auto* value_code = code + (code[0] == CODE_MAP ? code[1] : flip_bytes(code[1]));
					const auto* value_code_ = code_ + (code_[0] == CODE_MAP ? code_[1] : flip_bytes(code_[1]));
					for(uint32_t i = 0; i < std::min(size, size_); ++i) {
						if(const int res = compare(in, in_, code + 2, code_ + 2)) {
							return res;
						}
						if(const int res = compare(in, in_, value_code, value_code_)) {
							return res;
						}
					}
					return compare_value(size, size_);
				}
			}
			break;
		case CODE_MATRIX:
		case CODE_ALT_MATRIX:
			switch(code_[0]) {
				case CODE_MATRIX:
				case CODE_ALT_MATRIX: {
					const uint16_t N = (code[0] == CODE_MATRIX ? code[1] : flip_bytes(code[1]));
					const uint16_t N_ = (code_[0] == CODE_MATRIX ? code_[1] : flip_bytes(code_[1]));
					if(N == N_) {
						size_t total_size = 1;
						for(uint16_t i = 0; i < N; ++i) {
							const uint16_t M = (code[0] == CODE_MATRIX ? code[2 + i] : flip_bytes(code[2 + i]));
							const uint16_t M_ = (code_[0] == CODE_MATRIX ? code_[2 + i] : flip_bytes(code_[2 + i]));
							if(M != M_) {
								return M < M_ ? -1 : 1;
							}
							total_size *= M;
						}
						return compare_array(in, in_, total_size, total_size, code + 2 + N, code_ + 2 + N_);
					}
					return compare_value(N, N_);
				}
			}
			break;
		case CODE_IMAGE:
		case CODE_ALT_IMAGE:
			switch(code_[0]) {
				case CODE_IMAGE:
				case CODE_ALT_IMAGE: {
					const uint16_t N = (code[0] == CODE_IMAGE ? code[1] : flip_bytes(code[1]));
					const uint16_t N_ = (code_[0] == CODE_IMAGE ? code_[1] : flip_bytes(code_[1]));
					if(N == N_) {
						size_t total_size = 1;
						for(uint16_t i = 0; i < N; ++i) {
							uint32_t M = 0;
							uint32_t M_ = 0;
							vnx::read(in, M);
							vnx::read(in_, M_);
							if(code[0] == CODE_ALT_IMAGE) {
								M = flip_bytes(M);
							}
							if(code_[0] == CODE_ALT_IMAGE) {
								M_ = flip_bytes(M_);
							}
							if(M != M_) {
								return M < M_ ? -1 : 1;
							}
							total_size *= M;
						}
						return compare_array(in, in_, total_size, total_size, code + 2, code_ + 2);
					}
					return compare_value(N, N_);
				}
			}
			break;
		case CODE_DYNAMIC:
		case CODE_ALT_DYNAMIC: {
			uint16_t dyn_code[VNX_MAX_BYTE_CODE_SIZE];
			vnx::read_byte_code(in, dyn_code);
			return compare(in, in_, dyn_code, code_);
		}
		case CODE_ANY:
		case CODE_ALT_ANY:
			switch(code_[0]) {
				case CODE_ANY:
				case CODE_ALT_ANY: {
					auto value = vnx::read(in);
					auto value_ = vnx::read(in_);
					if(!value && value_) {
						return -1;
					}
					if(value && !value_) {
						return 1;
					}
					if(!value && !value_) {
						return 0;
					}
					if(value->get_type_hash() < value_->get_type_hash()) {
						return -1;
					}
					if(value->get_type_hash() > value_->get_type_hash()) {
						return 1;
					}
					return 0;
				}
			}
			break;
		case CODE_TUPLE:
		case CODE_ALT_TUPLE:
			switch(code_[0]) {
				case CODE_TUPLE:
				case CODE_ALT_TUPLE: {
					const uint16_t N = (code[0] == CODE_TUPLE ? code[1] : flip_bytes(code[1]));
					const uint16_t N_ = (code_[0] == CODE_TUPLE ? code_[1] : flip_bytes(code_[1]));
					if(N == N_) {
						for(uint16_t i = 0; i < N; ++i) {
							const uint16_t off = (code[0] == CODE_TUPLE ? code[2 + i] : flip_bytes(code[2 + i]));
							const uint16_t off_ = (code_[0] == CODE_TUPLE ? code_[2 + i] : flip_bytes(code_[2 + i]));
							if(const int res = compare(in, in_, code + off, code_ + off_)) {
								return res;
							}
						}
						return 0;
					}
					return compare_value(N, N_);
				}
			}
			break;
		case CODE_OBJECT:
		case CODE_ALT_OBJECT:
			switch(code_[0]) {
				case CODE_OBJECT:
				case CODE_ALT_OBJECT:
					return compare(in, in_,
							code[0] == CODE_OBJECT ? Object::CODE : Object::ALT_CODE,
							code_[0] == CODE_OBJECT ? Object::CODE : Object::ALT_CODE);
			}
			break;
		case CODE_OPTIONAL:
		case CODE_ALT_OPTIONAL: {
			bool flag = false;
			vnx::read(in, flag);
			if(flag) {
				return compare(in, in_, code + 1, code_);
			}
			const uint16_t null = CODE_NULL;
			return compare(in, in_, &null, code_);
		}
	}
	return compare_value(code[0], code_[0]);
}

template<typename T>
void calc_hash(TypeInput& in, const uint16_t* code, T& func);

template<typename T>
void calc_hash_array(TypeInput& in, size_t size, const uint16_t* code, T& func) {
	switch(code[0]) {
		case CODE_BOOL:
		case CODE_INT8:
		case CODE_UINT8:
		case CODE_ALT_BOOL:
		case CODE_ALT_INT8:
		case CODE_ALT_UINT8: {
			if(size < VNX_BUFFER_SIZE) {
				func.update(in.read(size), size);
			} else {
				std::vector<char> buf(size);
				in.read(buf.data(), size);
				func.update(buf.data(), size);
			}
			return;
		}
		default:
			for(size_t i = 0; i < size; ++i) {
				calc_hash(in, code, func);
			}
	}
}

template<typename T>
void calc_hash(TypeInput& in, const uint16_t* code, T& func) {
	switch(code[0]) {
		case CODE_BOOL:
		case CODE_INT8:
		case CODE_UINT8:
		case CODE_ALT_BOOL:
		case CODE_ALT_INT8:
		case CODE_ALT_UINT8:
			func.update(in.read(1), 1);
			return;
		case CODE_INT16:
		case CODE_UINT16:
			func.update(in.read(2), 2);
			return;
		case CODE_INT32:
		case CODE_UINT32:
		case CODE_FLOAT:
			func.update(in.read(4), 4);
			return;
		case CODE_INT64:
		case CODE_UINT64:
		case CODE_DOUBLE:
			func.update(in.read(8), 8);
			return;
		case CODE_ALT_INT16:
		case CODE_ALT_UINT16: {
			uint16_t tmp;
			vnx::read(in, tmp);
			tmp = vnx::flip_bytes(tmp);
			func.update((const char*)&tmp, sizeof(tmp));
			return;
		}
		case CODE_ALT_INT32:
		case CODE_ALT_UINT32:
		case CODE_ALT_FLOAT: {
			uint32_t tmp;
			vnx::read(in, tmp);
			tmp = vnx::flip_bytes(tmp);
			func.update((const char*)&tmp, sizeof(tmp));
			return;
		}
		case CODE_ALT_INT64:
		case CODE_ALT_UINT64:
		case CODE_ALT_DOUBLE: {
			uint64_t tmp;
			vnx::read(in, tmp);
			tmp = vnx::flip_bytes(tmp);
			func.update((const char*)&tmp, sizeof(tmp));
			return;
		}
		case CODE_STRING:
		case CODE_ALT_STRING: {
			uint32_t size = 0;
			vnx::read(in, size);
			if(code[0] == CODE_ALT_STRING) {
				size = flip_bytes(size);
			}
			const uint16_t value_code[] = {CODE_INT8};
			calc_hash_array(in, size, value_code, func);
			return;
		}
		case CODE_ARRAY:
		case CODE_ALT_ARRAY: {
			uint16_t size = code[1];
			if(code[0] == CODE_ALT_ARRAY) {
				size = flip_bytes(size);
			}
			calc_hash_array(in, size, code + 2, func);
			return;
		}
		case CODE_LIST:
		case CODE_ALT_LIST: {
			uint32_t size = 0;
			vnx::read(in, size);
			if(code[0] == CODE_ALT_LIST) {
				size = flip_bytes(size);
			}
			calc_hash_array(in, size, code + 1, func);
			return;
		}
		case CODE_MAP:
		case CODE_ALT_MAP: {
			uint32_t size = 0;
			vnx::read(in, size);
			if(code[0] == CODE_ALT_MAP) {
				size = flip_bytes(size);
			}
			T value;
			const auto* value_code = code + (code[0] == CODE_MAP ? code[1] : flip_bytes(code[1]));
			for(uint32_t i = 0; i < size; ++i) {
				calc_hash(in, code + 2, func);
				calc_hash(in, value_code, func);
			}
			return;
		}
		case CODE_MATRIX:
		case CODE_ALT_MATRIX: {
			const uint16_t N = (code[0] == CODE_MATRIX ? code[1] : flip_bytes(code[1]));
			size_t total_size = 1;
			for(uint16_t i = 0; i < N; ++i) {
				const uint16_t M = (code[0] == CODE_MATRIX ? code[2 + i] : flip_bytes(code[2 + i]));
				total_size *= M;
			}
			calc_hash_array(in, total_size, code + 2 + N, func);
			return;
		}
		case CODE_IMAGE:
		case CODE_ALT_IMAGE: {
			const uint16_t N = (code[0] == CODE_IMAGE ? code[1] : flip_bytes(code[1]));
			size_t total_size = 1;
			for(uint16_t i = 0; i < N; ++i) {
				uint32_t M = 0;
				vnx::read(in, M);
				if(code[0] == CODE_ALT_IMAGE) {
					M = flip_bytes(M);
				}
				total_size *= M;
			}
			calc_hash_array(in, total_size, code + 2, func);
			return;
		}
		case CODE_DYNAMIC:
		case CODE_ALT_DYNAMIC: {
			uint16_t dyn_code[VNX_MAX_BYTE_CODE_SIZE];
			vnx::read_byte_code(in, dyn_code);
			calc_hash(in, dyn_code, func);
			return;
		}
		case CODE_ANY:
		case CODE_ALT_ANY: {
			if(auto value = vnx::read(in)) {
				const auto type_hash = value->get_type_hash();
				func.update((const char*)&type_hash.value, sizeof(type_hash.value));
			}
			return;
		}
		case CODE_TUPLE:
		case CODE_ALT_TUPLE: {
			const uint16_t N = (code[0] == CODE_TUPLE ? code[1] : flip_bytes(code[1]));
			for(uint16_t i = 0; i < N; ++i) {
				const uint16_t off = (code[0] == CODE_TUPLE ? code[2 + i] : flip_bytes(code[2 + i]));
				calc_hash(in, code + off, func);
			}
			return;
		}
		case CODE_OBJECT:
			calc_hash(in, Object::CODE, func);
			return;
		case CODE_ALT_OBJECT:
			calc_hash(in, Object::ALT_CODE, func);
			return;
		case CODE_OPTIONAL:
		case CODE_ALT_OPTIONAL: {
			bool flag = false;
			vnx::read(in, flag);
			if(flag) {
				calc_hash(in, code + 1, func);
			}
			return;
		}
		default:
			func.update((const char*)code, sizeof(uint16_t));
	}
}

Hash64 calc_hash(TypeInput& in, const uint16_t* code) {
	CRC64 func;
	calc_hash(in, code, func);
	return Hash64(func.get());
}


} // vnx
