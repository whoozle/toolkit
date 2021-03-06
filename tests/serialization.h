#include <toolkit/serialization/Serialization.h>
#include <toolkit/serialization/Grammar.h>
#include <toolkit/serialization/JSON.h>
#include <toolkit/serialization/BSON.h>
#include <toolkit/serialization/bson/Number.h>
#include <toolkit/text/StringOutputStream.h>
#include <toolkit/log/Logger.h>
#include <toolkit/text/Formatters.h>
#include <lest/lest.hpp>
#include <string>

namespace
{
	namespace t = toolkit;
	namespace ts = t::serialization;
	t::log::Logger Log("test");

	class Test
	{
		int _p;
		float _q;
		std::string _c;

	public:
		Test(int p, int q, const std::string &c): _p(p), _q(q), _c(c) { }
		Test(): _p(), _q(), _c() { }

		static auto GetClassDescriptor()
		{
			return
				ts::ClassDescriptor("Test", 1) &
				ts::Member(&Test::_p) &
				ts::Member(&Test::_q) &
				ts::Member(&Test::_c, "comment");
		}
	};
	TOOLKIT_DECLARE_PTR(Test);

	namespace
	{
		bool NumberMatches(double x)
		{
			using namespace t;
			int exp;
			auto sign = frexp(x, &exp);
			auto newSign = ts::DecodeNumber<double, u64>(ts::EncodeNumber<u64, double>(sign));
			auto decoded = ldexp(newSign, exp);
			return fabs(x - decoded) < std::numeric_limits<double>::epsilon();
		}
	}


	const lest::test serialization[] =
	{
		CASE("NUMBER REPRESENTATION TEST")
		{
			EXPECT(NumberMatches(0));
			EXPECT(NumberMatches(1));
			EXPECT(NumberMatches(1 / 3.0));
			EXPECT(NumberMatches(1e77));
			EXPECT(NumberMatches(M_PI));
			EXPECT(NumberMatches(M_E));
		},
		// CASE( "JSON serialization test" )
		// {
		// 	auto jsonWriter = ts::MakeSerializator<Test, ts::JSONWriter>();
		// 	Test test(2, 3, "юникод\ncopyright ©1738\r\n");

		// 	auto state = jsonWriter.NewState(test);
		// 	toolkit::text::StringOutputStream ss;
		// 	jsonWriter.Write(ss, state);
		// 	EXPECT( ss.Get() == "{\"__classname\":\"Test\",\"__version\":1,\"p\":2,\"q\":3,\"comment\":\"\\u044e\\u043d\\u0438\\u043a\\u043e\\u0434\\ncopyright \\u00a91738\\r\\n\"}" );
		// },
		// CASE( "JSON deserialization test" )
		// {
		// 	auto jsonReader = ts::MakeSerializator<Test, ts::JSONReader>();
		// 	Test test(2, 3, "юникод\ncopyright ©1738\r\n");

		// 	auto state = jsonReader.NewState(test);
		// 	std::string text = "{\"__classname\":\"Test\",\"__version\":1,\"p\":2,\"q\":3,\"comment\":\"\\u044e\\u043d\\u0438\\u043a\\u043e\\u0434\\ncopyright \\u00a91738\\r\\n\"}";
		// 	for(auto ch : text)
		// 	{
		// 		jsonReader.Handle(state, ch);
		// 	}
		// },
		CASE( "BSON serialization test" )
		{
			ts::ClassDescriptorRegistry::Get().Register<Test>();
			toolkit::ByteArray data;
			{
				Test test(2, 3, "юникод\ncopyright ©1738\r\n");
				ts::BSON::Write(data.GetStorage(), test);
				Log.Debug() << "generated " << t::text::HexDump(data, "bson");
			}
			{
				size_t r = 0;
				TestPtr test = std::dynamic_pointer_cast<Test>(ts::BSON::Read(data.GetStorage(), r));
				EXPECT (test != nullptr);
				EXPECT (r == data.size());
			}
		}
	};
}
