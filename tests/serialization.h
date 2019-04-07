#include <toolkit/serialization/Serialization.h>
#include <toolkit/serialization/JSON.h>
#include <toolkit/text/StringOutputStream.h>
#include <lest/lest.hpp>
#include <string>

namespace
{
	namespace ts = toolkit::serialization;

	class Test
	{
		int _p;
		int _q;
		std::string _c;

	public:
		Test(int p, int q, const std::string &c): _p(p), _q(q), _c(c) { }

		static auto GetClassDescriptor()
		{
			return
				ts::ClassDescriptor("Test", 1) &
				ts::Member(&Test::_p, "p") &
				ts::Member(&Test::_q, "q") &
				ts::Member(&Test::_c, "comment");
		}
	};


	const lest::test serialization[] =
	{
		CASE( "JSON serialization test" )
		{
			auto jsonWriter = ts::MakeSerializator<Test, ts::JSONWriter>();
			Test test(2, 3, "юникод\ncopyright ©1738\r\n");

			auto state = jsonWriter.NewState(test);
			toolkit::text::StringOutputStream ss;
			jsonWriter.Write(ss, state);
			EXPECT( ss.Get() == "{\"__classname\":\"Test\",\"__version\":1,\"p\":2,\"q\":3,\"comment\":\"\\u044e\\u043d\\u0438\\u043a\\u043e\\u0434\\ncopyright \\u00a91738\\r\\n\"}" );
		},
	};
}
