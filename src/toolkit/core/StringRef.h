#ifndef TOOLKIT_CORE_STRINGREF
#define TOOLKIT_CORE_STRINGREF

#include <toolkit/core/Buffer.h>

namespace TOOLKIT_NS
{
	class StringRef : public BasicBuffer<s8>
    {
        using super = BasicBuffer<s8>;

    public:
        using super::super;
    };
}

#endif
