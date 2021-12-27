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
#include <vnx/Topic.h>
#include <vnx/TopicPtr.hpp>
#include <vnx/Sample.hxx>

#include <algorithm>


namespace vnx {

const std::string TopicPtr::empty = "null";

Topic::Topic(const std::string& topic_name_)
	:	topic_name(topic_name_)
{
	const size_t last_dot = topic_name.find_last_of('.');
	if(last_dot != std::string::npos) {
		parent = get_topic(topic_name.substr(0, last_dot));
	} else if(!topic_name.empty()) {
		parent = get_topic("");
	}
	topic_hash = Hash64(topic_name);
	info.name = topic_name;
}

Topic::~Topic() {
	close();
}

std::string Topic::get_domain_name() const {
	if(parent) {
		return parent->get_name();
	}
	return "";
}

std::string Topic::get_topic_name() const {
	if(parent) {
		return topic_name.substr(parent->get_name().size() + 1);
	}
	return topic_name;
}

TopicInfo Topic::get_info() const {
	std::lock_guard<std::mutex> lock(mutex);
	return info;
}

std::shared_ptr<Topic> Topic::get_parent() const {
	std::lock_guard<std::mutex> lock(mutex);
	return parent;
}

std::vector<std::shared_ptr<Topic>> Topic::get_children() const {
	std::lock_guard<std::mutex> lock(mutex);
	std::vector<std::shared_ptr<Topic>> res;
	for(const auto& entry : children) {
		res.push_back(entry.second);
	}
	return res;
}

void Topic::add_child(std::shared_ptr<Topic> child) {
	if(child->get_parent() && child->get_parent().get() != this) {
		throw std::logic_error("add_child(): parent mismatch");
	}
	std::lock_guard<std::mutex> lock(mutex);
	if(!is_closed) {
		children[child->topic_hash] = child;
		info.is_domain = true;
	}
}

void Topic::remove_child(Topic* child) {
	std::lock_guard<std::mutex> lock(mutex);
	children.erase(child->topic_hash);
	info.is_domain = !children.empty();
}

std::shared_ptr<Topic> Topic::remap(const std::string& new_domain) const {
	if(new_domain.empty()) {
		return get_topic(get_topic_name());
	}
	return get_topic(new_domain + "." + get_topic_name());
}

void Topic::close() {
	std::unordered_map<Hash64, std::shared_ptr<Topic>> children_;
	{
		std::lock_guard<std::mutex> lock(mutex);
		children_ = children;
		is_closed = true;
	}
	if(parent) {
		parent->remove_child(this);
	}
	for(const auto& entry : children_) {
		entry.second->close();
	}
}

void Topic::subscribe(std::shared_ptr<Pipe> pipe) {
	std::lock_guard<std::mutex> lock(mutex);
	output.push_back(pipe);
	info.num_subscribers++;
}

void Topic::unsubscribe(std::shared_ptr<Pipe> pipe) {
	std::lock_guard<std::mutex> lock(mutex);
	auto iter = std::find(output.begin(), output.end(), pipe);
	if(iter != output.end()) {
		output.erase(iter);
		info.num_subscribers--;
	}
}

void Topic::publish(std::shared_ptr<Sample> sample) {
	if(!sample->topic || sample->topic->topic_hash != topic_hash) {
		sample->topic = topic_name;
	}
	sample->recv_time = get_sync_time_micros();
	broadcast(sample);
}

void Topic::broadcast(std::shared_ptr<const Message> msg) {
	std::vector<std::shared_ptr<Pipe>> output_;
	output_.reserve(16);
	{
		std::lock_guard<std::mutex> lock(mutex);
		output_.insert(output_.end(), output.begin(), output.end());
		info.sample_count++;
	}
	for(const auto& pipe : output_) {
		send_msg(pipe, msg);
	}
	if(parent) {
		parent->broadcast(msg);
	}
}


class GlobalTopicMap {
public:
	GlobalTopicMap() {
		root = get_topic("");
	}
	
	std::shared_ptr<Topic> get_topic(const std::string& topic_name) {
		std::lock_guard<std::recursive_mutex> lock(mutex);
		std::weak_ptr<Topic>& p_weak = topic_map[topic_name];
		std::shared_ptr<Topic> p_topic = p_weak.lock();
		if(p_topic) {
			return p_topic;
		}
		p_topic = std::make_shared<Topic>(topic_name);
		p_weak = p_topic;
		std::shared_ptr<Topic> p_parent = p_topic->get_parent();
		if(p_parent) {
			p_parent->add_child(p_topic);
		} else if(topic_name != "") {
			root->add_child(p_topic);
		}
		return p_topic;
	}
	
private:
	std::recursive_mutex mutex;
	std::shared_ptr<Topic> root;
	std::unordered_map<std::string, std::weak_ptr<Topic>> topic_map;
	
};

static GlobalTopicMap& get_global_topic_map() {
	static GlobalTopicMap instance;
	return instance;
}

std::shared_ptr<Topic> get_topic(const std::string& topic_name) {
	return get_global_topic_map().get_topic(topic_name);
}

std::shared_ptr<Topic> get_topic(const std::string& domain, const std::string& topic) {
	std::string topic_name = domain;
	if(!topic.empty()) {
		topic_name += "." + topic;
	}
	return get_topic(topic_name);
}

static void get_all_topics(std::shared_ptr<Topic> topic, bool include_domains, std::vector<std::shared_ptr<Topic>>& all) {
	const auto children = topic->get_children();
	if(children.empty() || include_domains) {
		all.push_back(topic);
	}
	for(std::shared_ptr<Topic> child : children) {
		get_all_topics(child, include_domains, all);
	}
}

std::vector<std::shared_ptr<Topic>> get_all_topics(std::shared_ptr<Topic> domain, bool include_domains) {
	std::vector<std::shared_ptr<Topic>> all;
	get_all_topics(domain, include_domains, all);
	return all;
}

std::vector<std::shared_ptr<Topic>> get_all_topics(bool include_domains) {
	return get_all_topics(get_topic(""), include_domains);
}

std::shared_ptr<Topic> remap_topic(	std::shared_ptr<Topic> topic,
									const std::map<TopicPtr, TopicPtr>& map)
{
	std::string suffix;
	TopicPtr prefix = topic;
	while(prefix) {
		const auto iter = map.find(prefix);
		if(iter != map.end()) {
			if(suffix.empty()) {
				return iter->second;	// direct topic mapping
			} else {
				return get_topic(iter->second->get_name() + "." + suffix);		// domain mapping
			}
		}
		suffix = prefix->get_topic_name() + (suffix.empty() ? "" : "." + suffix);
		prefix = prefix->get_parent();
	}
	return topic;
}


} // vnx
