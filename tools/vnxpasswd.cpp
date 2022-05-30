/*
 * vnxpasswd.cpp
 *
 *  Created on: Nov 30, 2020
 *      Author: mad
 */

#include <vnx/vnx.h>
#include <vnx/SHA256.h>
#include <vnx/User.hxx>

#if __cplusplus >= 201703L
#include <filesystem>
#endif


int main(int argc, char** argv) {

	std::map<std::string, std::string> options;
	options["u"] = "user";
	options["p"] = "passwd";
	options["user"] = "user name";
	options["passwd"] = "password";

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

		std::string new_pass;
		if(!vnx::read_config("passwd", new_pass)) {
			new_pass = vnx::input_password("New Password: ");
			if(vnx::input_password("Repeat Password: ") != new_pass) {
				throw std::logic_error("passwords did not match");
			}
		}
		if(new_pass.size() < 6) {
			throw std::logic_error("password too short (needs to be >= 6)");
		}
		passwd[user] = vnx::get_auth_server()->final_hash(vnx::sha256_str(new_pass));

		std::ofstream stream(file_path, std::ofstream::out);
		vnx::PrettyPrinter printer(stream);
		vnx::accept(printer, passwd);
		stream.close();

#if __cplusplus >= 201703L
		std::filesystem::permissions(file_path, std::filesystem::perms::owner_read | std::filesystem::perms::owner_write);
#endif
		std::cerr << "Updated '" << file_path << "'" << std::endl;
	}
	catch(const std::exception& ex) {
		ret = -1;
		std::cerr << "ERROR: " << ex.what() << std::endl;
	}
	vnx::close();
	return ret;
}

