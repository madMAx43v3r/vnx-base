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
#include <vnx/Pipe.h>
#include <vnx/Request.hxx>
#include <vnx/Return.hxx>
#include <vnx/FlowMessage.hxx>

#include <unordered_map>


namespace vnx {

const int Pipe::UNLIMITED;

Pipe::Pipe()
	:	Pipe(Hash64::rand())
{
}

Pipe::Pipe(Hash64 mac_addr)
	:	mac_addr(mac_addr)
{
}

Pipe::~Pipe() {
	// nothing
}

std::shared_ptr<Pipe> Pipe::create() {
	return std::make_shared<Pipe>();
}

void connect(std::shared_ptr<Pipe> pipe, Node* node, int max_queue_ms, int max_queue_size, int priority, pipe_mode_e mode) {
	if(pipe) {
		pipe->connect(pipe, node, max_queue_ms, max_queue_size, priority, mode);
	}
}

pipe_code_e connect(std::shared_ptr<Pipe> pipe, std::shared_ptr<Pipe> peer) {
	if(!pipe || !peer) {
		return PIPE_INVALID;
	}
	return pipe->connect(peer);
}

pipe_code_e disconnect(std::shared_ptr<Pipe> pipe, Hash64 peer_mac) {
	if(pipe) {
		return pipe->disconnect(peer_mac);
	}
	return PIPE_INVALID;
}

pipe_code_e send_msg(std::shared_ptr<Pipe> pipe, std::shared_ptr<const Message> msg) {
	if(pipe) {
		return pipe->push(pipe, msg);
	}
	return PIPE_INVALID;
}

pipe_code_e send_msg(std::shared_ptr<Pipe> pipe, std::shared_ptr<Message> msg, uint16_t flags) {
	msg->flags |= flags;
	return send_msg(pipe, msg);
}

pipe_code_e open_flow(std::shared_ptr<Pipe> target, std::shared_ptr<Pipe> source, uint16_t flags) {
	if(!target || !source) {
		return PIPE_INVALID;
	}
	if(auto err = connect(source, target)) {
		return err;
	}
	auto msg = FlowMessage::create();
	msg->flags |= flags;
	msg->src_mac = source->get_mac_addr();
	msg->dst_mac = target->get_mac_addr();
	msg->flow_code = FlowMessage::OPEN;
	return send_msg(target, msg);
}

void Pipe::connect(std::shared_ptr<Pipe> self, Node* node_, int max_queue_ms_, int max_queue_size_, int priority_, pipe_mode_e mode_) {
	std::lock_guard<std::mutex> lock(mutex);
	if(node) {
		node->remove_pipe(this);
		node = nullptr;
	}
	if(node_ && node_->add_pipe(self)) {
		node = node_;
		mode = mode_;
		priority = priority_;
		max_queue_us = int64_t(max_queue_ms_) * 1000;
		max_queue_size = max_queue_size_;
	}
}

pipe_code_e Pipe::connect(std::shared_ptr<Pipe> peer) {
	const Hash64 peer_mac = peer->get_mac_addr();
	{
		std::lock_guard<std::mutex> lock(mutex);
		if(node && peer_mac) {
			peer_map[peer_mac] = peer;
			return PIPE_OK;
		}
	}
	return PIPE_INVALID;
}

pipe_code_e Pipe::disconnect(Hash64 peer_mac) {
	std::lock_guard<std::mutex> lock(mutex);
	return peer_map.erase(peer_mac) ? PIPE_OK : PIPE_INVALID;
}

pipe_code_e Pipe::push(std::shared_ptr<Pipe> self, std::shared_ptr<const Message> msg) {
	if(!msg) {
		return PIPE_OK;
	}
	std::unique_lock<std::mutex> lock(mutex);
	bool do_notify = true;
	while(!is_shutdown_) {
		const bool is_full = !queue.empty()
				&& ((max_queue_us > 0 && queue.back().second - queue.front().second >= max_queue_us)
					|| (max_queue_size > 0 && queue.size() >= size_t(max_queue_size)));
		if(!is_paused_ && is_full) {
			if(mode == PIPE_MODE_LATEST) {
				const auto& front = queue.front().first;
				if(!(front->flags & Message::BLOCKING)) {
					queue.pop();
					do_notify = false;
					break;
				}
			}
		}
		if(is_paused_ || is_full) {
			if(msg->flags & Message::BLOCKING) {
				if(msg->flags & Message::NON_BLOCKING) {
					throw std::overflow_error("pipe overflow");
				}
				condition.wait(lock);
			} else {
				return is_full ? PIPE_OVERFLOW : PIPE_PAUSED;
			}
		} else {
			break;
		}
	}
	if(is_shutdown_) {
		return PIPE_SHUTDOWN;
	}
	if(!node) {
		return PIPE_CLOSED;
	}
	queue.emplace(msg, vnx::get_wall_time_micros());
	if(do_notify) {
		node->notify(self);
	}
	return PIPE_OK;
}

std::shared_ptr<const Message> Pipe::pop() {
	std::shared_ptr<const Message> msg;
	{
		std::lock_guard<std::mutex> lock(mutex);
		if(!queue.empty()) {
			msg = queue.front().first;
			queue.pop();
		}
	}
	condition.notify_one();
	return msg;
}

void Pipe::pause() {
	std::lock_guard<std::mutex> lock(mutex);
	is_paused_ = true;
	condition.notify_all();
}

void Pipe::resume() {
	std::lock_guard<std::mutex> lock(mutex);
	is_paused_ = false;
	condition.notify_all();
}

void Pipe::shutdown() {
	std::lock_guard<std::mutex> lock(mutex);
	is_shutdown_ = true;
	condition.notify_all();
}

void Pipe::close() {
	std::vector<std::shared_ptr<const Message>> leftover;
	std::vector<std::pair<Hash64, std::shared_ptr<Pipe>>> peer_list;
	{
		std::lock_guard<std::mutex> lock(mutex);
		if(node) {
			node->remove_pipe(this);
			node = nullptr;
		}
		if(mac_addr) {
			remove_pipe(mac_addr);
		}
		while(!queue.empty()) {
			leftover.push_back(queue.front().first);
			queue.pop();
		}
		for(const auto& entry : peer_map) {
			peer_list.push_back(entry);
		}
		peer_map.clear();
		// notify that pipe is closed now
		condition.notify_all();
	}
	for(auto msg : leftover) {
		if(auto request = std::dynamic_pointer_cast<const Request>(msg)) {
			send_msg(request->src_mac, Return::create_no_such_service_return(request));
		}
	}
	for(const auto& entry : peer_list) {
		auto peer = entry.second;
		auto msg = FlowMessage::create();
		msg->src_mac = mac_addr;
		msg->dst_mac = entry.first;
		msg->flow_code = FlowMessage::CLOSE;
		send_msg(peer, msg);
		peer->disconnect(mac_addr);
	}
}

Node* Pipe::get_node() const {
	return node;
}

Hash64 Pipe::get_mac_addr() const {
	return mac_addr;
}

bool Pipe::is_paused() const {
	return is_paused_;
}

bool Pipe::is_closed() const {
	return !node;
}


class GlobalPipeMap {
public:
	void add_pipe(std::shared_ptr<Pipe> pipe) {
		if(pipe) {
			std::lock_guard<std::mutex> lock(mutex);
			pipe_map[pipe->get_mac_addr()] = pipe;
		}
	}
	
