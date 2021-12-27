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
#include <vnx/Recorder.h>
#include <vnx/Terminal.h>


int main(int argc, char** argv) {
	
	std::map<std::string, std::string> options;
	options["f"] = "file";
	options["s"] = "source";
	options["t"] = "topics";
	options["u"] = "user";
	options["file"] = "dir/ | dir/file_%.dat";
	options["source"] = "url";
	options["topics"] = "topic1 topic2 ...";
	options["user"] = "login as user";
	
	vnx::init("vnxrecord", argc, argv, options);
	
	vnx::Handle<vnx::Terminal> terminal = new vnx::Terminal("Terminal");
	terminal.start();
	
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
	proxy->time_sync = true;
	proxy.start();
	
	if(!user.empty()){
		vnx::ProxyClient pcl("Proxy");
		pcl.login(user, password);
	}

	vnx::Handle<vnx::Recorder> recorder = new vnx::Recorder("Recorder");
	recorder->proxy_name = "Proxy";
	recorder->receive_tunnel = tunnel;
	vnx::read_config("file", recorder->filename);
	vnx::read_config("topics", recorder->topics);
	recorder.start();
	
	recorder.wait();
	
	vnx::close();
	
}
