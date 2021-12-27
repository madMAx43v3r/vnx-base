/*
 * JRPC_Client.cpp
 *
 *  Created on: Jan 20, 2021
 *      Author: jaw
 */


#include <vnx/JRPC_Client.h>
#include <vnx/JRPC_Request.hxx>
#include <vnx/JRPC_Notification.hxx>
#include <vnx/JRPC_Response.hxx>
#include <vnx/JRPC_Success.hxx>
#include <vnx/JRPC_Failure.hxx>

#include <unistd.h>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif


namespace vnx{


JRPC_Client::JRPC_Client(std::shared_ptr<const Endpoint> endpoint_, int socket_):
	endpoint(endpoint_),
	socket(socket_),
	stream_in(socket_),
	stream_out(socket_),
	in(&stream_in),
	out(&stream_out)
{
	reader_thread = std::thread(&JRPC_Client::read_loop, this);
}


JRPC_Client::JRPC_Client(int socket_):
	JRPC_Client(nullptr, socket_)
{
}


JRPC_Client::JRPC_Client(const std::string &address):
	JRPC_Client(Endpoint::from_url(address))
{
}


JRPC_Client::~JRPC_Client(){
	{
		std::lock_guard<std::mutex> lock(mutex_keep_on_running);
		keep_on_running = false;
	}
	if(socket >= 0) {
#ifdef _WIN32
		::shutdown(socket, SD_BOTH);
#else
		::shutdown(socket, SHUT_RDWR);
#endif
	}
	reader_thread.join();
	close_socket();
}


Variant JRPC_Client::call(const std::string &method, const Object &args){
	if(!reconnect()){
		throw std::runtime_error("Could not establish connection");
	}

	const int request_id = jrpc_id++;
	JRPC_Request req;
	req.jsonrpc = jrpc_version;
	req.id = request_id;
	req.method = method;
	req.params = args;

	vnx::write(out, req);
	out.flush();


	while(do_run()){
		std::shared_ptr<JSON_Object> json_object = read_recv(-1);
		if(!json_object) continue;
		const Object object = json_object->to_object();

		JRPC_Request jrequest;
		JRPC_Response response;
		if(is_request_object(object, jrequest)){
			// not an answer
			buffer_queue.push(object);
		}else if(is_response_object(object, response)){
			if(response.id.to<int>() == request_id){
				// match
				JRPC_Success success;
				success.from_object(object);
				if(!success.result.empty()){
					// success
					return success.result;
				}else{
					// failure
					JRPC_Failure failure;
					failure.from_object(object);
					return Variant(failure.error);
				}
			}else{
				// some other response / failure
				buffer_queue.push(object);
			}
		}
	}
	return Variant();
}


void JRPC_Client::call_async(const std::string &method, const Object &args){
	if(!reconnect()){
		throw std::runtime_error("Could not establish connection");
	}

	JRPC_Notification req;
	req.jsonrpc = jrpc_version;
	req.method = method;
	req.params = args;

	vnx::write(out, req);
	out.flush();
}


std::shared_ptr<JRPC_Notification> JRPC_Client::poll(int timeout_ms){
	// first try the buffered
	while(!buffer_queue.empty()){
		Object object = buffer_queue.front();
		buffer_queue.pop();

		auto result = object_to_request_object(object);
		if(result) return result;
	}

	// read the fresh
	auto json_object = read_recv(timeout_ms);
	if(!json_object) return nullptr;

	const Object object = json_object->to_object();
	auto result = object_to_request_object(object);
	if(result) return result;

	return nullptr;
}


bool JRPC_Client::do_run(){
	std::lock_guard<std::mutex> lock(mutex_keep_on_running);
	return keep_on_running;
}


void JRPC_Client::close_socket(){
	std::lock_guard<std::mutex> lock(mutex_socket);
	if(socket < 0) return;
#ifdef _WIN32
	::closesocket(socket);
#else
	::close(socket);
#endif
	socket = -1;
}


bool JRPC_Client::reconnect(){
	std::lock_guard<std::mutex> lock(mutex_socket);
	if(socket >= 0) return true;
	if(!endpoint){
		log_warn() << "No endpoint to connect to";
		return false;
	}

	try{
		socket = endpoint->open();
		endpoint->connect(socket);
	}catch(const std::exception& ex) {
		log_warn() << ex.what() << " (" << endpoint->to_url() << ")";
#ifdef _WIN32
		::closesocket(socket);
#else
		::close(socket);
#endif
		socket = -1;
	}

	if(socket >= 0){
		stream_in.reset(socket);
		stream_out.reset(socket);
		return true;
	}
	return false;
}


void JRPC_Client::read_loop(){
	while(do_run()){
		if(!reconnect()){
			// connection attempt failed, sleep for a bit
			for(int i = 0; i < 10 && do_run(); ++i) {
				::usleep(100*1000);
			}
		}else{
			std::shared_ptr<JSON_Value> input;
			try{
				input = vnx::read_json(in);
			}catch(const std::exception& ex) {
				log_info() << ex.what();
				close_socket();
			}


			std::shared_ptr<JSON_Object> json_object;
			if(!input){
				close_socket();
			}else if((json_object = std::dynamic_pointer_cast<JSON_Object>(input))){
				{
					std::lock_guard<std::mutex> lock(mutex_recv_queue);
					recv_queue.push(json_object);
				}
				cv_recv_queue.notify_all();
			}
		}
	}
}


std::shared_ptr<JSON_Object> JRPC_Client::read_recv(int timeout_ms){
	const int64_t time_start = get_wall_time_millis();
	int time_remain = timeout_ms;

	std::unique_lock<std::mutex> lock(mutex_recv_queue);
	while(recv_queue.empty() && (timeout_ms < 0 || time_remain >= 0)){
		cv_recv_queue.wait_for(lock, std::chrono::milliseconds(time_remain));
		time_remain = timeout_ms - (get_wall_time_millis() - time_start);
	}

	if(recv_queue.empty()) return nullptr;
	std::shared_ptr<JSON_Object> json_object = recv_queue.front();
	recv_queue.pop();
	return json_object;
}


bool JRPC_Client::is_request_object(const Object &object, JRPC_Request &request) const{
	request.from_object(object);
	return request.is_request_object();
}


bool JRPC_Client::is_response_object(const Object &object, JRPC_Response &response) const{
	response.from_object(object);
	return response.is_response_object();
}


std::shared_ptr<JRPC_Notification> JRPC_Client::object_to_request_object(const Object &object) const{
	JRPC_Request request;
	if(!is_request_object(object, request)) return nullptr;

	if(request.id.empty()){
		return std::make_shared<JRPC_Notification>(std::move(request));
	}else{
		return std::make_shared<JRPC_Request>(std::move(request));
	}
}




} // vnx




