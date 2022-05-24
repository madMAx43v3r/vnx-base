/*
 * vnxpasswd.cpp
 *
 *  Created on: Nov 30, 2020
 *      Author: mad
 */

#include <vnx/vnx.h>
#include <vnx/SHA256.h>
#include <vnx/User.hxx>



int main(int argc, char** argv) {

	std::map<std::string, std::string> options;
	options["u"] = "user";
	options["user"] = "user name";

	vnx::set_config("log_level", "2");

	vnx::init("vnxpasswd", argc, argv, options);

	int ret = 0;
	try {
		std::vector<std::string> config;
		vnx::read_config("config", config);
		if(config.empty()) {
			throw std::logic_error("missing 'config' argument");
		}
		const std::string output_path = config.back() + "vnx/authentication/";
		const std::string file_path = output_path + "passwd";

		std::string user;
		if(!vnx::read_config("user", user)) {
			std::cerr << "ERROR: missing 'user' argument!" << std::endl;
			vnx::close();
			return -1;
		}

		std::vector<vnx::User> users;
		std::map<std::string, std::string> passwd;
		vnx::read_config("vnx.authentication.users", users);
		vnx::read_config("vnx.authentication.passwd", passwd);

		bool found = false;
		for(const auto& u : users) {
			if(u.name == user) {
				found = true; break;
			}
		}
		if(!found) {
			throw std::logic_error("no such user: '" + user + "'");
		}

		vnx::Directory dir(output_path);
		dir.create();

		std::string new_pass[2];
		new_pass[0] = vnx::input_password("New Password: ");
		new_pass[1] = vnx::input_password("Repeat Password: ");

		if(new_pass[0] != new_pass[1]) {
			throw std::logic_error("passwords did not match");
		}
		passwd[user] = vnx::get_auth_server()->final_hash(vnx::sha256_str(new_pass[0]));

		std::ofstream stream(file_path, std::ofstream::out);
		vnx::PrettyPrinter printer(stream);
		vnx::accept(printer, passwd);
		stream.close();

		std::cerr << "Updated '" << file_path << "'" << std::endl;
	}
	catch(const std::exception& ex) {
		ret = -1;
		std::cerr << "ERROR: " << ex.what() << std::endl;
	}
	vnx::close();
	return ret;
}

