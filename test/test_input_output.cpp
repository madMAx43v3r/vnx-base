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

#include <vnx/vnx.h>
#include <vnx/Sample.hxx>
#include <vnx/Marker.hxx>
#include <vnx/permission_e.hxx>
#include <vnx/SHA256.h>
#include <vnx/File.h>
#include <vnx/Directory.h>
#include <vnx/Util.hpp>

#include <vnx/test/Test.h>
#include <vnx/test/TestValue.hxx>

using namespace vnx::test;
using namespace vnx;


template<typename T>
void test_integral_conversion(const T v) {
	const Variant value = Variant(v);
	expect(value.to<bool>(), bool(v));
	expect(value.to<uint8_t>(), uint8_t(v));
	expect(value.to<uint16_t>(), uint16_t(v));
	expect(value.to<uint32_t>(), uint32_t(v));
	expect(value.to<uint64_t>(), uint64_t(v));
	expect(value.to<int8_t>(), int8_t(v));
	expect(value.to<int16_t>(), int16_t(v));
	expect(value.to<int32_t>(), int32_t(v));
	expect(value.to<int64_t>(), int64_t(v));
	expect(value.to<float32_t>(), float32_t(v));
	expect(value.to<float64_t>(), float64_t(v));
}

void test_skip(const Variant& value) {
	if(!value.empty()) {
		VectorInputStream stream(&value.data);
		TypeInput in(&stream);
		const uint16_t code[] = {CODE_DYNAMIC};
		vnx::skip(in, nullptr, code);
	}
}

static const std::string test_string("asdfkjsdflksdlfsfsfgfdhfghhgjhjgkkmsdlfksdlkfsdlkf");


