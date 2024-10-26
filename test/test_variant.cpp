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
#include <vnx/record_index_entry_t.hxx>
#include <vnx/Sample.hxx>
#include <vnx/Endpoint.hxx>

#include <vnx/test/Test.h>

using namespace vnx::test;
using namespace vnx;


void test_comparison(const Variant& value) {
	expect(value == value, true);
	expect(value != value, false);
	expect(value < value, false);
	expect(value > value, false);
}

void test_calc_hash(const Variant& value) {
	expect(value.get_hash() == 0, value.empty());
}

int main() {
	
	std::setlocale(LC_ALL, "de_DE.UTF-8");

	vnx::test::init("vnx.variant");
	
	VNX_TEST_BEGIN("memory")
		Memory memory;
		memset(memory.add_chunk(8), 1, 8);
		expect(memory.get_size(), size_t(8));
		memset(memory.add_chunk(8), 1, 8);
		expect(memory.get_size(), size_t(16));
		memset(memory.add_chunk(48), 1, 48);
		expect(memory.get_size(), size_t(64));
		{
			MemoryInputStream stream(&memory);
			InputBuffer in(&stream);
			for(int i = 0; i < 64; ++i) {
				char byte = 0;
				in.read(&byte, 1);
				expect(int(byte), 1);
			}
		}
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("empty")
	{
		const Variant value;
		std::cout << "[empty] sizeof(Variant) = " << sizeof(Variant) << std::endl;
		std::cout << "[empty] value.data.size() = " << value.data.size() << std::endl;
		test_comparison(value);
		test_calc_hash(value);
		expect(value.empty(), true);
		expect(value.is_null(), true);
		expect(value.is_bool(), false);
		expect(int(value), 0);
		expect(value.to<int>(), 0);
		expect(value.to<std::string>(), "");
		expect(value.to<std::shared_ptr<Value>>(), std::shared_ptr<Value>());
	}
	VNX_TEST_END()

	VNX_TEST_BEGIN("null")
	{
		const Variant value(nullptr);
		Variant other;
		value.to(other);
		ToObject visitor;
		vnx::accept(visitor, value);
		expect(value.empty(), false);
		expect(other.empty(), false);
		expect(value.is_null(), true);
		expect(other.is_null(), true);
		expect(visitor.get_value().empty(), false);
		expect(visitor.get_value().is_null(), true);
	}
	VNX_TEST_END()
		
	VNX_TEST_BEGIN("bool")
		Variant value(true);
		std::cout << "[bool] value.data.size() = " << value.data.size() << std::endl;
		std::cout << "[bool] value = " << value << std::endl;
		test_comparison(value);
		test_calc_hash(value);
		expect(value.is_bool(), true);
		expect(value.is_long(), false);
		expect(value.is_ulong(), false);
		expect(value.is_double(), false);
		expect(int(value), 1);
		expect(bool(value), true);
		expect(value.to<Variant>().to<bool>(), true);
		value = false;
		expect(int(value), 0);
		expect(bool(value), false);
		{
			std::stringstream in;
			in.str("1");
			vnx::read(in, value);
			expect(bool(value), true);
		}
		{
			std::stringstream in;
			in.str("0");
			vnx::read(in, value);
			expect(bool(value), false);
		}
		{
			std::stringstream in;
			in.str("true");
			vnx::read(in, value);
			expect(bool(value), true);
		}
		{
			std::stringstream in;
			in.str("false");
			vnx::read(in, value);
			expect(bool(value), false);
		}
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("integer")
		Variant value(1337);
		std::cout << "[integer] value.data.size() = " << value.data.size() << std::endl;
		std::cout << "[integer] value = " << value << std::endl;
		test_comparison(value);
		test_calc_hash(value);
		expect(value.is_long(), true);
		expect(value.is_ulong(), true);
		expect(value.is_double(), false);
		expect(int(value), 1337);
		expect(value.to<Variant>().to<int>(), 1337);
		value = 66;
		expect(int(value), 66);
	VNX_TEST_END()

	VNX_TEST_BEGIN("neg_integer")
		Variant value(-1337);
		std::cout << "[neg_integer] value.data.size() = " << value.data.size() << std::endl;
		std::cout << "[neg_integer] value = " << value << std::endl;
		test_comparison(value);
		test_calc_hash(value);
		expect(value.is_long(), true);
		expect(value.is_ulong(), false);
		expect(value.is_double(), false);
		expect(int(value), -1337);
		expect(value.to<Variant>().to<int>(), -1337);
		value = -66;
		expect(int(value), -66);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("integer_comparison")
		Variant A(int32_t(-1337));
		Variant B(uint16_t(4444));
		test_comparison(A);
		test_comparison(B);
		test_calc_hash(A);
		test_calc_hash(B);
		expect(A.is_integral(), true);
		expect(B.is_integral(), true);
		expect(A.is_long(), true);
		expect(B.is_long(), true);
		expect(A.is_ulong(), false);
		expect(B.is_ulong(), true);
		expect(A == A, true);
		expect(A != A, false);
		expect(A == B, false);
		expect(A != B, true);
		expect(A < B, true);
		expect(A > B, false);
		expect(A <= B, true);
		expect(A >= B, false);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("mixed_comparison")
		Variant A(uint32_t(1337));
		Variant B(double(4444.5555));
		Variant C(int32_t(-1337));
		Variant D(float(-1337));
		test_comparison(A);
		test_comparison(B);
		test_comparison(C);
		test_comparison(D);
		test_calc_hash(A);
		test_calc_hash(B);
		test_calc_hash(C);
		test_calc_hash(D);
		expect(A.is_integral(), true);
		expect(B.is_integral(), true);
		expect(C.is_integral(), true);
		expect(D.is_integral(), true);
		expect(A.is_long(), true);
		expect(B.is_long(), false);
		expect(C.is_long(), true);
		expect(D.is_long(), false);
		expect(A.is_ulong(), true);
		expect(B.is_ulong(), false);
		expect(C.is_ulong(), false);
		expect(D.is_ulong(), false);
		expect(A.is_double(), false);
		expect(B.is_double(), true);
		expect(C.is_double(), false);
		expect(D.is_double(), true);
		expect(A == B, false);
		expect(A != B, true);
		expect(A < B, true);
		expect(A > B, false);
		expect(A <= B, true);
		expect(A >= B, false);
		expect(A > C, true);
		expect(A < C, false);
		expect(A >= C, true);
		expect(A <= C, false);
		expect(A == C, false);
		expect(B > C, true);
		expect(B < C, false);
		expect(B == C, false);
		expect(B > D, true);
		expect(B == D, false);
		expect(C > D, false);
		expect(C >= D, true);
		expect(C <= D, true);
		expect(C == D, true);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("hash128_comparison")
		Variant A(std::make_pair(Hash64::rand(), Hash64::rand()));
		Variant B(std::make_pair(Hash64::rand(), Hash64::rand()));
		test_comparison(A);
		test_comparison(B);
		test_calc_hash(A);
		test_calc_hash(B);
		expect(A.is_integral(), false);
		expect(B.is_integral(), false);
		expect(A == B, false);
		expect(A != B, true);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("string")
		Variant A("1337");
		Variant B("4444");
		test_comparison(A);
		test_comparison(B);
		test_calc_hash(A);
		test_calc_hash(B);
		expect(A.is_string(), true);
		expect(B.is_string(), true);
		expect(A == B, false);
		expect(A != B, true);
		expect(A < B, true);
		expect(A > B, false);
		expect(A <= B, true);
		expect(A >= B, false);
		expect(A.to<std::string>(), "1337");
		expect(B.to<std::string>(), "4444");
	VNX_TEST_END()

	VNX_TEST_BEGIN("array")
		Variant A(std::vector<short>{1, 3, 3, 7});
		Variant B(std::vector<int>{1, 3, 3, 7});
		test_comparison(A);
		test_comparison(B);
		test_calc_hash(A);
		test_calc_hash(B);
		expect(A.is_array(), true);
		expect(B.is_array(), true);
		expect(A == B, true);
		expect(A != B, false);
		expect(A < B, false);
		expect(A > B, false);
		expect(A <= B, true);
		expect(A >= B, true);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("sample")
		auto sample = Sample::create();
		sample->topic = "test.topic";
		sample->hop_count = 11;
		Variant value(sample);
		std::cout << "[sample] value.data.size() = " << value.data.size() << std::endl;
		{
			PrettyPrinter printer(std::cout);
			accept(printer, value);
			std::cout << std::endl;
		}
		test_comparison(value);
		test_calc_hash(value);
		expect(value.is_null(), false);
		expect(value.is_object(), true);
		expect(bool(value.to<std::shared_ptr<Message>>()), true);
		expect(bool(value.to<std::shared_ptr<Endpoint>>()), true);
		expect(value.to<Message>().hop_count, 11);
		expect(value.to<Sample>().topic->get_name(), "test.topic");
		expect(value.to<std::shared_ptr<Message>>()->hop_count, 11);
		expect(value.to<std::shared_ptr<Sample>>()->topic->get_name(), "test.topic");
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("struct")
		record_index_entry_t sample;
		sample.pos = 1337;
		sample.time = 1111;
		Variant value(sample);
		std::cout << "[struct] value.data.size() = " << value.data.size() << std::endl;
		{
			PrettyPrinter printer(std::cout);
			accept(printer, value);
			std::cout << std::endl;
		}
		test_comparison(value);
		test_calc_hash(value);
		expect(value.is_null(), false);
		expect(value.is_object(), true);
		expect(value.to<record_index_entry_t>().pos, 1337);
		expect(value.to<record_index_entry_t>().time, 1111);
	VNX_TEST_END()

	VNX_TEST_BEGIN("nullptr")
		Variant value(nullptr);
		test_comparison(value);
		test_calc_hash(value);
		expect(value.is_null(), true);
		expect(value.is_integral(), false);
		expect(value.is_object(), false);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("shared_nullptr")
		Variant value = Variant(std::shared_ptr<const Sample>());
		test_comparison(value);
		test_calc_hash(value);
		expect(value.is_null(), true);
		expect(value.is_integral(), false);
		expect(value.is_object(), false);
	VNX_TEST_END()

	VNX_TEST_BEGIN("object")
		Object sample;
		sample["topic"] = "test.topic";
		sample["time"] = 1111;
		sample["array"] = std::array<bool, 3>({false, true, false});
		Variant value(sample);
		std::cout << "[object] value.data.size() = " << value.data.size() << std::endl;
		{
			PrettyPrinter printer(std::cout);
			accept(printer, value);
			std::cout << std::endl;
		}
		test_comparison(value);
		test_calc_hash(value);
		expect(value.is_object(), true);
		expect(value.to<Object>()["topic"].to<std::string>(), "test.topic");
		expect(value.to<Object>()["time"].to<int>(), 1111);
		expect(value.to<Object>()["array"].to<std::array<bool, 3>>()[1], true);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("to_from_object")
		auto sample = Sample::create();
		sample->topic = "test.topic";
		sample->hop_count = 11;
		const Object object = sample->to_object();
		{
			Sample value;
			value.from_object(object);
			expect(value.topic, sample->topic);
			expect(value.hop_count, sample->hop_count);
		}
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("from_string")
		vnx::Variant value;
		{
			vnx::from_string("0", value);
			expect(value.to<bool>(), false);
			vnx::from_string("1", value);
			expect(value.to<bool>(), true);
			vnx::from_string("false", value);
			expect(value.to<bool>(), false);
			vnx::from_string("true", value);
			expect(value.to<bool>(), true);
		}
		{
			vnx::from_string("0", value);
			expect(value.is_long(), true);
			expect(value.is_ulong(), true);
			expect(value.to<int32_t>(), 0);
			expect(value.to<uint32_t>(), uint32_t(0));
			vnx::from_string("1", value);
			expect(value.is_long(), true);
			expect(value.is_ulong(), true);
			expect(value.to<int32_t>(), 1);
			expect(value.to<uint32_t>(), uint32_t(1));
			vnx::from_string("-1", value);
			expect(value.is_long(), true);
			expect(value.is_ulong(), false);
			expect(value.to<int32_t>(), -1);
			expect(value.to<uint32_t>(), uint32_t(-1));
		}
		{
			vnx::from_string("1", value);
			expect(value.to<double>(), 1);
			vnx::from_string("-1", value);
			expect(value.to<double>(), -1);
			vnx::from_string("0.1", value);
			expect(value.is_double(), true);
			expect(value.to<double>(), 0.1);
			vnx::from_string("-0.1", value);
			expect(value.is_double(), true);
			expect(value.to<double>(), -0.1);
			vnx::from_string("1e3", value);
			expect(value.is_double(), true);
			expect(value.to<double>(), 1e3);
			vnx::from_string("1e-3", value);
			expect(value.is_double(), true);
			expect(value.to<double>(), 1e-3);
			vnx::from_string("1.0e3", value);
			expect(value.is_double(), true);
			expect(value.to<double>(), 1.0e3);
			vnx::from_string("-1.0e-3", value);
			expect(value.is_double(), true);
			expect(value.to<double>(), -1.0e-3);
		}
		{
			vnx::from_string("\"-1337\"", value);
			expect(value.is_long(), false);
			expect(value.is_string(), true);
		}
		{
			vnx::from_string_value("-1337", value);
			expect(value.is_long(), true);
			expect(value.is_string(), false);
		}
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("optional")
	{
		expect(vnx::optional<int>() == vnx::optional<int>(), true);
		expect(vnx::optional<int>() == vnx::optional<float>(), true);
		expect(vnx::optional<int>(1337) == vnx::optional<int>(), false);
		expect(vnx::optional<int>(1337) == vnx::optional<int>(1337), true);
		expect(vnx::optional<int>(1337) == vnx::optional<int>(-1337), false);
		expect(vnx::optional<int>(1337) == vnx::optional<double>(1337), true);
	}
	{
		const vnx::optional<int> value(1337);
		const auto other = value;
		expect(*value, *other);
	}
	{
		const vnx::optional<int> value(1337);

		const Variant var(value);
		test_comparison(var);
		test_calc_hash(var);
		expect(var.to<int>(), 1337);
		expect(bool(var.to<optional<int>>()), true);
		expect(*var.to<optional<int>>(), 1337);
	}
	{
		vnx::optional<Sample> value(new Sample());
		value->seq_num = 1337;

		const Variant var(value);
		test_comparison(var);
		test_calc_hash(var);
		expect(var.to<Sample>().seq_num, uint64_t(1337));
	}
	{
		const vnx::optional<int> value;

		const Variant var(value);
		test_comparison(var);
		test_calc_hash(var);
		expect(var.to<int>(), 0);
		expect(bool(var.to<optional<int>>()), false);
	}
	VNX_TEST_END()

	vnx::static_cleanup();

	return vnx::test::done();
}
