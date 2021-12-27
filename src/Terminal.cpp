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
#include <vnx/Terminal.h>
#include <vnx/SpyTool.h>

#include <iostream>
#include <algorithm>


namespace vnx {



template <>
void Terminal::output_value(std::ostream &out, const std::string &value) const{
	out << value;
}


template <>
void Terminal::output_value(std::ostream &out, const std::pair<const vnx::Hash64, std::shared_ptr<const vnx::Endpoint>> &value) const{
	if(value.second){
		out << value.second->to_url();
	}else{
		out << "(null)";
	}
}



Terminal::Terminal(const std::string& vnx_name_)
	:	TerminalBase(vnx_name_),
		process("vnx.process")
{
	service_addr = Hash64::rand();
#ifdef _WIN32
	read_config("use_ansi_escape", use_ansi_escape);
#endif
}

void Terminal::init() {
	open_pipe(vnx_name, this, UNLIMITED);
	subscribe(vnx::module_info);
}

void Terminal::main() {
	open_pipe(service_addr, this, UNLIMITED);
	
	set_terminal_mode();
	std::atexit(reset_terminal_mode);

	input.add_command("quit");
	input.add_command("debug");
	input.add_command("errors");
	input.add_command("topic");
	input.add_command("grep");
	input.add_command("spy");
	input.add_command("dump");
	input.add_command("module");
	input.add_command("htop");
	input.add_command("journal");
	input.add_command("config");
	input.add_command("exec");
	input.add_command("call");
	for(int i=ERROR; i<=DEBUG; i++){
		input.add_argument("debug", std::to_string(i));
	}
	input.add_argument("htop", "cur");
	input.add_argument("htop", "avg");
	set_timer_millis(1500, std::bind(&Terminal::update_hints, this));

	std::thread reader(&Terminal::read_loop, service_addr);
	reader.detach();
	
	Super::main();
}

void Terminal::read_loop(Hash64 service_addr) noexcept {
	try {
		read_loop_impl(service_addr);
	} catch(...) {
		// ignore
	}
}

void Terminal::command(const std::vector<std::string> &commandline) {
	if(state == INACTIVE) {
		process.pause_log();
		std::cerr << "Help: quit | debug [level] | errors | topic [expr] | module [expr] | exec/call <module>[.method [args...]] | config [expr] | grep <expr> | journal [expr] | spy [expr] | dump [expr] | htop [field]" << std::endl;
		std::cerr.flush();
		state = INPUT;
	} else if(state == INPUT) {
		auto cmditer = commandline.begin();
		std::string cmd = "";
		if(cmditer != commandline.end()) cmd = *cmditer++;

		if(cmd.empty()){
			state = INACTIVE;
			process.resume_log();
		}else if(cmd == "quit" || cmd == "q") {
			state = INACTIVE;
			process.resume_log();
			vnx::trigger_shutdown();
		} else if(cmd == "debug") {
			int level = DEBUG;
			if(cmditer != commandline.end()) {
				try {
					level = std::stoi(*cmditer++);
				} catch(...) {}
			}
			process.set_debug(level);
			state = INACTIVE;
			process.resume_log();
			log(INFO) << "process.set_debug(" << level << ")";
		} else if(cmd == "grep" || cmd == "g") {
			std::string arg = "";
			if(cmditer != commandline.end()) arg = *cmditer++;
			process.grep_log(arg);
			process.resume_log();
			state = GREP_RUN;
		} else if(cmd == "dump" || cmd == "d") {
			std::string arg = "";
			if(cmditer != commandline.end()) arg = *cmditer++;
			dump(arg);
			state = MODULE_RUN;
		} else if(cmd.find("spy") == 0 || cmd == "s") {
			std::string arg;
			if(cmditer != commandline.end()) arg = *cmditer++;
			spy(arg);
			state = MODULE_RUN;
		} else if(cmd == "topic" || cmd == "t") {
			if(cmditer == commandline.end()){
				topic_info("");
			}else{
				while(cmditer != commandline.end()){
					topic_info(*cmditer++);
					std::cerr << std::endl;
				}
			}
			state = DISPLAY;
		} else if(cmd == "errors" || cmd == "e") {
			process.show_error_log();
			state = DISPLAY;
		} else if(cmd == "module" || cmd == "m") {
			if(cmditer == commandline.end()){
				module_info("");
			}else{
				while(cmditer != commandline.end()){
					module_info(*cmditer++);
					std::cerr << std::endl;
				}
			}
			state = DISPLAY;
		} else if(cmd == "config"){
			std::string expr = "";
			if(cmditer != commandline.end()){
				expr = *cmditer++;
			}
			show_config(expr);
			state = DISPLAY;
		} else if(cmd == "htop"){
			bool order_by_avg = false;
			if(cmditer != commandline.end() && *cmditer == "avg"){
				order_by_avg = true;
			}
			htop(order_by_avg);
			interval = set_timer_millis(htop_interval, std::bind(&Terminal::htop, this, order_by_avg));
			state = HTOP_RUN;
		} else if(cmd == "journal"){
			std::string pattern = "";
			if(cmditer != commandline.end()) pattern = *cmditer++;
			process.journal_log(pattern);
			process.resume_log();
			state = GREP_RUN;
		} else if(cmd == "exec" || cmd == "call"){
			if(cmditer == commandline.end()){
				std::cerr << "Error: missing module name" << std::endl;
				state = DISPLAY;
			}else{
				const std::string &module_method = *cmditer;
				cmditer++;
				std::string module = module_method;
				std::string method = "";
				auto dot = module_method.find(".");
				if(dot != std::string::npos){
					module = module_method.substr(0, dot);
					method = module_method.substr(dot+1);
				}
				std::vector<std::string> args;
				while(cmditer != commandline.end()){
					args.push_back(*cmditer);
					cmditer++;
				}
				state = exec(module, method, args, cmd=="call");
			}
		} else {
			state = INACTIVE;
			process.resume_log();
			log(WARN) << "Invalid command: " << cmd;
		}
	} else if(state == GREP_RUN) {
		state = INACTIVE;
		process.ungrep_log();
	} else if(state == MODULE_RUN) {
		module.close();
		state = INACTIVE;
		process.resume_log();
	}else if(state == HTOP_RUN){
		interval->stop();
		state = INACTIVE;
		process.resume_log();
	} else if(state == EXEC_RUN){
		rem_async_client(exec_client);
		exec_client = nullptr;
		state = INACTIVE;
		process.resume_log();
	} else if(state == DISPLAY) {
		state = INACTIVE;
		process.resume_log();
	}
}

void Terminal::read_char(const signed char &c){
	if(state != INPUT) return;

	line.insert(cursor, 1, c);
	cursor++;
	tab_once = false;
	get_completion();

	write_editline(std::cerr);
}

void Terminal::read_event(const terminal_event_e &event){
	std::ostream &out = std::cerr;

	if(state != INPUT && event != terminal_event_e::KEY_LF && event != terminal_event_e::KEY_CR) return;

	switch(event){
	case terminal_event_e::KEY_ARROWUP:
		line = input.history_up();
		cursor = line.length();
		break;
	case terminal_event_e::KEY_ARROWDOWN:
		line = input.history_down();
		cursor = line.length();
		break;
	case terminal_event_e::KEY_ARROWLEFT:
		if(cursor > 0) cursor--;
		break;
	case terminal_event_e::KEY_ARROWRIGHT:
		if(cursor < line.length()) cursor++;
		break;
	case terminal_event_e::KEY_CR:
	case terminal_event_e::KEY_LF:
		if(!line.empty()) input.add_history(line);
		out << std::endl;

		{
			size_t comment = line.find('#');
			if(comment != std::string::npos) line = line.substr(0, comment);
		}
		try{
			command(vnx::string_split(line, ' ', true));
		} catch(...) {
			// oh, well ...
		}

		line = "";
		cursor = 0;
		break;
	case terminal_event_e::KEY_BACKSPACE:
		if(cursor > 0){
			cursor--;
			line.erase(cursor, 1);
		}
		break;
	case terminal_event_e::KEY_DEL:
		if(cursor < line.length()){
			line.erase(cursor, 1);
		}
		break;
	case terminal_event_e::KEY_POS1:
		cursor = 0;
		break;
	case terminal_event_e::KEY_END:
		cursor = line.length();
		break;
	case terminal_event_e::KEY_TAB:{
		std::string query = line.substr(0, cursor);
		size_t firstspace = query.find(' ');
		size_t lastspace = query.rfind(' ');
		if(firstspace == std::string::npos){
			// complete a command
			if(tab_once){
				std::vector<std::string> guesses = input.guess_command(query);
				out << std::endl;
				for(const std::string &g : guesses){
					out << g << std::endl;
				}
				out << std::endl;
			}else{
				if(completion.first.empty() && !completion.second){
					tab_once = true;
				}else{
					line.insert(cursor, completion.first);
					cursor += completion.first.length();
					if(completion.second){
						line.insert(cursor, " ");
						cursor++;
					}
				}
			}
		}else{
			// complete an argument
			std::string word = query.substr(lastspace+1);
			std::string command = query.substr(0, firstspace);
			if(tab_once){
				std::vector<std::string> guesses = input.guess_argument(command, word);
				out << std::endl;
				for(const std::string &g : guesses){
					out << g << std::endl;
				}
				out << std::endl;
			}else{
				if(completion.first.empty() && !completion.second){
					tab_once = true;
				}else{
					line.insert(cursor, completion.first);
					cursor += completion.first.length();
					if(completion.second){
						line.insert(cursor, " ");
						cursor++;
					}
				}
			}
		}

		break;
	}
	}

	get_completion();
	if(event != terminal_event_e::KEY_TAB) tab_once = false;
	if(state == INPUT) write_editline(out);
}

void Terminal::get_completion(){
	if(cursor < line.length() && line[cursor] != ' '){
		// don't autocomplete inside words
		completion = std::make_pair("", false);
		return;
	}

	std::string query = line.substr(0, cursor);
	size_t firstspace = query.find(' ');
	size_t lastspace = query.rfind(' ');
	if(firstspace == std::string::npos){
		// complete a command
		completion = input.complete_command(query);
	}else{
		// complete an argument
		std::string word = query.substr(lastspace+1);
		std::string command = query.substr(0, firstspace);
		completion = input.complete_argument(command, word);
	}
}

void Terminal::write_editline(std::ostream &out) const{
#ifdef _WIN32
	if(use_ansi_escape){
		write_editline_ansi(out);
	}else{
		write_editline_winapi(out);
	}
#else
	write_editline_ansi(out);
#endif
}

void Terminal::write_editline_ansi(std::ostream &out) const{
	// start of the line
	out << "\r";
	// display prompt and input part 1
	out << prompt << line.substr(0, cursor);
	// display current suggestion
	out << "\x1b[35m" << completion.first << "\x1b[m";
	// display input part 2
	out << line.substr(cursor);
	// erase the rest of the line
	out << "\x1b[0K";
	// place the cursor
	out << "\r\x1b[" << (prompt.length()+cursor) << "C";
}

void Terminal::spy(const std::string& expr) {
	SpyTool* tool = new SpyTool("SpyTool");
	tool->topic_expr = expr;
	tool->header_only = true;
	module = tool;
	module.start();
}

void Terminal::dump(const std::string& expr) {
	SpyTool* tool = new SpyTool("SpyTool");
	tool->topic_expr = expr;
	tool->max_list_size = max_list_size;
	module = tool;
	module.start();
}

void Terminal::topic_info(const std::string& expr) {
	const std::vector<std::shared_ptr<Topic>> all_topics = get_all_topics();

	std::shared_ptr<const Topic> exact_match;
	std::vector<std::shared_ptr<const Topic>> prefix_matches;
	std::vector<std::shared_ptr<const Topic>> any_matches;

	for(std::shared_ptr<const Topic> topic : all_topics) {
		const std::string topic_name = topic->get_name();
		const size_t pos = topic_name.find(expr);
		if(pos == 0 && topic_name.length() == expr.length()){
			exact_match = topic;
			break;
		}else if(pos == 0){
			prefix_matches.push_back(topic);
		}else if(pos != std::string::npos){
			any_matches.push_back(topic);
		}
	}

	std::vector<std::shared_ptr<const Topic>> results;
	if(exact_match){
		results = {exact_match};
	}else if(!prefix_matches.empty()){
		results = std::move(prefix_matches);
	}else if(!any_matches.empty()){
		results = std::move(any_matches);
	}else{
		std::cerr << "No topics found!" << std::endl;
	}

	std::sort(
		results.begin(),
		results.end(),
		[](std::shared_ptr<const Topic> a, std::shared_ptr<const Topic> b){
			return a->get_name() < b->get_name();
		}
	);
	size_t max_name_length = 0;
	for(auto topic : results) {
		max_name_length = std::max(max_name_length, topic->get_name().length());
	}
	for(auto topic : results){
		TopicInfo info = topic->get_info();
		std::cerr << info.name << std::string(max_name_length-info.name.length(), ' ');
		std::cerr << " : " << info.num_subscribers << " subs, " << info.sample_count << " samples" << std::endl;
	}
}


void Terminal::module_info(const std::string &expr){
	auto mod_info = process.get_module_info();
	std::sort(mod_info.begin(), mod_info.end(), [](const ModuleInfo &lhs, const ModuleInfo &rhs){ return lhs.name < rhs.name; });

	std::vector<const ModuleInfo*> exact_matches;
	for(const auto &entry : mod_info){
		if(entry.name == expr) exact_matches.push_back(&entry);
	}

	if(exact_matches.empty()){
		// no exact match, list all approximate matches
		bool hit = false;
		size_t max_name_length = 0;
		for(const auto &entry : mod_info){
			max_name_length = std::max(max_name_length, entry.name.length());
		}
		for(const auto &entry : mod_info){
			std::string name = entry.name;
			if(name.find(expr) == std::string::npos) continue;
			hit = true;
			std::cerr << name << std::string(max_name_length-name.size(), ' ') << " : ";
			std::cerr << entry.type;
			std::cerr << std::endl;
		}
		if(!hit) std::cerr << "No modules found!" << std::endl;
	}else{
		for(const ModuleInfo* mod : exact_matches){
			std::cerr << "===== Module " << mod->name << " =====" << std::endl;
			std::cerr << "id            : " << mod->id << std::endl;
			std::cerr << "type          : " << mod->type << std::endl;
			std::cerr << "CPU load      : "
					<< int(100 * mod->get_cpu_load()) << " % current, "
					<< int(100 * mod->get_cpu_load_total()) << " % average" << std::endl;
			std::cerr << "Async pending : " << mod->num_async_pending << std::endl;
			std::cerr << "Async process : " << mod->num_async_process << std::endl;
			std::cerr << "Subscriptions : ";
			output_list(std::cerr, mod->sub_topics.begin(), mod->sub_topics.end(), "None");
			std::cerr << std::endl;
			std::cerr << "Publishing    : ";
			output_list(std::cerr, mod->pub_topics.begin(), mod->pub_topics.end(), "None");
			std::cerr << std::endl;
			std::cerr << "Remotes       : ";
			output_list(std::cerr, mod->remotes.begin(), mod->remotes.end(), "None");
			std::cerr << std::endl;
		}
	}
}


void Terminal::show_config(const std::string &expr){
	Variant val_result = get_config(expr);
	if(!val_result.empty()){
		std::cerr << expr << ": " << val_result << std::endl;
	}else{
		std::string prefix = expr;
		std::string last_prefix = "";
		Object obj_result = get_config_object(prefix);
		if(obj_result.field.empty()){
			size_t last_dot = expr.rfind('.');
			if(last_dot != std::string::npos){
				prefix = expr.substr(0, last_dot);
				last_prefix = expr.substr(last_dot+1);
			}else{
				prefix = "";
				last_prefix = expr;
			}
			obj_result = get_config_object(prefix);
		}

		size_t longest = 0;
		for(const auto &entry : obj_result.field){
			longest = std::max(longest, entry.first.length());
		}
		if(obj_result.field.empty()){
			std::cerr << "No results!" << std::endl;
		}
		for(const auto &entry : obj_result.field){
			if(entry.first.find(last_prefix) == 0){
				std::cerr << prefix;
				if(!prefix.empty() && !entry.first.empty()){
					std::cerr << ".";
				}
				std::cerr << entry.first;
				std::cerr << ": " << std::string(longest-entry.first.length(), ' ') << entry.second << std::endl;
			}
		}
	}
}


void Terminal::htop(const bool &order_by_avg){
	auto mod_info = process.get_module_info();
	if(order_by_avg){
		std::sort(mod_info.begin(), mod_info.end(), [](const ModuleInfo &a, const ModuleInfo &b){
			return a.get_cpu_load_total() > b.get_cpu_load_total();
		});
	}else{
		std::sort(mod_info.begin(), mod_info.end(), [](const ModuleInfo &a, const ModuleInfo &b){
			return a.get_cpu_load() > b.get_cpu_load();
		});
	}

	size_t max_name = 6;
	for(const auto &entry : mod_info){
		max_name = std::max(max_name, entry.name.length());
	}
	max_name++;

	std::string header = "Module" + std::string(max_name-6, ' ') + "current (%)   avg (%)";
	std::cerr << header << std::endl;
	std::cerr << std::string(header.length(), '-') << std::endl;
	size_t shown = 0;
	for(const auto &entry : mod_info){
		int current = 100 * entry.get_cpu_load();
		int avg = 100 * entry.get_cpu_load_total();
		if((order_by_avg && avg == 0) || (!order_by_avg && current == 0)) break;
		size_t current_indent = current >= 100 ? 4 : (current >= 10 ? 5 : 6);
		size_t avg_indent = avg >= 100 ? 1 : (avg >= 10 ? 2 : 3);
		std::cerr << entry.name << std::string(max_name-entry.name.length()+current_indent, ' ') << current << "        " << std::string(avg_indent, ' ') << avg << std::endl;
		shown++;
	}
	size_t not_shown = mod_info.size() - shown;
	if(not_shown > 0) std::cerr << "(+" << not_shown << " module" << (not_shown == 1 ? "" : "s") << " with zero load)" << std::endl;
}


int Terminal::exec(const std::string &module, const std::string &method, const std::vector<std::string> &args, const bool &async){
	GenericClient client(module);

	TypeCode type_code;
	try{
		type_code = client.vnx_get_type_code();
	}catch(const std::exception &err){
		std::cerr << "Error: " << err.what() << std::endl;
		std::cerr << "Please make sure this module exists." << std::endl;
		return DISPLAY;
	}

	const TypeCode *method_type = nullptr;
	for(auto func : type_code.methods) {
		const auto method_name = func->get_method_name();
		if(!method_type && (func->name == method || method_name == method)) {
			method_type = func;
		}
	}
	if(!method_type){
		std::cerr << "Error: method name not found" << std::endl;
		std::cerr << "Did you mean one of these?" << std::endl;
		std::cerr << std::endl;
		for(auto func : type_code.methods) {
			func->print(std::cerr);
		}
		return DISPLAY;
	}


	Object args_obj;
	for(size_t i = 0; i < args.size(); ++i) {
		if(i < method_type->fields.size()) {
			Variant arg;
			vnx::from_string_value(args[i], arg);
			args_obj[method_type->fields[i].name] = arg;
		}
	}
	try{
		if(async){
			process.resume_log();
			client.call_async(method, args_obj);
			return INACTIVE;
		}else{
			exec_client = std::make_shared<GenericAsyncClient>(module);
			add_async_client(exec_client);
			exec_client->call(
				method,
				args_obj,
				std::bind(&Terminal::exec_return, this, std::placeholders::_1),
				std::bind(&Terminal::exec_err, this, std::placeholders::_1)
			);
			std::cerr << "Call sent, waiting for return ..." << std::endl;
			return EXEC_RUN;
		}
	}catch(const std::exception &err){
		std::cerr << "Error: " << err.what() << std::endl;
		return DISPLAY;
	}
}


void Terminal::exec_return(std::shared_ptr<const Value> result){
	if(!result){
		std::cerr << "(No result)" << std::endl;
	} else if(result->is_void()) {
		std::cerr << "(void)" << std::endl;
	} else{
		vnx::PrettyPrinter printer(std::cerr);
		const auto ret = result->get_field_by_index(0);
		vnx::accept(printer, ret);
		std::cerr << std::endl;
	}
	rem_async_client(exec_client);
	exec_client = nullptr;
	state = DISPLAY;
}


void Terminal::exec_err(const exception &err){
	std::cerr << "Error: " << err.what() << std::endl;
	rem_async_client(exec_client);
	exec_client = nullptr;
	state = DISPLAY;
}


void Terminal::update_hints(){
	for(std::shared_ptr<Topic> topic : vnx::get_all_topics(true)) {
		const std::string name = topic->get_name();
		input.add_argument("dump", name);
		input.add_argument("d", name);
		input.add_argument("topic", name);
		input.add_argument("t", name);
		input.add_argument("spy", name);
		input.add_argument("s", name);
	}

	input.clear_arguments("grep");
	input.clear_arguments("g");
	input.clear_arguments("module");
	input.clear_arguments("m");
	input.clear_arguments("journal");
	input.clear_arguments("exec");
	input.clear_arguments("call");

	for(const auto &p : process.get_module_info()){
		input.add_argument("grep", p.name);
		input.add_argument("g", p.name);
		input.add_argument("module", p.name);
		input.add_argument("m", p.name);
		input.add_argument("journal", p.name);
		input.add_argument("exec", p.name);
		input.add_argument("call", p.name);
		for(auto func : p.type_code.methods) {
			const auto method_name = func->get_method_name();
			input.add_argument("exec", p.name+"."+method_name);
			input.add_argument("call", p.name+"."+method_name);
		}
	}

	input.clear_arguments("config");
	for(const auto &c : vnx::get_all_configs()){
		input.add_argument("config", c.first);
	}
}


} // vnx
