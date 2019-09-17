#ifndef TOOLKIT_CORE_STRINGREF
#define TOOLKIT_CORE_STRINGREF

#include <toolkit/core/Buffer.h>
#include <string>

FORWARD_DECLARE_CLASS(text, StringOutputStream);

namespace TOOLKIT_NS
{

	template <typename Char>
	class BasicStringRef : public BasicBuffer<Char>
    {
        using super = BasicBuffer<Char>;

    public:
        using super::super;

		std::string ToString() const
		{ return std::string(static_cast<const Char *>(this->data()), this->size()); }

		void ToString(text::StringOutputStream & ss) const;
    };

	using StringRef = BasicStringRef<char>;
	using ConstStringRef = BasicStringRef<const char>;
}

#endif
