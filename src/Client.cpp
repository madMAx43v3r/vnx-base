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
#include <vnx/Client.h>
#include <vnx/Request.hxx>
#include <vnx/Return.hxx>
#include <vnx/FlowMessage.hxx>
#include <vnx/NoSuchService.hxx>
#include <vnx/OverflowException.hxx>
#include <vnx/GatewayInterface_forward.hxx>
#include <vnx/GatewayInterface_forward_return.hxx>


namespace vnx {

Client::Client(Hash64 service_addr) {
	vnx_src_mac = Hash64::rand();
	vnx_tunnel_addr = service_addr;
	vnx_service_addr = service_addr;
	vnx_return_pipe = open_pipe(vnx_src_mac, this, Pipe::UNLIMITED);	// never block on returns
}

Client::~Client() {
	Client::exit();
}

void Client::vnx_set_session(Hash64 id) {
	std::lock_guard<std::mutex> lock(vnx_mutex);
	vnx_session_id = id;
}

void Client::vnx_set_tunnel(Hash64 tunnel_addr) {
	std::lock_guard<std::mutex> lock(vnx_mutex);
	vnx_tunnel_addr = tunnel_addr;
	vnx_service_pipe = nullptr;
}

void Client::vnx_set_tunnel(const std::string& tunnel_name) {
	vnx_set_tunnel(Hash64(tunnel_name));
}

void Client::vnx_set_gateway(Hash64 gateway_addr) {
	std::lock_guard<std::mutex> lock(vnx_mutex);
	vnx_tunnel_addr = gateway_addr;
	vnx_gateway_addr = gateway_addr;
	vnx_service_pipe = nullptr;
}

void Client::vnx_set_gateway(const std::string& gateway_name) {
	vnx_set_gateway(Hash64(gateway_name));
}

void Client::vnx_set_non_blocking(bool non_blocking_mode) {
	std::lock_guard<std::mutex> lock(vnx_mutex);
	vnx_is_non_blocking = non_blocking_mode;
}

std::shared_ptr<const Value> Client::vnx_request(std::shared_ptr<const Value> method, const bool is_async) {
	std::shared_ptr<Request> request = Request::create();
	std::shared_ptr<Pipe> service_pipe;
	Hash64 gateway_addr;
	{
		std::lock_guard<std::mutex> lock(vnx_mutex);
		if(!vnx_service_pipe || vnx_service_pipe->is_closed()) {
			vnx_service_pipe = get_pipe(vnx_tunnel_addr);
			connect(vnx_service_pipe, vnx_return_pipe);
		}
		if(vnx_is_non_blocking) {
			request->flags |= Message::NON_BLOCKING;
		}
		request->session = vnx_session_id;
		service_pipe = vnx_service_pipe;
		gateway_addr = vnx_gateway_addr;
	}
	if(!service_pipe) {
		if(is_async) {
			return nullptr;
		} else {
			auto ex = NoSuchService::create();
			ex->dst_mac = vnx_service_addr;
			ex->what = "get_pipe() failed";
			throw vnx::exception(ex);
		}
	}
	if(is_async) {
		request->flags |= Message::NO_RETURN;
	}
	request->flags |= Message::BLOCKING;
	request->request_id = ++vnx_next_id;
	request->src_mac = vnx_src_mac;
	request->dst_mac = vnx_service_addr;
	request->method = method;
	
	if(gateway_addr) {
		auto forward = GatewayInterface_forward::create();
		forward->request = vnx::clone(request);
		request->flags |= Message::FORWARD;
		request->dst_mac = gateway_addr;
		request->method = forward;
	}
	try {
		if(auto code = send_msg(service_pipe, request)) {
			{
				std::lock_guard<std::mutex> lock(vnx_mutex);
				if(vnx_service_pipe == service_pipe) {
					vnx_service_pipe = nullptr;
				}
			}
			if(is_async) {
				return nullptr;
			}
			auto ex = NoSuchService::create();
			ex->dst_mac = vnx_service_addr;
			ex->what = "send_msg() failed with: " + get_error_string(code);
			throw vnx::exception(ex);
		}
	} catch(const std::overflow_error& ex_) {
		auto ex = OverflowException::create();
		ex->dst_mac = vnx_service_addr;
		ex->what = ex_.what();
		throw vnx::exception(ex);
	}
	if(is_async) {
		return nullptr;
	}
	
	std::shared_ptr<const Return> result;
	while(!result || result->request_id != request->request_id) {
		std::shared_ptr<const Message> msg = read_blocking();
		if(!msg) {
			auto ex = NoSuchService::create();
			ex->dst_mac = vnx_service_addr;
			ex->what = "read_blocking() failed";
			throw vnx::exception(ex);
		}
		result = std::dynamic_pointer_cast<const Return>(msg);
	}
	if(!result || !result->value) {
		throw std::logic_error("Client: missing return value");
	}
	if(auto except = std::dynamic_pointer_cast<const Exception>(result->value)) {
		throw vnx::exception(except);
	}
	return result->value;
}


} // vnx
