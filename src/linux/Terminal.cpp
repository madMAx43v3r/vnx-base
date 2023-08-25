/*
 * Terminal.cpp
 *
 *  Created on: Sep 16, 2020
 *      Author: jaw
 */

#include <vnx/vnx.h>
#include <vnx/Terminal.h>
#include <termios.h>
#include <limits>


namespace vnx {

termios Terminal::saved_attributes = {0};


void Terminal::read_loop_impl(Hash64 service_addr) {
	TerminalClient terminal(service_addr);

	while(vnx::do_run()) {
		const auto c = std::getchar();
		if(c == EOF){
			break;
		}else if(c == 27){
			// ESC
			const auto c2 = getchar();
			if(c2 == EOF){
				break;
			}else if(c2 == '['){
				const auto c3 = getchar();
				if(c3 == EOF){
					break;
				}else if(c3 == 'A'){
					terminal.read_event_async(terminal_event_e::KEY_ARROWUP);
				}else if(c3 == 'B'){
					terminal.read_event_async(terminal_event_e::KEY_ARROWDOWN);
				}else if(c3 == 'C'){
					terminal.read_event_async(terminal_event_e::KEY_ARROWRIGHT);
				}else if(c3 == 'D'){
					terminal.read_event_async(terminal_event_e::KEY_ARROWLEFT);
				}else if(c3 == 'H'){
					terminal.read_event_async(terminal_event_e::KEY_POS1);
				}else if(c3 == 'F'){
					terminal.read_event_async(terminal_event_e::KEY_END);
				}else if(c3 == '3'){
					char c4 = getchar();
					if(c4 == '~'){
						terminal.read_event_async(terminal_event_e::KEY_DEL);
					}
				}
			}
		}else if(c == '\r'){
			terminal.read_event_async(terminal_event_e::KEY_CR);
		}else if(c == '\n'){
			terminal.read_event_async(terminal_event_e::KEY_LF);
		}else if(c == 127 || c == 8){
			terminal.read_event_async(terminal_event_e::KEY_BACKSPACE);
		}else if(c == '\t'){
			terminal.read_event_async(terminal_event_e::KEY_TAB);
		}else if(c >= 0 && c <= std::numeric_limits<int8_t>::max()){
			terminal.read_char(c);
		}
	}
}


void Terminal::set_terminal_mode(){
	tcgetattr(0, &saved_attributes);

	termios tmp = saved_attributes;
	tmp.c_lflag &= ~ICANON;
	tmp.c_lflag &= ~ECHO;
	tmp.c_cc[VMIN] = 1;
	tmp.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &tmp);
}


void Terminal::reset_terminal_mode(){
	tcsetattr(0, TCSANOW, &saved_attributes);
}


} // vnx
