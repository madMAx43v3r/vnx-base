/*
 * JRPC_Request.cpp
 *
 *  Created on: Jan 22, 2021
 *      Author: jaw
 */

#include <vnx/JRPC_Request.hxx>


namespace vnx {


bool JRPC_Request::is_request() const{
	return is_request_object() && !id.empty();
}


bool JRPC_Request::is_notification() const{
	return is_request_object() && id.empty();
}


bool JRPC_Request::is_request_object() const{
	return jsonrpc == "2.0" && !method.empty() && (id.empty() || id.is_string() || id.is_integral() || id.is_null()) && (params.empty() || params.is_object() || params.is_array());
}


} // vnx
