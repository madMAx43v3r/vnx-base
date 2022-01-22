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
#include <vnx/Server.h>
#include <vnx/Player.h>
#include <vnx/PlayerClient.hxx>
#include <vnx/TcpEndpoint.hxx>
#include <vnx/UnixEndpoint.hxx>

#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <thread>
#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#endif


static void read_loop();

int main(int argc, char** argv) {
	
	std::map<std::string, std::string> options;
	options["f"] = "files";
	options["x"] = "speed";
	options["b"] = "blocking";
	options["r"] = "repeat";
	options["p"] = "port";
	options["u"] = "unix-path";
	options["files"] = "file ...";
	options["port"] = "port";
	options["unix-path"] = "unix path";
	options["seek"] = "0-1";
	
	vnx::init("vnxplay", argc, argv, options);
	
	std::cout << "Controls: space=play/pause, s=stop, x=-10min, c=-1min, v=-10s, b=+10s, n=+1min, m=+10min, [1234567890]=faster, [qwertyuiop]=slower" << std::endl;
	
	{
		std::shared_ptr<vnx::TcpEndpoint> endpoint = vnx::clone(vnx::TcpEndpoint::create_ex("0.0.0.0", 4444));
		vnx::read_config("port", endpoint->port);
		
		vnx::Handle<vnx::Server> server = new vnx::Server("TcpServer", endpoint);
		server->export_list.push_back(vnx::time_control);
		server.start_detached();
	}
	{
		std::shared_ptr<vnx::UnixEndpoint> endpoint = vnx::clone(vnx::UnixEndpoint::create_ex("/tmp/vnxplayer.sock"));
		vnx::read_config("unix-path", endpoint->domain_path);
		
		vnx::Handle<vnx::Server> server = new vnx::Server("UnixServer", endpoint);
		server->export_list.push_back(vnx::time_control);
		server.start_detached();
	}
	
	vnx::Handle<vnx::Player> player = new vnx::Player("Player");
	vnx::read_config("files", player->files);
	vnx::read_config("speed", player->play_speed);
	vnx::read_config("blocking", player->is_blocking);
	vnx::read_config("repeat", player->is_repeat);
	player->is_autostart = true;
	player.start();
	
	vnx::PlayerClient client("Player");
	
	double seek_to = 0;
	if(vnx::read_config("seek", seek_to)) {
		client.seek_to_position(seek_to);
	}
	
#ifdef _WIN32
	HANDLE console = GetStdHandle(STD_INPUT_HANDLE);

	DWORD mode;
	GetConsoleMode(console, &mode);
	SetConsoleMode(console, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));
#else
	termios old = {0};
	tcgetattr(0, &old);
	{
		termios tmp = old;
		tmp.c_lflag &= ~ICANON;
		tmp.c_lflag &= ~ECHO;
		tmp.c_cc[VMIN] = 1;
		tmp.c_cc[VTIME] = 0;
		tcsetattr(0, TCSANOW, &tmp);
	}
#endif
	
	std::thread reader(&read_loop);
	reader.detach();
	
	player.wait();
	
	vnx::close();
	
#ifdef _WIN32
	SetConsoleMode(console, mode);
#else
	tcsetattr(0, TCSANOW, &old);
#endif
	
}

void read_loop() {
	vnx::PlayerClient client("Player");
	while(vnx::do_run()) {
		const char c = ::getchar();
		try {
			if(c == ' ') {
				client.toggle();
			}
			if(c == 's') {
				client.stop();
			}
			if(c == 'x') {
				client.seek_by_time(-10 * 60 * 1000 * 1000);
			}
			if(c == 'c') {
				client.seek_by_time(-1 * 60 * 1000 * 1000);
			}
			if(c == 'v') {
				client.seek_by_time(-10 * 1000 * 1000);
			}
			if(c == 'b') {
				client.seek_by_time(10 * 1000 * 1000);
			}
			if(c == 'n') {
				client.seek_by_time(1 * 60 * 1000 * 1000);
			}
			if(c == 'm') {
				client.seek_by_time(10 * 60 * 1000 * 1000);
			}
			if(c == '1') {
				client.set_speed(1);
			}
			if(c == '2') {
				client.set_speed(2);
			}
			if(c == '3') {
				client.set_speed(3);
			}
			if(c == '4') {
				client.set_speed(5);
			}
			if(c == '5') {
				client.set_speed(10);
			}
			if(c == '6') {
				client.set_speed(20);
			}
			if(c == '7') {
				client.set_speed(30);
			}
			if(c == '8') {
				client.set_speed(50);
			}
			if(c == '9') {
				client.set_speed(100);
			}
			if(c == '0') {
				client.set_speed(1000);
			}
			if(c == 'q') {
				client.set_speed(0.5);
			}
			if(c == 'w') {
				client.set_speed(0.3);
			}
			if(c == 'e') {
				client.set_speed(0.2);
			}
			if(c == 'r') {
				client.set_speed(0.1);
			}
			if(c == 't') {
				client.set_speed(0.05);
			}
			if(c == 'y') {
				client.set_speed(0.03);
			}
			if(c == 'u') {
				client.set_speed(0.02);
			}
			if(c == 'i') {
				client.set_speed(0.01);
			}
			if(c == 'o') {
				client.set_speed(0.001);
			}
			if(c == 'p') {
				client.set_speed(0.0001);
			}
		} catch(...) {
			// oh, well
		}
	}
}

