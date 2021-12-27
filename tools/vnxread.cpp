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


int main(int argc, char** argv) {
	
	std::map<std::string, std::string> options;
	options["1"] = "compact";
	options["t"] = "type-only";
	options["f"] = "file";
	options["file"] = "filename";
	
	vnx::set_config("log_level", "2");

	vnx::init("vnxread", argc, argv, options);
	
	bool compact = false;
	bool type_only = false;
	vnx::read_config("compact", compact);
	vnx::read_config("type-only", type_only);
	
	std::vector<std::string> file_names;
	vnx::read_config("file", file_names);
	
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
				if(type_only) {
					auto binary = std::dynamic_pointer_cast<const vnx::Binary>(value);
					if(binary) {
						std::cout << binary->type_code->name << std::endl;
					} else {
						std::cout << value->get_type_name() << std::endl;
					}
				} else if(compact) {
					std::cout << *value << std::endl;
				} else {
					vnx::PrettyPrinter printer(std::cout);
					value->accept(printer);
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


