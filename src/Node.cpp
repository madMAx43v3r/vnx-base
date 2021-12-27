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
#include <vnx/Node.h>


namespace vnx {

Node::~Node() {
	close();
}

std::shared_ptr<const Message> Node::read() {
	while(vnx_do_run) {
		{
			std::lock_guard<std::mutex> lock(vnx_mutex);
			vnx_pipe = pop_next();
		}
		if(vnx_pipe) {
			if(auto msg = pop_msg(vnx_pipe)) {
				return msg;
			}
		} else {
			break;
		}
	}
	return nullptr;
}

std::shared_ptr<const Message> Node::read_blocking() {
	while(vnx_do_run) {
		{
			std::unique_lock<std::mutex> lock(vnx_mutex);
			while(vnx_do_run && notify_map.empty()) {
				condition.wait(lock);
			}
			if(vnx_do_run) {
				vnx_pipe = pop_next();
			} else {
				vnx_pipe = nullptr;
			}
		}
		if(vnx_pipe) {
			if(auto msg = pop_msg(vnx_pipe)) {
				return msg;
			}
		} else {
			break;
		}
	}
	return nullptr;
}

std::shared_ptr<const Message> Node::read_blocking(int64_t timeout_us) {
	while(vnx_do_run) {
		{
			std::unique_lock<std::mutex> lock(vnx_mutex);
			if(vnx_do_run && notify_map.empty()) {
				if(timeout_us > 0) {
					condition.wait_for(lock, std::chrono::microseconds(timeout_us));
				} else if(timeout_us < 0) {
					condition.wait(lock);
				}
			}
			if(vnx_do_run) {
				vnx_pipe = pop_next();
			} else {
				vnx_pipe = nullptr;
			}
		}
		if(vnx_pipe) {
			if(auto msg = pop_msg(vnx_pipe)) {
				return msg;
			}
		} else {
			break;
		}
		timeout_us = 0;		// only wait once
	}
	return nullptr;
}

void Node::notify(std::shared_ptr<Pipe> pipe) {
	if(pipe) {
		std::lock_guard<std::mutex> lock(vnx_mutex);
		notify_map[pipe->priority].push(pipe);
	}
	condition.notify_one();
}


bool Node::add_pipe(std::shared_ptr<Pipe> pipe) {
	std::lock_guard<std::mutex> lock(vnx_mutex);
	if(vnx_do_run) {
		pipe_list.push_back(pipe);
		return true;
	}
	return false;
}

void Node::remove_pipe(Pipe* pipe) {
	std::lock_guard<std::mutex> lock(vnx_mutex);
	for(auto iter = pipe_list.begin(); iter != pipe_list.end(); ++iter) {
		if(iter->get() == pipe) {
			pipe_list.erase(iter);
			break;
		}
	}
}

std::shared_ptr<Pipe> Node::pop_next() {
	// mutex needs to be locked by caller
	std::shared_ptr<Pipe> pipe;
	if(!notify_map.empty()) {
		const auto iter = notify_map.begin();
		auto& queue = iter->second;
		pipe = queue.front();
		queue.pop();
		if(queue.empty()) {
			notify_map.erase(iter);
		}
	}
	return pipe;
}

std::shared_ptr<const Message> Node::pop_msg(std::shared_ptr<Pipe> pipe) {
	if(auto msg = pipe->pop()) {
		if(msg->flags & Message::SHUTDOWN) {
			std::lock_guard<std::mutex> lock(vnx_mutex);
			vnx_do_run = false;
		} else {
			return msg;
		}
	}
	return nullptr;
}

void Node::exit() {
	// copy the list of pipes
	std::list<std::shared_ptr<Pipe>> pipe_list_copy;
	{
		std::lock_guard<std::mutex> lock(vnx_mutex);
		vnx_do_run = false;
		pipe_list_copy = pipe_list;
	}
	// shutdown all pipes
	for(auto pipe : pipe_list_copy) {
		pipe->shutdown();
	}
	// additional sync lock
	std::lock_guard<std::mutex> lock(vnx_mutex);
	condition.notify_all();
}

void Node::close() {
	// make sure no more pipes can connect
	exit();
	// copy the list of pipes
	std::list<std::shared_ptr<Pipe>> pipe_list_copy;
	{
		std::lock_guard<std::mutex> lock(vnx_mutex);
		pipe_list_copy = pipe_list;
	}
	// close all pipes (without holding the mutex!)
	for(auto pipe : pipe_list_copy) {
		pipe->close();
	}
	// clear pipe list (since close() can be called multiple times)
	{
		std::lock_guard<std::mutex> lock(vnx_mutex);
		pipe_list.clear();
		vnx_pipe = nullptr;
	}
}


} // vnx
