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

#include <vnx/test/Test.h>

using namespace vnx::test;


int main(int argc, char** argv) {
	
	std::setlocale(LC_ALL, "de_DE.UTF-8");

	std::cerr << std::endl;

	vnx::test::init("vnx.config");
	
	vnx::init("test_config", argc, argv);
	
	VNX_TEST_BEGIN("files")
		{
			bool value = true;
			vnx::read_config("bool", value);
			expect(value, false);
		}
		{
			int value = -1;
			vnx::read_config("value", value);
			expect(value, 1337);
		}
		{
			std::string value;
			vnx::read_config("string", value);
			expect(value, "sdkjfhsdkjf");
		}
		{
			std::string value;
			vnx::read_config("string2", value);
			expect(value, "[{sdk jfh\"sdk\\jf]}");
		}
		{
			int value = -1;
			vnx::read_config("test.value", value);
			expect(value, 123456);
		}
		{
			std::string text;
			vnx::read_config("text", text);
			expect(text, "skda lsndj asnd [] {\"\": 0123}");
		}
		{
			std::vector<std::string> array;
			vnx::read_config("slist", array);
			expect(array.size(), size_t(4));
			expect(array[0], "1");
			expect(array[1], "2");
			expect(array[2], "3");
			expect(array[3], "test test");
		}
		{
			std::vector<vnx::Variant> array;
			vnx::read_config("jarray", array);
			expect(array.size(), size_t(4));
			expect(array[0].to<int>(), 1);
			expect(array[1].to<int>(), 2);
			expect(array[2].to<int>(), 3);
			expect(array[3].to<std::string>(), "test test");
		}
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("json")
		{
			bool value = true;
			vnx::read_config("Test.bool", value);
			expect(value, false);
		}
		{
			bool value = true;
			vnx::read_config("Test.false", value);
			expect(value, false);
		}
		{
			bool value = true;
			vnx::read_config("Test.null", value);
			expect(value, false);
		}
		{
			int value = 1;
			vnx::read_config("Test.null", value);
			expect(value, 0);
		}
		{
			int value = -1;
			vnx::read_config("Test.test", value);
			expect(value, 1337);
		}
		{
			bool value = false;
			vnx::read_config("Test.test", value);
			expect(value, true);
		}
		{
			int value = 1;
			vnx::read_config("Test.negative", value);
			expect(value, -1337);
		}
		{
			double value = 1;
			vnx::read_config("Test.double", value);
			expect(value, 123.456);
		}
		{
			bool value = false;
			vnx::read_config("Test.double", value);
			expect(value, true);
		}
		{
			std::string value;
			vnx::read_config("Test.string", value);
			expect(value, "[{sdk jfh\"sdk\\jf]}");
		}
		{
			std::vector<int> value;
			vnx::read_config("Test.value.array", value);
			expect(value.size(), size_t(4));
			expect(value[0], 1);
			expect(value[1], 2);
			expect(value[2], 3);
			expect(value[3], 4);
		}
		{
			std::map<std::string, std::string> map;
			vnx::read_config("Test.map", map);
			expect(map.size(), size_t(2));
			expect(map["key"], "value");
			expect(map["key2"], "value2");
		}
		{
			std::map<std::string, std::string> map;
			vnx::read_config("Test.map2", map);
			expect(map.size(), size_t(2));
			expect(map["key"], "value");
			expect(map["key2"], "value2");
		}
		{
			std::map<std::string, vnx::Object> map;
			vnx::read_config("Test.map3", map);
			expect(map.size(), size_t(1));
			expect(map["nested"]["key"].to<int>(), 1337);
		}
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("value")
		{
			vnx::Sample tmp;
			vnx::read_config("sample", tmp);
			expect(tmp.seq_num, uint64_t(1337));
			expect(tmp.recv_time, int64_t(-1337));
			expect(tmp.topic->get_name(), "test.topic");
		}
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("append")
	{
		std::vector<int> tmp;
		vnx::read_config("array", tmp);
		expect(tmp.size(), size_t(6));
		for(size_t i = 0; i < tmp.size(); ++i) {
			expect(tmp[i], int(i + 1));
		}
	}
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("parse_args")
	{
		const std::map<std::string, std::string> options {
			std::make_pair("c", "config"),
			std::make_pair("d", "debug"),
			std::make_pair("1", "one")
		};
		const std::vector<std::string> args {"abcd", "-c", "config/dir/", "config/dir2/", "-d", "-1", "--some", "-2", "-3", "--Module.config", "1 2 3", "--test", "\"-1337\"", "--bool"};
		const auto out = vnx::parse_args(args, options);
		expect(out.size(), size_t(8));
		std::map<std::string, std::vector<vnx::Variant>> map(out.begin(), out.end());
		expect(map[""].size(), size_t(1));
		expect(map["c"].size(), size_t(0));
		expect(map["d"].size(), size_t(0));
		expect(map["config"].size(), size_t(2));
		expect(map["config"][0].to<std::string>(), "config/dir/");
		expect(map["config"][1].to<std::string>(), "config/dir2/");
		expect(map["debug"].size(), size_t(0));
		expect(map["one"].size(), size_t(0));
		expect(map["some"].size(), size_t(2));
		expect(map["Module.config"].size(), size_t(1));
		expect(map["test"].size(), size_t(1));
		expect(map["bool"].size(), size_t(0));
		expect(map[""].front(), std::string("abcd"));
		expect(map["config"].front(), std::string("config/dir/"));
		expect(map["some"][0], -2);
		expect(map["some"][1], -3);
		expect(map["Module.config"].front(), std::string("1 2 3"));
		expect(map["test"].front(), std::string("-1337"));
	}
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("parse_args_json")
	{
		const std::vector<std::string> args {"--test", "[", "1,", "2", ",", "3]", "--some", "[", "test 123", ",", "-1337]", "--object", "{test:[1,2,3]}"};
		const auto out = vnx::parse_args(args);
		expect(out.size(), size_t(3));
		expect(out[0].first, std::string("test"));
		expect(out[1].first, std::string("some"));
		expect(out[2].first, std::string("object"));
		expect(out[0].second.size(), size_t(1));
		expect(out[1].second.size(), size_t(1));
		expect(out[2].second.size(), size_t(1));
		expect(out[0].second[0], std::vector<int>{1, 2, 3});
		expect(out[1].second[0], std::vector<std::string>{"test 123", "-1337"});
		expect(out[2].second[0].to_object()["test"], std::vector<int>{1, 2, 3});
	}
	VNX_TEST_END()

	vnx::close();
	
	return vnx::test::done();
}
