#include <toolkit/serialization/Serialization.h>
#include <toolkit/serialization/Grammar.h>
#include <toolkit/serialization/JSON.h>
#include <toolkit/serialization/BSON.h>
#include <toolkit/text/StringOutputStream.h>
#include <lest/lest.hpp>
#include <string>

namespace
{
	namespace ts = toolkit::serialization;

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


	const lest::test serialization[] =
	{
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
			// using Serializator = ts::BSON<Test>;
			// toolkit::ByteArray data;
			// Test test(2, 3, "юникод\ncopyright ©1738\r\n");
			// Serializator::Write(data.GetStorage(), test);

			//ts::GrammarDescriptor grammar(ts::ClassDescriptorHolder<Test>::Get());
			auto & desc = ts::GrammarDescriptorHolder<Test>::Get();
			// auto bsonWriter = ts::MakeSerializator<Test, ts::bson::Writer>();

			// auto state = bsonWriter.NewState(test);
			// toolkit::text::StringOutputStream ss;
			// bsonWriter.Write(ss, state);
			// EXPECT( ss.Get() == "{\"__classname\":\"Test\",\"__version\":1,\"p\":2,\"q\":3,\"comment\":\"\\u044e\\u043d\\u0438\\u043a\\u043e\\u0434\\ncopyright \\u00a91738\\r\\n\"}" );
		}
	};
}
