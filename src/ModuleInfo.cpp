/*
 * ModuleInfo.cpp
 *
 *  Created on: Jan 12, 2021
 *      Author: mad
 */

#include <vnx/ModuleInfo.hxx>


namespace vnx {

float64_t ModuleInfo::get_cpu_load() const {
	const auto sum = time_idle + time_running;
	return sum > 0 ? time_running / double(sum) : 0;
}

float64_t ModuleInfo::get_cpu_load_total() const {
	const auto sum = time_idle_total + time_running_total;
	return sum > 0 ? time_running_total / double(sum) : 0;
}


} // vnx
