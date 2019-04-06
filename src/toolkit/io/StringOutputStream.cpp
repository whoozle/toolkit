#include <toolkit/io/StringOutputStream.h>
#include <toolkit/io/SystemException.h>
#include <stdio.h>

namespace TOOLKIT_NS { namespace io
{
#define TOOLKIT_IO_SS_DECLARE_WRITE(FMT, TYPE) \
	template<> \
	size_t StringOutputStream::WriteImpl< TYPE >( TYPE value ) \
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

#undef TOOLKIT_IO_SS_DECLARE_WRITE

	size_t StringOutputStream::Write(const char *value, size_t size)
	{ return _stream.Write(ConstBuffer(static_cast<const u8 *>(static_cast<const void *>(value)), size)); }

	template<>
	size_t StringOutputStream::WriteImpl<const char *>(const char * value)
	{ return Write(value, strlen(value)); }

	template<>
	size_t StringOutputStream::WriteImpl<char *>(char * value)
	{ return Write(value, strlen(value)); }

	template<>
	size_t StringOutputStream::WriteImpl<std::string>(std::string value)
	{ return Write(value.data(), value.size()); }

}}
