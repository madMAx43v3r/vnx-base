/*
 * Terminal.cpp
 *
 *  Created on: Sep 16, 2020
 *      Author: jaw
 */

#include <vnx/vnx.h>
#include <vnx/Terminal.h>
#include <windows.h>


namespace vnx {

DWORD Terminal::saved_attributes = 0;


void Terminal::write_editline_winapi(std::ostream &out) const{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO scrbufinfo;
	GetConsoleScreenBufferInfo(console, &scrbufinfo);
	// erase the current line
	COORD firstcell = {0, scrbufinfo.dwCursorPosition.Y};
	DWORD dummy_var;
	FillConsoleOutputCharacter(console, ' ', scrbufinfo.dwSize.X, firstcell, &dummy_var);
	// start of the line
	out << "\r";
	// display prompt and input part 1
	out << prompt << line.substr(0, cursor);
	// display current suggestion
	SetConsoleTextAttribute(console, (scrbufinfo.wAttributes & ~FOREGROUND_GREEN) | FOREGROUND_RED | FOREGROUND_BLUE);
	out << completion.first;
	SetConsoleTextAttribute(console, scrbufinfo.wAttributes);
	// display input part 2
	out << line.substr(cursor);
	// place the cursor
	COORD cursorpos = scrbufinfo.dwCursorPosition;
	cursorpos.X = (prompt.length()+cursor);
	SetConsoleCursorPosition(console, cursorpos);
}


void Terminal::read_loop_impl(Hash64 service_addr) {
	TerminalClient terminal(service_addr);
	HANDLE console = GetStdHandle(STD_INPUT_HANDLE);

	while(vnx::do_run()) {
		INPUT_RECORD input_record;
		DWORD dummy_var;
		ReadConsoleInput(console, &input_record, 1, &dummy_var);
		if(input_record.EventType != KEY_EVENT || !input_record.Event.KeyEvent.bKeyDown) continue;
		char c = input_record.Event.KeyEvent.uChar.AsciiChar;
		if(c == 0){
			// use virtual key code
			WORD vk = input_record.Event.KeyEvent.wVirtualKeyCode;
			if(vk == VK_UP){
				terminal.read_event_async(terminal_event_e::KEY_ARROWUP);
			}else if(vk == VK_DOWN){
				terminal.read_event_async(terminal_event_e::KEY_ARROWDOWN);
			}else if(vk == VK_LEFT){
				terminal.read_event_async(terminal_event_e::KEY_ARROWLEFT);
			}else if(vk == VK_RIGHT){
				terminal.read_event_async(terminal_event_e::KEY_ARROWRIGHT);
			}else if(vk == VK_HOME){
				terminal.read_event_async(terminal_event_e::KEY_POS1);
			}else if(vk == VK_END){
				terminal.read_event_async(terminal_event_e::KEY_END);
			}else if(vk == VK_DELETE){
				terminal.read_event_async(terminal_event_e::KEY_DEL);
			}
		}else if(c == '\t'){
			terminal.read_event_async(terminal_event_e::KEY_TAB);
		}else if(c == 127 || c == 8){
			terminal.read_event_async(terminal_event_e::KEY_BACKSPACE);
		}else if(c == '\r'){
			terminal.read_event_async(terminal_event_e::KEY_CR);
		}else if(c == '\n'){
			terminal.read_event_async(terminal_event_e::KEY_LF);
		}else{
			terminal.read_char(c);
		}
	}
}


void Terminal::set_terminal_mode(){
	HANDLE console = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(console, &saved_attributes);
	SetConsoleMode(console, saved_attributes & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));
}


void Terminal::reset_terminal_mode(){
	HANDLE console = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(console, saved_attributes);
}


} // vnx
