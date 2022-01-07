#ifndef TOOLKIT_IO_STRINGOUTPUTSTREAM_H
#define TOOLKIT_IO_STRINGOUTPUTSTREAM_H

#include <toolkit/core/Buffer.h>
#include <toolkit/core/MemberCheck.h>
#include <toolkit/io/MemoryOutputStream.h>
#include <type_traits>

namespace TOOLKIT_NS { namespace text
{
	DECLARE_METHOD_CHECK(ToString);

	template <typename Stream, typename Type>
	typename std::enable_if<HasMethod_ToString<Type>::Value, Stream &>::type operator << (Stream & stream, const Type & value)
	{ value.ToString(stream); return stream; }

	class StringOutputStream
	{
		io::MemoryOutputStream	_stream;

#define TOOLKIT_IO_SS_DECLARE_WRITE(TYPE) \
		public: StringOutputStream & operator << ( TYPE value) { Write(value); return *this; } \
		private: size_t Write( TYPE value )

		TOOLKIT_IO_SS_DECLARE_WRITE(char);
		TOOLKIT_IO_SS_DECLARE_WRITE(unsigned char);
		TOOLKIT_IO_SS_DECLARE_WRITE(short);
		TOOLKIT_IO_SS_DECLARE_WRITE(unsigned short);
		TOOLKIT_IO_SS_DECLARE_WRITE(int);
		TOOLKIT_IO_SS_DECLARE_WRITE(unsigned int);
		TOOLKIT_IO_SS_DECLARE_WRITE(long);
		TOOLKIT_IO_SS_DECLARE_WRITE(unsigned long);
		TOOLKIT_IO_SS_DECLARE_WRITE(long long);
		TOOLKIT_IO_SS_DECLARE_WRITE(unsigned long long);
		TOOLKIT_IO_SS_DECLARE_WRITE(const char *);
		TOOLKIT_IO_SS_DECLARE_WRITE(char *);
		TOOLKIT_IO_SS_DECLARE_WRITE(float);
		TOOLKIT_IO_SS_DECLARE_WRITE(double);
		TOOLKIT_IO_SS_DECLARE_WRITE(long double);
		TOOLKIT_IO_SS_DECLARE_WRITE(const std::string &);
		TOOLKIT_IO_SS_DECLARE_WRITE(const void *);
		TOOLKIT_IO_SS_DECLARE_WRITE(void *);
		TOOLKIT_IO_SS_DECLARE_WRITE(const std::exception &);
		TOOLKIT_IO_SS_DECLARE_WRITE(const std::type_info &);

#undef TOOLKIT_IO_SS_DECLARE_WRITE

	public:
		static constexpr size_t InitialCapacity = 128;

		StringOutputStream(size_t initialCapacity = InitialCapacity): _stream(initialCapacity)
		{ }

		size_t Write(ConstBuffer buffer)
		{ return _stream.Write(buffer); }

		size_t Write(const char *value, size_t size);

		std::string Get() const
		{
			auto & storage = _stream.GetStorage();
			return std::string(storage.begin(), storage.end());
		}

		size_t GetSize() const
		{ return _stream.GetSize(); }
	};

#define TOOLKIT_DECLARE_SIMPLE_TOSTRING() \
	std::string ToString() const { TOOLKIT_NS :: text :: StringOutputStream ss; ToString(ss); return ss.Get(); }

}}

#endif
