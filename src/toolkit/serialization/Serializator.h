#ifndef TOOLKIT_SERIALIZATION_SERIALIZATOR_H
#define TOOLKIT_SERIALIZATION_SERIALIZATOR_H

#include <toolkit/serialization/ISerializationStream.h>
#include <toolkit/core/types.h>
#include <string>

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
	{ static void Write(IOutputStream & out, TYPE value) \
	{ out.Write(static_cast<CTYPE>(value)); } }

	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(const Undefined &, const Undefined &);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(std::nullptr_t, std::nullptr_t);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(bool, bool);

	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(char, s64);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(short, s64);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(int, s64);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(long, s64);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(long long, s64);

	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(unsigned char, s64);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(unsigned short, s64);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(unsigned int, s64);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(unsigned long, s64);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(unsigned long long, s64);

	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(float, double);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(double, double);

	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(std::string, const std::string &);

#undef TOOLKIT_SERIALIZATOR_FORWARD_TYPE


	template<typename ... Type>
	void Serialize(IOutputStream & out, const Type & ... value)
	{
		using expand_type = int[];
		expand_type { (Serializator<Type>::Write(out, value), 0) ... };
	}

}}

#endif
