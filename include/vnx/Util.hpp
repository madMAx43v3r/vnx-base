/*
 * Util.hpp
 *
 *  Created on: Nov 30, 2020
 *      Author: jaw
 */

#ifndef INCLUDE_VNX_UTIL_HPP_
#define INCLUDE_VNX_UTIL_HPP_

#include <vnx/Util.h>
#include <vnx/Input.hpp>
#include <vnx/Generic.hxx>

#include <string>


namespace vnx {

template<typename T>
std::string string_join(const T& collection, const std::string& sep) {
	std::string result = "";
	auto iter = collection.begin();
	while(iter != collection.end()) {
		result += vnx::to_string_value(*iter);
		iter++;
		if(iter != collection.end()) {
			result += sep;
		}
	}
	return result;
}

/// Generic fall-back to reading at top level
template<typename T>
void read_generic(TypeInput& in, T& value) {
	if(auto generic = std::dynamic_pointer_cast<Generic>(vnx::read(in))) {
		generic->data.to(value);
	} else {
		value = T();
	}
}

/// Converts native integer to big-endian
template<typename T>
T to_big_endian(const T& value)
{
	if(is_big_endian()) {
		return value;
	}
	union {
		T out;
		uint8_t buf[sizeof(T)];
	} tmp;
	for(size_t i = 0; i < sizeof(T); ++i) {
		tmp.buf[i] = (value >> ((sizeof(T) - i - 1) * 8));
	}
	return tmp.out;
}

/// Converts native integer to little-endian
template<typename T>
T to_little_endian(const T& value)
{
	if(is_little_endian()) {
		return value;
	}
	union {
		T out;
		uint8_t buf[sizeof(T)];
	} tmp;
	for(size_t i = 0; i < sizeof(T); ++i) {
		tmp.buf[i] = (value >> (i * 8));
	}
	return tmp.out;
}

/// Converts big-endian to native integer
template<typename T>
T from_big_endian(const T& value)
{
	if(is_big_endian()) {
		return value;
	}
	union {
		T in;
		uint8_t buf[sizeof(T)];
	} tmp;
	tmp.in = value;
	T out = 0;
	for(size_t i = 0; i < sizeof(T); ++i) {
		out |= (T(tmp.buf[i]) << ((sizeof(T) - i - 1) * 8));
	}
	return out;
}

/// Converts little-endian to native integer
template<typename T>
T from_little_endian(const T& value)
{
	if(is_little_endian()) {
		return value;
	}
	union {
		T in;
		uint8_t buf[sizeof(T)];
	} tmp;
	tmp.in = value;
	T out = 0;
	for(size_t i = 0; i < sizeof(T); ++i) {
		out |= (T(tmp.buf[i]) << (i * 8));
	}
	return out;
}


} // vnx

#endif /* INCLUDE_VNX_UTIL_HPP_ */
