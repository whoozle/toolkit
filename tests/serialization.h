#include <toolkit/serialization/Serialization.h>
#include <toolkit/serialization/JSON.h>
#include <lest/lest.hpp>
#include <string>

namespace
{
	class Test
	{
		int _p;
		int _q;

	public:
		Test(int p, int q): _p(p), _q(q) { }

		static auto RegisterMembers()
		{
			using namespace toolkit::serialization;
			return
				ClassDescriptor("Test", 1) &
				Member(&Test::_p, "p") &
				Member(&Test::_q, "q");
		}
	};

	const lest::test serialization[] =
	{
		CASE( "Empty string has length zero (succeed)" )
		{
			toolkit::serialization::JSONWriter<Test> writer;
			EXPECT( 0 == std::string(  ).length() );
			EXPECT( 0 == std::string("").length() );
		},
	};
}
