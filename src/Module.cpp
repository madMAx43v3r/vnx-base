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
#include <vnx/Module.h>
#include <vnx/Client.h>
#include <vnx/Util.h>
#include <vnx/Sample.hxx>
#include <vnx/Request.hxx>
#include <vnx/Return.hxx>
#include <vnx/Exception.hxx>
#include <vnx/InternalError.hxx>
#include <vnx/ModuleInfo.hxx>
#include <vnx/FlowMessage.hxx>

#include <algorithm>

#ifdef _GNU_SOURCE
#include <pthread.h>
#endif


namespace vnx {

class GlobalModuleMap {
public:
	void add_module(std::shared_ptr<Module> module) {
		if(module) {
			std::lock_guard<std::mutex> lock(mutex);
			module_map.emplace(module->vnx_get_name(), module);
		}
	}

	bool remove_module(std::shared_ptr<Module> module) {
		if(module) {
			std::lock_guard<std::mutex> lock(mutex);
			const auto range = module_map.equal_range(module->vnx_get_name());
			for(auto iter = range.first; iter != range.second; ++iter) {
				if(iter->second.lock() == module) {
					module_map.erase(iter);
					return true;
				}
			}
		}
		return false;
	}

	std::vector<std::shared_ptr<Module>> get_modules(const std::string& name) {
		std::lock_guard<std::mutex> lock(mutex);
		std::vector<std::shared_ptr<Module>> result;
		const auto range = module_map.equal_range(name);
		for(auto iter = range.first; iter != range.second; ++iter) {
			if(auto module = iter->second.lock()) {
				result.push_back(module);
			}
		}
		return result;
	}

	std::vector<std::shared_ptr<Module>> get_all_modules() {
		std::lock_guard<std::mutex> lock(mutex);
		std::vector<std::shared_ptr<Module>> result;
		for(auto iter = module_map.begin(); iter != module_map.end(); ++iter) {
			if(auto module = iter->second.lock()) {
				result.push_back(module);
			}
		}
		return result;
	}

private:
	std::mutex mutex;
	std::multimap<std::string, std::weak_ptr<Module>> module_map;

};

static GlobalModuleMap& get_global_module_map() {
	static GlobalModuleMap instance;
	return instance;
}

void add_module(std::shared_ptr<Module> module) {
	get_global_module_map().add_module(module);
}

void remove_module(std::shared_ptr<Module> module) {
	get_global_module_map().remove_module(module);
}

Handle<Module> get_module(const std::string& name) {
	const auto list = get_modules(name);
	if(!list.empty()) {
		return list.back();
	}
	return nullptr;
}

std::vector<Handle<Module>> get_modules(const std::string& name) {
	const auto list = get_global_module_map().get_modules(name);
	std::vector<Handle<Module>> result;
	for(const auto& module : list) {
		result.emplace_back(module);
	}
	return result;
}

std::vector<Handle<Module>> get_all_modules() {
	const auto list = get_global_module_map().get_all_modules();
	std::vector<Handle<Module>> result;
	for(const auto& module : list) {
		result.emplace_back(module);
	}
	return result;
}


Module::Module(const std::string& name_)
	:	ModuleBase(name_)
{
}

Module::~Module() {
	// nothing to do
}

bool Module::vnx_do_run() const {
	std::lock_guard<std::mutex> lock(vnx_mutex);
	return Node::vnx_do_run;
}

void Module::main()
{
	while(vnx_do_run()) {
		vnx_process(true);
	}
	vnx_cleanup();
}

void Module::exit() {
	Super::exit();
}

void Module::vnx_entry() {
	process::num_modules++;
	{
		std::string thread_name = vnx_name;
		// limit the name to 15 chars, otherwise pthread_setname_np() fails
		if(thread_name.size() > 15) {
			thread_name.resize(15);
		}
#ifdef _GNU_SOURCE
		pthread_setname_np(pthread_self(), thread_name.c_str());
#endif
	}
	while(vnx_do_run()) {
		try {
			log(DEBUG) << "Started with " << (*this);
			main();
		} catch(const std::exception& ex) {
			log(ERROR) << ex.what();
		} catch(...) {
			log(ERROR) << "Unknown exception!";
		}
		close();		// close the node (in case it has not yet been closed)
		log(DEBUG) << "Closed";
		{
			std::unique_lock<std::mutex> lock(vnx_mutex);
			if(do_restart) {
				do_restart = false;
				Node::vnx_do_run = true;
				lock.unlock();

				vnx_setup();
				init();
			}
		}
	}
	std::shared_ptr<Module> self_ptr_;
	{
		std::lock_guard<std::mutex> lock(vnx_mutex);
		self_ptr_ = self_ptr;
		self_ptr = nullptr;
	}
	remove_module(self_ptr_);	// unregister module
	{
		std::lock_guard<std::mutex> lock(vnx_mutex);
		if(!is_waiting) {
			thread.detach();	// detach thread since object will be deleted first
		}
	}
	self_ptr_ = nullptr;		// this may delete us (ie. this object)
	{
		std::lock_guard<std::mutex> lock(process::mutex);
		process::num_modules--;
		process::condition.notify_all();
	}
}

void Module::vnx_start(std::shared_ptr<Module> self_ptr_) {
	std::unique_lock<std::mutex> lock(vnx_mutex);
	if(!thread.joinable()) {
		self_ptr = self_ptr_;
		Node::vnx_do_run = true;
		lock.unlock();

		vnx_setup();
		init();
		add_module(self_ptr_);
		{
			std::lock_guard<std::mutex> lock(vnx_mutex);
			thread = std::thread(std::bind(&Module::vnx_entry, this));
		}
	}
}

void Module::vnx_stop() {
	add_task([this]() {
		if(vnx_shutdown()) {
			exit();		// shutdown now
		}
	});
}

void Module::vnx_restart() {
	{
		std::lock_guard<std::mutex> lock(vnx_mutex);
		do_restart = true;
	}
	vnx_stop();
}

bool Module::vnx_shutdown() {
	return true;	// by default shutdown immediately
}

bool Module::vnx_is_running() {
	std::lock_guard<std::mutex> lock(vnx_mutex);
	return thread.joinable();
}

void Module::vnx_wait() {
	std::unique_lock<std::mutex> lock(vnx_mutex);
	if(thread.joinable()) {
		is_waiting = true;
		lock.unlock();
		thread.join();
	}
}


} // vnx
