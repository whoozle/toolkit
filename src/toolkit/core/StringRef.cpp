#include <toolkit/core/StringRef.h>
#include <toolkit/text/StringOutputStream.h>

namespace TOOLKIT_NS
{
	void StringRef::ToString(text::StringOutputStream & ss) const
	{ ss.Write(data(), size()); }
}
