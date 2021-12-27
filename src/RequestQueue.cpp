/*
 * RequestQueue.cpp
 *
 *  Created on: Jul 13, 2021
 *      Author: jaw
 */

#include <vnx/RequestQueue.h>
#include <vnx/NoSuchService.hxx>


namespace vnx {

RequestQueue::RequestQueue(const std::string& _vnx_name)
	:	RequestQueueBase(_vnx_name)
{
}

void RequestQueue::init() {
	vnx::open_pipe(vnx_name, this, max_queue_ms, max_queue_size);
}

void RequestQueue::forward(std::shared_ptr<const Request> request) {
	auto& fake_src = src_map[request->src_mac];
	if(!fake_src){
		fake_src = Hash64::rand();
		vnx::open_pipe(fake_src, this, Pipe::UNLIMITED);	// never block on returns
	}
	pending[std::make_pair(fake_src, request->request_id)] = request;

	auto copy = vnx::clone(request);
	copy->flags &= ~Message::NO_RETURN;
	copy->src_mac = fake_src;
	copy->session = vnx_request->session;

	queue.push(copy);
	send_next();
}

void RequestQueue::purge() {
	while(!queue.empty()) {
		auto request = queue.front();
		auto iter = pending.find(std::make_pair(request->src_mac, request->request_id));
		if(iter != pending.end()) {
			auto request = iter->second;
			auto ex = Exception::create();
			ex->what = "request queue purged";
			auto ret = Return::create();
			ret->flags = Message::BLOCKING;
			ret->src_mac = request->src_mac;
			ret->request_id = request->request_id;
			ret->value = ex;
			vnx::send_msg(request->src_mac, ret);
			pending.erase(iter);
		}
		queue.pop();
	}
}

void RequestQueue::clear() {
	purge();
	active = nullptr;
}

bool RequestQueue::handle(std::shared_ptr<const Return> result) {
	auto iter = pending.find(std::make_pair(result->src_mac, result->request_id));
	if(iter != pending.end()) {
		auto request = iter->second;
		if(!(request->flags & Message::NO_RETURN)) {
			auto copy = vnx::clone(result);
			copy->src_mac = request->src_mac;
			if(auto code = vnx::send_msg(request->src_mac, copy)) {
				log(WARN) << "Return send_msg() failed with: " << vnx::get_error_string(code);
			}
		}
		if(active && result->src_mac == active->src_mac && result->request_id == active->request_id) {
			active = nullptr;
			send_next();
		}
		pending.erase(iter);
		return true;
	}
	return Super::handle(result);			// not a forwarded request
}

void RequestQueue::send_next() {
	while(!active && !queue.empty()) {
		const auto request = queue.front();
		queue.pop();
		if(auto code = vnx::send_msg(request->dst_mac, request)) {
			auto ex = NoSuchService::create();
			ex->dst_mac = request->dst_mac;
			ex->what = "send_msg() failed with: " + vnx::get_error_string(code);
			auto ret = Return::create();
			ret->flags = Message::BLOCKING;
			ret->src_mac = request->src_mac;
			ret->request_id = request->request_id;
			ret->value = ex;
			vnx::send_msg(request->src_mac, ret);
		} else {
			active = request;
		}
	}
}


} // vnx
