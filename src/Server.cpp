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
#include <vnx/Server.h>
#include <vnx/Proxy.h>


namespace vnx {


Server::Server(const std::string& name_, std::shared_ptr<const Endpoint> endpoint_):
	ServerBase(name_)
{
	endpoint = endpoint_;
}


void Server::spawn_proxy(int socket){
	Handle<Proxy> proxy = new Proxy(vnx_name + ".Proxy." + std::to_string(socket), 0, socket);
	proxy->export_list = export_list;
	proxy->max_queue_ms = max_queue_ms;
	proxy->max_queue_size = max_queue_size;
	proxy->recv_buffer_size = recv_buffer_size;
	proxy->send_buffer_size = send_buffer_size;
	proxy->allow_login = allow_login;
	proxy->use_authentication = use_authentication;
	proxy->default_access = default_access;
	proxy.start_detached();
}


} // vnx
