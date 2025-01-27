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
#include <vnx/Process.h>
#include <vnx/Server.h>
#include <vnx/JRPC_Server.h>
#include <vnx/Proxy.h>
#include <vnx/JRPC_Proxy.h>
#include <vnx/Directory.h>
#include <vnx/Authentication.h>
#include <vnx/TopicInfoList.hxx>
#include <vnx/thread_priority_e.hxx>

#include <csignal>
#include <cstdlib>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#undef ERROR
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif


namespace vnx {

namespace process {
	
	std::atomic<bool> do_run(true);
	std::atomic<bool> do_force(false);
	std::atomic<bool> is_waiting(false);
	std::atomic<int> num_modules(0);
	Hash64 id;
	std::string name;
	Handle<Process> instance;
	std::mutex mutex;
	std::thread thread;
	std::condition_variable condition;
	
} // process


static void shutdown_thread() {
	{
		std::unique_lock<std::mutex> lock(process::mutex);
		while(process::do_run) {
			process::condition.wait(lock);
		}
	}
	{
		// trigger soft shutdown
		vnx::Publisher publisher;
		publisher.publish(nullptr, vnx::shutdown);
	}
	{
		bool used_force = false;
		std::unique_lock<std::mutex> lock(process::mutex);
		while(process::num_modules > (process::instance.is_running() ? 1 : 0) || (!process::is_waiting && !used_force)) {
			if(process::do_force && !used_force) {
				shutdown_pipes();	// hard shutdown
				used_force = true;
			}
			process::condition.wait(lock);
		}
		if(!process::is_waiting) {
			process::thread.detach();
		}
	}
	process::instance.close();		// close and wait for instance to be deleted
	vnx::static_cleanup();
#ifdef _WIN32
	WSACleanup();
#endif
}

static void signal_handler(int sig) {
	if(process::do_run) {
		trigger_shutdown();
	} else {
		force_shutdown();
	}
	std::cerr << std::endl;
}

std::vector<std::pair<std::string, std::vector<Variant>>>
parse_args(const std::vector<std::string>& args, const std::map<std::string, std::string>& options)
{
	std::stringstream stream;
	for(const auto& arg : args) {
		if(arg.find_first_of(" \n\r\t") != std::string::npos) {
			stream << vnx::to_string(arg);
		} else {
			stream << arg;
		}
		stream << ' ';
	}
	stream << "--";
	
	std::string key;
	std::vector<Variant> array;
	std::vector<std::pair<std::string, std::vector<Variant>>> result;
	try {
		while(auto json = vnx::read_json(stream, true, true)) {
			Variant value;
			std::string new_key;
			if(auto string = std::dynamic_pointer_cast<JSON_String>(json)) {
				value = string->to_variant();
				new_key = string->get_value();
			} else if(auto variant = std::dynamic_pointer_cast<JSON_Variant>(json)) {
				value = variant->to_variant();
				new_key = variant->to_string();		// for special case of "-1" etc
			} else {
				value = json->to_variant();
			}

			bool is_end = false;
			if(new_key.substr(0, 2) == "--") {
				new_key = new_key.substr(2);
				is_end = new_key.empty();
			} else if(new_key.size() == 2 && new_key[0] == '-') {
				auto iter = options.find(new_key.substr(1));
				if(iter != options.end()) {
					new_key = iter->second;
				} else {
					new_key.clear();
				}
			} else {
				new_key.clear();
			}

			if(new_key.empty() && !is_end) {
				array.push_back(value);
			} else {
				if(!key.empty() || !array.empty()) {
					result.emplace_back(key, array);
				}
				key = new_key;
				array.clear();
			}
			if(is_end) {
				break;
			}
		}
	} catch(...) {
		std::cerr << stream.str() << std::endl;
		throw;
	}
	return result;
}

#ifdef _WIN32
BOOL CtrlHandler(DWORD fdwCtrlType)
{
	signal_handler(0);
	::usleep(30e6);
	return FALSE;
}
#endif

void init(const std::string& process_name, int argc, char** argv, std::map<std::string, std::string> options)
{
#ifdef _WIN32
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
	{
		WSADATA data;
		const int wsaret = WSAStartup(MAKEWORD(1, 1), &data);
		if(wsaret != 0) {
			std::cerr << "WSAStartup() failed with error: " << wsaret << "\n";
		}
	}
	bool use_ansi_escape = true;
	read_config("use_ansi_escape", use_ansi_escape);
	if(use_ansi_escape){
		HANDLE console_out = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD mode = 0;
		GetConsoleMode(console_out, &mode);
		mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(console_out, mode);
	}
#else
	std::signal(SIGINT, signal_handler);
	std::signal(SIGTERM, signal_handler);
#endif
	
	options["h"] = "help";
	options["c"] = "config";
	options["d"] = "debug";
	options["config"] = "directory";
	{
		std::vector<std::string> args;
		for(int i = 1; i < argc; ++i) {
			args.emplace_back(argv[i]);
		}
		std::vector<std::string> config;
		for(const auto& entry : parse_args(args, options)) {
			const auto& key = entry.first;
			const auto& values = entry.second;
			if(key == "config") {
				for(const auto& path : values) {
					const auto path_ = path.to_string_value();
					read_config_tree(path_);
					config.emplace_back(path_);
				}
			} else if(key.empty()) {
				int i = 1;
				for(const auto& value : values) {
					set_config("$" + std::to_string(i++), value);
				}
			} else {
				if(values.empty()) {
					set_config(key, Variant(true));
				} else if(values.size() == 1) {
					set_config(key, values[0]);
				} else {
					write_config(key, values);
				}
			}
		}
		if(!config.empty()) {
			write_config("config", config);
		}
	}
	{
		std::string help;
		if(read_config("help", help)) {
			std::cerr << "Usage: " << process_name;
			std::set<std::string> tmp;
			for(const auto& entry : options) {
				if(entry.first.size() == 1) {
					std::cerr << " [-" << entry.first << " | --" << entry.second;
					auto iter = options.find(entry.second);
					if(iter != options.end()) {
						std::cerr << " <" << iter->second << ">";
						tmp.insert(entry.second);
					}
					std::cerr << "]";
				}
			}
			for(const auto& entry : options) {
				if(entry.first.size() > 1 && tmp.find(entry.first) == tmp.end()) {
					std::cerr << " [--" << entry.first;
					if(!entry.second.empty()) {
						std::cerr << " <" << entry.second << ">";
					}
					std::cerr << "]";
				}
			}
			std::cerr << std::endl;
			::exit(0);
		}
	}
	{
		bool debug = false;
		read_config("debug", debug);
		if(debug) {
			write_config("log_level", 4);
		}
	}
	{
		std::vector<std::string> protect;
		read_config("vnx.protected_config", protect);
		for(const auto& key : protect) {
			protect_config(key);
		}
		protect_config("vnx");
	}

	thread_priority_e process_priority = thread_priority_e::NORMAL;
	vnx::read_config("process_priority", process_priority);
	if(process_priority != thread_priority_e::NORMAL){
		set_current_process_priority(process_priority);
	}

	int log_level = 3;
	read_config("log_level", log_level);

	if(log_level >= 3) {
		for(const auto& entry : get_all_configs()) {
			if(!entry.second.is_object()) {
				std::cerr << entry.first << " = " << entry.second << std::endl;
			}
		}
	}
	for(const auto& key : {"vnx.type_map", "vnx_type_map"})
	{
		std::map<std::string, std::string> type_map;
		vnx::read_config(key, type_map);
		for(const auto& entry : type_map) {
			if(!vnx::register_type_alias(Hash64(entry.second), Hash64(entry.first))) {
				std::cerr << "WARNING: Unknown type '" << entry.second << "'" << std::endl;
			}
		}
	}
	{
		{
			std::lock_guard<std::mutex> lock(process::mutex);
			process::id = Hash64::rand();
			process::name = process_name;
			process::auth_server = std::make_shared<AuthenticationServer>();
		}
		vnx::Handle<vnx::Process> module = new Process(process_name);
		module.start();
		{
			std::lock_guard<std::mutex> lock(process::mutex);
			process::instance = module;
			process::thread = std::thread(&shutdown_thread);
		}
	}
	for(const auto& key : {"vnx.server_map", "vnx_server_map"})
	{
		std::map<std::string, std::string> server_map;
		vnx::read_config(key, server_map);
		for(const auto& entry : server_map) {
			vnx::Handle<vnx::Server> server = new vnx::Server(entry.first, vnx::Endpoint::from_url(entry.second));
			server.start_detached();
		}
	}
	for(const auto& key : {"vnx.jrpc_server_map", "vnx_jrpc_server_map"})
	{
		std::map<std::string, std::string> jrpc_server_map;
		vnx::read_config(key, jrpc_server_map);
		for(const auto &entry : jrpc_server_map){
			vnx::Handle<vnx::JRPC_Server> server = new vnx::JRPC_Server(entry.first, vnx::Endpoint::from_url(entry.second));
			server.start_detached();
		}
	}
	for(const auto& key : {"vnx.proxy_map", "vnx_proxy_map"})
	{
		std::map<std::string, std::string> proxy_map;
		vnx::read_config(key, proxy_map);
		for(const auto& entry : proxy_map) {
			vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy(entry.first, vnx::Endpoint::from_url(entry.second));
			proxy.start_detached();
		}
	}
	for(const auto &key : {"vnx.jrpc_proxy_map", "vnx_jrpc_proxy_map"})
	{
		std::map<std::string, std::string> jrpc_proxy_map;
		vnx::read_config(key, jrpc_proxy_map);
		for(const auto &entry : jrpc_proxy_map){
			vnx::Handle<vnx::JRPC_Proxy> proxy = new vnx::JRPC_Proxy(entry.first, vnx::Endpoint::from_url(entry.second));
			proxy.start_detached();
		}
	}
}

bool do_run() {
	return process::do_run;
}

void wait() {
	std::unique_lock<std::mutex> lock(process::mutex);
	if(process::thread.joinable()) {
		process::is_waiting = true;
		process::condition.notify_all();
		lock.unlock();
		process::thread.join();
	}
}

void close() {
	trigger_shutdown();
	wait();
}

void static_cleanup() {
	process::auth_server = nullptr;
	vnx::topic_shutdown();
}

std::string get_process_name() {
	std::lock_guard<std::mutex> lock(process::mutex);
	return process::name;
}

Hash64 get_process_id() {
	std::lock_guard<std::mutex> lock(process::mutex);
	return process::id;
}

void trigger_shutdown() {
	process::do_run.store(false);
	process::condition.notify_all();
}

void force_shutdown() {
	const auto prev_force = process::do_force.exchange(true);
	trigger_shutdown();
	if(!prev_force) {
		::usleep(100*1000);
		::exit(-1);
	}
}


Process::Process(const std::string& name)
	:	ProcessBase(name)
{
	vnx_clean_exit = true;			// process last log messages on shutdown
	vnx_auto_shutdown = false;		// shutdown_thread() will handle us
#ifdef _WIN32
	read_config("use_ansi_escape", use_ansi_escape);
#endif
}

Process::~Process() {}

void Process::pause_log() {
	is_log_paused++;
}

void Process::resume_log() {
	is_log_paused--;
}

void Process::set_debug(const int32_t& level) {
	log_level = level;
}

void Process::show_error_log(){
	for(std::shared_ptr<const LogMsg> msg : error_history) {
		show_log_message(msg);
	}
	if(error_history.empty()) {
		std::cerr << "No errors!" << std::endl;
	}
}

void Process::grep_log(const std::string &expr){
	grep_filter = expr;
}

void Process::journal_log(const std::string &expr){
	for(std::shared_ptr<const LogMsg> msg : log_history){
		if(is_visible_in_grep(msg) && is_grep_match(msg, expr)) show_log_message(msg);
	}
	grep_log(expr);
}

void Process::ungrep_log(){
	grep_filter.clear();
}

void Process::trigger_shutdown() {
	vnx::trigger_shutdown();
}

void Process::init() {
	open_pipe(vnx_name, this, UNLIMITED, 100);
	open_pipe("vnx.process", this, UNLIMITED, 100);
	open_pipe("vnx.time_server", this, UNLIMITED, 100);
	
	subscribe(vnx::time_control);
	subscribe(vnx::time_sync);
	subscribe(vnx::log_out, 1000);
	subscribe(vnx::module_info);
}

void Process::main() {
	read_config("log_level", log_level);

	open_log_file();

	if(log_file_auto_restart) {
		set_timer_millis(1000, std::bind(&Process::open_log_file, this));
	}
	set_timer_millis(update_interval_ms, std::bind(&Process::update, this));
	
	Super::main();

	if(log_file.is_open()) {
		log_file << vnx::get_date_string_ex(log_file_entry_prefix) << "[@] Shutdown " << vnx_name << std::endl;
		log_file.close();
	}
}

void Process::handle(std::shared_ptr<const LogMsg> value) {
	if(value->level <= LogMsg::ERROR) {
		error_history.push_back(value);
		while(error_history.size() > error_history_size) {
			error_history.pop_front();
		}
	}
	log_history.push_back(value);
	while(log_history.size() > log_history_size){
		log_history.pop_front();
	}
	if(log_file.good() && value->level <= log_file_level) {
		log_file << vnx::get_date_string_ex(log_file_entry_prefix) << value->get_output() << std::endl;
		log_file.flush();
	}

	bool show = (grep_filter.empty() && is_visible_in_log(value)) || (!grep_filter.empty() && is_visible_in_grep(value) && is_grep_match(value));
	if(is_log_paused <= 0 && show) {
		show_log_message(value);
	}
}

void Process::handle(std::shared_ptr<const TimeControl> value) {
	const int64_t delay = vnx::get_sync_time_micros() - value->time;
	if(std::abs(delay) < 1000000) {
		set_time_control(*value);
	} else {
		log(DEBUG) << "Ignoring time_control sample with delay=" << delay/1000 << " ms";
	}
}

void Process::handle(std::shared_ptr<const TimeSync> value) {
	const int64_t delay = vnx::get_wall_time_micros() - value->wall_time;
	if(std::abs(delay) < 1000000) {
		set_time_sync(*value);
	} else {
		log(DEBUG) << "Ignoring time_sync sample with delay=" << delay/1000 << " ms";
	}
}

void Process::handle(std::shared_ptr<const ModuleInfo> value) {
	auto& entry = module_map[value->id];
	entry.info = value;
}

std::string Process::get_name() const {
	return vnx_name;
}

TimeSync Process::get_sync_time() const {
	TimeSync value = get_time_sync();
	value.time = value.get_time_micros();
	value.order += 1;
	return value;
}

std::vector<TopicInfo> Process::get_topic_info(const bool_t& include_domains) const {
	std::vector<TopicInfo> result;
	for(const auto& topic : vnx::get_all_topics(include_domains)) {
		result.emplace_back(topic->get_info());
	}
	return result;
}

std::vector<ModuleInfo> Process::get_module_info() const {
	std::vector<ModuleInfo> result;
	for(const auto& entry : module_map) {
		if(entry.second.info) {
			result.emplace_back(*entry.second.info);
		}
	}
	return result;
}

ProcessInfo Process::get_process_info() const {
	ProcessInfo info;
	info.time = vnx::get_time_micros();
	info.id = vnx::get_process_id();
	info.name = vnx_name;
	info.topics = get_topic_info(true);
	info.modules = get_module_info();
	return info;
}

Variant Process::get_global_config(const std::string& key) const {
	const bool protect = !vnx::is_allowed(vnx_request->session, permission_e::PROTECTED_CONFIG);
	return vnx::get_config(key, protect);
}

void Process::set_global_config(const std::string& key, const Variant& value) {
	const bool protect = !vnx::is_allowed(vnx_request->session, permission_e::PROTECTED_CONFIG);
	vnx::set_config(key, value, protect);
}

void Process::reload_config() {
	std::vector<std::string> paths;
	vnx::read_config("config", paths);
	for(const auto& path : paths) {
		vnx::read_config_tree(path);
		log(INFO) << "Loaded config '" << path << "'";
	}
}

void Process::self_test_all_async(const request_id_t& req_id) {
	auto job = std::make_shared<self_test_job_t>();
	job->req_id = req_id;

	for(const auto& module : vnx::get_all_modules()) {
		auto& entry = module_map[module.get_id()];
		if(!entry.client_async) {
			entry.client_async = std::make_shared<ModuleInterfaceAsyncClient>(module.get_id());
			add_async_client(entry.client_async);
		}
		const auto name = module.get_name();
		log(DEBUG) << "Running self test for module " << name << " ...";
		entry.client_async->vnx_self_test(
				std::bind(&Process::self_test_callback, this, job, name, std::placeholders::_1),
				std::bind(&Process::self_test_failed, this, job, name, std::placeholders::_1));
		job->remaining++;
	}
}

void Process::self_test_callback(std::shared_ptr<self_test_job_t> job, const std::string& module, const vnx::bool_t& result) const {
	if(result) {
		job->passed++;
	} else {
		log(WARN) << "Self test for " << module << " failed!";
		job->failed++;
	}
	job->remaining--;
	self_test_check(job);
}

void Process::self_test_failed(std::shared_ptr<self_test_job_t> job, const std::string& module, const vnx::exception& ex) const {
	if(!std::dynamic_pointer_cast<const vnx::NoSuchService>(ex.value())) {
		job->failed++;
	}
	job->remaining--;
	log(WARN) << "Self test for " << module << " failed with: " << ex.what();
	self_test_check(job);
}

void Process::self_test_check(std::shared_ptr<self_test_job_t> job) const {
	if(job->remaining == 0) {
		if(!job->failed) {
			log(INFO) << "All " << job->passed << " modules passed self test.";
			self_test_all_async_return(job->req_id);
		} else {
			log(ERROR) << job->failed << " out of " << (job->failed + job->passed) << " modules failed self test!";
			vnx_async_return(job->req_id, Exception::from_what("Self test failed!"));
		}
	}
}

void Process::update() {
	{
		auto out = TopicInfoList::create();
		out->time = vnx::get_time_micros();
		out->process = vnx_name;
		out->list = get_topic_info(true);
		publish(out, vnx::topic_info);
	}
	{
		std::unordered_set<Hash64> existing;
		for(const auto& module : vnx::get_all_modules()) {
			existing.insert(module.get_id());
		}
		for(auto iter = module_map.begin(); iter != module_map.end();) {
			if(existing.count(iter->first)) {
				iter++;
			} else {
				const auto& entry = iter->second;
				if(entry.client_async) {
					rem_async_client(entry.client_async);
				}
				iter = module_map.erase(iter);
			}
		}
	}
}

bool Process::is_grep_match(std::shared_ptr<const LogMsg> message) const{
	return is_grep_match(message, grep_filter);
}

bool Process::is_grep_match(std::shared_ptr<const LogMsg> message, const std::string &expr) const{
	if(expr.empty()) return true;
	return (message->module.find(expr) != std::string::npos) || (message->message.find(expr) != std::string::npos);
}

bool Process::is_visible_in_log(std::shared_ptr<const LogMsg> message) const{
	return message->level <= std::max(message->display_level, log_level);
}

bool Process::is_visible_in_grep(std::shared_ptr<const LogMsg> message) const{
	return message->level <= log_level;
}

void Process::show_log_message(std::shared_ptr<const LogMsg> message) const{
#ifdef _WIN32
	if(use_ansi_escape){
		show_log_message_ansi(message);
	}else{
		show_log_message_winapi(message);
	}
#else
	show_log_message_ansi(message);
#endif
}

void Process::show_log_message_ansi(std::shared_ptr<const LogMsg> message) const{
	if(message->level == ERROR){
		std::cerr << "\x1b[31m";
	}else if(message->level == WARN){
		std::cerr << "\x1b[33m";
	}
	std::cerr << message->get_output() << std::endl;
	if(message->level == ERROR || message->level == WARN){
		std::cerr << "\x1b[m";
	}
}

void Process::open_log_file() {
	if(!log_file_name.empty()) {
		if(!log_file_path.empty()) {
			vnx::Directory(log_file_path).create();
		}
		const auto file_name = log_file_path + log_file_name + vnx::get_date_string_ex(log_file_name_suffix);
		if(file_name != curr_log_file_name) {
			bool is_start = true;
			if(log_file.is_open()) {
				is_start = false;
				log_file.close();
			}
			log_file.open(file_name, std::ofstream::out | std::ofstream::app);
			if(log_file.good()) {
				log(INFO) << "Writing to log file: " << file_name;
				if(is_start) {
					log_file << std::endl << vnx::get_date_string_ex(log_file_entry_prefix) << "[@] Started " << vnx_name << std::endl << std::endl;
				}
				curr_log_file_name = file_name;
			} else {
				log(WARN) << "Failed to open log file for writing: " << file_name;
			}
		}
	}
}


} // vnx
