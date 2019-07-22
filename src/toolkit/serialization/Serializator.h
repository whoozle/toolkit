#ifndef TOOLKIT_SERIALIZATION_SERIALIZATOR_H
#define TOOLKIT_SERIALIZATION_SERIALIZATOR_H

#include <toolkit/serialization/ISerializationStream.h>
#include <toolkit/core/types.h>

namespace TOOLKIT_NS { namespace serialization
{
	template<typename Type>
	struct Serializator
	{
		static void Write(IOutputStream & out, const Type & value)
		{ value.Write(out); }
	};

#define TOOLKIT_SERIALIZATOR_FORWARD_TYPE(TYPE, CTYPE)  \
	template<> \
	struct Serializator< TYPE > \
	{ static void Write(IOutputStream & out, TYPE value) { out.Write(static_cast<CTYPE>(value)); } }

	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(const Undefined &, const Undefined &);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(std::nullptr_t, std::nullptr_t);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(bool, bool);

	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(u8,  s64);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(u16, s64);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(u32, s64);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(u64, s64);

	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(s8,  s64);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(s16, s64);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(s32, s64);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(s64, s64);

	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(float, double);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(double, double);

	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(const std::string &, const std::string &);

#undef TOOLKIT_SERIALIZATOR_FORWARD_TYPE
}}

#endif
