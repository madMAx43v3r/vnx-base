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
		std::getchar();		// blocking read on stdin
	});

	std::thread close_thread([]() {
		// try to close stdin, will lock list_all_lock and block on _IO_flockfile(stdin)
		close(0);
	});

	std::ofstream out("out.txt");
	out << "test" << std::endl;
	out.close();

	read_thread.join();
	close_thread.join();

	return 0;
}


