#ifndef TOOLKIT_CORE_STRINGREF
#define TOOLKIT_CORE_STRINGREF

#include <toolkit/core/Buffer.h>
#include <string>

namespace TOOLKIT_NS
{
	class StringRef : public BasicBuffer<char>
    {
        using super = BasicBuffer<char>;

    public:
        using super::super;

		std::string ToString() const
		{ return std::string(static_cast<const char *>(data()), size()); }
    };
}

#endif
