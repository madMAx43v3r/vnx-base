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
#include <vnx/Terminal.h>
#include <vnx/Proxy.h>
#include <vnx/Server.h>
#include <vnx/TcpEndpoint.hxx>


int main(int argc, char** argv) {
	
	std::map<std::string, std::string> options;
	options["p"] = "port";
	options["port"] = "port";
	options["s"] = "source";
	options["source"] = "url";
	
	vnx::init("vnxtimeserver", argc, argv, options);
	
	int port = 5555;
	vnx::read_config("port", port);
	
	vnx::Handle<vnx::Terminal> terminal = new vnx::Terminal("Terminal");
	terminal.start();
	
	std::string source;
	if(vnx::read_config("source", source)) {
		vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("Proxy", vnx::Endpoint::from_url(source));
		proxy->time_sync = true;
		proxy.start_detached();
	}
	
	{
		vnx::Handle<vnx::Server> server = new vnx::Server("TcpServer", vnx::TcpEndpoint::create_ex("0.0.0.0", port));
		server.start_detached();
	}
	
	vnx::wait();
	
}
