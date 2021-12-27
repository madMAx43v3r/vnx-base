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
#include <vnx/TimeServer.h>


namespace vnx {

TimeServer::TimeServer(const std::string& _vnx_name) : TimeServerBase(_vnx_name) {}

void TimeServer::main() {
	set_timer_millis(interval_ms, std::bind(&TimeServer::update, this));
	set_timer_millis(1000, std::bind(&TimeServer::print_stats, this));
	
	client = std::make_shared<ProcessClient>("vnx.time_server");
	client->vnx_set_non_blocking(true);
	if(remote_tunnel) {
		client->vnx_set_tunnel(remote_tunnel);
	}
	
	Super::main();
}

void TimeServer::update() {
	try {
		const int64_t send_time = vnx::get_wall_time_micros();
		TimeSync sample = client->get_sync_time();
		const int64_t recv_time = vnx::get_wall_time_micros();
		const int64_t offset = sample.time - (send_time + recv_time) / 2;
		const int64_t jitter = std::abs(offset - state.offset);
		if(have_init) {
			if(jitter < 1000000) {
				state.time = sample.time;
				state.jitter = (state.jitter * 9 + jitter) / 10;
				state.offset = (state.offset * (inv_gain - 1) + offset) / inv_gain;
			} else {
				log(WARN) << "Detected time jump of " << jitter/1000 << " ms";
				have_init = false;
			}
		} else {
			have_init = true;
			state = sample;
			state.offset = offset;
			log(INFO) << "Initialized with offset=" << offset;
		}
		state.wall_time = recv_time;
		latency = recv_time - send_time;
	} catch (const std::exception& ex) {
		log(DEBUG) << "Query failed with: " << ex.what();
	}
	if(have_init) {
		publish(state, vnx::time_sync);
	}
}

void TimeServer::print_stats() {
	log(DEBUG) << "offset=" << state.offset << ", jitter=" << float(state.jitter)/1000
					<< " ms, latency=" << float(latency)/1000.f << " ms";
}


} // vnx
