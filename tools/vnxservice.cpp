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
#include <vnx/Proxy.h>
#include <vnx/ProxyClient.hxx>
#include <vnx/GenericClient.h>

#include <unistd.h>
#ifdef _WIN32
#include <winsock2.h>
#endif


int main(int argc, char** argv) {

	std::map<std::string, std::string> options;
	options["n"] = "node";
	options["i"] = "info";
	options["x"] = "call";
	options["u"] = "user";
	options["g"] = "gateway";
	options["node"] = "url";
	options["info"] = "<service> [method] ...";
	options["call"] = "<service> <method> [arg] ...";
	options["user"] = "login as user";
	options["gateway"] = "gateway name";

	vnx::set_config("log_level", "2");

	vnx::init("vnxservice", argc, argv, options);

	std::string node = "localhost";
	std::string user;
	std::string gateway;
	std::vector<vnx::Variant> call;
	std::vector<std::string> info;
	vnx::read_config("node", node);
	vnx::read_config("user", user);
	vnx::read_config("gateway", gateway);
	vnx::read_config("call", call);
	vnx::read_config("info", info);

	auto endpoint = vnx::Endpoint::from_url(node);

	const int socket = endpoint->open();
	try {
		endpoint->connect(socket);
	} catch(const std::exception& ex) {
		std::cerr << "ERROR: " << ex.what() << std::endl;
#ifdef _WIN32
		closesocket(socket);
#else
		::close(socket);
#endif
		vnx::close();
		return -1;
	}

	const bool is_info = info.size() && info[0] != "1";
	const bool is_call = !is_info && call.size();
	const bool is_list = !is_info && !is_call;

	std::string service;
	std::string call_method;
	std::vector<std::string> info_methods;
	std::vector<vnx::Variant> call_args;

	if(is_info) {
		service = info[0];
		for(size_t i = 1; i < info.size(); ++i) {
			info_methods.push_back(info[i]);
		}
	}
	if(is_call) {
		if(call.size() < 2) {
			std::cerr << "ERROR: Too few arguments for call." << std::endl;
			vnx::close();
			return -1;
		}
		service = call[0].to_string_value();
		call_method = call[1].to_string_value();
		for(size_t i = 2; i < call.size(); ++i) {
			call_args.push_back(call[i]);
		}
	}
	if(is_list) {
		std::cout << "TODO" << std::endl;
		vnx::close();
		return 0;
	}

	const vnx::Hash64 tunnel_addr = vnx::Hash64::rand();

	std::string password;
	if(!user.empty()){
		password = vnx::input_password("Password for user " + user + ": ");
	}

	vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("Proxy", endpoint, socket);
	proxy->tunnel_list.push_back(tunnel_addr);
	proxy.start();

	if(!user.empty()){
		vnx::ProxyClient pcl("Proxy");
		pcl.login(user, password);
	}

	int retcode = 0;
	vnx::PrettyPrinter printer(std::cout);

	vnx::GenericClient client(service);
	client.vnx_set_tunnel(tunnel_addr);
	if(!gateway.empty()) {
		client.vnx_set_gateway(gateway);
	}
	try {
		const vnx::TypeCode type_code = client.vnx_get_type_code();

		const vnx::TypeCode* method_type = nullptr;
		for(auto func : type_code.methods) {
			const auto method_name = func->get_method_name();
			if(is_call) {
				if(func->name == call_method || method_name == call_method) {
					method_type = func;
					break;
				}
			}
			if(is_info) {
				if(info.size() == 1) {
					func->print(std::cout);
				} else {
					for(const auto& method : info_methods) {
						if(func->name == method || method_name == method) {
							func->print(std::cout);
							break;
						}
					}
				}
			}
		}

		if(is_call) {
			if(!method_type) {
				throw std::logic_error("no such method: '" + call_method + "'");
			}
			vnx::Object args;
			for(size_t i = 0; i < call_args.size(); ++i) {
				if(i < method_type->fields.size()) {
					args[method_type->fields[i].name] = call_args[i];
				}
			}
			auto result = client.call(call_method, args);
			if(auto ex = std::dynamic_pointer_cast<const vnx::Exception>(result)) {
				vnx::accept(printer, ex);
				std::cerr << std::endl;
			} else if(!result->is_void()) {
				const auto ret = result->get_field_by_index(0);
				vnx::accept(printer, ret);
				std::cerr << std::endl;
			}
		}
	}
	catch(const std::exception& ex) {
		retcode = -1;
		std::cerr << "ERROR: " << ex.what() << std::endl;
	}

	vnx::close();
	return retcode;
}

