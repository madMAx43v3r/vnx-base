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

#include <vnx/LogMsg.hxx>


namespace vnx {

std::string LogMsg::get_output() const {
	std::string out = "[" + module + "] ";
	switch(level) {
		case ERROR: out += "ERROR: "; break;
		case WARN: out += "WARN: "; break;
		case INFO: out += "INFO: "; break;
		case DEBUG: out += "DEBUG: "; break;
		default: out += "L" + std::to_string(level);
	}
	out += message;
	return out;
}


} // vnx
