/*
 * JRPC_Proxy.cpp
 *
 *  Created on: Dec 4, 2020
 *      Author: jaw
 */

#include <vnx/vnx.h>
#include <vnx/JRPC_Proxy.h>
#include <vnx/GenericClient.h>
#include <vnx/Generic.hxx>
#include <vnx/JRPC_Notification.hxx>
#include <vnx/JRPC_Request.hxx>
#include <vnx/JRPC_Success.hxx>
#include <vnx/JRPC_Failure.hxx>
#include <vnx/PermissionDenied.hxx>


namespace vnx {

JRPC_Proxy::JRPC_Proxy(const std::string& name_, std::shared_ptr<const Endpoint> endpoint_, int socket_)
	:	JRPC_ProxyBase(name_),
		out(&stream_out)
{
	endpoint = endpoint_;
	stream_out.reset(socket_);
	socket = socket_;
	current_dst = remote_addr;
}


bool JRPC_Proxy::handle(std::shared_ptr<const Return> result) {
	if(!is_connected) return false;

	Variant jrpc_request_id;
	{
		std::lock_guard<std::mutex> lock(mutex_request_id_incoming_jrpcmap);
		auto find_request = request_id_incoming_jrpcmap.find(result->request_id);
		if(find_request == request_id_incoming_jrpcmap.end()){
			// no answer has been requested, don't send any.
			return true;
		}else{
			jrpc_request_id = find_request->second;
			request_id_incoming_jrpcmap.erase(find_request);
		}
	}

	std::shared_ptr<JRPC_Response> response;
	if(auto ex = std::dynamic_pointer_cast<const Exception>(result->value)){
		std::shared_ptr<JRPC_Failure> fail = std::make_shared<JRPC_Failure>();
		if(std::dynamic_pointer_cast<const NoSuchService>(ex)){
			fail->error.code = JRPC_Error::METHOD_NOT_FOUND;
		}else if(std::dynamic_pointer_cast<const PermissionDenied>(ex)){
			fail->error.code = JRPC_Error::PERMISSION_DENIED;
		}else{
			fail->error.code = JRPC_Error::EXCEPTION;
		}
		fail->error.message = ex->what;
		fail->error.data = ex;
		response = fail;
	}else{
		std::shared_ptr<JRPC_Success> success = std::make_shared<JRPC_Success>();
		success->result = result->value->get_field_by_index(0);
		response = success;
	}
	response->jsonrpc = "2.0";
	response->id = jrpc_request_id;

	try {
		vnx::write(out, response);
		out.flush();
		return true;
	} catch(...) {
		// just ignore it
	}

	return false;
}


void JRPC_Proxy::select_service(const std::string& service_name){
	bool self = false;
	if(service_name.empty() || service_name == "."){
		self = true;
		current_dst = remote_addr;
	}else{
		current_dst = Hash64(service_name);
	}

	log(INFO) << "Selected service " << (self ? vnx_name : service_name) << " (" << current_dst << ")";
}


void JRPC_Proxy::send_outgoing(std::shared_ptr<Sample> sample){
	JRPC_Notification notf;
	notf.jsonrpc = "2.0";
	notf.method = "!" + remap_topic(sample->topic, export_map)->get_name();
	Object params;
	params["value"] = sample->value;
	params["seq_num"] = sample->seq_num;
	notf.params = params;

	// no try + catch here
	vnx::write(out, notf);
	out.flush();
}


void JRPC_Proxy::send_outgoing(std::shared_ptr<Request> request){
	std::shared_ptr<JRPC_Notification> jmsg;
	if(request->flags & Message::NO_RETURN){
		jmsg = std::make_shared<JRPC_Notification>();
	}else{
		std::shared_ptr<JRPC_Request> jrequest = std::make_shared<JRPC_Request>();
		const uint64_t id = unique_jrpc_id++;
		jrequest->id = id;
		jmsg = jrequest;
		std::lock_guard<std::mutex> lock(mutex_request_id_outgoing_jrpcmap);
		auto &entry = request_id_outgoing_jrpcmap[id];
		entry.first = request->src_mac;
		entry.second = request->request_id;
	}
	jmsg->jsonrpc = "2.0";
	const Object params = request->method->to_object();
	std::string method_name = params["__type"].to_string_value();
	const auto pos = method_name.rfind('.');
	if(pos != std::string::npos) {
		method_name = method_name.substr(pos+1);
	}
	std::string module_name = "";
	if(request->dst_mac != remote_addr){
		// the request is NOT for the remote proxy, so prepend the actual module name
		module_name = forward_names[request->dst_mac];
	}
	jmsg->method = module_name + "." + method_name;
	jmsg->params = params;

	// no try + catch here
	vnx::write(out, jmsg);
	out.flush();
}


void JRPC_Proxy::connect_reset(){
	out.clear();
}


void JRPC_Proxy::read_socket_until_error(std::shared_ptr<Pipe> service_pipe){
	SocketInputStream stream_in(socket);
	vnx::istream in(&stream_in);
	while(true) {
		std::shared_ptr<JSON_Value> input = vnx::read_json(in);
		if(!input) {
			throw std::underflow_error("EOF");
		}
		std::shared_ptr<JSON_Object> json_object = std::dynamic_pointer_cast<JSON_Object>(input);
		if(!json_object) {
			// skip anything which is not an object
			continue;
		}
		const Object object = json_object->to_object();

		JRPC_Request jrequest;
		jrequest.from_object(object);
		const bool is_request_object = jrequest.is_request_object();
		const bool is_sample = jrequest.is_notification() && jrequest.method.front() == '!' && jrequest.params.is_object();
		const bool is_request_or_notification = is_request_object && jrequest.method.front() != '!';

		JRPC_Success jsuccess;
		JRPC_Failure jfailure;
		bool is_result = false;
		bool is_failure = false;
		if(!is_request_object){
			jsuccess.from_object(object);
			jfailure.from_object(object);
			const bool is_response_object = jsuccess.is_response_object();
			is_result = is_response_object && !jsuccess.result.empty();
			is_failure = is_response_object && !is_result;
		}

		if(is_sample){
			const Object params = jrequest.params.to_object();
			std::shared_ptr<Sample> sample = Sample::create();
			sample->src_mac = vnx_get_id();		// use module id
			sample->seq_num = params["seq_num"];
			sample->topic = jrequest.method.substr(1);
			// publish as Object, since we might not have the native type linked in
			sample->value = params["value"].to<std::shared_ptr<Object>>();

			process(sample, get_session());
		}else if(is_request_or_notification){
			std::shared_ptr<Request> request = Request::create();
			request->flags |= Message::BLOCKING;
			request->src_mac = vnx_get_id();	// use module id
			request->dst_mac = current_dst;
			request->request_id = unique_request_id++;
			if(jrequest.id.empty()){
				request->flags |= Message::NO_RETURN;
			}else{
				std::lock_guard<std::mutex> lock(mutex_request_id_incoming_jrpcmap);
				request_id_incoming_jrpcmap[request->request_id] = jrequest.id;
			}

			// Module.method / .method
			std::string actual_method = jrequest.method;
			const size_t dot = jrequest.method.rfind('.');
			if(dot != std::string::npos){
				if(dot == 0){
					request->dst_mac = remote_addr;
				}else{
					request->dst_mac = Hash64(jrequest.method.substr(0, dot));
				}
				actual_method = jrequest.method.substr(dot+1);
			}

			auto method = std::make_shared<Object>();
			(*method)["__type"] = actual_method;
			if(!jrequest.params.empty()){
				Hash64 dst = request->dst_mac;
				if(dst == remote_addr) dst = public_service_addr;
				const Object params = jrequest.params.is_object() ? jrequest.params.to_object() : map_parameters(dst, actual_method, jrequest.params.to<std::vector<Variant>>());
				for(const auto &pair : params.field){
					(*method)[pair.first] = pair.second;
				}
			}
			request->method = method;

			process(request, get_session(), service_pipe);
		}else if(is_result || is_failure){
			std::shared_ptr<Return> return_msg;

			const uint64_t unique_id = jsuccess.id.to<uint64_t>();
			{
				std::lock_guard<std::mutex> lock(mutex_request_id_outgoing_jrpcmap);
				const auto found = request_id_outgoing_jrpcmap.find(unique_id);
				if(found == request_id_outgoing_jrpcmap.end()){
					log(WARN) << "Ignoring incoming return with unknown id " << unique_id;
				}else{
					return_msg = Return::create();
					return_msg->src_mac = found->second.first;
					return_msg->request_id = found->second.second;
					request_id_outgoing_jrpcmap.erase(found);
				}
			}

			if(return_msg){
				return_msg->flags |= Message::BLOCKING;
				if(is_result){
					return_msg->value = Generic::from_data(jsuccess.result);
				}else if(is_failure){
					return_msg->value = jfailure.error.data;
				}
				process(return_msg);
			}
		}else if(!jrequest.id.empty()){
			JRPC_Failure fail;
			fail.jsonrpc = "2.0";
			fail.id = jrequest.id;
			fail.error.code = JRPC_Error::INVALID_REQUEST;
			fail.error.message = "invalid request";

			try {
				vnx::write(out, fail);
				out.flush();
			} catch(...) {
				// just ignore it
			}
		}
	}
}


Object JRPC_Proxy::map_parameters(const Hash64 &service, const std::string &method, const std::vector<Variant> &parameters){
	GenericClient client(service);
	const TypeCode typecode = client.vnx_get_type_code();

	const vnx::TypeCode* method_type = nullptr;
	for(auto func : typecode.methods) {
		if(func->name == method || func->method_name == method) {
			method_type = func;
			break;
		}
	}

	Object result;
	if(!method_type) return result;

	for(size_t i=0; i<parameters.size(); ++i) {
		if(i < method_type->fields.size()) {
			result[method_type->fields[i].name] = parameters[i];
		}
	}

	return result;
}


} // vnx
