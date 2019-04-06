#include <toolkit/serialization/Serialization.h>
#include <toolkit/serialization/JSON.h>
#include <lest/lest.hpp>
#include <string>

namespace
{
	namespace ts = toolkit::serialization;

	class Test
	{
		int _p;
		int _q;

	public:
		Test(int p, int q): _p(p), _q(q) { }

		static auto GetClassDescriptor()
		{
			return
				ts::ClassDescriptor("Test", 1) &
				ts::Member(&Test::_p, "p") &
				ts::Member(&Test::_q, "q");
		}
	};


	const lest::test serialization[] =
	{
		CASE( "Empty string has length zero (succeed)" )
		{
			auto jsonWriter = ts::MakeSerializator<Test, ts::JSONWriter>();
			EXPECT( 0 == std::string(  ).length() );
			EXPECT( 0 == std::string("").length() );
		},
	};
}
