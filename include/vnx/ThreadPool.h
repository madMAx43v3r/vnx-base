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

#ifndef INCLUDE_VNX_THREADPOOL_H_
#define INCLUDE_VNX_THREADPOOL_H_

#include <set>
#include <map>
#include <atomic>
#include <queue>
#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>


namespace vnx {

class ThreadPool {
public:
	/*
	 * @param num_threads_ Number of threads. 0 = no threads, synchronous processing.
	 * 		-1 = new thread for each task.
	 * @param max_queue_size_ Maximum number of tasks waiting to be processed.
	 * 		add_task() will block if max_queue_size_ is reached. 0 = unlimited queue size.
	 */
	ThreadPool(int num_threads_, int max_queue_size_ = 0);
	
	~ThreadPool();

	/// Adds a new task to the queue. (thread-safe)
	int64_t add_task(const std::function<void()>& func);
	
	/// Returns number of pending tasks (ie. which are in the queue)
	size_t get_num_pending() const;
	
	/// Returns number of active non-idle threads (ie. which are doing some work)
	size_t get_num_running() const;
	
	/// Returns get_num_pending() + get_num_running()
	size_t get_num_pending_total() const;

	/// Trigger all threads to exit. (thread-safe)
	void exit();
	
	/// Wait for all threads to finish. Needs to be called before destructor. (thread-safe)
	void wait();
	
	/// Wait for all tasks to finish.
	void sync();

	/// Wait for all jobs up to and including barrier to finish.
	void sync(const int64_t barrier);

	/// Wait for a set of jobs to finish.
	void sync(const std::vector<int64_t>& jobs);

	/// exit() + wait() (thread-safe)
	void close();
	
protected:
	std::atomic_bool do_run {true};
	
private:
	void main(const int64_t thread_id);
	
private:
	mutable std::mutex mutex;
	mutable std::condition_variable condition;
	mutable std::condition_variable reverse_condition;
	
	std::set<int64_t> pending_jobs;
	std::map<int64_t, std::thread> threads;
	std::queue<std::pair<std::function<void()>, int64_t>> queue;

	int num_threads = 0;
	int max_queue_size = 0;
	int64_t next_job_id = 0;
	std::atomic<size_t> num_running {0};
	
};


} // vnx

#endif /* INCLUDE_VNX_THREADPOOL_H_ */
