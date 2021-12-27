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
#include <vnx/Subscriber.h>


namespace vnx {

Subscriber::Subscriber() {
	// nothing
}

Subscriber::~Subscriber() {
	unsubscribe_all();
}

std::shared_ptr<Pipe> Subscriber::subscribe(TopicPtr topic, int max_queue_ms, int max_queue_size,
											int priority, pipe_mode_e mode)
{
	if(topic) {
		subscription_t& subs = topic_map[topic];
		if(subs.count == 0) {
			subs.pipe = std::make_shared<Pipe>();
			connect(subs.pipe, this, max_queue_ms, max_queue_size, priority, mode);
			topic->subscribe(subs.pipe);
		}
		subs.count++;
		return subs.pipe;
	}
	return nullptr;
}

void Subscriber::unsubscribe(TopicPtr topic) {
	if(topic) {
		subscription_t& subs = topic_map[topic];
		subs.count--;
		if(subs.count == 0) {
			if(subs.pipe) {
				topic->unsubscribe(subs.pipe);
				subs.pipe->close();
			}
			topic_map.erase(topic);
		}
	}
}

void Subscriber::unsubscribe_all() {
	for(auto& entry : topic_map) {
		subscription_t& subs = entry.second;
		if(subs.pipe) {
			entry.first->unsubscribe(subs.pipe);
			subs.pipe->close();
		}
	}
	topic_map.clear();
}

std::vector<TopicPtr> Subscriber::get_topics() const {
	std::vector<TopicPtr> list;
	for(const auto& topic : topic_map) {
		list.emplace_back(topic.first);
	}
	return list;
}

void Subscriber::close() {
	unsubscribe_all();
	Node::close();
}


} // vnx
