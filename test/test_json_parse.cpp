/*
 * test_json_parse.cpp
 *
 *  Created on: May 26, 2021
 *      Author: mad
 */

#include <vnx/vnx.h>


int main(int argc, char** argv)
{
	vnx::init("test_json_parse", argc, argv);

	while(vnx::do_run())
	{
		vnx::Variant var;
		try {
			var = vnx::read(std::cin);
			vnx::write(std::cout, var);
		} catch(std::underflow_error& ex) {
			break;
		} catch(std::exception& ex) {
			std::cerr << ex.what();
		}
		std::cout << std::endl;
	}

	vnx::close();
}


