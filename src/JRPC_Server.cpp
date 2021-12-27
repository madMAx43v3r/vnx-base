/*
 * JRPC_Server.cpp
 *
 *  Created on: Dec 4, 2020
 *      Author: jaw
 */

#include <vnx/JRPC_Server.h>
#include <vnx/JRPC_Proxy.h>


namespace vnx {


JRPC_Server::JRPC_Server(const std::string& name_, std::shared_ptr<const Endpoint> endpoint_):
	JRPC_ServerBase(name_)
{
	endpoint = endpoint_;
}


void JRPC_Server::spawn_proxy(int socket){
	Handle<JRPC_Proxy> proxy = new JRPC_Proxy(vnx_name + ".Proxy." + std::to_string(socket), 0, socket);
	proxy->export_list = export_list;
	proxy->max_queue_ms = max_queue_ms;
	proxy->allow_login = allow_login;
	proxy->use_authentication = use_authentication;
	proxy->default_access = default_access;
	proxy.start_detached();
}


} // vnx
