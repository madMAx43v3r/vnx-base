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
#include <vnx/Server.h>
#include <vnx/Proxy.h>
#include <vnx/TcpEndpoint.hxx>
#include <vnx/UnixEndpoint.hxx>


namespace vnx {

Router::Router(const std::string& _vnx_name)
	:	RouterBase(_vnx_name)
{
}

void Router::main() {
	Handle<Server> tcp_server = new Server("TcpServer", TcpEndpoint::create_ex("0.0.0.0", port));
	tcp_server->export_list = export_list;
	tcp_server.start();
	
	Handle<Server> unix_server = new Server("UnixServer", UnixEndpoint::create_ex(unix_path));
	unix_server->export_list = export_list;
	unix_server.start();
	
	std::list<Handle<Proxy>> clients;
	for(const auto& entry : import_map) {
		std::shared_ptr<const Endpoint> endpoint = Endpoint::from_url(entry.first);
		Handle<Proxy> proxy = new Proxy(endpoint->to_url(), endpoint);
		proxy->import_list = entry.second;
		proxy.start();
		clients.push_back(proxy);
	}
	for(const auto& entry : export_map) {
		std::shared_ptr<const Endpoint> endpoint = Endpoint::from_url(entry.first);
		Handle<Proxy> proxy = new Proxy(endpoint->to_url(), endpoint);
		proxy->export_list = entry.second;
		proxy.start();
		clients.push_back(proxy);
	}
	for(const auto& entry : forward_map) {
		std::shared_ptr<const Endpoint> endpoint = Endpoint::from_url(entry.first);
		Handle<Proxy> proxy = new Proxy(endpoint->to_url(), endpoint);
		proxy->forward_list = entry.second;
		proxy.start();
		clients.push_back(proxy);
	}
	
	if(clients.empty()) {
		log(WARN) << "No clients configured!";
	}
	
	Super::main();
}


} // vnx
