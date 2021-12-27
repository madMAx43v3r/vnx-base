/*
 * Session.cpp
 *
 *  Created on: Nov 26, 2020
 *      Author: mad
 */

#include <vnx/Session.hxx>


namespace vnx {

vnx::bool_t Session::is_anonymous() const {
	return user.empty();
}

vnx::bool_t Session::has_permission(const std::string& perm) const {
	return permissions.find(perm) != permissions.end();
}

vnx::bool_t Session::has_permission_vnx(const vnx::permission_e& perm) const {
	return has_permission(perm.to_string_value_full());
}


} // vnx
