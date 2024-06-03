/*
 * test_glibc_bug_31841.cpp
 *
 *  Created on: Jun 4, 2024
 *      Author: mad
 */

#include <thread>
#include <fstream>
#include <unistd.h>

int main()
{
	std::thread read_thread([]() {
		// blocking read on stdin
		std::getchar();
	});

	std::thread close_thread([]() {
		// _IO_un_link() will lock `list_all_lock` and get stuck on _IO_flockfile(stdin)
		fclose(stdin);
	});

	// make sure the threads have started
	usleep(100000);

	// will get stuck here trying to lock `list_all_lock` in _IO_link_in()
	std::ofstream out("out.txt");
	out << "test" << std::endl;
	out.close();

	read_thread.join();
	close_thread.join();

	return 0;
}


