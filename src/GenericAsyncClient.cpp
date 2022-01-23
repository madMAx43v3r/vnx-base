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

#include <vnx/GenericAsyncClient.h>


namespace vnx {

GenericAsyncClient::GenericAsyncClient(const std::string& service_name)
	:	ModuleInterfaceAsyncClient(service_name)
{
}

GenericAsyncClient::GenericAsyncClient(vnx::Hash64 service_addr)
	:	ModuleInterfaceAsyncClient(service_addr)
{
}

uint64_t GenericAsyncClient::call(
			const std::string& method, const Object& args,
			const std::function<void(std::shared_ptr<const Value>)>& callback,
			const std::function<void(const vnx::exception&)>& error_callback)
{
	const auto value = vnx::clone(args);
	(*value)["__type"] = method;
	return call(value, callback, error_callback);
}

uint64_t GenericAsyncClient::call(
			std::shared_ptr<const Value> method,
			const std::function<void(std::shared_ptr<const Value>)>& callback,
			const std::function<void(const vnx::exception&)>& error_callback)
{
	const auto request_id = ++vnx_next_id;
	{
		std::lock_guard<std::mutex> lock(vnx_mutex);
		vnx_pending[request_id] = -1;
		vnx_queue_generic[request_id] = std::make_pair(callback, error_callback);
	}
	vnx_request(method, request_id);
	return request_id;
}

int32_t GenericAsyncClient::vnx_purge_request(uint64_t request_id, const vnx::exception& ex) {
	const auto index = ModuleInterfaceAsyncClient::vnx_purge_request(request_id, ex);
	if(index == -1) {
		std::unique_lock<std::mutex> lock(vnx_mutex);
		const auto _iter = vnx_queue_generic.find(request_id);
		if(_iter != vnx_queue_generic.end()) {
			const auto _callback = std::move(_iter->second.second);
			vnx_queue_generic.erase(_iter);
			lock.unlock();
			if(_callback) {
				_callback(ex);
			}
		}
	}
	return index;
}

int32_t GenericAsyncClient::vnx_callback_switch(uint64_t request_id, std::shared_ptr<const vnx::Value> value) {
	const auto index = ModuleInterfaceAsyncClient::vnx_callback_switch(request_id, value);
	if(index == -1) {
		std::unique_lock<std::mutex> lock(vnx_mutex);
		const auto _iter = vnx_queue_generic.find(request_id);
		if(_iter != vnx_queue_generic.end()) {
			const auto _callback = std::move(_iter->second.first);
			vnx_queue_generic.erase(_iter);
			lock.unlock();
			if(_callback) {
				_callback(value);
			}
		}
	}
	return index;
}


} // vnx
