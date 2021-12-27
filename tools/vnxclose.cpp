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
#include <vnx/Terminal.h>
#include <vnx/ProcessClient.hxx>

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
	
	vnx::init("vnxclose", argc, argv, options);
	
	std::string node;
	std::string user;
	std::shared_ptr<const vnx::Endpoint> endpoint;
	if(vnx::read_config("node", node)) {
		endpoint = vnx::Endpoint::from_url(node);
	} else {
		std::cerr << "ERROR: Missing node argument" << std::endl;
		vnx::close();
		return -1;
	}
	vnx::read_config("user", user);
	
	const int socket = endpoint->open();
	try {
		endpoint->connect(socket);
	} catch(...) {
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
	
	if(!user.empty()){
		vnx::ProxyClient pcl("Proxy");
		pcl.login(user, password);
	}

	vnx::ProcessClient client("vnx.process");
	client.vnx_set_tunnel(tunnel_addr);
	try {
		std::string name = client.get_name();
		client.trigger_shutdown_async();
		std::cerr << "Closed process '" << name << "'" << std::endl;
	} catch(const std::exception& ex) {
		std::cerr << "ERROR: " << ex.what() << std::endl;
	}
	
	vnx::close();
	
}


