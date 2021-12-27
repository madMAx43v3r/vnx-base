/*
 * Process.cpp
 *
 *  Created on: Jan 20, 2021
 *      Author: jaw
 */


#include <vnx/Process.h>
#include <vnx/thread_priority_e.hxx>
#include <windows.h>



namespace vnx {


void set_current_process_priority(const thread_priority_e &priority){
	HANDLE hProc = GetCurrentProcess();
	switch(priority){
	case thread_priority_e::VERY_LOW:
		SetPriorityClass(hProc, IDLE_PRIORITY_CLASS);
		break;
	case thread_priority_e::LOW:
		SetPriorityClass(hProc, BELOW_NORMAL_PRIORITY_CLASS);
		break;
	case thread_priority_e::NORMAL:
		SetPriorityClass(hProc, NORMAL_PRIORITY_CLASS);
		break;
	case thread_priority_e::HIGH:
		SetPriorityClass(hProc, ABOVE_NORMAL_PRIORITY_CLASS);
		break;
	case thread_priority_e::VERY_HIGH:
		SetPriorityClass(hProc, HIGH_PRIORITY_CLASS);
		break;
	}
}

void shutdown_host(){
	const char *appname = "C:\\Windows\\system32\\shutdown.exe";
	char commandline[] = "shutdown.exe -s -f -t 1";
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi;
	CreateProcessA(appname, commandline, NULL, NULL, false, 0, NULL, NULL, &si, &pi);
}

void Process::show_log_message_winapi(std::shared_ptr<const LogMsg> message) const{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO scrinfo;
	GetConsoleScreenBufferInfo(hConsole, &scrinfo);
	if(message->level == ERROR){
		SetConsoleTextAttribute(hConsole, (scrinfo.wAttributes & ~(FOREGROUND_GREEN | FOREGROUND_BLUE)) | FOREGROUND_RED);
	}else if(message->level == WARN){
		SetConsoleTextAttribute(hConsole, (scrinfo.wAttributes & ~FOREGROUND_BLUE) | FOREGROUND_RED | FOREGROUND_GREEN);
	}
	std::cerr << message->get_output() << std::endl;
	if(message->level == ERROR || message->level == WARN){
		SetConsoleTextAttribute(hConsole, scrinfo.wAttributes);
	}
}



} // vnx
