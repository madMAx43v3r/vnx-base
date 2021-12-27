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

#include <algorithm>
#include <unistd.h>
#ifdef _WIN32
#include <winsock2.h>
#endif


int main(int argc, char** argv) {
	
	std::map<std::string, std::string> options;
	options["n"] = "node";
	options["u"] = "user";
	options["node"] = "url";
	options["user"] = "login as user";
	
	vnx::set_config("log_level", "2");
	
	vnx::init("vnxtopic", argc, argv, options);
	
	std::string node = "localhost";
	std::string user;
	vnx::read_config("node", node);
	vnx::read_config("user", user);
	
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
	try {
		vnx::ProxyClient pcl("Proxy");
		if(!user.empty()) {
			pcl.login(user, password);
		} else {
			pcl.wait_on_connect();
		}
	} catch(...) {
		vnx::close();
		return -1;
	}
	vnx::ProcessClient process("vnx.process");
	process.pause_log();

	vnx::ProcessClient client("vnx.process");
	client.vnx_set_tunnel(tunnel_addr);
	try {
		std::vector<vnx::TopicInfo> list = client.get_topic_info(false);
		std::sort(list.begin(), list.end(), [](const vnx::TopicInfo& A, const vnx::TopicInfo& B) -> bool {
			return A.name < B.name;
		});
		for(const auto& topic : list) {
			std::cout << topic.name;
			for(size_t i = topic.name.size(); i < 30; ++i) {
				std::cout << " ";
			}
			std::cout << " : " << topic.num_subscribers << " subscribers, " << topic.sample_count << " samples" << std::endl;
		}
	} catch(const std::exception& ex) {
		std::cerr << "ERROR: " << ex.what() << std::endl;
	}
	process.resume_log_async();
	
	vnx::close();
	return 0;
}

