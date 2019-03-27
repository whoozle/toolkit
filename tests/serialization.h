#include <toolkit/serialization/Serialization.h>
#include <lest/lest.hpp>
#include <string>

namespace
{
    const lest::test serialization[] =
    {
        CASE( "Empty string has length zero (succeed)" )
        {
            EXPECT( 0 == std::string(  ).length() );
            EXPECT( 0 == std::string("").length() );
        },
    };
}