	std::shared_ptr<Pipe> get_pipe(Hash64 mac_addr) {
		std::lock_guard<std::mutex> lock(mutex);
		auto iter = pipe_map.find(mac_addr);
		if(iter != pipe_map.end()) {
			return iter->second;
		}
		return nullptr;
	}
	
	std::shared_ptr<Pipe> remove_pipe(Hash64 mac_addr) {
		std::lock_guard<std::mutex> lock(mutex);
		std::shared_ptr<Pipe> pipe;
		auto iter = pipe_map.find(mac_addr);
		if(iter != pipe_map.end()) {
			pipe = iter->second;
			pipe_map.erase(iter);
		}
		return pipe;
	}
	
	std::vector<std::shared_ptr<Pipe>> get_all_pipes() {
		std::lock_guard<std::mutex> lock(mutex);
		std::vector<std::shared_ptr<Pipe>> list;
		for(const auto& entry : pipe_map) {
			list.push_back(entry.second);
		}
		return list;
	}
	
private:
	std::mutex mutex;
	std::unordered_map<Hash64, std::shared_ptr<Pipe>> pipe_map;
	
};


static GlobalPipeMap& get_global_pipe_map() {
	static GlobalPipeMap global_pipe_map;
	return global_pipe_map;
}

std::shared_ptr<Pipe> open_pipe(const std::string& service_name, Node* node, int max_queue_ms, int max_queue_size, int priority, pipe_mode_e mode) {
	return open_pipe(Hash64(service_name), node, max_queue_ms, max_queue_size, priority, mode);
}

std::shared_ptr<Pipe> open_pipe(Hash64 mac_addr, Node* node, int max_queue_ms, int max_queue_size, int priority, pipe_mode_e mode) {
	auto pipe = Pipe::create(mac_addr);
	connect(pipe, node, max_queue_ms, max_queue_size, priority, mode);
	return pipe;
}

std::shared_ptr<Pipe> get_pipe(const std::string& service_name) {
	return get_pipe(Hash64(service_name));
}

std::shared_ptr<Pipe> get_pipe(Hash64 mac_addr) {
	return get_global_pipe_map().get_pipe(mac_addr);
}

pipe_code_e send_msg(const std::string& service_name, std::shared_ptr<const Message> msg) {
	return send_msg(Hash64(service_name), msg);
}

pipe_code_e send_msg(Hash64 mac_addr, std::shared_ptr<const Message> msg) {
	if(auto pipe = get_pipe(mac_addr)) {
		return send_msg(pipe, msg);
	}
	return PIPE_NOT_FOUND;
}

void remove_pipe(const std::string& service_name) {
	remove_pipe(Hash64(service_name));
}

void remove_pipe(Hash64 mac_addr) {
	get_global_pipe_map().remove_pipe(mac_addr);
}

void close_pipe(const std::string& service_name) {
	close_pipe(Hash64(service_name));
}

void close_pipe(Hash64 mac_addr) {
	auto pipe = get_global_pipe_map().remove_pipe(mac_addr);
	if(pipe) {
		pipe->close();
	}
}

void shutdown_pipes() {
	for(auto pipe : get_global_pipe_map().get_all_pipes()) {
		auto msg = Message::create();
		msg->flags |= Message::SHUTDOWN;
		send_msg(pipe, msg);
	}
}

std::string get_error_string(const pipe_code_e& code) {
	switch(code) {
		case PIPE_OK: return "success";
		case PIPE_PAUSED: return "pipe paused";
		case PIPE_CLOSED: return "pipe closed";
		case PIPE_SHUTDOWN: return "pipe shutdown";
		case PIPE_OVERFLOW: return "pipe overflow";
		case PIPE_INVALID: return "invalid pipe";
		case PIPE_NOT_FOUND: return "no such pipe";
	}
	return std::to_string(code);
}

std::shared_ptr<Pipe> Pipe::create(Hash64 mac_addr) {
	auto pipe = std::make_shared<Pipe>(mac_addr);
	get_global_pipe_map().add_pipe(pipe);
	return pipe;
}


} // vnx
