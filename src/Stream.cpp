/*
 * Stream.cpp
 *
 *  Created on: Feb 2, 2021
 *      Author: mad
 */

#include <vnx/Stream.h>
#include <vnx/FlowMessage.hxx>


namespace vnx {

Stream::Stream(const Hash64& dst_mac_)
	:	src_mac(Hash64::rand()),
		dst_mac(dst_mac_)
{
}

Stream::Stream(const std::string& service)
	:	Stream(Hash64(service))
{
}

Stream::~Stream() {
	close();
}

std::shared_ptr<Stream> Stream::create(const Hash64& dst_mac_) {
	return std::make_shared<vnx::Stream>(dst_mac_);
}

std::shared_ptr<Stream> Stream::create(const std::string& service) {
	return std::make_shared<vnx::Stream>(service);
}

void Stream::open() {
	if(auto pipe = get_pipe(dst_mac)) {
		send_pipe = pipe;
	} else {
		throw std::runtime_error("Stream::open(): get_pipe() failed");
	}
}

void Stream::connect(Node* node, int max_queue_ms, int max_queue_size, int priority) {
	open();
	if(recv_pipe) {
		recv_pipe->close();
	}
	recv_pipe = open_pipe(src_mac, node, max_queue_ms, max_queue_size, priority);
	if(open_flow(send_pipe, recv_pipe, Message::BLOCKING)) {
		throw std::runtime_error("Stream::connect(): open_flow() failed");
	}
}

pipe_code_e Stream::send(const Value& value, uint16_t flags) {
	return send(vnx::clone(value), flags);
}

pipe_code_e Stream::send(std::shared_ptr<const Value> value, uint16_t flags) {
	auto frame = Frame::create();
	frame->flags = flags | Message::BLOCKING;
	frame->seq_num = next_seq++;
	frame->src_mac = src_mac;
	frame->dst_mac = dst_mac;
	frame->value = value;
	return send_msg(send_pipe, frame);
}

void Stream::close() {
	send_pipe = nullptr;
	if(recv_pipe) {
		recv_pipe->close();
	}
}


} // vnx
