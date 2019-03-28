#include <toolkit/serialization/Serialization.h>
#include <lest/lest.hpp>
#include <string>

namespace
{
	class Test
	{
		int _p;

	public:
		Test(int p): _p(p) { }

		template<typename RegistryType>
		static void RegisterMembers(RegistryType & reg)
		{
			using namespace toolkit::serialization;
			reg % Member(&Test::_p, "p");
		}
	};

    const lest::test serialization[] =
    {
        CASE( "Empty string has length zero (succeed)" )
        {
            EXPECT( 0 == std::string(  ).length() );
            EXPECT( 0 == std::string("").length() );
        },
    };
}