int main() {
	
	std::setlocale(LC_ALL, "de_DE.UTF-8");

	vnx::test::init("vnx.input_output");
	
	VNX_TEST_BEGIN("flip_bytes")
		expect(flip_bytes(uint8_t(0xAA)), 0xAA);
		expect(flip_bytes(uint16_t(0xAABB)), 0xBBAA);
		expect(flip_bytes(uint32_t(0xAABBCCDD)), 0xDDCCBBAA);
		expect(flip_bytes(uint64_t(0xAABBCCDDEEFF0011ull)), uint64_t(0x1100FFEEDDCCBBAAull));
		expect(flip_bytes(flip_bytes(float32_t(123.456f))), 123.456f);
		expect(flip_bytes(flip_bytes(float64_t(123.456))), 123.456);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("null_input")
		const Variant value = Variant();
		test_skip(value);
		expect(value.to<bool>(), false);
		expect(value.to<int>(), 0);
		expect(value.to<double>(), 0);
		expect(value.to<std::string>(), "");
		expect(value.to<std::array<int, 4>>()[0], 0);
		expect(value.to<std::vector<int>>().size(), size_t(0));
		expect(value.to<std::map<int, int>>().size(), size_t(0));
		expect(value.to<std::pair<int, int>>(), std::pair<int, int>(0, 0));
		expect(value.to<Object>().field.size(), size_t(0));
		expect(bool(value.to<std::shared_ptr<Value>>()), false);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("integral_conversion")
		test_integral_conversion(0);
		test_integral_conversion(false);
		test_integral_conversion(true);
		test_integral_conversion(1337);
		test_integral_conversion(-1337);
		test_integral_conversion(uint64_t(0x45d0ffa41b4b394eull));
		test_integral_conversion(int64_t(0x45d0ffa41b4b394eull));
		test_integral_conversion(123.456f);
		test_integral_conversion(-123.456);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("value")
		Sample value;
		value.hop_count = 11;
		value.topic = "test.topic";
		value.src_mac = Hash64::rand();
		value.seq_num = 123456789;
		value.value = Marker::create();
		const Variant tmp(value);
		test_skip(tmp);
		expect(bool(tmp.to<std::shared_ptr<Sample>>()), true);
		expect(tmp.to<std::shared_ptr<Sample>>()->hop_count, value.hop_count);
		expect(tmp.to<std::shared_ptr<Sample>>()->topic, value.topic);
		expect(tmp.to<std::shared_ptr<Sample>>()->src_mac, value.src_mac);
		expect(tmp.to<std::shared_ptr<Sample>>()->seq_num, value.seq_num);
		expect(bool(std::dynamic_pointer_cast<const Marker>(tmp.to<std::shared_ptr<Sample>>()->value)), true);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("buffer")
		Buffer buffer = test_string;
		const Variant tmp(buffer);
		test_skip(tmp);
		expect(tmp.to<Buffer>().as_string(), buffer.as_string());
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("memory")
		Memory memory;
		::memcpy(memory.add_chunk(test_string.size()), test_string.c_str(), test_string.size());
		::memcpy(memory.add_chunk(test_string.size()), test_string.c_str(), test_string.size());
		expect(memory.as_string(), (test_string + test_string));
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("string")
		const Variant tmp(test_string);
		test_skip(tmp);
		expect(tmp.to<std::string>(), test_string);
		expect(tmp.to<int>(), 0);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("integral_array")
		const std::array<int, 3> value = {-1, 1, 2};
		const Variant tmp(value);
		test_skip(tmp);
		expect(tmp.to<std::array<int, 3>>(), value);
		expect(tmp.to<std::array<int, 4>>()[2], value[2]);
		expect(tmp.to<std::array<int, 4>>()[3], 0);
		expect(tmp.to<std::array<int, 2>>()[1], value[1]);
		expect(tmp.to<std::array<int, 1>>()[0], value[0]);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("integral_vector")
		const std::vector<int> value = {-1, 1, 2};
		const Variant tmp(value);
		test_skip(tmp);
		expect(tmp.to<std::vector<int>>().size(), value.size());
		expect(tmp.to<std::vector<int>>()[0], value[0]);
		expect(tmp.to<std::vector<int>>()[2], value[2]);
		expect(tmp.to<std::vector<double>>()[1], value[1]);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("bool_vector")
		const std::vector<bool> value = {false, true, false};
		const Variant tmp(value);
		test_skip(tmp);
		expect(tmp.to<std::vector<bool>>().size(), value.size());
		expect((bool)tmp.to<std::vector<bool>>()[0], value[0]);
		expect((bool)tmp.to<std::vector<bool>>()[2], value[2]);
		expect(tmp.to<std::vector<int>>()[1], value[1]);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("value_vector")
		std::vector<Marker> value;
		value.emplace_back();
		value.back().message = "sdfsdfdsf";
		const Variant tmp(value);
		test_skip(tmp);
		expect(tmp.to<std::vector<int>>().size(), size_t(1));
		expect(tmp.to<std::vector<int>>()[0], 0);
		expect(tmp.to<std::vector<std::shared_ptr<Marker>>>().size(), value.size());
		expect(bool(tmp.to<std::vector<std::shared_ptr<Marker>>>()[0]), true);
		expect(tmp.to<std::vector<std::shared_ptr<Marker>>>()[0]->message, value[0].message);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("list")
		const std::list<int> value = {-1, 1, 2};
		const Variant tmp(value);
		test_skip(tmp);
		expect(tmp.to<std::list<int>>().size(), value.size());
		expect(tmp.to<std::vector<int>>()[0], -1);
		expect(tmp.to<std::vector<int>>()[2], 2);
		expect(tmp.to<std::vector<double>>()[1], 1);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("set")
		std::set<int> value;
		value.insert(-1);
		value.insert(1);
		const Variant tmp(value);
		test_skip(tmp);
		expect(tmp.to<std::set<int>>().size(), value.size());
		expect(tmp.to<std::set<int>>().count(-1), size_t(1));
		expect(tmp.to<std::set<int>>().count(1), size_t(1));
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("pair")
		std::pair<std::string, std::string> value;
		value.first = "sdfsdfsdfsdf";
		value.second = test_string;
		const Variant tmp(value);
		test_skip(tmp);
		expect(tmp.to<std::pair<std::string, std::string>>(), value);
		expect(tmp.to<std::pair<int, int>>(), std::pair<int, int>(0, 0));
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("string_map")
		std::map<std::string, int> value;
		value["test"] = 123;
		value["test2"] = 456;
		const Variant tmp(value);
		test_skip(tmp);
		expect(tmp.to<std::map<std::string, int>>().size(), value.size());
		expect(tmp.to<std::map<std::string, int>>()["test"], value["test"]);
		expect(tmp.to<std::map<std::string, int>>()["test2"], value["test2"]);
		expect(tmp.to<std::map<std::string, int64_t>>()["test"], value["test"]);
		expect(tmp.to<std::map<std::string, double>>()["test"], value["test"]);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("object")
		Object object;
		object["test"] = 1234;
		object["test2"] = test_string;
		const Variant tmp(object);
		test_skip(tmp);
		expect(tmp.to<Object>()["test"], object["test"]);
		expect(tmp.to<Object>()["test2"], object["test2"]);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("matrix")
		const uint16_t code[] = {CODE_MATRIX, 2, 2, 3, CODE_INT32};
		const int32_t value[6] = {1, 2, 3, 4, 5, 6};
		Memory data;
		{
			MemoryOutputStream stream(&data);
			TypeOutput out(&stream);
			write_matrix<int32_t, 2>(out, value, {2, 3}, code);
			out.flush();
		}
		{
			MemoryInputStream stream(&data);
			TypeInput in(&stream);
			vnx::skip(in, 0, code);
		}
		{
			MemoryInputStream stream(&data);
			TypeInput in(&stream);
			int32_t tmp[6] = {};
			read_matrix<int32_t, 2>(in, tmp, {2, 3}, code);
			for(int i = 0; i < 6; ++i) {
				expect(tmp[i], value[i]);
			}
		}
		{
			MemoryInputStream stream(&data);
			TypeInput in(&stream);
			int32_t tmp[4] = {1, 2, 3, 4};
			read_matrix<int32_t, 2>(in, tmp, {2, 2}, code);
			for(int i = 0; i < 4; ++i) {
				expect(tmp[i], 0);
			}
		}
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("image")
		const uint16_t code[] = {CODE_IMAGE, 2, CODE_INT32};
		const int32_t value[6] = {1, 2, 3, 4, 5, 6};
		Memory data;
		{
			MemoryOutputStream stream(&data);
			TypeOutput out(&stream);
			write_image<int32_t, 2>(out, value, {2, 3}, code);
			out.flush();
		}
		{
			MemoryInputStream stream(&data);
			TypeInput in(&stream);
			vnx::skip(in, 0, code);
		}
		{
			MemoryInputStream stream(&data);
			TypeInput in(&stream);
			int32_t tmp[6] = {};
			std::vector<size_t> size;
			read_image_size(in, size, code);
			expect(size.size(), size_t(2));
			expect(size[0], size_t(2));
			expect(size[1], size_t(3));
			read_image_data<int32_t>(in, tmp, size, code);
			for(int i = 0; i < 6; ++i) {
				expect(tmp[i], value[i]);
			}
		}
		{
			MemoryInputStream stream(&data);
			TypeInput in(&stream);
			std::vector<size_t> size;
			read_image_size(in, size, code);
			read_image_data<int32_t>(in, 0, size, code);
		}
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("from_string")
		{
			bool value = false;
			vnx::from_string("0", value);
			expect(value, false);
			vnx::from_string("1", value);
			expect(value, true);
			vnx::from_string("false", value);
			expect(value, false);
			vnx::from_string("true", value);
			expect(value, true);
			vnx::from_string("null", value);
			expect(value, false);
		}
		{
			char value = 0;
			vnx::from_string("0", value);
			expect(value, '0');
			vnx::from_string("1", value);
			expect(value, '1');
			vnx::from_string("-", value);
			expect(value, '-');
			vnx::from_string("-1", value);
			expect(value, '-');
			vnx::from_string("A", value);
			expect(value, 'A');
			vnx::from_string("*", value);
			expect(value, '*');
			vnx::from_string("null", value);
			expect(value, 'n');
		}
		{
			int8_t value = 0;
			vnx::from_string("1", value);
			expect(value, 1);
			vnx::from_string("-1", value);
			expect(value, -1);
			vnx::from_string("null", value);
			expect(value, int8_t(0));
		}
		{
			uint8_t value = 0;
			vnx::from_string("1", value);
			expect(value, 1);
			vnx::from_string("-1", value);
			expect(value, uint8_t(-1));
			vnx::from_string("null", value);
			expect(value, uint8_t(0));
		}
		{
			int32_t value = 0;
			vnx::from_string("1", value);
			expect(value, 1);
			vnx::from_string("-1", value);
			expect(value, -1);
			vnx::from_string("+1", value);
			expect(value, 1);
			vnx::from_string("null", value);
			expect(value, int32_t(0));
		}
		{
			uint32_t value = 0;
			vnx::from_string("1", value);
			expect(value, uint32_t(1));
			vnx::from_string("-1", value);
			expect(value, uint32_t(-1));
			vnx::from_string("null", value);
			expect(value, uint32_t(0));
		}
		{
			double value = 0;
			vnx::from_string("1", value);
			expect(value, 1);
			vnx::from_string("-1", value);
			expect(value, -1);
			vnx::from_string("0.1", value);
			expect(value, 0.1);
			vnx::from_string("0,1", value);
			expect(value, 0.1);
			vnx::from_string("-0.1", value);
			expect(value, -0.1);
			vnx::from_string("-0,1", value);
			expect(value, -0.1);
			vnx::from_string("1e3", value);
			expect(value, 1e3);
			vnx::from_string("1e-3", value);
			expect(value, 1e-3);
			vnx::from_string("1e+3", value);
			expect(value, 1e+3);
			vnx::from_string("1E-3", value);
			expect(value, 1e-3);
			vnx::from_string("1.0e3", value);
			expect(value, 1.0e3);
			vnx::from_string("-1.0e-3", value);
			expect(value, -1.0e-3);
			vnx::from_string("null", value);
			expect(value, 0);
		}
		{
			std::string value;
			vnx::from_string("\"A\\tB\nC\\n\"", value);
			expect(value, "A\tB\nC\n");
		}
		{
			std::vector<int> vector;
			vnx::from_string("null", vector);
			expect(vector.size(), size_t(1));
			expect(vector[0], 0);
		}
		{
			std::vector<int> vector;
			vnx::from_string("[]", vector);
			expect(vector.size(), size_t(0));
		}
		{
			std::vector<int> vector;
			vnx::from_string("[1]", vector);
			expect(vector.size(), size_t(1));
			expect(vector[0], 1);
		}
		{
			std::vector<int> vector;
			vnx::from_string("1", vector);
			expect(vector.size(), size_t(1));
			expect(vector[0], 1);
		}
		{
			std::vector<int> vector;
			vnx::from_string("-1", vector);
			expect(vector.size(), size_t(1));
			expect(vector[0], -1);
		}
		{
			std::vector<int> vector;
			vnx::from_string("[1,-1]", vector);
			expect(vector.size(), size_t(2));
			expect(vector[0], 1);
			expect(vector[1], -1);
		}
		{
			std::vector<std::string> vector;
			vnx::from_string("sdsdsd", vector);
			expect(vector.size(), size_t(1));
			expect(vector[0], "sdsdsd");
		}
		{
			std::vector<std::string> vector;
			vnx::from_string("[1, 2, 3]", vector);
			expect(vector.size(), size_t(3));
			expect(vector[0], "1");
			expect(vector[1], "2");
			expect(vector[2], "3");
		}
		{
			vnx::Variant value;
			vnx::from_string("0xabcdefABCDEF", value);
			expect(value.to<uint64_t>(), uint64_t(0xabcdefABCDEF));
			vnx::from_string("0x1abcdefABCDEF1", value);
			expect(value.to<uint64_t>(), uint64_t(0x1abcdefABCDEF1));
		}
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("to_string")
		expect(vnx::to_string(true), "true");
		expect(vnx::to_string(false), "false");
		expect(vnx::to_string('A'), "A");
		expect(vnx::to_string(0), "0");
		expect(vnx::to_string(1), "1");
		expect(vnx::to_string(-1), "-1");
#ifndef _MSC_VER
		expect(vnx::to_string<double>(0./0.), "null");
		expect(vnx::to_string<double>(1./0.), "null");
		expect(vnx::to_string<float>(0.f/0.f), "null");
		expect(vnx::to_string<float>(1.f/0.f), "null");
#endif
		expect(vnx::to_string(std::string("[\\asasa\"]")), "\"[\\\\asasa\\\"]\"");
		expect(vnx::to_string(std::string("A\tB\nC")), "\"A\\tB\\nC\"");
		expect(vnx::to_string(vnx::Variant()), "null");
		expect(vnx::to_string(vnx::Variant(true)), "true");
		expect(vnx::to_string(vnx::Variant(false)), "false");
		expect(vnx::to_string(vnx::Variant(1337)), "1337");
		expect(vnx::to_string(std::shared_ptr<vnx::Value>()), "null");
		{
			std::vector<bool> tmp {true, false, true};
			expect(vnx::to_string(tmp), "[true, false, true]");
		}
	VNX_TEST_END()

	VNX_TEST_BEGIN("to_from_object")
		auto value = Sample::create();
		value->seq_num = 1337;
		auto marker = Marker::create();
		marker->message = "test";
		value->value = marker;
		auto object = value->to_object();
		expect(object["seq_num"].to<int>(), 1337);
		{
			auto marker2 = object["value"].to<std::shared_ptr<Marker>>();
			expect(bool(marker2), true);
			expect(marker2->message, marker->message);
		}
		{
			auto value2 = std::dynamic_pointer_cast<Sample>(object.to_value());
			expect(bool(value2), true);
			expect(bool(value2->value), true);
			expect(value2->seq_num, value->seq_num);
		}
		{
			const auto str = value->to_string();
			std::shared_ptr<Value> value2;
			vnx::from_string(str, value2);
			expect(bool(value2), true);
			auto sample2 = std::dynamic_pointer_cast<Sample>(value2);
			expect(bool(sample2), true);
			expect(bool(sample2->value), true);
			expect(sample2->seq_num, value->seq_num);
		}
	VNX_TEST_END()

	VNX_TEST_BEGIN("enum")
		{
			permission_e value;
			expect(value, uint32_t(0));
			expect(value.value, uint32_t(0));
			expect(value.to_string(), "0");
			expect(value.to_string_value(), "0");
			expect(vnx::to_string(value), "0");
			expect(vnx::to_string_value(value), "0");
			{
				const vnx::Variant tmp(value);
				expect(tmp.is_ulong(), true);
				expect(tmp.to<int>(), 0);
			}
		}
		{
			permission_e value = permission_e::enum_t(1337);
			expect(value.to_string(), "1337");
			expect(value.to_string_value(), "1337");
			{
				const vnx::Variant tmp(value);
				expect(tmp.is_ulong(), true);
				expect(tmp.to<int>(), 1337);
			}
		}
		{
			permission_e value = permission_e::INTERNAL;
			expect(value.to_string(), "\"INTERNAL\"");
			expect(value.to_string_value(), "INTERNAL");
			expect(value.to_string_value_full(), "vnx.permission_e.INTERNAL");
			expect(vnx::to_string(value), "\"INTERNAL\"");
			expect(vnx::to_string_value(value), "INTERNAL");
			{
				const vnx::Variant tmp(value);
				expect(tmp.is_string(), true);
				expect(tmp.to<std::string>(), "INTERNAL");
			}
			{
				permission_e tmp;
				tmp.from_string(value.to_string());
				expect(tmp, permission_e::INTERNAL);
			}
			{
				permission_e tmp;
				tmp.from_string_value(value.to_string_value());
				expect(tmp, permission_e::INTERNAL);
			}
		}
		{
			permission_e value;
			vnx::from_string("0", value);
			expect(value, uint32_t(0));
		}
		{
			permission_e value;
			vnx::from_string("1337", value);
			expect(value, uint32_t(1337));
		}
		{
			permission_e value;
			vnx::from_string("INTERNAL", value);
			expect(value, permission_e::INTERNAL);
		}
		{
			permission_e value;
			vnx::from_string("\"INTERNAL\"", value);
			expect(value, permission_e::INTERNAL);
		}
		{
			permission_e value;
			vnx::from_string_value("INTERNAL", value);
			expect(value, permission_e::INTERNAL);
		}
		{
			permission_e value;
			vnx::from_string("__FOOBAR__", value);
			expect(value, uint32_t(vnx::hash64("__FOOBAR__")));
		}
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("vector_output")
		std::vector<uint8_t> vector;
		VectorOutputStream stream(&vector);
		stream.write("abc", 3);
		stream.write("def", 3);
		expect(vector.size(), size_t(6));
		expect(std::string((const char*)vector.data(), 6), "abcdef");
	VNX_TEST_END()

	VNX_TEST_BEGIN("type_code")
		const auto* type_code = vnx::Sample::static_get_type_code();
		const Variant tmp(*type_code);
		test_skip(tmp);
		{
			TypeCode test;
			tmp.to(test);
			expect(test.code_hash, type_code->code_hash);
			expect(test.type_hash, type_code->type_hash);
			expect(test.fields.size(), type_code->fields.size());
		}
		{
			std::vector<uint16_t> code;
			vnx::create_dynamic_code(code, int(1));
			expect(vnx::code_to_string(code.data()), "int");
		}
		{
			std::vector<uint16_t> code;
			vnx::create_dynamic_code(code, std::string());
			expect(vnx::code_to_string(code.data()), "string");
		}
		{
			std::vector<uint16_t> code;
			vnx::create_dynamic_code(code, std::array<int, 4>());
			expect(vnx::code_to_string(code.data()), "int[4]");
		}
		{
			std::vector<uint16_t> code;
			vnx::create_dynamic_code(code, std::vector<int>());
			expect(vnx::code_to_string(code.data()), "vector<int>");
		}
		{
			std::vector<uint16_t> code;
			vnx::create_dynamic_code(code, std::pair<int, std::string>());
			expect(vnx::code_to_string(code.data()), "pair<int, string>");
		}
		{
			std::vector<uint16_t> code;
			vnx::create_dynamic_code(code, std::map<int, std::string>());
			expect(vnx::code_to_string(code.data()), "map<int, string>");
		}
		{
			expect(vnx::code_to_string(type_code->fields[7].code.data(), type_code), "vnx.Value*");
		}
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("get_field")
		{
			Sample value;
			value.src_mac = Hash64(1337);
			value.topic = "test.topic";
			expect(value.get_field("src_mac").to<Hash64>(), value.src_mac);
			expect(value.get_field("topic").to<std::string>(), value.topic->get_name());

			Binary binary;
			binary.type_code = value.get_type_code();
			{
				MemoryOutputStream stream(&binary.data);
				TypeOutput out(&stream);
				vnx::write(out, value, value.get_type_code(), nullptr);
				out.flush();
			}
			expect(binary.get_field("src_mac").to<Hash64>(), value.src_mac);
			expect(binary.get_field("topic").to<std::string>(), value.topic->get_name());
		}
	VNX_TEST_END()

	VNX_TEST_BEGIN("set_field")
		{
			Sample value;
			value.set_field("src_mac", Variant(1337));
			value.set_field("topic", Variant("test.topic"));
			expect(value.src_mac, Hash64(1337));
			expect(value.topic->get_name(), std::string("test.topic"));
		}
		{
			Object value;
			value.set_field("src_mac", Variant(1337));
			value.set_field("topic", Variant("test.topic"));
			expect(value["src_mac"].to<Hash64>(), Hash64(1337));
			expect(value["topic"].to<std::string>(), std::string("test.topic"));
		}
	VNX_TEST_END()

	VNX_TEST_BEGIN("std_input_output")
	{
		{
			std::vector<uint8_t> data;
			vnx::VectorOutputStream stream_out(&data);
			vnx::VectorInputStream stream_in(&data);
			vnx::ostream out(&stream_out);
			vnx::istream in(&stream_in);

			Sample value;
			value.topic = "test.topic";
			out << value;
			out.flush();

			Sample tmp;
			vnx::read(in, tmp);
			expect(tmp.topic->get_name(), "test.topic");
		}
		{
			std::vector<uint8_t> data;
			vnx::VectorOutputStream stream_out(&data);
			vnx::VectorInputStream stream_in(&data);
			vnx::ostream out(&stream_out);
			vnx::istream in(&stream_in);

			std::vector<Object> list;
			list.resize(2);
			list[0]["test"] = "test1";
			list[1]["test"] = "test2";
			out << vnx::to_string(list);
			out.flush();

			std::vector<Object> tmp;
			vnx::read(in, tmp);
			expect(tmp.size(), size_t(2));
			expect(tmp[0]["test"].to<std::string>(), "test1");
			expect(tmp[1]["test"].to<std::string>(), "test2");
		}
		{
			std::vector<uint8_t> data;
			vnx::VectorOutputStream stream_out(&data);
			vnx::VectorInputStream stream_in(&data);
			vnx::ostream out(&stream_out);
			vnx::istream in(&stream_in);

			vnx::Variant value("test");
			out << value;
			out.flush();

			std::string tmp;
			vnx::read(in, tmp);
			expect(tmp, "test");
		}
		{
			std::vector<uint8_t> data;
			vnx::VectorOutputStream stream_out(&data);
			vnx::VectorInputStream stream_in(&data);
			vnx::ostream out(&stream_out);
			vnx::istream in(&stream_in);

			vnx::Variant value(-1337);
			out << value;
			out.flush();

			int tmp = 0;
			vnx::read(in, tmp);
			expect(tmp, -1337);
		}
	}
	VNX_TEST_END()

	VNX_TEST_BEGIN("binary_to_object")
	{
		Sample value;
		value.seq_num = 1337;
		value.topic = "test.topic";
		auto object = Object::create();
		(*object)["test"] = "value";
		(*object)["test2"] = std::vector<int>{1, 3, 3, 7};
		value.value = object;

		Binary binary;
		binary.type_code = value.get_type_code();
		BinaryOutputStream stream(&binary);
		TypeOutput out(&stream);
		vnx::write(out, value, value.get_type_code(), nullptr);
		out.flush();

		const auto result = binary.to_object();
		expect(result["seq_num"], value.seq_num);
		expect(result["topic"], value.topic->get_name());
		expect(result["value"].is_object(), true);
		expect(result["value"].to_object()["test"], (*object)["test"]);
		expect(result["value"].to_object()["test2"], (*object)["test2"]);
	}
	VNX_TEST_END()

	VNX_TEST_BEGIN("test_value")
	{
		TestValue value;
		value.tuple_ = std::make_tuple<int, float, double, std::string>(12, 34.56, 78.90, "Hello World");
		value.vector_ = {1, 2, 3, 4};
		value.variant_ = "test string";
		value.user_vector_.resize(3);
		value.variant_vector_.resize(2);
		value.variant_vector_[1] = 1337;
		value.enum_vector_.emplace_back(permission_e::INTERNAL);
		value.enum_vector_.emplace_back(permission_e::HOST_SHUTDOWN);
		value.set_.emplace("test");
		value.set_.emplace("test1");
		value.map_["test"] = 1337;
		value.map_["test1"] = -1337;
		value.pointer_ = User::create();
		value.optional_struct_ = test_struct_t();
		value.optional_enum_ = permission_e::INTERNAL;

		Memory data;
		{
			MemoryOutputStream stream(&data);
			TypeOutput out(&stream);
			vnx::write(out, value);
			out.flush();
		}
		{
			MemoryInputStream stream(&data);
			TypeInput in(&stream);
			vnx::skip(in);
		}
		MemoryInputStream stream(&data);
		TypeInput in(&stream);
		auto result = TestValue::create();
		result->vector_ = {99, 999};
		result->set_.emplace("old");
		result->map_["old"] = -1;
		result->optional_int_ = 1337;
		vnx::read(in, result);
		expect(bool(result), true);
		expect(result->tuple_, value.tuple_);
		expect(result->vector_, value.vector_);
		expect(result->variant_, value.variant_);
		expect(result->user_vector_.size(), value.user_vector_.size());
		expect(result->variant_vector_.size(), value.variant_vector_.size());
		expect(result->enum_vector_.size(), value.enum_vector_.size());
		expect(result->set_.size(), value.set_.size());
		expect(result->map_.size(), value.map_.size());
		expect(result->set_.count("test"), size_t(1));
		expect(result->map_["test"], value.map_["test"]);
		expect(bool(result->pointer_), true);
		expect(bool(result->optional_int_), false);
		expect(bool(result->optional_struct_), true);
		expect(bool(result->optional_enum_), true);
		expect(result->optional_int_, value.optional_int_);
		expect(result->optional_enum_, value.optional_enum_);
	}
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("is_equivalent")
	{
		const std::vector<uint16_t> code {CODE_TUPLE, 2, 4, 5, CODE_UINT32, CODE_FLOAT};
		expect(is_equivalent<std::pair<uint32_t, float>>{}(code.data(), nullptr), true);
		expect(is_equivalent<std::pair<int32_t, float>>{}(code.data(), nullptr), true);
		expect(is_equivalent<std::pair<uint32_t, uint32_t>>{}(code.data(), nullptr), false);
		expect(is_equivalent<std::pair<float, float>>{}(code.data(), nullptr), false);
	}
	{
		expect(TestValue::static_get_type_code()->is_equivalent, false);
		expect(test_struct_t::static_get_type_code()->is_equivalent, true);
	}
	{
		const TypeCode* type_code = TestValue::static_get_type_code();
		for(const auto& field : type_code->fields) {
			const uint16_t* code = field.code.data();
			const bool test = code[0] == CODE_STRUCT && type_code->depends[code[1]]->type_hash == test_struct_t::VNX_TYPE_HASH;
			expect(is_equivalent<test_struct_t>{}(code, type_code), test);
		}
	}
	VNX_TEST_END()

	VNX_TEST_BEGIN("sha256")
	{
		expect(vnx::sha256_str("", true), std::string("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"));
		expect(vnx::sha256_str("abc", true), std::string("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"));
		expect(vnx::sha256_str(std::string(1000000, 'a'), true), std::string("cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0"));
	}
	VNX_TEST_END()

	VNX_TEST_BEGIN("secure_random_bytes")
	{
		std::vector<uint8_t> buf(32);
		vnx::secure_random_bytes(buf.data(), buf.size());
		expect(buf == std::vector<uint8_t>(32), false);

		std::vector<uint8_t> buf1(32);
		vnx::secure_random_bytes(buf1.data(), buf1.size());
		expect(buf == buf1, false);
	}
	VNX_TEST_END()

	VNX_TEST_BEGIN("safe_read")
	{
		{
			vnx::Memory buffer;
			vnx::MemoryOutputStream ostream(&buffer);
			vnx::TypeOutput out(&ostream);

			std::string data;
			for(int i = 0; i < 4096; ++i) {
				data += "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";
			}
			vnx::write_dynamic(out, data);

			vnx::MemoryInputStream istream(&buffer);
			vnx::TypeInput in(&istream);
			in.safe_read = true;

			std::string tmp;
			vnx::read_dynamic(in, tmp);
			expect(tmp == data, true);
		}
		{
			vnx::Memory buffer;
			vnx::MemoryOutputStream ostream(&buffer);
			vnx::TypeOutput out(&ostream);

			std::vector<uint32_t> data;
			for(int i = 0; i < 16 * 1024; ++i) {
				data.push_back(i);
			}
			vnx::write_dynamic(out, data);

			vnx::MemoryInputStream istream(&buffer);
			vnx::TypeInput in(&istream);
			in.safe_read = true;

			std::vector<uint32_t> tmp;
			vnx::read_dynamic(in, tmp);
			expect(tmp == data, true);
		}
	}
	VNX_TEST_END()

	VNX_TEST_BEGIN("from_hex_string")
	{
		expect(vnx::from_hex_string("0x"), std::vector<uint8_t>{});
		expect(vnx::from_hex_string("00"), std::vector<uint8_t>{0x00});
		expect(vnx::from_hex_string("0xAA"), std::vector<uint8_t>{0xAA});
		expect(vnx::from_hex_string("0x00FF"), std::vector<uint8_t>{0x00, 0xFF});
		expect(vnx::from_hex_string("0x0123456789abcdefABCDEF"), std::vector<uint8_t>{0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xAB, 0xCD, 0xEF});
		expect(vnx::from_hex_string("0x00123456789abcdefABCDEF0"), std::vector<uint8_t>{0x00, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xfA, 0xBC, 0xDE, 0xF0});
	}
	VNX_TEST_END()

	VNX_TEST_BEGIN("File::get_name()")
	{
		{
			vnx::File file("/test.dat");
			expect(file.get_name(false), "test");
			expect(file.get_name(true), "test.dat");
		}
		{
			vnx::File file("//test.dat");
			expect(file.get_name(false), "test");
			expect(file.get_name(true), "test.dat");
		}
		{
			vnx::File file("//tmp/foo//test.dat");
			expect(file.get_name(false), "test");
			expect(file.get_name(true), "test.dat");
		}
		{
			vnx::File file("test.dat");
			expect(file.get_name(false), "test");
			expect(file.get_name(true), "test.dat");
		}
		{
			vnx::File file("test.dat/");
			expect(file.get_name(false), "test");
			expect(file.get_name(true), "test.dat");
		}
		{
			vnx::File file("test.dat//");
			expect(file.get_name(false), "test");
			expect(file.get_name(true), "test.dat");
		}
		{
			vnx::File file("C:");
			expect(file.get_name(false), "C:");
			expect(file.get_name(true), "C:");
		}
		{
			vnx::File file("C:\\");
			expect(file.get_name(false), "C:");
			expect(file.get_name(true), "C:");
		}
		{
			vnx::File file("C:\\test.dat");
			expect(file.get_name(false), "test");
			expect(file.get_name(true), "test.dat");
		}
		{
			vnx::File file("C:\\\\test.dat");
			expect(file.get_name(false), "test");
			expect(file.get_name(true), "test.dat");
		}
		{
			vnx::File file("C:\\\\tmp\\foo\\\\test.dat");
			expect(file.get_name(false), "test");
			expect(file.get_name(true), "test.dat");
		}
		{
			vnx::File file("test.dat");
			expect(file.get_name(false), "test");
			expect(file.get_name(true), "test.dat");
		}
		{
			vnx::File file("test.dat\\");
			expect(file.get_name(false), "test");
			expect(file.get_name(true), "test.dat");
		}
		{
			vnx::File file("test.dat\\\\");
			expect(file.get_name(false), "test");
			expect(file.get_name(true), "test.dat");
		}
	}
	VNX_TEST_END()

	VNX_TEST_BEGIN("File::get_extension()")
	{
		{
			vnx::File file("test.dat");
			expect(file.get_extension(), ".dat");
		}
		{
			vnx::File file("/test.dat");
			expect(file.get_extension(), ".dat");
		}
		{
			vnx::File file("/tmp/test.dat");
			expect(file.get_extension(), ".dat");
		}
		{
			vnx::File file("C:/test.dat");
			expect(file.get_extension(), ".dat");
		}
		{
			vnx::File file("C:\\test.dat");
			expect(file.get_extension(), ".dat");
		}
		{
			vnx::File file("C:/tmp/test.dat");
			expect(file.get_extension(), ".dat");
		}
		{
			vnx::File file("C:\\tmp\\test.dat");
			expect(file.get_extension(), ".dat");
		}
	}
	VNX_TEST_END()

	VNX_TEST_BEGIN("Directory::get_path()")
	{
#ifdef _WIN32
		{
			vnx::Directory dir("\\");
			expect(dir.get_path(), "\\");
		}
		{
			vnx::Directory dir("\\\\\\");
			expect(dir.get_path(), "\\");
		}
		{
			vnx::Directory dir("C:");
			expect(dir.get_path(), "C:\\");
		}
		{
			vnx::Directory dir("C:\\");
			expect(dir.get_path(), "C:\\");
		}
		{
			vnx::Directory dir("C:\\tmp");
			expect(dir.get_path(), "C:\\tmp\\");
		}
		{
			vnx::Directory dir("C:\\tmp\\\\\\");
			expect(dir.get_path(), "C:\\tmp\\");
		}
#else
		{
			vnx::Directory dir("/");
			expect(dir.get_path(), "/");
		}
		{
			vnx::Directory dir("///");
			expect(dir.get_path(), "/");
		}
		{
			vnx::Directory dir("/tmp");
			expect(dir.get_path(), "/tmp/");
		}
		{
			vnx::Directory dir("/tmp///");
			expect(dir.get_path(), "/tmp/");
		}
#endif
	}
	VNX_TEST_END()

	VNX_TEST_BEGIN("Directory::get_name()")
	{
#ifdef _WIN32
		{
			vnx::Directory dir("\\");
			expect(dir.get_name(), "\\");
		}
		{
			vnx::Directory dir("\\\\\\");
			expect(dir.get_name(), "\\");
		}
		{
			vnx::Directory dir("C:");
			expect(dir.get_name(), "C:");
		}
		{
			vnx::Directory dir("C:\\");
			expect(dir.get_name(), "C:");
		}
		{
			vnx::Directory dir("C:\\tmp");
			expect(dir.get_name(), "tmp");
		}
		{
			vnx::Directory dir("C:\\tmp\\\\\\");
			expect(dir.get_name(), "tmp");
		}
#else
		{
			vnx::Directory dir("/");
			expect(dir.get_name(), "/");
		}
		{
			vnx::Directory dir("///");
			expect(dir.get_name(), "/");
		}
		{
			vnx::Directory dir("/tmp");
			expect(dir.get_name(), "tmp");
		}
		{
			vnx::Directory dir("/tmp///");
			expect(dir.get_name(), "tmp");
		}
#endif
	}
	VNX_TEST_END()

	VNX_TEST_BEGIN("to_big_endian()")
	{
		expect(to_big_endian<uint8_t>(0x11), 0x11);
		const auto test_16 = to_big_endian<uint16_t>(0xFF00);
		expect(to_hex_string(&test_16, 2, false, false), "FF00");
		const auto test_32 = to_big_endian<uint32_t>(0xAABB0000);
		expect(to_hex_string(&test_32, 4, false, false), "AABB0000");
		const auto test_64 = to_big_endian<uint64_t>(0xAABB0000);
		expect(to_hex_string(&test_64, 8, false, false), "00000000AABB0000");
	}
	VNX_TEST_END()

	VNX_TEST_BEGIN("to_little_endian()")
	{
		expect(to_little_endian<uint8_t>(0x11), 0x11);
		const auto test_16 = to_little_endian<uint16_t>(0xFF00);
		expect(to_hex_string(&test_16, 2, true, false), "FF00");
		const auto test_32 = to_little_endian<uint32_t>(0xAABB0000);
		expect(to_hex_string(&test_32, 4, true, false), "AABB0000");
		const auto test_64 = to_little_endian<uint64_t>(0xAABB0000);
		expect(to_hex_string(&test_64, 8, true, false), "00000000AABB0000");
	}
	VNX_TEST_END()

	VNX_TEST_BEGIN("from_big_endian()")
	{
		expect(from_big_endian(to_big_endian<uint8_t>(0x11)), 0x11);
		expect(from_big_endian(to_big_endian<uint16_t>(0xFF00)), 0xFF00);
		expect(from_big_endian(to_big_endian<uint32_t>(0xAABB0000)), 0xAABB0000);
		expect(from_big_endian(to_big_endian<uint64_t>(0xAABB0000)), 0xAABB0000);
	}
	VNX_TEST_END()

	VNX_TEST_BEGIN("from_little_endian()")
	{
		expect(from_little_endian(to_little_endian<uint8_t>(0x11)), 0x11);
		expect(from_little_endian(to_little_endian<uint16_t>(0xFF00)), 0xFF00);
		expect(from_little_endian(to_little_endian<uint32_t>(0xAABB0000)), 0xAABB0000);
		expect(from_little_endian(to_little_endian<uint64_t>(0xAABB0000)), 0xAABB0000);
	}
	VNX_TEST_END()

	vnx::static_cleanup();

	return vnx::test::done();
}



