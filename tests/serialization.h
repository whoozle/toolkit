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
		CASE( "JSON serialization test" )
		{
			auto jsonWriter = ts::MakeSerializator<Test, ts::JSONWriter>();
			Test test(2, 3);

			auto state = jsonWriter.NewState();
			toolkit::text::StringOutputStream ss;
			jsonWriter.Write(ss, state, test);
			EXPECT( ss.Get() == "{}" );
		},
	};
}
