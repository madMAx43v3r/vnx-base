/*
 * Process.cpp
 *
 *  Created on: Jan 20, 2021
 *      Author: jaw
 */


#include <vnx/Process.h>
#include <vnx/thread_priority_e.hxx>
#include <iostream>
#include <unistd.h>
#include <sys/resource.h>


namespace vnx {


void set_current_process_priority(const thread_priority_e &priority){
	switch(priority){
	case thread_priority_e::VERY_LOW:
		setpriority(PRIO_PROCESS, 0, 10);
		break;
	case thread_priority_e::LOW:
		setpriority(PRIO_PROCESS, 0, 5);
		break;
	case thread_priority_e::NORMAL:
		setpriority(PRIO_PROCESS, 0, 0);
		break;
	case thread_priority_e::HIGH:
		setpriority(PRIO_PROCESS, 0, -5);
		break;
	case thread_priority_e::VERY_HIGH:
		setpriority(PRIO_PROCESS, 0, -10);
		break;
	}
}

void shutdown_host(){
	if(vfork() == 0){
		execl("/usr/bin/sudo", "sudo", "halt", "-p", NULL);
		_exit(0);
	}
}


} // vnx
