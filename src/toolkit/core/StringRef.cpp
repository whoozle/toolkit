#include <toolkit/core/StringRef.h>
#include <toolkit/text/StringOutputStream.h>

namespace TOOLKIT_NS
{
	template<typename T>
	void BasicStringRef<T>::ToString(text::StringOutputStream & ss) const
	{ ss.Write(this->data(), this->size()); }

	template class BasicStringRef<char>;
	template class BasicStringRef<const char>;
}
