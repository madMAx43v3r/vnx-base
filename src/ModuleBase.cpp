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

#include <vnx/ModuleBase.h>
#include <vnx/Client.h>
#include <vnx/Util.h>
#include <vnx/Sample.hxx>
#include <vnx/Request.hxx>
#include <vnx/Return.hxx>
#include <vnx/Exception.hxx>
#include <vnx/InternalError.hxx>
#include <vnx/NoSuchService.hxx>
#include <vnx/PermissionDenied.hxx>
#include <vnx/ModuleInfo.hxx>
#include <vnx/vnx.h>


namespace vnx {

const int ModuleBase::UNLIMITED;
const int ModuleBase::PRIORITY_DEFAULT;
const int ModuleBase::PRIORITY_HIGH;
const int ModuleBase::PRIORITY_LOW;
const pipe_mode_e ModuleBase::MODE_DEFAULT;
const pipe_mode_e ModuleBase::MODE_LATEST;
const int ModuleBase::BLOCKING;
const int ModuleBase::NON_BLOCKING;
const int ModuleBase::RESEND;
const int ModuleBase::ERROR;
const int ModuleBase::WARN;
const int ModuleBase::INFO;
const int ModuleBase::DEBUG;


ModuleBase::ModuleBase(const std::string& name_)
	:	vnx_name(name_),
		module_id(Hash64::rand())
{
	vnx::read_config("log_level", vnx_log_level);
	vnx::read_config(vnx_name + ".vnx_virtual_time", vnx_virtual_time);
	vnx::read_config(vnx_name + ".vnx_clean_exit", vnx_clean_exit);
	vnx::read_config(vnx_name + ".vnx_auto_shutdown", vnx_auto_shutdown);
	vnx::read_config(vnx_name + ".vnx_auto_decompress", vnx_auto_decompress);
	vnx::read_config(vnx_name + ".vnx_log_level", vnx_log_level);
	vnx::read_config(vnx_name + ".vnx_task_priority", vnx_task_priority);
	vnx::read_config(vnx_name + ".vnx_default_queue_ms", vnx_default_queue_ms);
	vnx::read_config(vnx_name + ".vnx_default_queue_size", vnx_default_queue_size);
	vnx::read_config(vnx_name + ".vnx_time_slice_us", vnx_time_slice_us);
	vnx::read_config(vnx_name + ".vnx_heartbeat_interval_ms", vnx_heartbeat_interval_ms);

	module_info.id = module_id;
	module_info.name = vnx_name;
}

ModuleBase::~ModuleBase() {
	// nothing to do
}

Node* ModuleBase::vnx_get_node() {
	return this;
}

Hash64 ModuleBase::vnx_get_id() const {
	return module_id;
}

std::string ModuleBase::vnx_get_name() const {
	return vnx_name;
}

TypeCode ModuleBase::vnx_get_type_code() const {
	return *get_type_code();
}

std::shared_ptr<const ModuleInfo> ModuleBase::vnx_get_module_info() const {
	std::shared_ptr<ModuleInfo> info = ModuleInfo::create();
	*info = module_info;
	info->time = vnx::get_time_micros();
	info->type = get_type_name();
	info->src_mac = publisher->get_src_mac();
	for(const auto& entry : async_clients) {
		info->num_async_pending += entry.second->vnx_get_num_pending();
	}
	{
		std::lock_guard<std::mutex> lock(async_mutex);
		info->num_async_process = async_requests.size();
	}
	for(const auto& topic : Subscriber::get_topics()) {
		info->sub_topics.emplace_back(topic->get_name());
	}
	for(const auto& topic : publisher->get_topics()) {
		info->pub_topics.emplace_back(topic->get_name());
	}
	info->remotes = vnx_remotes;
	info->type_code = vnx_get_type_code();
	return info;
}

Object ModuleBase::vnx_get_config_object() const {
	const bool protect = !vnx::is_allowed(vnx_request->session, permission_e::PROTECTED_CONFIG);
	if(protect && vnx::is_config_protected(vnx_name)) {
		throw std::runtime_error("config is protected");
	}
	auto obj = to_object();
	std::vector<std::string> list;
	for(const auto& entry : obj.field) {
		if(protect && vnx::is_config_protected(vnx_name + "." + entry.first)) {
			list.push_back(entry.first);
		}
	}
	for(const auto& key : list) {
		obj.erase(key);
	}
	return obj;
}

Variant ModuleBase::vnx_get_config(const std::string& name) {
	const bool protect = !vnx::is_allowed(vnx_request->session, permission_e::PROTECTED_CONFIG);
	if(protect && vnx::is_config_protected(vnx_name + "." + name)) {
		throw std::runtime_error("config is protected");
	}
	return get_field(name);
}

void ModuleBase::vnx_set_config_object(const Object& config) {
	from_object(config);
}

void ModuleBase::vnx_set_config(const std::string& name, const Variant& value) {
	set_field(name, value);
}

bool ModuleBase::add_task(const std::function<void()>& func) const {
	auto task = Task::create();
	task->function = func;
	return send_msg(task_pipe, task) == PIPE_OK;
}

std::shared_ptr<Timer> ModuleBase::add_timer(const std::function<void()>& func) {
	auto timer = std::make_shared<Timer>();
	timer->func = func;
	timers.insert(timer);
	return timer;
}

std::weak_ptr<Timer> ModuleBase::set_timeout_micros(int64_t interval_us, const std::function<void()>& func) {
	auto timer = set_timer_micros(interval_us, func);
	timer->is_repeat = false;
	timer->is_one_shot = true;
	return timer;
}

std::weak_ptr<Timer> ModuleBase::set_timeout_millis(int64_t interval_ms, const std::function<void()>& func) {
	return set_timeout_micros(interval_ms * 1000, func);
}

std::shared_ptr<Timer> ModuleBase::set_timer_micros(int64_t interval_us, const std::function<void()>& func) {
	auto timer = add_timer(func);
	timer->interval = interval_us;
	timer->is_repeat = true;
	timer->reset();
	return timer;
}

std::shared_ptr<Timer> ModuleBase::set_timer_millis(int64_t interval_ms, const std::function<void()>& func) {
	return set_timer_micros(interval_ms * 1000, func);
}

void ModuleBase::add_async_client(std::shared_ptr<AsyncClient> client) {
	client->vnx_set_node(this);
	async_clients[client->vnx_get_src_mac()] = client;
}

void ModuleBase::rem_async_client(std::shared_ptr<AsyncClient> client) {
	client->vnx_set_node(nullptr);
	async_clients.erase(client->vnx_get_src_mac());
}

void ModuleBase::handle(std::shared_ptr<const Value> value) {
	// nothing
}

void ModuleBase::handle(std::shared_ptr<const Message> msg)
{
	switch(msg->get_type_hash()) {
		case Frame::VNX_TYPE_ID:
			vnx_frame = std::static_pointer_cast<const Frame>(msg);
			try {
				handle(vnx_frame);
			} catch(const std::exception& ex) {
				log(WARN) << ex.what();
			}
			vnx_frame = nullptr;
			break;
		case Sample::VNX_TYPE_ID: {
			auto sample = std::static_pointer_cast<const Sample>(msg);
			if(!sample->topic) {
				return;
			}
			// check the sequence number for duplicate samples
			uint64_t& last_seq = seq_map[std::make_pair(sample->src_mac, sample->topic->get_hash())];
			const bool is_resend = (msg->flags & RESEND) && sample->seq_num == last_seq;
			if(sample->seq_num > last_seq || is_resend) {
				last_seq = std::max(sample->seq_num, last_seq);
				if(vnx_auto_decompress && sample->value) {
					const auto tmp = sample->value->vnx_decompress();
					if(tmp) {
						const auto copy = vnx::clone(sample);
						copy->value = tmp;
						sample = copy;
					}
				}
				if(auto object = std::dynamic_pointer_cast<const Object>(sample->value)) {
					if(auto value = object->to_value(type_map)) {
						auto copy = vnx::clone(sample);
						copy->value = value;
						sample = copy;
					}
				}
				vnx_sample = sample;
				try {
					if(is_resend) {
						handle_resend(sample);
					} else {
						handle(sample);
					}
				} catch(const std::exception& ex) {
					log(WARN) << ex.what();
				}
				vnx_sample = nullptr;
			}
			break;
		}
		case Request::VNX_TYPE_ID: {
			vnx_request = std::static_pointer_cast<const Request>(msg);
			std::shared_ptr<const Return> ret;
			try {
				ret = handle(vnx_request);
			} catch(const std::exception& ex) {
				log(WARN) << ex.what();
			}
			vnx_request = nullptr;
			if(ret && !(msg->flags & Message::NO_RETURN)
				&& (!(msg->flags & Message::FORWARD) || std::dynamic_pointer_cast<const Exception>(ret->value)))
			{
				send_msg(ret->src_mac, ret);
			}
			break;
		}
		case Return::VNX_TYPE_ID:
			vnx_return = std::static_pointer_cast<const Return>(msg);
			try {
				handle(vnx_return);
			} catch(const std::exception& ex) {
				log(WARN) << ex.what();
			}
			vnx_return = nullptr;
			break;
		case Task::VNX_TYPE_ID:
			vnx_task = std::static_pointer_cast<const Task>(msg);
			try {
				handle(vnx_task);
			} catch(const std::exception& ex) {
				log(WARN) << ex.what();
			}
			vnx_task = nullptr;
			break;
		case FlowMessage::VNX_TYPE_ID:
			handle(std::static_pointer_cast<const FlowMessage>(msg));
			break;
	}
}

void ModuleBase::handle(std::shared_ptr<const Frame> frame) {
	if(frame->value) {
		vnx_handle_switch(frame->value);
	}
}

void ModuleBase::handle(std::shared_ptr<const Sample> sample)
{
	if(vnx_virtual_time && sample->topic == vnx::time_control) {
		if(auto value = std::dynamic_pointer_cast<const TimeControl>(sample->value)) {
			time_state = *value;
		}
	}
	if(vnx_auto_shutdown && sample->topic == vnx::shutdown) {
		vnx_stop();
	}
	if(sample->value) {
		vnx_handle_switch(sample->value);
	}
}

void ModuleBase::handle(std::shared_ptr<const Task> task) {
	if(task->function) {
		task->function();
	}
}

void ModuleBase::handle(std::shared_ptr<const FlowMessage> msg) {
	if(msg->flow_code == FlowMessage::CLOSE) {
		// delete corresponding seq_map entry if any
		auto iter = seq_map.find(std::make_pair(msg->src_mac, msg->dst_mac));
		if(iter != seq_map.end()) {
			seq_map.erase(iter);
		}
	}
}

std::shared_ptr<const Return> ModuleBase::handle(std::shared_ptr<const Request> request)
{
	std::shared_ptr<Return> ret = Return::create();
	ret->flags = request->flags & (Message::BLOCKING);
	ret->request_id = request->request_id;
	ret->src_mac = request->src_mac;

	auto method = request->method;
	if(!method) {
		ret->value = InternalError::from_what("method == null");
		return ret;
	}
	if(auto object = std::dynamic_pointer_cast<const Object>(method)) {
		// try to convert object to a native method type
		method = object->to_value(type_map);
		if(!method) {
			auto ex = vnx::NoSuchMethod::create();
			ex->dst_mac = request->dst_mac;
			ex->method = (*object)["__type"].to<std::string>();
			ret->value = ex;
			return ret;
		}
	} else if(auto* type_code = method->get_type_code()) {
		// check if method is mapped to a different type
		auto iter = type_map.find(type_code->method_name);
		if(iter != type_map.end() && iter->second != type_code->name) {
			// try to convert to new method type
			if(auto* new_type_code = vnx::get_type_code(iter->second)) {
				if(auto value = new_type_code->create_value()) {
					value->from_object(method->to_object());
					method = value;
				}
			}
		}
	}
	if(!method) {
		ret->value = InternalError::from_what("method == null");
		return ret;
	}
	if(request->session) {
		auto* type_code = method->get_type_code();
		if(!type_code) {
			ret->value = InternalError::from_what("type_code == null");
			return ret;
		}
		auto* native_type_code = type_code->native;
		if(!native_type_code) {
			// to be safe, return an exception here already (instead of calling vnx_call_switch())
			auto ex = NoSuchMethod::create();
			ex->dst_mac = request->dst_mac;
			ex->method = type_code->name;
			ret->value = ex;
			return ret;
		}
		std::string permission = native_type_code->permission;
		if(permission.empty()) {
			// assume default permissions
			const permission_e perm = native_type_code->is_const ? permission_e::CONST_REQUEST : permission_e::REQUEST;
			permission = perm.to_string_value_full();
		}
		if(!vnx::is_allowed(request->session, permission)) {
			log(WARN) << "Denied " << type_code->get_method_name() << "() due to missing: " << permission;
			auto ex = PermissionDenied::create();
			ex->dst_mac = request->dst_mac;
			ex->method = type_code->name;
			ex->permission = permission;
			ex->what = "permission denied (" + permission + ")";
			ret->value = ex;
			return ret;
		}
	}
	const auto request_id = std::make_pair(request->src_mac, request->request_id);
	{
		std::lock_guard<std::mutex> lock(async_mutex);
		async_requests[request_id] = ret;	// add it before vnx_call_switch() to avoid race-condition
	}
	try {
		std::shared_ptr<Value> return_value = vnx_call_switch(method, request_id);
		if(return_value) {
			ret->value = return_value;
		} else {
			ret = nullptr;	// async return
		}
	} catch(const Exception& ex) {
		ret->value = ex.clone();
	} catch(const std::exception& ex) {
		ret->value = InternalError::from_what(ex.what());
	} catch(...) {
		ret->value = InternalError::create();
	}
	if(ret) {
		std::lock_guard<std::mutex> lock(async_mutex);
		async_requests.erase(request_id);	// remove it again, since it's not async
	}
	return ret;
}

bool ModuleBase::handle(std::shared_ptr<const Return> result) {
	auto iter = async_clients.find(result->src_mac);
	if(iter != async_clients.end()) {
		// client might get removed during the call, make a copy
		auto client = iter->second;
		client->vnx_callback(result);
		return true;
	}
	return false;
}

bool ModuleBase::vnx_async_return(const vnx::request_id_t& request_id, std::shared_ptr<const Value> value) const
{
	std::shared_ptr<Return> ret;
	{
		std::lock_guard<std::mutex> lock(async_mutex);
		auto iter = async_requests.find(request_id);
		if(iter != async_requests.end()) {
			ret = iter->second;
			async_requests.erase(iter);
		} else {
			return false;
		}
	}
	if(ret) {
		ret->value = value;
		if(ret->flags & Message::NO_RETURN) {
			return true;
		} else {
			return send_msg(ret->src_mac, ret) == PIPE_OK;
		}
	}
	return false;
}

bool ModuleBase::vnx_async_return_ex(const vnx::request_id_t& request_id, const std::exception& ex) const
{
	return vnx_async_return(request_id, Exception::from_what(ex.what()));
}

bool ModuleBase::vnx_async_return_ex_what(const vnx::request_id_t& request_id, const std::string& ex_what) const
{
	return vnx_async_return(request_id, Exception::from_what(ex_what));
}

int64_t ModuleBase::vnx_process(bool blocking)
{
	int64_t next_deadline = 0;
	int64_t wall_time = vnx::get_wall_time_micros();
	const int64_t now = time_state.get_time_micros();

	// record idle time
	time_idle += wall_time - last_time;
	last_time = wall_time;

	// go through all the timers and compute the lowest timeout
	for(auto iter = timers.begin(); iter != timers.end();)
	{
		const auto timer = *iter;
		// check if we should reset timer
		if(timer->do_reset) {
			timer->reset(now);
		}
		if(timer->is_active) {
			// check if we jumped back in time
			if(timer->deadline - timer->interval > now) {
				timer->reset(now);
			}
			// check if deadline has passed (ie. if timer expired)
			if(now >= timer->deadline) {
				if(timer->is_repeat) {
					// in case of repeat we reset the deadline to the next cycle
					timer->deadline += timer->interval;
					if(timer->deadline < now) {
						// in this case we are not running in realtime
						timer->deadline = now;	// so we just run as fast as possible
					}
				} else {
					// if timer is not on repeat we deactivate it
					timer->is_active = false;
				}
				try {
					// call the timer function
					if(timer->func) {
						timer->func();
					}
				} catch(const std::exception& ex) {
					log(WARN) << ex.what();
				}
			}
			// check if we should reset timer again
			if(timer->do_reset) {
				timer->reset(now);
			}
			// check if timer is still active or has been re-activated by timer function
			if(timer->is_active) {
				// check if this timer is the next closest to expire
				if(timer->deadline < next_deadline || !next_deadline) {
					next_deadline = timer->deadline;
				}
			}
		}
		if(!timer->is_active && timer->is_one_shot) {
			// delete one shot timer
			iter = timers.erase(iter);
		} else {
			iter++;
		}
	}

	// record timer runtime
	wall_time = vnx::get_wall_time_micros();
	time_running += wall_time - last_time;
	last_time = wall_time;

	// process messages, but not forever
	const auto time_slice_end = wall_time + vnx_time_slice_us;
	do {
		// read next message
		std::shared_ptr<const Message> msg;
		if(blocking) {
			if(next_deadline && !time_state.is_paused) {
				const auto timeout_us = next_deadline - time_state.get_time_micros();
				if(timeout_us <= 0) {
					msg = Node::read();
				} else if(time_state.is_realtime) {
					msg = Node::read_blocking(timeout_us);
				} else {
					msg = Node::read_blocking(timeout_us / time_state.time_speed);
				}
			} else {
				msg = Node::read_blocking();
			}
		} else {
			msg = Node::read();
		}
		if(!msg) {
			break;		// timeout or shutdown
		}
		// record idle time
		wall_time = vnx::get_wall_time_micros();
		time_idle += wall_time - last_time;
		last_time = wall_time;
		try {
			handle(msg);
		} catch(const std::exception& ex) {
			log(WARN) << ex.what();
		}
		// record running time
		wall_time = vnx::get_wall_time_micros();
		time_running += wall_time - last_time;
		last_time = wall_time;
	}
	while(Node::vnx_do_run && wall_time < time_slice_end);

	return (next_deadline && !time_state.is_paused) ?
			std::max(next_deadline - time_state.get_time_micros(), int64_t(0)) : -1;
}

void ModuleBase::vnx_cleanup()
{
	exit();		// prevent new messages from being received

	if(vnx_clean_exit) {
		// process remaining messages
		while(auto msg = Node::read()) {
			try {
				handle(msg);
			} catch(const std::exception& ex) {
				log(WARN) << ex.what();
			}
		}
	}
	close();	// close the node

	// check for pending async requests and cancel them
	{
		std::lock_guard<std::mutex> lock(async_mutex);

		for(const auto& entry : async_requests) {
			auto ret = entry.second;
			auto ex = NoSuchService::create();
			ex->what = "module has shut down";
			ret->value = ex;
			send_msg(ret->src_mac, ret);
		}
		async_requests.clear();
	}

	// check for pending async calls and cancel them
	for(const auto& entry : async_clients) {
		auto client = entry.second;
		client->vnx_shutdown("module shutdown");
	}
}

void ModuleBase::vnx_heartbeat() const {
	module_info.time_idle = time_idle;
	module_info.time_running = time_running;
	module_info.time_idle_total += std::max(time_idle, int64_t(0));
	module_info.time_running_total += std::max(time_running, int64_t(0));
	time_idle = 0;
	time_running = 0;
	publish(vnx_get_module_info(), vnx::module_info);
}

bool_t ModuleBase::vnx_self_test() const {
	return true;
}

void ModuleBase::vnx_setup() {
	last_time = vnx::get_wall_time_micros();
	time_state = TimeControl();
	timers.clear();
	type_map.clear();
	async_clients.clear();
	async_requests.clear();
	module_info.time_started = last_time;
	vnx_reset();

	publisher = std::make_shared<Publisher>();
	task_pipe = Pipe::create();
	connect(task_pipe, this, UNLIMITED, UNLIMITED, vnx_task_priority);
	open_pipe(module_id, this, vnx_default_queue_ms, vnx_default_queue_size);	// open default pipe

	if(vnx_virtual_time) {
		subscribe(vnx::time_control);
	}
	subscribe(vnx::shutdown);
	set_timer_millis(vnx_heartbeat_interval_ms, std::bind(&ModuleBase::vnx_heartbeat, this));

	// add method shortcuts
	for(const auto* method : get_type_code()->methods) {
		type_map[method->method_name] = method->name;
	}
}

void ModuleBase::vnx_reset() {
	seq_map.clear();
}

std::ostream& operator<<(std::ostream& _out, const ModuleBase& _module) {
	_module.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, ModuleBase& _module) {
	_module.read(_in);
	return _in;
}


} // vnx
