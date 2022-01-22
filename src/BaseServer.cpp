/*
 * BaseServer.cpp
 *
 *  Created on: Dec 4, 2020
 *      Author: jaw
 */

#include <vnx/vnx.h>
#include <vnx/BaseServer.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif


namespace vnx {


void BaseServer::main() {
	if(!endpoint || address.size()) {
		if(auto tmp = Endpoint::from_url(address)) {
			endpoint = tmp;
		} else {
			throw std::logic_error("invalid address: " + address);
		}
	}
	
	setup_timer = set_timer_millis(3000, std::bind(&BaseServer::setup, this));
	setup();
	
	Super::main();
	
	if(server >= 0) {
#ifdef _WIN32
		::shutdown(server, SD_BOTH);
		::closesocket(server);
#else
		::shutdown(server, SHUT_RDWR);
#endif
	}
	if(thread.joinable()) {
		thread.join();
	}
}

void BaseServer::setup() {
	try {
		if(server < 0) {
			server = endpoint->open();
		}
		endpoint->bind(server);
		endpoint->listen(server);
	} catch(std::exception& ex) {
		log(WARN) << ex.what();
		return;
	}
	log(INFO) << "Running on " << endpoint->to_url();
	thread = std::thread(&BaseServer::accept_loop, this);
	setup_timer->stop();
}

void BaseServer::accept_loop() {
	try {
		while(true) {
			const int socket = endpoint->accept(server);
			spawn_proxy(socket);
		}
	} catch(...) {
#ifdef _WIN32
		::closesocket(server);
#else
		::close(server);
#endif
		server = -1;
	}
	exit();
}


} // vnx
