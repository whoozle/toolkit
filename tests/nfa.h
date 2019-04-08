#include <toolkit/fa/NFA.h>
#include <lest/lest.hpp>
#include <string>

namespace
{
	const lest::test nfa[] =
	{
		CASE( "NFA test" )
		{
			using namespace toolkit::fa;
			NFA<8> nfa;
			auto state = nfa.Initialize(1);
		},
	};
}
