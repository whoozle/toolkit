#include <toolkit/text/StringOutputStream.h>
#include <toolkit/io/SystemException.h>
#include <stdio.h>
#include <cxxabi.h>

namespace TOOLKIT_NS { namespace text
{
#define TOOLKIT_IO_SS_DECLARE_WRITE(FMT, TYPE) \
	size_t StringOutputStream::Write( TYPE value ) \
	{ \
		char buf[3 * sizeof(TYPE) + 1]; \
		auto r = snprintf(buf, sizeof(buf), FMT , value); \
		if (r < 0) throw io::SystemException("snprintf(" #FMT ", value)"); \
		return Write(buf, r); \
	}

	TOOLKIT_IO_SS_DECLARE_WRITE("%c", char)
	TOOLKIT_IO_SS_DECLARE_WRITE("%hhu", unsigned char)
	TOOLKIT_IO_SS_DECLARE_WRITE("%hd", short)
	TOOLKIT_IO_SS_DECLARE_WRITE("%hu", unsigned short)
	TOOLKIT_IO_SS_DECLARE_WRITE("%d", int)
	TOOLKIT_IO_SS_DECLARE_WRITE("%u", unsigned int)
	TOOLKIT_IO_SS_DECLARE_WRITE("%ld", long)
	TOOLKIT_IO_SS_DECLARE_WRITE("%lu", unsigned long)
	TOOLKIT_IO_SS_DECLARE_WRITE("%lld", long long)
	TOOLKIT_IO_SS_DECLARE_WRITE("%llu", unsigned long long)
	TOOLKIT_IO_SS_DECLARE_WRITE("%g", float)
	TOOLKIT_IO_SS_DECLARE_WRITE("%g", double)
	TOOLKIT_IO_SS_DECLARE_WRITE("%Lg", long double)
	TOOLKIT_IO_SS_DECLARE_WRITE("%p", void *)
	TOOLKIT_IO_SS_DECLARE_WRITE("%p", const void *)

#undef TOOLKIT_IO_SS_DECLARE_WRITE

	size_t StringOutputStream::Write(const char *value, size_t size)
	{ return _stream.Write(ConstBuffer(static_cast<const u8 *>(static_cast<const void *>(value)), size)); }

	size_t StringOutputStream::Write(const char * value)
	{ return Write(value, strlen(value)); }

	size_t StringOutputStream::Write(char * value)
	{ return Write(value, strlen(value)); }

	size_t StringOutputStream::Write(const std::string & value)
	{ return Write(value.data(), value.size()); }

	size_t StringOutputStream::Write(const std::exception & ex)
	{
		size_t r = Write(typeid(ex));
		r += Write(": \"", 3);
		r += Write(ex.what());
		r += Write('"');
		return r;
	}

	size_t StringOutputStream::Write(const std::type_info & ti)
	{
		auto name = ti.name();
		int status = -1;
		std::unique_ptr<char, decltype(std::free) *> demangled(abi::__cxa_demangle(name, NULL, NULL, &status), &std::free);
		return Write(demangled.get());
	}
}}
