/*
 * AsyncClient.cpp
 *
 *  Created on: Mar 7, 2020
 *      Author: mad
 */

#include <vnx/vnx.h>
#include <vnx/AsyncClient.h>
#include <vnx/OverflowException.hxx>
#include <vnx/GatewayInterfaceClient.hxx>
#include <vnx/GatewayInterface_forward.hxx>
#include <vnx/GatewayInterface_forward_return.hxx>


namespace vnx {

AsyncClient::AsyncClient(Hash64 service_addr) {
	vnx_tunnel_addr = service_addr;
	vnx_service_addr = service_addr;
	vnx_src_mac = Hash64::rand();
}
	
AsyncClient::~AsyncClient() {
	// all automatic
}

void AsyncClient::vnx_set_node(Node* node) {
	std::lock_guard<std::mutex> lock(vnx_mutex);
	if(vnx_return_pipe) {
		vnx_return_pipe->close();
		vnx_return_pipe = nullptr;
	}
	vnx_node = node;
	if(node) {
		vnx_return_pipe = open_pipe(vnx_src_mac, node, Pipe::UNLIMITED);	// never block on returns
	}
}

void AsyncClient::vnx_set_session(Hash64 id) {
	std::lock_guard<std::mutex> lock(vnx_mutex);
	vnx_session_id = id;
}

void AsyncClient::vnx_set_tunnel(Hash64 tunnel_addr) {
	std::lock_guard<std::mutex> lock(vnx_mutex);
	vnx_tunnel_addr = tunnel_addr;
	vnx_service_pipe = nullptr;
}

void AsyncClient::vnx_set_tunnel(const std::string& tunnel_name) {
	vnx_set_tunnel(Hash64(tunnel_name));
}

void AsyncClient::vnx_set_gateway(Hash64 gateway_addr) {
	std::lock_guard<std::mutex> lock(vnx_mutex);
	vnx_tunnel_addr = gateway_addr;
	vnx_gateway_addr = gateway_addr;
	vnx_service_pipe = nullptr;
}

void AsyncClient::vnx_set_gateway(const std::string& gateway_name) {
	vnx_set_gateway(Hash64(gateway_name));
}

void AsyncClient::vnx_set_non_blocking(bool non_blocking_mode) {
	std::lock_guard<std::mutex> lock(vnx_mutex);
	vnx_is_non_blocking = non_blocking_mode;
}

void AsyncClient::vnx_set_error_callback(const std::function<void(uint64_t, const vnx::exception&)>& callback) {
	std::lock_guard<std::mutex> lock(vnx_mutex);
	vnx_error_callback = callback;
}

std::vector<uint64_t> AsyncClient::vnx_get_pending_ids() const {
	std::lock_guard<std::mutex> lock(vnx_mutex);
	std::vector<uint64_t> result;
	for(const auto& entry : vnx_pending) {
		result.push_back(entry.first);
	}
	return result;
}

void AsyncClient::vnx_callback(std::shared_ptr<const Return> result) {
	if(result->src_mac != vnx_src_mac) {
		throw std::logic_error("src_mac mismatch");
	}
	if(auto except = std::dynamic_pointer_cast<const Exception>(result->value)) {
		vnx_callback_error(result->request_id, vnx::exception(except));
	} else {
		try {
			if(!result->value) {
				throw std::logic_error("missing return value");
			}
			vnx_callback_switch(result->request_id, result->value);
		}
		catch(const std::exception& ex) {
			vnx_purge_request(result->request_id, vnx::exception(ex.what()));
			throw;
		}
	}
}

void AsyncClient::vnx_shutdown(const std::string& message) {
	{
		std::lock_guard<std::mutex> lock(vnx_mutex);
		vnx_service_pipe = nullptr;
	}
	auto ex = NoSuchService::create();
	ex->dst_mac = vnx_service_addr;
	ex->what = message;
	
	const auto list = vnx_get_pending_ids();
	for(auto request_id : list) {
		try {
			vnx_callback_error(request_id, vnx::exception(ex));
		} catch(...) {
			// ignore user callback exceptions here
		}
	}
}

void AsyncClient::vnx_callback_error(uint64_t request_id, const vnx::exception& ex) {
	vnx_purge_request(request_id, ex);

	std::function<void(uint64_t, const vnx::exception&)> callback;
	{
		std::lock_guard<std::mutex> lock(vnx_mutex);
		callback = vnx_error_callback;
	}
	if(callback) {
		callback(request_id, ex);
	}
}

void AsyncClient::vnx_request(std::shared_ptr<const Value> method, const uint64_t request_id) {
	if(!vnx_node) {
		throw std::logic_error("return pipe not connected (forgot Module::add_async_client()?)");
	}
	// below here no exceptions !!! (async requests don't throw exceptions, except the one above)

	std::shared_ptr<Request> request = Request::create();
	std::shared_ptr<Pipe> service_pipe;
	{
		std::lock_guard<std::mutex> lock(vnx_mutex);
		if(!vnx_service_pipe || vnx_service_pipe->is_closed()) {
			vnx_service_pipe = get_pipe(vnx_tunnel_addr);
			connect(vnx_service_pipe, vnx_return_pipe);
		}
		service_pipe = vnx_service_pipe;
		request->session = vnx_session_id;
	}
	if(!service_pipe) {
		auto ex = NoSuchService::create();
		ex->dst_mac = vnx_service_addr;
		ex->what = "get_pipe() failed";
		auto ret = Return::create();
		ret->src_mac = vnx_src_mac;
		ret->request_id = request_id;
		ret->value = ex;
		send_msg(vnx_return_pipe, ret, Message::BLOCKING);
		return;
	}
	if(vnx_is_non_blocking) {
		request->flags |= Message::NON_BLOCKING;
	}
	request->flags |= Message::BLOCKING;
	request->request_id = request_id;
	request->src_mac = vnx_src_mac;
	request->dst_mac = vnx_service_addr;
	request->method = method;
	
	if(vnx_gateway_addr) {
		auto forward = GatewayInterface_forward::create();
		forward->request = vnx::clone(request);
		request->flags |= Message::FORWARD;
		request->dst_mac = vnx_gateway_addr;
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
			auto ex = NoSuchService::create();
			ex->dst_mac = vnx_service_addr;
			ex->what = "send_msg() failed with: " + get_error_string(code);
			auto ret = Return::create();
			ret->src_mac = vnx_src_mac;
			ret->request_id = request_id;
			ret->value = ex;
			send_msg(vnx_return_pipe, ret, Message::BLOCKING);
		}
	} catch(const std::overflow_error& ex_) {
		auto ex = OverflowException::create();
		ex->dst_mac = vnx_service_addr;
		ex->what = ex_.what();
		auto ret = Return::create();
		ret->src_mac = vnx_src_mac;
		ret->request_id = request_id;
		ret->value = ex;
		send_msg(vnx_return_pipe, ret, Message::BLOCKING);
	}
}


} // vnx
