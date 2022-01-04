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
#include <vnx/Thread.h>


namespace vnx {

Thread::Thread(const std::string& vnx_name_)
	:	vnx_name(vnx_name_)
{
	publisher = std::make_shared<Publisher>();
}

Thread::~Thread() {
	stop();
}

void Thread::start() {
	if(!thread.joinable()) {
		init();
		thread = std::thread(&Thread::entry, this);
	}
}

void Thread::join() {
	if(thread.joinable()) {
		thread.join();
	}
}

void Thread::stop() {
	exit();
	notify(nullptr);
	join();
}

void Thread::entry() {
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
	try {
		main();
	} catch(const std::exception& ex) {
		log(ERROR) << ex.what();
	} catch(...) {
		log(ERROR) << "unknown exception!";
	}
	close();
}


} // vnx
