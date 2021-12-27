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
#include <vnx/Time.h>

#include <chrono>
#include <mutex>


namespace vnx {

int64_t get_wall_time_seconds() {
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int64_t get_wall_time_millis() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int64_t get_wall_time_micros() {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int64_t get_wall_time_nanos() {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string get_date_string(bool UTC, int64_t time_secs) {
	return get_date_string_ex("%Y-%m-%d_%H:%M:%S", UTC, time_secs);
}

std::string get_date_string_ex(const std::string& format, bool UTC, int64_t time_secs) {
	::time_t time_;
	if(time_secs < 0) {
		::time(&time_);
	} else {
		time_ = time_secs;
	}
	::tm* tmp;
	if(UTC) {
		tmp = ::gmtime(&time_);
	} else {
		tmp = ::localtime(&time_);
	}
	char buf[256];
	::strftime(buf, sizeof(buf), format.c_str(), tmp);
	return std::string(buf);
}


int64_t TimeControl::get_time_micros() const {
	if(is_realtime) {
		return get_sync_time_micros();
	} else if(!is_paused) {
		return int64_t((get_sync_time_micros() - time_start) * time_speed) + time_start + time_offset;
	} else {
		return time_start + time_offset;
	}
}

int64_t TimeSync::get_time_micros() const {
	return get_wall_time_micros() + offset;
}


class GlobalTimeState {
public:
	GlobalTimeState() {}
	
	TimeControl get_time_control() {
		std::lock_guard<std::mutex> lock(mutex);
		return control;
	}
	
	void set_time_control(const TimeControl& state_) {
		std::lock_guard<std::mutex> lock(mutex);
		control = state_;
	}
	
	TimeSync get_time_sync() {
		std::lock_guard<std::mutex> lock(mutex);
		return sync;
	}
	
	void set_time_sync(const TimeSync& state_) {
		std::lock_guard<std::mutex> lock(mutex);
		sync = state_;
	}
	
private:
	std::mutex mutex;
	TimeControl control;
	TimeSync sync;
	
};

static GlobalTimeState& get_global_time_state() {
	static GlobalTimeState state;
	return state;
}

int64_t get_sync_time_micros() {
	return get_global_time_state().get_time_sync().get_time_micros();
}

int64_t get_time_seconds() {
	return get_time_micros() / 1000000;
}

int64_t get_time_millis() {
	return get_time_micros() / 1000;
}

int64_t get_time_micros() {
	return get_global_time_state().get_time_control().get_time_micros();
}

TimeControl get_time_control() {
	return get_global_time_state().get_time_control();
}

void set_time_control(const TimeControl& state_) {
	get_global_time_state().set_time_control(state_);
}

TimeSync get_time_sync() {
	return get_global_time_state().get_time_sync();
}

void set_time_sync(const TimeSync& state_) {
	get_global_time_state().set_time_sync(state_);
}


} // vnx
