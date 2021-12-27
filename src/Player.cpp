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
#include <vnx/Player.h>


namespace vnx {

Player::Player(const std::string& _vnx_name)
	:	PlayerBase(_vnx_name)
{
	topic_map[vnx::log_out] = "org.vnx.log_out";
	topic_map[vnx::shutdown] = "org.vnx.shutdown";
	topic_map[vnx::topic_info] = "org.vnx.topic_info";
	topic_map[vnx::module_info] = "org.vnx.module_info";
	topic_map[vnx::time_control] = "org.vnx.time_control";
	topic_map[vnx::time_sync] = "org.vnx.time_sync";
}

void Player::init() {
	vnx_virtual_time = false;					// we need real time
	open_pipe(vnx_name, this, UNLIMITED);		// make us available as a service
}

void Player::main() {
	timer = add_timer(std::bind(&Player::process, this));
	set_timer_millis(interval_ms, std::bind(&Player::update, this));
	set_timer_millis(1000, std::bind(&Player::update_stats, this, false));
	
	open();
	if(is_autostart) {
		play();
	}
	
	Super::main();
	
	if(reader) {
		reader->close();
	}
}

void Player::open() {
	for(const std::string& filename : files) {
		log(INFO) << "open('" << filename << "')";
	}
	
	reader = std::make_shared<MultiRecordReader>(files);
	sample = reader->get_sample();
	header = reader->get_header();
	
	control = TimeControl();
	control.is_realtime = false;
	control.is_paused = true;
	control.time_speed = play_speed;
	
	status = PlayerStatus();
	status.filename = header->filename;
	status.begin_time = header->begin_time;
	status.end_time = header->end_time;
	status.current_time = reader->get_time_micros();
	status.info = *header;
	
	log(INFO) << "Start    : " << get_date_string_ex("%Y-%m-%d %H:%M:%S %Z", false, header->begin_time/1000000) << " (" << header->begin_time/1000000 << " UTC)";
	log(INFO) << "End      : " << get_date_string_ex("%Y-%m-%d %H:%M:%S %Z", false, header->end_time/1000000) << " (" << header->end_time/1000000 << " UTC)";
	log(INFO) << "Duration : " << get_date_string_ex("%H:%M:%S", true, (header->end_time - header->begin_time)/1000000) << " (HH:MM:SS)";
	log(INFO) << "Total    : " << header->num_samples << " samples, " << header->num_samples_lost << " lost, " << (header->num_bytes/1024/1024) << " MB";
	for(const auto& topic : header->topics) {
		log(INFO) << "'" << topic.name << "': " << topic.num_samples << " samples, " << topic.num_samples_lost
				<< " lost (" << (100*topic.num_samples_lost/(topic.num_samples_lost+topic.num_samples)) << " %), "
				<< (topic.num_bytes/1024/1024) << " MB";
	}
	
	trigger();
	update();
}

RecordHeader Player::get_info() const {
	return *header;
}

void Player::play() {
	log(DEBUG) << "play()";
	control.is_paused = false;
	trigger();
	update();
	process();
}

void Player::pause() {
	log(DEBUG) << "pause()";
	control.is_paused = true;
	trigger();
	update();
	update_stats();
}

void Player::toggle() {
	if(control.is_paused) {
		play();
	} else {
		pause();
	}
}

void Player::stop() {
	log(DEBUG) << "stop()";
	reader->seek_to_position(-1);
	sample = reader->get_sample();
	control.is_paused = true;
	trigger();
	update();
	update_stats();
}

void Player::seek_by_count(const int64_t& delta_count) {
	log(DEBUG) << "seek_by_count(" << delta_count << ")";
	reader->seek_by_count(delta_count);
	sample = reader->get_sample();
	trigger();
	update();
	process();
	update_stats();
}

void Player::seek_by_time(const int64_t& delta_us) {
	const int64_t delta_virtual = delta_us * control.time_speed;
	log(DEBUG) << "seek_by_time(" << delta_virtual << ")";
	reader->seek_by_time(delta_virtual);
	sample = reader->get_sample();
	trigger();
	update();
	process();
	update_stats(control.is_paused);
}

void Player::seek_to_position(const float64_t& position) {
	log(DEBUG) << "seek_to_position(" << position << ")";
	reader->seek_to_position(position);
	sample = reader->get_sample();
	trigger();
	update();
	process();
	update_stats(control.is_paused);
}

void Player::seek_to_time(const int64_t& time_us) {
	log(DEBUG) << "seek_to_time(" << time_us << ")";
	reader->seek_to_time(time_us);
	sample = reader->get_sample();
	trigger();
	update();
	process();
	update_stats(control.is_paused);
}

void Player::set_speed(const float64_t& speed) {
	log(DEBUG) << "set_speed(" << speed << ")";
	control.time_speed = speed;
	trigger();
	update();
	process();
	update_stats(control.is_paused);
}

void Player::process() {
	// limit the maximum time we spend here so we don't get stuck in case of maxed out CPU or disc
	const int64_t time_start = get_wall_time_micros();
	while(vnx_do_run() && get_wall_time_micros() - time_start < interval_ms * 1000) {
		
		TopicPtr topic = sample->topic;
		{
			auto iter = topic_map.find(topic);
			if(iter != topic_map.end()) {
				topic = iter->second;		// redirect to new topic
			}
		}
		
		// publish sample
		// TODO: check for duplicate samples from multiple recordings
		publish(sample->value, topic, Message::REPLAY | (is_blocking ? Message::BLOCKING : 0));
		sample_count++;
		
		// try to read the next sample
		if(reader->read_next()) {
			sample = reader->get_sample();
			// compute the sleep time until the next sample should be published
			timer->interval = int64_t((reader->get_time_micros() - control.get_time_micros()) / control.time_speed);
			if(timer->interval <= 0) {
				// in this case we don't sleep at all and continue straight away (for better performance)
				continue;
			}
		} else if(!control.is_paused) {
			// we are at the end of the recording, so we stop the player
			stop();
			if(is_repeat) {
				play();		// start over from the beginning
			} else if(is_autoshutdown) {
				log(INFO) << "Triggering shutdown ...";
				vnx::trigger_shutdown();	// shut down the process
			} else if(is_autoclose) {
				log(INFO) << "Closing ...";
				exit();		// shut down the player
			}
			return;
		}
		// in this case we need to go back to sleep
		break;
	}
	// reset the timer so we get called again when it is time to process the next sample
	if(!control.is_paused) {
		if(timer->interval / 1000 > max_time_gap_ms) {
			timer->interval = 0;		// we jump the gap
			trigger();					// reset time offset
		}
		timer->reset(get_sync_time_micros());		// important: we are running on sync time, not virtual time!
	}
}

void Player::trigger() {
	const int64_t trigger_time = get_sync_time_micros();
	control.time_start = trigger_time;
	control.time_offset = reader->get_time_micros() - trigger_time;
}

void Player::update() {
	control.time = get_sync_time_micros();
	status.current_time = reader->get_time_micros();
	const double total_time = (status.end_time - status.begin_time) / 1e6;
	const double time_elapsed = (status.current_time - status.begin_time) / 1e6;
	status.progress = time_elapsed / total_time;
	status.actual_speed =
			double(reader->get_time_micros() - (control.time_offset + control.time_start))
			/ (get_sync_time_micros() - control.time_start + 1);
	status.control = control;
	publish(control, vnx::time_control);
	publish(status, output_status);
}

void Player::update_stats(bool force) {
	const double total_time = (status.end_time - status.begin_time) / 1e6;
	const double time_elapsed = (status.current_time - status.begin_time) / 1e6;
	status.bandwidth = double(reader->get_input_pos() - last_input_pos) / 1024 / 1024;
	status.sample_rate = sample_count - last_sample_count;

	std::string bar = "[";
	for(int i = 0; i < 50; ++i) {
		if((i+1)*2 < 100 * status.progress) {
			bar += "=";
		} else {
			if(bar.back() == '=' || bar.back() == '[') {
				bar += ">";
			} else {
				bar += " ";
			}
		}
	}
	bar += "]";

	if(!control.is_paused || force) {
		log(INFO) << bar << " " << int(100 * status.progress) << "%, " << time_elapsed << "/" << total_time << " s, " << status.bandwidth << " MB/s, "
			<< status.sample_rate << " s/s, " << control.time_speed << " x (" << (control.is_paused ? 0 : status.actual_speed) << " x)";
	}
	last_input_pos = reader->get_input_pos();
	last_sample_count = sample_count;
}


} // vnx
