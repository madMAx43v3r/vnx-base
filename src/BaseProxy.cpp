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
#include <vnx/BaseProxy.h>
#include <vnx/Request.hxx>
#include <vnx/Return.hxx>
#include <vnx/NoSuchService.hxx>
#include <vnx/PermissionDenied.hxx>
#include <vnx/UnixEndpoint.hxx>
#include <vnx/TcpEndpoint.hxx>
#include <vnx/FlowMessage.hxx>
#include <vnx/SHA256.h>
#include <vnx/Util.hpp>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif


namespace vnx {

BaseProxy::BaseProxy(const std::string &vnx_name):
	BaseProxyBase(vnx_name)
{
	remote_addr = Hash64("vnx.remote");
	service_addr = Hash64::rand();				// our own unique service address
	public_service_addr = Hash64(vnx_name);
	vnx_auto_decompress = false;				// forward compressed values
	login_credentials = auto_login;
}

void BaseProxy::init() {
	vnx_virtual_time = false;							// don't use virtual time
	open_pipe(service_addr, this, 100);					// private service
	open_pipe(public_service_addr, this, 1000);			// public service
	
	for(auto topic : export_list) {
		enable_export(topic->get_name());
	}
	for(const std::string& service : forward_list) {
		enable_forward(service, max_queue_ms, max_queue_size);
	}
	for(const Hash64& addr : tunnel_list) {
		enable_tunnel(addr, max_queue_ms, max_queue_size);
	}
}

void BaseProxy::main() {
	if(!endpoint && !address.empty()) {
		if(auto tmp = vnx::clone(Endpoint::from_url(address))) {
			tmp->receive_buffer_size = recv_buffer_size;
			tmp->send_buffer_size = send_buffer_size;
			endpoint = tmp;
		} else {
			throw std::logic_error("invalid address: " + address);
		}
	}
	{
		const Hash64 tunnel_addr = Hash64::rand();
		enable_tunnel(tunnel_addr, UNLIMITED, UNLIMITED);					// setup tunnel for the Proxy on the other end
		remote = std::make_shared<ProxyInterfaceClient>("vnx.remote");		// client for the other end
		remote->vnx_set_tunnel(tunnel_addr);
	}

	auth_server = get_auth_server();
	if(use_authentication){
		default_session = auth_server->login_anonymous(default_access);
		internal_session = auth_server->login_anonymous(access_role_e::UNAUTHENTICATED);
		session = default_session;
	}
	
	for(auto topic : import_list) {
		if(topic == vnx::time_sync) {
			time_sync = true;
		} else if(topic) {
			import_table[topic->get_name()]++;
		}
	}
	for(auto entry : import_map) {
		if(auto topic = entry.first) {
			import_table[topic->get_name()]++;
		}
	}
	
	if(time_sync) {
		if(!std::dynamic_pointer_cast<const UnixEndpoint>(endpoint)) {
			// setup tunnel for the Process on the other end
			const Hash64 tunnel_addr = Hash64::rand();
			enable_tunnel(tunnel_addr, max_queue_ms, max_queue_size);
			time_server = new TimeServer("TimeServer");
			time_server->remote_tunnel = tunnel_addr;
			time_server.start();
			log(INFO) << "Enabling remote time sync";
		} else {
			import_table["vnx.time_sync"]++;	// in this case the remote is on the same machine
			log(INFO) << "Enabling local time sync";
		}
	}
	if(auto_import) {
		set_timer_millis(100, std::bind(&BaseProxy::update_topics, this));
	}
	if(socket >= 0) {
		on_connect();	// in this case we are already connected
	}
	
	set_timer_millis(1000, std::bind(&BaseProxy::print_stats, this));
	
	std::thread read_thread(&BaseProxy::read_loop, this, vnx::clone(endpoint));
	
	Super::main();
	
	{
		std::lock_guard<std::mutex> lock(mutex_socket);
		if(socket >= 0) {
#ifdef _WIN32
			::shutdown(socket, SD_BOTH);
#else
			::shutdown(socket, SHUT_RDWR);		// force read loop to exit
#endif
		}
	}
	read_thread.join();		// wait for read loop to exit
	close_socket();			// read_loop() may not close socket
	on_disconnect();		// clean up

	if(default_session) {
		auth_server->logout(default_session->id);
	}
	if(internal_session) {
		auth_server->logout(internal_session->id);
	}
}

void BaseProxy::handle(std::shared_ptr<const Sample> sample) {
	// check hop count for cycles
	if(sample->hop_count >= max_hop_count) {
		log(WARN) << "Cycle detected on topic '" << sample->topic->get_name() << "'!";
		return;
	}
	if(sample->topic == vnx::shutdown) {
		Super::handle(sample);
		return;
	}
	uint64_t* last_seq_num = nullptr;
	
	std::shared_ptr<Sample> copy = vnx::clone(sample);
	if(vnx_pipe) {
		copy->stream = vnx_pipe->get_mac_addr();
	}
	if(sample->topic) {
		// check for re-send
		last_seq_num = &channel_map[Hash128(sample->src_mac, sample->topic->get_hash())];
		if(sample->seq_num == *last_seq_num) {
			copy->flags |= Message::RESEND;
			copy->value = nullptr;		// re-send without data
		} else {
			copy->flags &= ~(Message::RESEND);
		}
	}
	copy->topic = remap_topic(sample->topic, export_map);

	// send to other side
	if(is_connected && (!session || session->has_permission_vnx(permission_e::VIEW))) {
		try {
			send_outgoing(copy);
			if(last_seq_num) {
				*last_seq_num = sample->seq_num;
			}
			num_samples_send++;
		} catch(...) {
			// just ignore it
		}
	}
}

std::shared_ptr<const Return> BaseProxy::handle(std::shared_ptr<const Request> request)
{
	if(	   request->dst_mac == service_addr
		|| request->dst_mac == public_service_addr)
	{
		return Super::handle(request);
	}
	else if(is_connected) {
		std::shared_ptr<Request> copy = vnx::clone(request);
		if(vnx_pipe) {
			copy->stream = vnx_pipe->get_mac_addr();
		}
		try {
			// send it to the other side
			send_outgoing(copy);
			num_requests_send++;
		} catch(...) {
			// in case the request cannot be sent we return an error
			return Return::create_no_such_service_return(request);
		}
		if(request->flags & Message::BLOCKING) {
			std::lock_guard<std::mutex> lock(mutex_request_map);
			// keep track of this request in case the connection breaks down
			request_map[request->src_mac][request->request_id] = request;
		}
	}
	else if(request->flags & Message::BLOCKING) {
		// in case the request cannot be sent we return an error
		return Return::create_no_such_service_return(request);
	}
	return nullptr;
}

void BaseProxy::handle_resend(std::shared_ptr<const Sample> sample) {
	handle(sample);
}

void BaseProxy::login_async(const std::string &name, const std::string &password, const request_id_t& request_id) const {
	const std::string hashed_password = vnx::sha256_str(password);
	login_credentials = std::make_pair(name, hashed_password);
	if(is_connected){
		remote->on_login_async(name, hashed_password);
	}
	login_request = vnx::request_t<std::shared_ptr<const Session>>(
		std::bind(&BaseProxy::login_async_return, this, request_id, std::placeholders::_1),
		std::bind(&BaseProxy::vnx_async_return, this, request_id, std::placeholders::_1)
	);
}

void BaseProxy::enable_import(const std::string& topic_name) {
	import_table[topic_name]++;
	if(is_connected) {
		remote->enable_export_async(topic_name);	// tell other end to export this topic
		log(INFO) << "remote->enable_export('" << topic_name << "')";
	}
}

void BaseProxy::disable_import(const std::string& topic_name) {
	import_table[topic_name]--;
	if(is_connected) {
		remote->disable_export_async(topic_name);		// tell other end to stop exporting this topic
		log(INFO) << "remote->disable_export('" << topic_name << "')";
	}
}

void BaseProxy::enable_export(const std::string& topic_name) {
	export_table[topic_name]++;
	subscribe(topic_name, max_queue_ms, max_queue_size);
	log(INFO) << "enable_export('" << topic_name << "')";
}

void BaseProxy::disable_export(const std::string& topic_name) {
	export_table[topic_name]--;
	unsubscribe(topic_name);
	log(INFO) << "disable_export('" << topic_name << "')";
}

void BaseProxy::enable_forward(const std::string& service_name, const int32_t& max_queue_ms, const int32_t& max_queue_size) {
	const Hash64 tunnel_addr(service_name);
	enable_tunnel(tunnel_addr, max_queue_ms, max_queue_size);
	forward_names[tunnel_addr] = service_name;
	log(INFO) << "enable_forward('" << service_name << "', " << max_queue_ms << ", " << max_queue_size << ")";
}

void BaseProxy::enable_tunnel(const Hash64& tunnel_addr, const int32_t& max_queue_ms, const int32_t& max_queue_size) {
	uint64_t& counter = forward_table[tunnel_addr];
	if(!counter) {
		// open a pipe so we can receive requests for this service
		auto pipe = open_pipe(tunnel_addr, this, max_queue_ms, max_queue_size);
		if(!is_connected && (block_until_reconnect || (never_connected && block_until_connect))) {
			pipe->pause();
		}
		request_pipes[tunnel_addr] = pipe;
	}
	counter++;
}

void BaseProxy::disable_forward(const std::string& service_name) {
	disable_tunnel(Hash64(service_name));
	log(INFO) << "disable_forward('" << service_name << "')";
}

void BaseProxy::disable_tunnel(const Hash64& tunnel_addr) {
	uint64_t& counter = forward_table[tunnel_addr];
	if(counter == 1) {
		request_pipes.erase(tunnel_addr);
		close_pipe(tunnel_addr);
		counter = 0;
		forward_names.erase(tunnel_addr);
	} else if(counter > 0) {
		counter--;
	}
}

void BaseProxy::wait_on_connect_async(const request_id_t& request_id) const {
	if(is_connected) {
		wait_on_connect_async_return(request_id, remote_process_id);
	} else {
		waiting_on_connect.emplace_back(
			std::bind(&BaseProxy::wait_on_connect_async_return, this, request_id, std::placeholders::_1),
			std::bind(&BaseProxy::vnx_async_return, this, request_id, std::placeholders::_1));
	}
}

void BaseProxy::wait_on_disconnect_async(const request_id_t &request_id) const {
	if(is_connected){
		waiting_on_disconnect.emplace_back(
			std::bind(&BaseProxy::wait_on_disconnect_async_return, this, request_id, std::placeholders::_1),
			std::bind(&BaseProxy::vnx_async_return, this, request_id, std::placeholders::_1)
		);
	}else{
		wait_on_disconnect_async_return(request_id, remote_process_id);
	}
}

void BaseProxy::on_connect() {
	stream_out.reset(socket);	// set new socket
	connect_reset();

	// resume pipes
	for(const auto& entry : request_pipes) {
		entry.second->resume();
	}

	// announce ourselves
	remote->on_remote_connect_async(vnx::get_process_id());

	// automatically login (again)
	if(!login_credentials.first.empty()){
		remote->on_login_async(login_credentials.first, login_credentials.second);
	}

	// tell other end to export the topics we want
	for(const auto& entry : import_table) {
		if(entry.second > 0) {
			remote->enable_export_async(entry.first);
			log(INFO) << "remote->enable_export('" << entry.first << "')";
		}
	}

	is_connected = true;
	never_connected = false;

	if(endpoint) {
		log(INFO) << "Connected to " << endpoint->to_url();
	}
}

void BaseProxy::on_disconnect() {
	if(block_until_reconnect) {
		// pause pipes
		for(const auto& entry : request_pipes) {
			entry.second->pause();
		}
	}
	for(auto &req : waiting_on_disconnect){
		req.callback(remote_process_id);
	}
	waiting_on_disconnect.clear();
	
	// for any pending requests we have to return an error
	{
		std::lock_guard<std::mutex> lock(mutex_request_map);
		for(const auto& client : request_map) {
			for(const auto& entry : client.second) {
				send_msg(entry.second->src_mac, Return::create_no_such_service_return(entry.second));
			}
		}
		request_map.clear();
	}
	
	// close open connections
	for(const auto& entry : outgoing) {
		auto flow_msg = FlowMessage::create();
		flow_msg->src_mac = entry.B();
		flow_msg->dst_mac = entry.A();
		flow_msg->flow_code = FlowMessage::CLOSE;
		send_msg(entry.A(), flow_msg);
	}
	
	outgoing.clear();
	channel_map.clear();
	vnx_remotes.clear();
	is_connected = false;
	disconnect_cleanup();

	if(session && session != default_session) {
		// logout and reset to default
		auth_server->logout(session->id);
		{
			std::lock_guard<std::mutex> lock(mutex_session);
			session = default_session;
		}
	}
}

void BaseProxy::on_remote_connect(const Hash64& process_id) {
	remote_process_id = process_id;
	vnx_remotes[process_id] = endpoint;
	// notify waiting
	for(auto& request : waiting_on_connect) {
		request.callback(process_id);
	}
	waiting_on_connect.clear();
}

void BaseProxy::on_login(const std::string &name, const std::string &password) {
	if(use_authentication && allow_login){
		if(session && session != default_session) {
			auth_server->logout(session->id);
		}
		auto new_session = auth_server->login(name, password, default_access);
		{
			std::lock_guard<std::mutex> lock(mutex_session);
			session = new_session;
		}
		if(new_session->user == name) {
			log(INFO) << "User '" << name << "' logged in with permissions: " << vnx::to_string(session->permissions);
		} else {
			log(WARN) << "Login for user '" << name << "' failed, still got permissions: " << vnx::to_string(session->permissions);
		}
	}else{
		log(WARN) << "Authentication disabled, ignoring login attempt.";
	}
	remote->on_remote_login_async(session);
}

void BaseProxy::on_remote_login(std::shared_ptr<const Session> remote_session) {
	if(remote_session) {
		if(remote_session->user == login_credentials.first) {
			log(INFO) << "Logged in as '" << remote_session->user << "' with permissions: " << vnx::to_string(remote_session->permissions);
		} else {
			log(WARN) << "Login failed, still got permissions: " << vnx::to_string(remote_session->permissions);
		}
	} else {
		log(WARN) << "Authentication disabled on remote server!";
	}
	login_request.callback(remote_session);
}

void BaseProxy::update_topics() {
	for(const auto& topic : vnx::get_all_topics()) {
		if(topic->get_domain_name() == "vnx") {
			continue;
		}
		const TopicInfo& info = topic->get_info();

		bool is_import = false;
		bool is_export = false;
		{
			auto topic = get_topic(info.name);
			while(topic) {
				if(import_table[topic->get_name()] > 0) {
					is_import = true;
				}
				if(export_table[topic->get_name()] > 0) {
					is_export = true;
				}
				topic = topic->get_parent();
			}
		}
		if(info.num_subscribers > 0) {
			if(!is_import && !is_export) {
				enable_import(info.name);
			}
		} else {
			if(import_table[info.name] > 0) {
				disable_import(info.name);
			}
		}
	}
}

void BaseProxy::print_stats() {
	log(DEBUG) << "send: " << num_samples_send << " smp/s, " << num_requests_send << " req/s, " << num_frames_send << " frm/s"
				<< ", recv: " << num_samples_recv << " smp/s, " << num_requests_recv << " req/s, " << num_frames_recv << " frm/s";
	num_frames_recv = 0;
	num_frames_send = 0;
	num_samples_recv = 0;
	num_samples_send = 0;
	num_requests_recv = 0;
	num_requests_send = 0;
}

bool BaseProxy::rewire_connection() {
	if(socket >= 0) return true;

	if(!endpoint) {
		// in this case we can't do nothing
		return false;
	}
	while(vnx_do_run()) {
		// if there was an error before we wait some time
		for(int i = 0; i < 10 && is_error && vnx_do_run(); ++i) {
			::usleep(100*1000);		// don't sleep too long at a time though
		}
		if(!vnx_do_run()) {
			return false;
		}
		try {
			{
				std::lock_guard<std::mutex> lock(mutex_socket);
				if(socket < 0) {
					socket = endpoint->open();
				}
			}
			endpoint->connect(socket);
			// success
			return true;
		} catch(const std::exception& ex) {
			log(WARN) << ex.what() << " (" << endpoint->to_url() << ")";
		}
		is_error = true;
	}
	return false;
}

std::shared_ptr<const Session> BaseProxy::get_session() {
	if(use_authentication) {
		std::lock_guard<std::mutex> lock(mutex_session);
		return session;
	}
	return nullptr;
}

std::shared_ptr<Pipe> BaseProxy::add_return_pipe(	Hash64 src_mac, std::shared_ptr<Pipe> pipe,
													int max_queue_ms, int max_queue_size, bool reconnect)
{
	std::shared_ptr<Pipe>& return_pipe = return_map[src_mac];
	if(!return_pipe) {
		// open a return pipe to forward the response to the other end (never block on returns)
		return_pipe = open_pipe(src_mac, this, max_queue_ms, max_queue_size);
		// connect pipes, such that client gets notified via return_pipe if service closes
		connect(pipe, return_pipe);
		log(DEBUG) << "Added return pipe for src_mac " << src_mac;
	} else if(reconnect) {
		connect(pipe, return_pipe);
	}
	return return_pipe;
}

void BaseProxy::read_loop(std::shared_ptr<const Endpoint> endpoint)
{
	ProxyInterfaceClient proxy(service_addr);						// client for this module
	std::shared_ptr<Pipe> service_pipe = get_pipe(service_addr);	// pipe to send requests to this module

	while(vnx_do_run()) {
		{
			// synchronize with main thread for socket
			std::lock_guard<std::mutex> lock(mutex_socket);
		}
		if(socket < 0){
			if(!rewire_connection()) break;
			try {
				proxy.on_connect();		// tell our module that we are connected now
			} catch(...) {
				// in this case we are shutting down
				break;
			}
		}

		try {
			read_socket_until_error(service_pipe);
		} catch(const std::exception& ex) {
			if(vnx_do_run()) {
				log(DEBUG) << ex.what();
			}
			is_error = true;
		}
		close_socket();

		try {
			proxy.on_disconnect();		// notify our module that we lost the connection
		} catch(...) {
			// no problem
		}
		// close open connections
		for(const auto& entry : incoming) {
			auto flow_msg = FlowMessage::create();
			flow_msg->src_mac = entry.A();
			flow_msg->dst_mac = entry.B();
			flow_msg->flow_code = FlowMessage::CLOSE;
			send_msg(entry.B(), flow_msg);
		}
		incoming.clear();
		recv_buffer.clear();
	}
	exit();
}

void BaseProxy::process(std::shared_ptr<Frame> frame) noexcept {
	frame->stream = public_service_addr;
	try {
		send_msg(frame->dst_mac, frame);
	} catch(...) {
		// ignore overflow
	}
	num_frames_recv++;
}

void BaseProxy::process(std::shared_ptr<Request> request,
						std::shared_ptr<const Session> session_,
						std::shared_ptr<Pipe> service_pipe) noexcept
{
	request->session = session_ ? session_->id : Hash64();

	std::shared_ptr<Pipe> pipe;
	if(request->dst_mac == remote_addr) {
		// the request is actually for us
		request->stream = remote_addr;
		request->dst_mac = service_addr;
		request->session = internal_session ? internal_session->id : Hash64();
		pipe = service_pipe;
	} else {
		if(request_tunnel) {
			request->stream = request_tunnel;
			pipe = get_pipe(request_tunnel);
		} else {
			request->stream = public_service_addr;
			pipe = get_pipe(request->dst_mac);
		}
	}
	if(pipe) {
		add_return_pipe(request->src_mac, pipe, Pipe::UNLIMITED, Pipe::UNLIMITED, false);
		try {
			if(send_msg(pipe, request) != PIPE_OK) {
				send_msg(service_pipe, Return::create_no_such_service_return(request));
			}
		} catch(const std::overflow_error& ex) {
			send_msg(service_pipe, Return::create_overflow_return(request));
		}
	} else {
		send_msg(service_pipe, Return::create_no_such_service_return(request));
	}
	num_requests_recv++;
}

void BaseProxy::process(std::shared_ptr<Sample> sample,
						std::shared_ptr<const Session> session_) noexcept
{
	const permission_e needed = permission_e::PUBLISH;
	if(session_ && !session_->has_permission_vnx(needed)) {
		log(WARN) << "Denied publish to topic '" << sample->topic->get_name() << "' due to missing: " << needed.to_string_value_full();
		return;
	}
	if(sample->topic) {
		auto& last = recv_buffer[Hash128(sample->src_mac, sample->topic->get_hash())];
		// check for re-send
		if((sample->flags & Message::RESEND) && last) {
			sample->value = last->value;
		} else {
			last = sample;
		}
	}
	sample->topic = remap_topic(sample->topic, import_map);
	if(receive_tunnel) {
		sample->stream = receive_tunnel;
		sample->recv_time = get_sync_time_micros();		// normally set by Topic::publish()
		try {
			send_msg(receive_tunnel, sample);
		} catch(...) {
			// ignore overflow
		}
	} else {
		sample->stream = public_service_addr;
		if(sample->topic) {
			try {
				sample->topic->publish(sample);
			} catch(...) {
				// ignore overflow
			}
		}
	}
	num_samples_recv++;
}

void BaseProxy::process(std::shared_ptr<Return> return_msg) noexcept {
	return_msg->stream = public_service_addr;
	try {
		send_msg(return_msg->src_mac, return_msg);
	} catch(...) {
		// ignore overflow
	}
	{
		std::lock_guard<std::mutex> lock(mutex_request_map);
		// we got a return so we can forget about the request now
		request_map[return_msg->src_mac].erase(return_msg->request_id);
	}
}

void BaseProxy::process(std::shared_ptr<FlowMessage> flow_msg) noexcept {
	flow_msg->flags &= ~(Message::NON_BLOCKING);		// not allowed
	if(flow_msg->dst_mac == remote_addr) return;

	if(flow_msg->flow_code == FlowMessage::OPEN) {
		std::shared_ptr<Pipe> pipe;
		if(request_tunnel) {
			flow_msg->stream = request_tunnel;
			pipe = get_pipe(request_tunnel);
		} else {
			flow_msg->stream = public_service_addr;
			pipe = get_pipe(flow_msg->dst_mac);
		}
		add_return_pipe(flow_msg->src_mac, pipe, max_queue_ms, max_queue_size, true);
		incoming.emplace(flow_msg->src_mac, flow_msg->dst_mac);
		send_msg(pipe, flow_msg);
		log(DEBUG) << "Got FlowMessage OPEN from " << flow_msg->src_mac << " to " << flow_msg->dst_mac;
	}
	if(flow_msg->flow_code == FlowMessage::CLOSE) {
		std::shared_ptr<Pipe> return_pipe;
		std::vector<std::shared_ptr<const Request>> failed_requests;
		{
			// check if client closed
			auto iter = return_map.find(flow_msg->src_mac);
			if(iter != return_map.end()) {
				return_pipe = iter->second;
				return_map.erase(iter);
			}
		}
		{
			std::lock_guard<std::mutex> lock(mutex_request_map);
			{
				// check if service closed
				auto iter = request_map.find(flow_msg->dst_mac);
				if(iter != request_map.end()) {
					for(const auto& entry : iter->second) {
						failed_requests.push_back(entry.second);
					}
					request_map.erase(iter);
				}
			}
		}
		if(return_pipe) {
			return_pipe->close();
		}
		for(auto request : failed_requests) {
			send_msg(request->src_mac, Return::create_no_such_service_return(request));
		}
		if(request_tunnel) {
			flow_msg->stream = request_tunnel;
			send_msg(request_tunnel, flow_msg);
		} else {
			flow_msg->stream = public_service_addr;
			send_msg(flow_msg->dst_mac, flow_msg);
		}
		incoming.erase(Hash128(flow_msg->src_mac, flow_msg->dst_mac));
		log(DEBUG) << "Got FlowMessage CLOSE from " << flow_msg->src_mac << " to " << flow_msg->dst_mac;
	}
}

void BaseProxy::close_socket() noexcept {
	std::lock_guard<std::mutex> lock(mutex_socket);
	if(socket >= 0) {
#ifdef _WIN32
		::closesocket(socket);
#else
		::close(socket);
#endif
		socket = -1;
	}
}


} // vnx
