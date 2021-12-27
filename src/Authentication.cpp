/*
 * Authentication.cpp
 *
 *  Created on: Nov 24, 2020
 *      Author: jaw
 */

#include <vnx/vnx.h>
#include <vnx/Authentication.h>
#include <vnx/SHA256.h>


namespace vnx {

const std::string AuthenticationServer::salt = "xZJIdIv8ByEP6fCmzkyS7ca";


AuthenticationServer::AuthenticationServer()
{
	add_permissions(access_role_e::UNAUTHENTICATED, {
			// internal use only
			permission_e::PROXY_EXPORT,
			permission_e::PROXY_INTERNAL,
	});
	add_permissions(access_role_e::DEFAULT, {
			// no permissions for failed logins by default
	});
	add_permissions(access_role_e::VIEWER, {
			permission_e::VIEW,
			permission_e::TIME_SYNC,
	});
	add_permissions(access_role_e::OBSERVER, {
			permission_e::VIEW,

			permission_e::TIME_SYNC,
			permission_e::CONST_REQUEST,
	});
	add_permissions(access_role_e::USER, {
			permission_e::VIEW,

			permission_e::TIME_SYNC,
			permission_e::CONST_REQUEST,

			permission_e::READ_CONFIG,
	});
	add_permissions(access_role_e::INSTALLER, {
			permission_e::VIEW,

			permission_e::TIME_SYNC,
			permission_e::CONST_REQUEST,
			permission_e::PUBLISH,

			permission_e::READ_CONFIG,
			permission_e::WRITE_CONFIG,

			permission_e::START,
			permission_e::STOP,
			permission_e::RESTART,
			permission_e::SHUTDOWN,
			permission_e::SELF_TEST,
	});
	add_permissions(access_role_e::ADMIN, {
			permission_e::VIEW,

			permission_e::TIME_SYNC,
			permission_e::CONST_REQUEST,
			permission_e::PUBLISH,
			permission_e::REQUEST,

			permission_e::READ_CONFIG,
			permission_e::WRITE_CONFIG,
			permission_e::PROTECTED_CONFIG,

			permission_e::PROXY_IMPORT,
			permission_e::PROXY_EXPORT,
			permission_e::PROXY_FORWARD,

			permission_e::START,
			permission_e::STOP,
			permission_e::RESTART,
			permission_e::SHUTDOWN,
			permission_e::SELF_TEST,
			permission_e::HOST_SHUTDOWN,

			permission_e::LOCAL,
	});

	std::vector<User> users_;
	std::map<std::string, std::string> passwd_;
	decltype(permissions) permissions_;
	vnx::read_config("vnx.authentication.users", users_);
	vnx::read_config("vnx.authentication.passwd", passwd_);
	vnx::read_config("vnx.authentication.permissions", permissions_);
	for(const auto &user : users_) {
		users[user.name] = user;
	}
	for(const auto &entry : passwd_) {
		auto& user = users[entry.first];
		user.name = entry.first;
		user.hashed_password = entry.second;
	}
	for(const auto &entry : permissions_) {
		auto &set = permissions[entry.first];
		for(auto perm : entry.second) {
			const bool negate = !perm.empty() && perm[0] == '!';
			if(negate) {
				perm.erase(0, 1);
			}
			if(perm.find('.') == std::string::npos) {
				perm = "vnx.permission_e." + perm;
			}
			if(negate) {
				set.erase(perm);
			} else {
				set.insert(perm);
			}
		}
	}
}


std::shared_ptr<const Session> AuthenticationServer::login(	const std::string &name,
															const std::string &password,
															const std::string &default_access)
{
	const std::string hashed_password = final_hash(password);
	auto session = Session::create();
	session->id = new_session_id();
	session->login_time = vnx::get_time_micros();
	session->permissions = get_permissions(default_access);
	if(auto user = get_user(name)){
		if(user->hashed_password == hashed_password){
			session->user = user->name;
			for(auto role : user->access_roles){
				auto perms = get_permissions(role);
				session->permissions.insert(perms.begin(), perms.end());
			}
			session->permissions.insert(user->permissions.begin(), user->permissions.end());
		}
	}
	add_session(session);
	return session;
}


std::shared_ptr<const Session> AuthenticationServer::login_anonymous(const std::string &access_role){
	return login_anonymous(std::vector<std::string>{access_role});
}


std::shared_ptr<const Session> AuthenticationServer::login_anonymous(const std::vector<std::string> &access_roles){
	auto session = Session::create();
	session->id = new_session_id();
	session->login_time = vnx::get_time_micros();
	for(auto role : access_roles){
		auto perms = get_permissions(role);
		session->permissions.insert(perms.begin(), perms.end());
	}
	add_session(session);
	return session;
}


void AuthenticationServer::logout(Hash64 session_id){
	std::lock_guard<std::mutex> lock(mutex);

	auto found = sessions.find(session_id);
	if(found == sessions.end()) return;

	std::shared_ptr<const Session> session = found->second;
	sessions.erase(found);
	for(const auto& perm : session->permissions) {
		permissions_lookup.erase(std::make_pair(session->id, perm));
	}
}


std::shared_ptr<User> AuthenticationServer::get_user(const std::string &name) const{
	std::lock_guard<std::mutex> lock(mutex);
	auto iter = users.find(name);
	if(iter != users.end()) {
		return vnx::clone(iter->second);
	}
	return nullptr;
}


std::shared_ptr<const Session> AuthenticationServer::get_session(Hash64 session_id) const{
	std::lock_guard<std::mutex> lock(mutex);
	auto iter = sessions.find(session_id);
	if(iter != sessions.end()) {
		return iter->second;
	}
	return nullptr;
}


std::set<std::string> AuthenticationServer::get_permissions(const std::string &role) const{
	std::lock_guard<std::mutex> lock(mutex);
	auto iter = permissions.find(role);
	if(iter != permissions.end()) {
		return iter->second;
	}
	return {};
}


bool AuthenticationServer::is_allowed(Hash64 session_id, const std::string &perm) const{
	if(!session_id) {
		return true;	// really fast path
	}
	std::lock_guard<std::mutex> lock(mutex);
	// perform fast lookup
	return permissions_lookup.find(std::make_pair(session_id, perm)) != permissions_lookup.end();
}


std::string AuthenticationServer::final_hash(const std::string &input) const{
	return vnx::sha256_str(salt + input + salt);
}


Hash64 AuthenticationServer::new_session_id(){
	Hash64 id;
	do {
		id = vnx::Hash64::rand();
	} while(get_session(id));
	return id;
}


void AuthenticationServer::add_session(std::shared_ptr<Session> session){
	std::lock_guard<std::mutex> lock(mutex);
	sessions.emplace(session->id, session);
	// add fast lookup entries
	for(const auto& perm : session->permissions) {
		permissions_lookup.emplace(session->id, perm);
	}
}


void AuthenticationServer::add_permissions(const access_role_e &access_role, const std::set<vnx::permission_e> &perms){
	std::lock_guard<std::mutex> lock(mutex);
	auto& level = permissions[access_role.to_string_value()];
	for(auto perm : perms) {
		level.insert(perm.to_string_value_full());
	}
}


std::shared_ptr<AuthenticationServer> get_auth_server(){
	if(!process::auth_server) {
		throw std::logic_error("invalid access to AuthenticationServer");
	}
	return process::auth_server;
}


std::shared_ptr<const Session> get_session(Hash64 session_id){
	return get_auth_server()->get_session(session_id);
}


bool is_allowed(Hash64 session_id, const std::string& perm){
	return get_auth_server()->is_allowed(session_id, perm);
}


namespace process {

std::shared_ptr<AuthenticationServer> auth_server;

} // process

} // vnx
