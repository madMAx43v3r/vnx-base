/*
 * Util.cpp
 *
 *  Created on: Apr 7, 2024
 *      Author: mad
 */

#include <vnx/Util.h>

#include <stdexcept>
#include <windows.h>


/* `RtlGenRandom` is used over `CryptGenRandom` on Microsoft Windows based systems:
 *  - `CryptGenRandom` requires pulling in `CryptoAPI` which causes unnecessary
 *     memory overhead if this API is not being used for other purposes
 *  - `RtlGenRandom` is thus called directly instead. A detailed explanation
 *     can be found here: https://blogs.msdn.microsoft.com/michael_howard/2005/01/14/cryptographically-secure-random-number-on-windows-without-using-cryptoapi/
 *
 * In spite of the disclaimer on the `RtlGenRandom` documentation page that was
 * written back in the Windows XP days, this function is here to stay. The CRT
 * function `rand_s()` directly depends on it, so touching it would break many
 * applications released since Windows XP.
 *
 * Also note that Rust, Firefox and BoringSSL (thus, Google Chrome and everything
 * based on Chromium) also depend on it, and that libsodium allows the RNG to be
 * replaced without patching nor recompiling the library.
 */
#define RtlGenRandom SystemFunction036
#if defined(__cplusplus)
extern "C"
#endif
BOOLEAN NTAPI RtlGenRandom(PVOID RandomBuffer, ULONG RandomBufferLength);
#pragma comment(lib, "advapi32.lib")
#endif


namespace vnx {

void secure_random_bytes(void* data, const size_t length)
{
	if(!RtlGenRandom((PVOID)data, (ULONG)length))) {
		throw std::runtime_error("RtlGenRandom() failed");
	}
}


} // vnx
