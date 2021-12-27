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

#include <vnx/Return.hxx>
#include <vnx/NoSuchService.hxx>
#include <vnx/OverflowException.hxx>
#include <vnx/PermissionDenied.hxx>


namespace vnx {


std::shared_ptr<const Return> create_exception_return(std::shared_ptr<const Request> request, std::shared_ptr<Exception> ex){
	if(request->flags & Message::NO_RETURN) {
		return nullptr;
	}
	auto return_msg = Return::create();
	return_msg->flags = request->flags & (Message::BLOCKING);
	return_msg->request_id = request->request_id;
	return_msg->src_mac = request->src_mac;
	return_msg->value = ex;
	return return_msg;
}

std::shared_ptr<const Return> Return::create_overflow_return(std::shared_ptr<const Request> request) {
	auto ex = OverflowException::create();
	ex->dst_mac = request->dst_mac;
	return create_exception_return(request, ex);
}

std::shared_ptr<const Return> Return::create_no_such_service_return(std::shared_ptr<const Request> request) {
	auto ex = NoSuchService::create();
	ex->dst_mac = request->dst_mac;
	return create_exception_return(request, ex);
}

std::shared_ptr<const Return> Return::create_permission_denied_return(std::shared_ptr<const Request> request){
	auto ex = PermissionDenied::create();
	ex->dst_mac = request->dst_mac;
	return create_exception_return(request, ex);
}


} // vnx
