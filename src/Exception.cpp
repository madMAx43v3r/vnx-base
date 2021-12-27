/*
 * Exception.cpp
 *
 *  Created on: Oct 29, 2020
 *      Author: mad
 */

#include <vnx/Exception.hxx>


namespace vnx {

std::shared_ptr<const Exception> Exception::from_what(const std::string& what) {
	auto ex = Exception::create();
	ex->what = what;
	return ex;
}


} // vnx
