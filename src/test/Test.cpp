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

#include <vnx/test/Test.h>

#include <atomic>
#include <iostream>


std::string _vnx_test_name = "";

namespace vnx {
namespace test {

static std::atomic<int> num_passed;
static std::atomic<int> num_failed;

void init(const std::string& test_domain) {
	_vnx_test_name = test_domain;
}

void pass(const std::string& test_name) {
	num_passed++;
}

void fail(const std::string& test_name, const std::exception& exception) {
	std::cerr << std::endl << "FAIL " << test_name << ": " << exception.what() << std::endl;
	num_failed++;
}

int done() {
	std::cerr << "PASSED " << num_passed << "/" << (num_passed + num_failed) << std::endl;
	if(num_failed > 0) {
		return -1;
	} else {
		return 0;
	}
}


} // test
} // vnx
