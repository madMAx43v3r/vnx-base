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

#include <vnx/ThreadPool.h>
#include <vnx/Process.h>


namespace vnx {

ThreadPool::ThreadPool(int num_threads_, int max_queue_size_)
	:	num_threads(num_threads_),
		max_queue_size(max_queue_size_)
{
	if(num_threads > 0) {
		std::lock_guard<std::mutex> lock(mutex);
		for(int i = 0; i < num_threads; ++i) {
			threads[i] = std::thread(&ThreadPool::main, this, i);
		}
		next_thread_id = num_threads;
	}
}

ThreadPool::~ThreadPool()
{
	close();
}

void ThreadPool::add_task(const std::function<void()>& func) {
	if(num_threads > 0) {
		{
			std::unique_lock<std::mutex> lock(mutex);
			while(max_queue_size > 0 && queue.size() >= size_t(max_queue_size)) {
				reverse_condition.wait(lock);
			}
			queue.push(func);
		}
		condition.notify_one();
	} else if(num_threads < 0) {
		std::lock_guard<std::mutex> lock(mutex);
		queue.push(func);
		const auto thread_id = next_thread_id++;
		threads[thread_id] = std::thread(&ThreadPool::main, this, thread_id);
	} else if(func) {
		try {
			func();
		} catch(const std::exception& ex) {
			vnx::log_warn() << "ThreadPool: " << ex.what();
		}
	}
}

size_t ThreadPool::get_num_pending() const {
	std::lock_guard<std::mutex> lock(mutex);
	return queue.size();
}

size_t ThreadPool::get_num_running() const {
	return num_running;
}

void ThreadPool::exit() {
	std::lock_guard<std::mutex> lock(mutex);
	do_run = false;
	condition.notify_all();
}

void ThreadPool::wait() {
	std::unique_lock<std::mutex> lock(mutex);
	while(!threads.empty()) {
		reverse_condition.wait(lock);
	}
}

void ThreadPool::close() {
	exit();
	wait();
}

void ThreadPool::main(const int64_t thread_id) {
	while(do_run) {
		std::function<void()> func;
		{
			std::unique_lock<std::mutex> lock(mutex);
			while(do_run && queue.empty()) {
				condition.wait(lock);
			}
			if(do_run) {
				func = std::move(queue.front());
				queue.pop();
			} else {
				break;
			}
		}
		reverse_condition.notify_one();
		
		num_running++;
		try {
			if(func) {
				func();
			}
		} catch(const std::exception& ex) {
			vnx::log_warn() << "ThreadPool: " << ex.what();
		}
		num_running--;
		
		if(num_threads <= 0) {
			break;
		}
	}
	{
		std::lock_guard<std::mutex> lock(mutex);
		const auto iter = threads.find(thread_id);
		if(iter != threads.end()) {
			iter->second.detach();
			threads.erase(iter);
			reverse_condition.notify_all();
		}
	}
}


} // vnx
