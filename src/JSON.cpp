/*
 * JSON.cpp
 *
 *  Created on: Dec 3, 2020
 *      Author: mad
 */

#include <vnx/vnx.h>
#include <vnx/JSON.h>


namespace vnx {

void write(std::ostream& out, const JSON_Value& value) {
	value.write(out);
}

std::string JSON_String::to_string() const {
	return vnx::to_string(data);
}

Variant JSON_Array::to_variant() const {
	std::vector<Variant> tmp;
	for(const auto& value : values) {
		if(value) {
			tmp.emplace_back(value->to_variant());
		}
	}
	return Variant(tmp);
}

std::string JSON_Array::to_string() const {
	return vnx::to_string(values);
}

Object JSON_Object::to_object() const {
	Object obj;
	for(const auto& entry : field) {
		if(entry.second) {
			obj[entry.first] = entry.second->to_variant();
		}
	}
	return obj;
}

std::string JSON_Object::to_string() const {
	return to_object().to_string();
}


std::shared_ptr<JSON_Array> read_array(std::istream& in);

std::shared_ptr<JSON_Object> read_object(std::istream& in);

std::shared_ptr<JSON_Value> read_json(std::istream& in, bool until_eof, bool until_space) {
	std::string value;
	{
		bool escape = false;
		bool is_string = false;
		while(true) {
			const char c = in.peek();
			if(!in.good()) {
				break;
			}
			if(is_string) {
				if(c == '"' && !escape) {
					in.get();
					return std::make_shared<JSON_String>(value);
				} else if(c == '\\' && !escape) {
					escape = true;
					in.get();
					continue;
				} else if(escape) {
					switch(c) {
						case 't': value.push_back('\t'); break;
						case 'r': value.push_back('\r'); break;
						case 'n': value.push_back('\n'); break;
						case 'b': value.push_back('\b'); break;
						case 'f': value.push_back('\f'); break;
						default: value.push_back(c);
					}
					escape = false;
					in.get();
					continue;
				}
			} else if(c == '"') {
				is_string = true;
				in.get();
				continue;
			} else if(c == '[') {
				return read_array(in);
			} else if(c == '{') {
				return read_object(in);
			} else if(c == ':' || c == ',' || c == '}' || c == ']') {
				if(!until_eof) {
					break;
				}
			} else if(until_space && !value.empty() && std::isspace(c)) {
				break;
			}
			if(is_string || !std::isspace(c)) {
				value.push_back(c);
			}
			in.get();
			escape = false;
		}
	}
	if(value.empty()) {
		return nullptr;
	}
	bool is_number = false;
	bool is_hex = false;
	bool is_float = false;
	bool is_negative = false;
	bool has_exp = false;
	bool has_exp_sign = false;

	size_t length = 0;
	for(const char c : value) {
		if(std::isdigit(c)) {
			is_number = true;
		} else if(c == '-' && !is_number) {
			is_negative = true;
		} else if(c == '+' && !is_number) {
			// maybe positive number
		} else if((c == '-' || c == '+') && has_exp && !has_exp_sign) {
			has_exp_sign = true;
		} else if((c == '.' || c == ',') && is_number && !is_float) {
			is_float = true;
		} else if((c == 'e' || c == 'E') && is_number && !has_exp && !is_hex) {
			is_float = true;
			has_exp = true;
		} else if(c == 'x' && is_number && !is_hex && length == 1) {
			is_hex = true;
		} else if(is_hex) {
			const auto h = std::tolower(c);
			if(h < 97 && h > 102) {
				is_number = false;
				break;
			}
		} else {
			is_number = false;
			break;
		}
		length++;
	}

	Variant var;
	if(is_number) {
		if(is_float) {
			std::replace(value.begin(), value.end(), ',', '.');
			std::stringstream ss(value);
			double tmp = 0;
			ss >> tmp;
			var = tmp;
		} else {
			if(is_negative) {
				var = int64_t(std::stoll(value, nullptr, is_hex ? 16 : 10));
			} else {
				var = uint64_t(std::stoull(value, nullptr, is_hex ? 16 : 10));
			}
		}
	} else {
		if(value == "true") {
			var = true;
		} else if(value == "false") {
			var = false;
		} else if(value == "null") {
			var = nullptr;
		} else {
			// support strings without quotes
			return std::make_shared<JSON_String>(value);
		}
	}
	return std::make_shared<JSON_Variant>(Variant(var));
}

std::shared_ptr<JSON_Array> read_array(std::istream& in) {
	auto array = std::make_shared<JSON_Array>();
	int stack = 0;
	while(true) {
		const char c = in.peek();
		if(!in.good()) {
			throw std::runtime_error("invalid json: unexpected end of data when parsing array");
		}
		if(!stack && c == '[') {
			stack++;
			in.get();
		} else if(stack && c == ']') {
			in.get();
			break;
		} else if(std::isspace(c) || c == ',') {
			if(c == ',' && !stack) {
				throw std::runtime_error("invalid json: unexpected ',' when parsing array");
			}
			in.get();
		} else if(stack) {
			if(auto value = read_json(in)) {
				array->push_back(value);
			} else {
				break;
			}
		} else if(std::isspace(c)) {
			in.get();
		} else {
			if(auto value = read_json(in)) {
				array->push_back(value);
			}
			break;
		}
	}
	return array;
}

std::shared_ptr<JSON_Object> read_object(std::istream& in) {
	auto object = std::make_shared<JSON_Object>();
	int stack = 0;
	bool done = false;
	while(!done) {
		const char c = in.get();
		if(!in.good()) {
			throw std::runtime_error("invalid json: unexpected end of data when parsing object");
		}
		if(c == '{') {
			if(stack > 0) {
				throw std::runtime_error("invalid json: excess '{' when parsing object");
			}
			stack++;
		}
		while(stack == 1) {
			if(auto field = std::dynamic_pointer_cast<JSON_String>(read_json(in)))
			{
				const auto name = field->get_value();
				while(true) {
					const char c = in.get();
					if(c == ':') {
						break;
					} else if(!in.good() || !std::isspace(c)) {
						throw std::runtime_error("invalid json: expected ':' after object field \"" + name + "\"");
					}
				}
				if(auto value = read_json(in)) {
					(*object)[name] = value;
				} else {
					throw std::runtime_error("invalid json: unexpected end of data when parsing object value \"" + name + "\"");
				}
			}
			while(true) {
				const char c = in.get();
				if(c == ',') {
					break;
				} else if(c == '}') {
					stack--;
					done = true;
					break;
				} else if(!in.good() || !std::isspace(c)) {
					throw std::runtime_error("invalid json: expected ',' or '}' after field value");
				}
			}
		}
	}
	return object;
}


} // vnx
