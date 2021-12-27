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
#include <vnx/SpyTool.h>
#include <vnx/ProcessClient.hxx>


int main(int argc, char** argv) {
	
	std::map<std::string, std::string> options;
	options["x"] = "compact";
	options["i"] = "info";
	options["1"] = "one";
	options["t"] = "topic";
	options["s"] = "source";
	options["u"] = "user";
	options["source"] = "url";
	options["topic"] = "expr";
	options["max-list-size"] = "size, 0=unlimited";
	options["user"] = "login as user";
	
	vnx::init("vnxdump", argc, argv, options);
	
	std::string source = "localhost";
	std::string user;
	vnx::read_config("source", source);
	vnx::read_config("user", user);
	
	const vnx::Hash64 tunnel = vnx::Hash64::rand();
	
	std::string password;
	if(!user.empty()){
		password = vnx::input_password("Password for user " + user + ": ");
	}

	vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("Proxy", vnx::Endpoint::from_url(source));
	proxy->receive_tunnel = tunnel;
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

	bool compact = false;
	vnx::read_config("compact", compact);
	
	vnx::Handle<vnx::SpyTool> tool = new vnx::SpyTool("SpyTool");
	tool->proxy_name = "Proxy";
	tool->receive_tunnel = tunnel;
	tool->pretty_print = !compact;
	vnx::read_config("info", tool->header_only);
	vnx::read_config("one", tool->only_once);
	vnx::read_config("topic", tool->topic_expr);
	vnx::read_config("max-list-size", tool->max_list_size);
	tool.start();
	
	tool.wait();
	process.resume_log_async();

	vnx::close();
	return 0;
}

