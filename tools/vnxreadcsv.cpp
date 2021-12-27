/*
 * vnxreadcsv.cpp
 *
 *  Created on: Apr 13, 2021
 *      Author: mad
 */

#include <vnx/vnx.h>
#include <vnx/Binary.h>


int main(int argc, char** argv) {

	std::map<std::string, std::string> options;
	options["t"] = "topic";
	options["n"] = "name";
	options["v"] = "values";
	options["f"] = "file";
	options["file"] = "filename";
	options["topic"] = "topic name";
	options["name"] = "type name";
	options["values"] = "list of field names";

	vnx::set_config("log_level", "2");

	vnx::init("vnxreadcsv", argc, argv, options);

	std::string type_name;
	std::string topic_name;
	vnx::read_config("name", type_name);
	vnx::read_config("topic", topic_name);

	std::vector<std::string> fields;
	std::vector<std::string> file_names;
	vnx::read_config("values", fields);
	vnx::read_config("file", file_names);

	{
		int i = 0;
		std::cout << "# ";
		for(const auto& field : fields) {
			if(i++) {
				std::cout << ", ";
			}
			std::cout << field;
		}
		std::cout << std::endl;
	}

	for(const auto& file_name : file_names) {
		::FILE* file = ::fopen(file_name.c_str(), "rb");
		if(!file) {
			std::cerr << "fopen() failed for: '" << file_name << "'" << std::endl;
			continue;
		}
		vnx::FileInputStream stream(file);
		vnx::TypeInput in(&stream);

		try {
			while(std::shared_ptr<const vnx::Value> value = vnx::read(in)) {
				if(auto decompressed = value->vnx_decompress()) {
					value = decompressed;
				}
				bool match = true;
				if(!topic_name.empty()) {
					match = false;
					if(auto sample = std::dynamic_pointer_cast<const vnx::Sample>(value)) {
						if(sample->topic->get_name() == topic_name) {
							if(type_name.empty() || (sample->value && sample->value->get_type_name() == type_name)) {
								match = true;
								value = sample->value;
							}
						}
					}
				}
				else if(!type_name.empty()) {
					match = value->get_type_name() == type_name;
				}
				if(match && value) {
					int i = 0;
					for(const auto& field : fields) {
						if(i++) {
							std::cout << ", ";
						}
						const auto var = value->get_field(field);
						std::cout << var.to_string();
					}
					std::cout << std::endl;
				}
			}
		} catch(std::underflow_error& ) {
			// ignore
		} catch(std::exception& ex) {
			std::cerr << ex.what() << std::endl;
		}
		::fclose(file);
	}

	vnx::close();
}


