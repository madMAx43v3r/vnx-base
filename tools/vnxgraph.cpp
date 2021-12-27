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
#include <vnx/ProcessClient.hxx>

#include <unistd.h>
#ifdef _WIN32
#include <winsock2.h>
#endif


int main(int argc, char** argv) {
	
	std::map<std::string, std::string> options;
	options["n"] = "nodes";
	options["u"] = "user";
	options["nodes"] = "list of node urls";
	options["user"] = "login as user";
	
	vnx::set_config("log_level", "2");
	
	vnx::init("vnxgraph", argc, argv, options);
	
	std::vector<std::string> nodes = {"localhost"};
	std::string user;
	vnx::read_config("nodes", nodes);
	vnx::read_config("user", user);
	
	std::string password;
	if(!user.empty()){
		password = vnx::input_password("Password for user " + user + ": ");
	}

	std::cout << "digraph vnxgraph {" << std::endl;
	std::cout << "  compound = true;" << std::endl;
	std::cout << "  concentrate = true;" << std::endl;
	std::cout << "  splines = true;" << std::endl;
	
	std::vector<std::ostringstream> global;
	
	for(const std::string& node : nodes) {
		
		auto endpoint = vnx::Endpoint::from_url(node);
		const int socket = endpoint->open();
		try {
			endpoint->connect(socket);
		} catch(const std::exception& ex) {
			std::cerr << "ERROR: " << ex.what() << " (" << endpoint->to_url() << ")" << std::endl;
#ifdef _WIN32
			closesocket(socket);
#else
			::close(socket);
#endif
			continue;
		}
		
		const vnx::Hash64 tunnel_addr = vnx::Hash64::rand();
		
		vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("Proxy", endpoint, socket);
		proxy->tunnel_list.push_back(tunnel_addr);
		proxy.start();
		
		if(!user.empty()){
			vnx::ProxyClient pcl("Proxy");
			pcl.login(user, password);
		}

		vnx::ProcessClient client("vnx.process");
		client.vnx_set_tunnel(tunnel_addr);
		try {
			const vnx::ProcessInfo info = client.get_process_info();
			std::cout << "  subgraph cluster_" << info.id.to_hex_string() << " {" << std::endl;
			std::cout << "    label = \"" << info.name << "\";" << std::endl;
			std::cout << "    labeljust = \"l\";" << std::endl;
			std::cout << "    \"" << info.id.to_hex_string() << "\" [shape=point, style=invis];" << std::endl;
			for(const auto& module : info.modules) {
				if(module.name == info.name) {
					continue;
				}
				if(std::abs(info.time - module.time) > 3000000) {
					continue;
				}
				std::cout << "    \"" << module.id.to_hex_string() << "\" [label=\"" << module.name << "\", style=filled, fillcolor=lightgrey, shape=box];" << std::endl;
				for(const auto& topic : module.sub_topics) {
					if(topic.substr(0, 4) != "vnx.") {
						std::cout << "    \"" << topic << "\" -> \"" << module.id.to_hex_string() << "\" [color=red3];" << std::endl;
					}
				}
				for(const auto& topic : module.pub_topics) {
					if(topic.substr(0, 4) != "vnx.") {
						std::cout << "    \"" << module.id.to_hex_string() << "\" -> \"" << topic << "\" [color=blue3];" << std::endl;
					}
				}
				for(const auto& entry : module.remotes) {
					global.emplace_back();
					global.back() << "  \"" << module.id.to_hex_string() << "\" -> \"" << entry.first.to_hex_string() << "\" [lhead=\"cluster_" << entry.first.to_hex_string() << "\", color=green3];" << std::endl;
				}
			}
			std::cout << "  }" << std::endl;
			
		} catch(const std::exception& ex) {
			std::cerr << "ERROR: node '" << node << "': " << ex.what() << std::endl;
		}
		
		proxy.close();
	}
	
	for(const auto& entry : global) {
		std::cout << entry.str();
	}
	
	std::cout << "}" << std::endl;
	
	vnx::close();
	
}

