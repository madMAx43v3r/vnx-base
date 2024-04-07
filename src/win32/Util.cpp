/*
 * Util.cpp
 *
 *  Created on: Apr 7, 2024
 *      Author: mad
 */

#include <vnx/Util.h>

#include <windows.h>
#include <Wincrypt.h>

#include <mutex>
#include <stdexcept>


namespace vnx {

void secure_random_bytes(void* data, const size_t length)
{
	static std::mutex mutex;
	static bool have_init = false;
	static HCRYPTPROV hCryptProv;

	std::lock_guard<std::mutex> lock(mutex);

	if(!have_init) {
		if(!CryptAcquireContext(&hCryptProv, NULL,
				(LPCWSTR)L"Microsoft Base Cryptographic Provider v1.0",
				PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
			throw std::runtime_error("CryptAcquireContext() failed");
		}
		have_init = true;
	}

	if(!CryptGenRandom(hCryptProv, length, (BYTE*)data)) {
		throw std::runtime_error("CryptGenRandom() failed");
	}
}


} // vnx
