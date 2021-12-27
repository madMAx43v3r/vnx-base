/*
 * JRPC_Response.cpp
 *
 *  Created on: Jan 22, 2021
 *      Author: jaw
 */

#include <vnx/JRPC_Response.hxx>


namespace vnx {


bool JRPC_Response::is_response_object() const{
	return jsonrpc == "2.0" && !id.empty();
}


} // vnx
