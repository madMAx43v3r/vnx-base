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

#include <vnx/GenericClient.h>


namespace vnx {

GenericClient::GenericClient(const std::string& service_name)
	:	ModuleInterfaceClient(service_name)
{
}

GenericClient::GenericClient(vnx::Hash64 service_addr)
	:	ModuleInterfaceClient(service_addr)
{
}

std::shared_ptr<const Value> GenericClient::call(const std::string& method, const Object& args) {
	const auto value = vnx::clone(args);
	(*value)["__type"] = method;
	return vnx_request(value, false);
}

void GenericClient::call_async(const std::string& method, const Object& args) {
	const auto value = vnx::clone(args);
	(*value)["__type"] = method;
	vnx_request(value, true);
}


} // vnx
