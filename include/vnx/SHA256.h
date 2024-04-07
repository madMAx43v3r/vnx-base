/*
 * SHA256.h
 *
 *  Created on: 11.01.2014
 *      Author: mad
 *  Imported on: 30.11.2020
 *      Auther: jaw
 */


#ifndef INCLUDE_VNX_SHA256_H_
#define INCLUDE_VNX_SHA256_H_

#include <string>
#include <cstdint>


namespace vnx {

class SHA256 {
public:
	SHA256();

	void init();

	void update(const uint8_t* message, const uint32_t len);

	void finalize(uint8_t* digest);

	static constexpr uint32_t DIGEST_SIZE = (256 / 8);

private:
	void transform(const uint8_t* message, const uint32_t block_nb);

	static constexpr uint32_t SHA224_256_BLOCK_SIZE = (512 / 8);

	uint32_t m_tot_len;
	uint32_t m_len;
	uint32_t m_h[8];
	uint8_t  m_block[2 * SHA224_256_BLOCK_SIZE];
};

std::string sha256_str(const std::string& input, const bool lower_case = true);


} // vnx


#endif /* INCLUDE_VNX_SHA256_H_ */


