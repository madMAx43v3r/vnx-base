/*
 * Gateway.cpp
 *
 *  Created on: Jul 12, 2021
 *      Author: mad
 */

#include <vnx/Gateway.h>


namespace vnx {

Gateway::Gateway(const std::string& _vnx_name)
	:	GatewayBase(_vnx_name)
{
}

void Gateway::init() {
	vnx::open_pipe(vnx_name, this, max_queue_ms, max_queue_size);
}

void Gateway::forward(std::shared_ptr<const Request> request) {
	if(!request) {
		return;
	}
	if(vnx_request) {
		if(auto session = vnx_request->session) {
			auto copy = vnx::clone(request);
			copy->session = session;
			request = copy;
		}
	}
	if(auto code = vnx::send_msg(request->dst_mac, request)) {
		throw std::runtime_error("forward() failed with: " + get_error_string(code));
	}
}


} // vnx
