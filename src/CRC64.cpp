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

#include <vnx/CRC64.h>

#include <cstring>


namespace vnx {

uint64_t hash64(const char* str) {
	CRC64 func;
	func.update(str, std::strlen(str));
	return func.get();
}

uint64_t hash64(const std::string& str) {
	CRC64 func;
	func.update(str.c_str(), str.size());
	return func.get();
}

uint64_t hash64(const char* buf, size_t len) {
	CRC64 func;
	func.update(buf, len);
	return func.get();
}

} // vnx
