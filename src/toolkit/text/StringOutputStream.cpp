#include <toolkit/text/StringOutputStream.h>
#include <toolkit/io/SystemException.h>
#include <stdio.h>

namespace TOOLKIT_NS { namespace text
{
#define TOOLKIT_IO_SS_DECLARE_WRITE(FMT, TYPE) \
	size_t StringOutputStream::WriteImpl( TYPE value ) \
	{ \
		char buf[3 * sizeof(TYPE) + 1]; \
		auto r = snprintf(buf, sizeof(buf), FMT , value); \
		if (r < 0) throw io::SystemException("snprintf(" #FMT ", value)"); \
		return Write(buf, r); \
	}

	TOOLKIT_IO_SS_DECLARE_WRITE("%c", char);
	TOOLKIT_IO_SS_DECLARE_WRITE("%hhd", unsigned char);
	TOOLKIT_IO_SS_DECLARE_WRITE("%hd", short);
	TOOLKIT_IO_SS_DECLARE_WRITE("%hu", unsigned short);
	TOOLKIT_IO_SS_DECLARE_WRITE("%d", int);
	TOOLKIT_IO_SS_DECLARE_WRITE("%u", unsigned int);
	TOOLKIT_IO_SS_DECLARE_WRITE("%ld", long);
	TOOLKIT_IO_SS_DECLARE_WRITE("%lu", unsigned long);
	TOOLKIT_IO_SS_DECLARE_WRITE("%lld", long long);
	TOOLKIT_IO_SS_DECLARE_WRITE("%llu", unsigned long long);
	TOOLKIT_IO_SS_DECLARE_WRITE("%g", float);
	TOOLKIT_IO_SS_DECLARE_WRITE("%g", double);
	TOOLKIT_IO_SS_DECLARE_WRITE("%Lg", long double);

#undef TOOLKIT_IO_SS_DECLARE_WRITE

	size_t StringOutputStream::Write(const char *value, size_t size)
	{ return _stream.Write(ConstBuffer(static_cast<const u8 *>(static_cast<const void *>(value)), size)); }

	size_t StringOutputStream::WriteImpl(const char * value)
	{ return Write(value, strlen(value)); }

	size_t StringOutputStream::WriteImpl(char * value)
	{ return Write(value, strlen(value)); }

	size_t StringOutputStream::WriteImpl(const std::string & value)
	{ return Write(value.data(), value.size()); }

}}
