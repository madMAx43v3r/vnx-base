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
	}
}

ThreadPool::~ThreadPool() {
	close();
}

int64_t ThreadPool::add_task(const std::function<void()>& func) {
	int64_t job = -1;
	if(num_threads > 0) {
		{
			std::unique_lock<std::mutex> lock(mutex);
			while(do_run && max_queue_size > 0 && queue.size() >= size_t(max_queue_size)) {
				reverse_condition.wait(lock);
			}
			if(do_run) {
				job = next_job_id++;
				queue.emplace(func, job);
				pending_jobs.insert(job);
			}
		}
		condition.notify_one();
	} else if(num_threads < 0) {
		std::lock_guard<std::mutex> lock(mutex);
		job = next_job_id++;
		queue.emplace(func, job);
		pending_jobs.insert(job);
		threads[job] = std::thread(&ThreadPool::main, this, job);
	} else if(func) {
		try {
			func();
		} catch(const std::exception& ex) {
			vnx::log_warn() << "ThreadPool: " << ex.what();
		}
	}
	return job;
}

size_t ThreadPool::get_num_pending() const {
	std::lock_guard<std::mutex> lock(mutex);
	return queue.size();
}

size_t ThreadPool::get_num_running() const {
	return num_running;
}

size_t ThreadPool::get_num_pending_total() const {
	std::lock_guard<std::mutex> lock(mutex);
	return num_running + queue.size();
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

void ThreadPool::sync() {
	std::unique_lock<std::mutex> lock(mutex);
	while(num_running || queue.size()) {
		reverse_condition.wait(lock);
	}
}

void ThreadPool::sync(const int64_t barrier) {
	std::unique_lock<std::mutex> lock(mutex);
	while(true) {
		if(!pending_jobs.empty() && barrier >= *pending_jobs.begin()) {
			reverse_condition.wait(lock);
		} else {
			break;
		}
	}
}

void ThreadPool::sync(const std::vector<int64_t>& jobs) {
	std::unique_lock<std::mutex> lock(mutex);
	bool do_wait = true;
	while(do_wait) {
		do_wait = false;
		for(const auto& job : jobs) {
			if(pending_jobs.count(job)) {
				do_wait = true;
				reverse_condition.wait(lock);
				break;
			}
		}
	}
}

void ThreadPool::close() {
	exit();
	wait();
}

void ThreadPool::detach() {
	exit();
	for(auto& entry : threads) {
		entry.second.detach();
	}
	threads.clear();
}

void ThreadPool::main(const int64_t thread_id)
{
	std::pair<std::function<void()>, int64_t> entry;
	entry.second = -1;

	while(do_run) {
		{
			std::unique_lock<std::mutex> lock(mutex);

			pending_jobs.erase(entry.second);

			while(do_run && queue.empty()) {
				reverse_condition.notify_all();		// notify about previous task done
				condition.wait(lock);
			}
			if(do_run) {
				entry = std::move(queue.front());
				queue.pop();
			} else {
				break;
			}
			num_running++;
		}
		reverse_condition.notify_all();
		
		try {
			if(entry.first) {
				entry.first();
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

		pending_jobs.erase(entry.second);

		const auto iter = threads.find(thread_id);
		if(iter != threads.end()) {
			iter->second.detach();
			threads.erase(iter);
		}
	}
	reverse_condition.notify_all();
}


} // vnx
