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

#include <stdint.h>
#include <iostream>


int main() {
	
	uint64_t poly = 0xC96C5795D7870F42ull;
	uint64_t table[256];
	
	for (int b = 0; b < 256; ++b) {
		uint64_t r = b;
		for (int i = 0; i < 8; ++i) {
				if ((r & 1) == 1) {
						r = (r >> 1) ^ poly;
				} else {
						r >>= 1;
				}
		}
		table[b] = r;
	}
	
	std::cout << std::hex << "{ ";
	for(int row = 0; row < 16; ++row) {
		std::cout << "\t";
		for(int col = 0; col < 16; ++col) {
			std::cout << "0x" << table[row * 16 + col] << "ull, ";
		}
		std::cout << std::endl;
	}
	std::cout << "};" << std::endl;
	
}

