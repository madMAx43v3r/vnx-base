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
#include <vnx/Router.h>
#include <vnx/Terminal.h>


int main(int argc, char** argv) {
	
	std::map<std::string, std::string> options;
	options["p"] = "port";
	options["u"] = "unix-path";
	options["port"] = "port";
	options["unix-path"] = "unix path";
	
	vnx::init("vnxrouter", argc, argv, options);
	
	vnx::Handle<vnx::Terminal> terminal = new vnx::Terminal("Terminal");
	terminal.start();
	
	vnx::Handle<vnx::Router> router = new vnx::Router("Router");
	vnx::read_config("port", router->port);
	vnx::read_config("unix-path", router->unix_path);
	router.start();
	
	vnx::wait();
	
}
