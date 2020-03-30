#ifndef TOOLKIT_SERIALIZATION_SERIALIZATOR_H
#define TOOLKIT_SERIALIZATION_SERIALIZATOR_H

#include <toolkit/serialization/ISerializationStream.h>
#include <toolkit/core/types.h>
#include <string>

namespace TOOLKIT_NS { namespace serialization
{
	template<typename SourceType>
	struct MapToSerializationType
	{
		using Type = SourceType;
	};

	template<typename Type>
	struct Serializator
	{
		static void Write(ISerializationStream & out, const Type & value)
		{ value.Write(out); }
	};

#define TOOLKIT_SERIALIZATOR_FORWARD_TYPE(TYPE, CTYPE)  \
	template<> \
	struct Serializator< TYPE > \
	{ static void Write(ISerializationStream & out, TYPE value) \
	{ out.Write(static_cast<CTYPE>(value)); } }; \
	template <> struct MapToSerializationType<TYPE> { using Type = CTYPE; }

	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(Undefined, Undefined);
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
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(const char *, const std::string &);
	TOOLKIT_SERIALIZATOR_FORWARD_TYPE(char *, const std::string &);

#undef TOOLKIT_SERIALIZATOR_FORWARD_TYPE

	namespace { template <typename ... Type> void DummyFunc(Type && ... args) { } }

	template<typename ... Type>
	void Serialize(ISerializationStream & out, const Type & ... value)
	{
		DummyFunc((Serializator<Type>::Write(out, value), 0) ...);
	}

}}

#endif
