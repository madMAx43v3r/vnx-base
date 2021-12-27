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
#include <vnx/Publisher.h>
#include <vnx/LogMsg.hxx>
#include <vnx/FlowMessage.hxx>


namespace vnx {

Publisher::Publisher()
	:	src_mac(Hash64::rand())
{
}

Publisher::~Publisher() {
	close();
}

void Publisher::publish(const Value& value, TopicPtr topic, uint16_t flags) {
	if(topic) {
		publish(value.clone(), topic, flags);
	}
}

void Publisher::publish(std::shared_ptr<const Value> value, TopicPtr topic, uint16_t flags) {
	if(topic) {
		std::shared_ptr<Sample> sample = Sample::create();
		sample->src_mac = src_mac;
		bool is_new_channel = false;
		{
			std::lock_guard<std::mutex> lock(mutex);
			if(!is_open_) {
				return;
			}
			auto& seq_num = topic_map[topic];
			is_new_channel = !seq_num;
			if(!(flags & Message::RESEND) || is_new_channel) {
				seq_num++;
			}
			sample->seq_num = seq_num;
		}
		if(is_new_channel) {
			// send opening flow message first
			auto msg = FlowMessage::create();
			msg->src_mac = src_mac;
			msg->dst_mac = topic->get_hash();
			msg->flow_code = FlowMessage::OPEN;
			topic->broadcast(msg);
		}
		sample->flags = flags;
		sample->topic = topic;
		sample->value = value;
		topic->publish(sample);
	}
}

std::vector<TopicPtr> Publisher::get_topics() const {
	std::lock_guard<std::mutex> lock(mutex);

	std::vector<TopicPtr> list;
	for(const auto& topic : topic_map) {
		list.emplace_back(topic.first);
	}
	return list;
}

bool Publisher::is_open() const {
	return is_open_;
}

void Publisher::close() {
	std::lock_guard<std::mutex> lock(mutex);

	// send closing flow messages
	for(const auto& entry : topic_map) {
		auto topic = entry.first;
		auto msg = FlowMessage::create();
		msg->src_mac = src_mac;
		msg->dst_mac = topic->get_hash();
		msg->flow_code = FlowMessage::CLOSE;
		topic->broadcast(msg);
	}
	topic_map.clear();
	is_open_ = false;
}


LogPublisher::LogPublisher(const LogPublisher& other)
	:	out(*this), publisher(other.publisher), module(other.module),
		level(other.level), display_level(other.display_level)
{
	out.str(other.str());
}

LogPublisher::LogPublisher(std::shared_ptr<Publisher> publisher, const std::string& module, int level, int display_level)
	:	out(*this), publisher(publisher), module(module), level(level), display_level(display_level)
{
}

LogPublisher::~LogPublisher() {
	const auto message = out.str();
	if(!message.empty()) {
		std::shared_ptr<LogMsg> value = LogMsg::create();
		value->time = vnx::get_sync_time_micros();
		value->process = vnx::get_process_name();
		value->module = module;
		value->level = level;
		value->display_level = display_level;
		value->message = message;
		if(publisher) {
			publisher->publish(value, vnx::log_out);
		}
	}
}

LogPublisher log_error() {
	static std::shared_ptr<Publisher> publisher = std::make_shared<Publisher>();
	return LogPublisher(publisher, vnx::get_process_name(), LogMsg::ERROR, LogMsg::INFO);
}

LogPublisher log_warn() {
	static std::shared_ptr<Publisher> publisher = std::make_shared<Publisher>();
	return LogPublisher(publisher, vnx::get_process_name(), LogMsg::WARN, LogMsg::INFO);
}

LogPublisher log_info() {
	static std::shared_ptr<Publisher> publisher = std::make_shared<Publisher>();
	return LogPublisher(publisher, vnx::get_process_name(), LogMsg::INFO, LogMsg::INFO);
}

LogPublisher log_debug() {
	static std::shared_ptr<Publisher> publisher = std::make_shared<Publisher>();
	return LogPublisher(publisher, vnx::get_process_name(), LogMsg::DEBUG, LogMsg::INFO);
}


} // vnx
