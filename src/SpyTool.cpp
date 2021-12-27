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
#include <vnx/SpyTool.h>
#include <vnx/PrettyPrinter.h>

#include <iostream>


namespace vnx {

SpyTool::SpyTool(const std::string& vnx_name_)
	:	SpyToolBase(vnx_name_)
{
}

void SpyTool::main() {
	if(receive_tunnel) {
		open_pipe(receive_tunnel, this, max_queue_ms);
	} else {
		subscribe(vnx::topic_info);
	}
	process = std::make_shared<ProcessClient>("vnx.process");
	if(!proxy_name.empty()) {
		const Hash64 tunnel_addr = Hash64::rand();
		proxy = std::make_shared<ProxyClient>(proxy_name);
		proxy->enable_tunnel(tunnel_addr, 100, UNLIMITED);
		proxy->enable_import("vnx.topic_info");
		process->vnx_set_tunnel(tunnel_addr);
	}
	
	update(process->get_topic_info(true));		// will block until connected
	
	Super::main();
	
	if(proxy) {
		try {
			proxy->disable_import("vnx.topic_info");
			for(const std::string& topic : topic_set) {
				proxy->disable_import(topic);
			}
		} catch(...) {
			// no problem
		}
	}
}

void SpyTool::handle(std::shared_ptr<const Sample> sample) {
	bool found = false;
	{
		auto topic = sample->topic;
		while(topic) {
			if(topic_set.count(topic->get_name())) {
				found = true;
				break;
			}
			topic = topic->get_parent();
		}
	}
	if(	found && vnx_do_run()
		// in case of a tunnel make sure the sample came from there
		&& (!receive_tunnel || (sample->stream == receive_tunnel)))
	{
		std::cout << sample->recv_time << " " << sample->seq_num << " " << sample->topic->get_name();
		if(!header_only) {
			if(pretty_print) {
				std::cout << " ";
				PrettyPrinter printer(std::cout);
				printer.max_size = size_t(max_list_size > 0 ? max_list_size : 0);
				vnx::accept(printer, sample->value);
			} else {
				std::cout << std::endl;
				vnx::write(std::cout, sample->value);
			}
		}
		std::cout << std::endl;
		if(only_once) {
			exit();
		}
	}
	Super::handle(sample);
}

void SpyTool::handle_resend(std::shared_ptr<const Sample> sample) {
	if(show_resend) {
		handle(sample);
	}
}

void SpyTool::handle(std::shared_ptr<const ::vnx::TopicInfoList> value) {
	if(value) {
		update(value->list);
	}
}

void SpyTool::update(const std::vector<TopicInfo>& info) {
	if(topic_expr.empty()) {
		add_topic("");
	} else {
		for(const TopicInfo& topic : info) {
			if(exact_match) {
				if(topic.name == topic_expr) {
					add_topic(topic.name);
				}
			} else {
				if(topic.name.find(topic_expr) != std::string::npos) {
					add_topic(topic.name);
				}
			}
		}
	}
}

void SpyTool::add_topic(const std::string& name) {
	if(!topic_set.count(name)) {
		if(!receive_tunnel) {
			subscribe(name, max_queue_ms);		// in case of a tunnel we don't need to subscribe
		}
		if(proxy) {
			proxy->enable_import(name);
		}
		topic_set.insert(name);
		log(INFO) << "Matched '" << name << "'";
	}
}


} // vnx
