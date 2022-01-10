#ifndef TOOLKIT_TEXT_FROMSTRING_H
#define TOOLKIT_TEXT_FROMSTRING_H

#include <toolkit/text/String.h>
#include <toolkit/core/Exception.h>
#include <string>
#if __cplusplus >= 201703L
#	include <optional>
#endif

namespace TOOLKIT_NS { namespace text
{
	namespace impl
	{
		template<typename T>
		struct FromString;

#if __cplusplus >= 201703L
		template<typename Type>
		struct FromString<std::optional<Type>>
		{
			static std::optional<Type> Parse(const std::string & str)
			{ return FromString<Type>::Parse(str); }
		};
#endif

		template<> struct FromString<int>
		{
			static int Parse(const std::string & str)
			{ return std::stoi(str); }
		};

		template<> struct FromString<long>
		{
			static long Parse(const std::string & str)
			{ return std::stol(str); }
		};

		template<> struct FromString<long long>
		{
			static long long Parse(const std::string & str)
			{ return std::stoll(str); }
		};

		template<> struct FromString<std::string>
		{
			static std::string Parse(const std::string & str)
			{ return str; }
		};

		template<> struct FromString<std::wstring>
		{
			static std::wstring Parse(const std::string & str)
			{ return FromUtf8(str); }
		};

		template<> struct FromString<bool>
		{
			static bool Parse(const std::string & str)
			{
				if (str == "yes" || str == "true")
					return true;
				else if (str == "no" || str == "false")
					return false;
				else
				 	throw Exception("invalid boolean value " + str);
			}
		};
	}

	template <typename T>
	T FromString(const std::string & str)
	{ return impl::FromString<T>::Parse(str); }

}}

#endif
