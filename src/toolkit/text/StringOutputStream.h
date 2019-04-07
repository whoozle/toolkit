#ifndef TOOLKIT_IO_STRINGOUTPUTSTREAM_H
#define TOOLKIT_IO_STRINGOUTPUTSTREAM_H

#include <toolkit/core/Buffer.h>
#include <toolkit/io/MemoryOutputStream.h>

namespace TOOLKIT_NS { namespace text
{
	class StringOutputStream
	{
		io::MemoryOutputStream	_stream;

		template<typename ValueType>
		size_t WriteImpl(ValueType value)
		{
			size_t before = _stream.GetSize();
			value.ToString(*this);
			return _stream.GetSize() - before;
		}

#define TOOLKIT_IO_SS_DECLARE_WRITE(TYPE) \
		size_t WriteImpl( TYPE value );

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

#undef TOOLKIT_IO_SS_DECLARE_WRITE

	public:
		static constexpr size_t InitialCapacity = 128;

		StringOutputStream(size_t initialCapacity = InitialCapacity): _stream(initialCapacity)
		{ }

		size_t Write(ConstBuffer buffer)
		{ return _stream.Write(buffer); }

		size_t Write(const char *value, size_t size);

		template<typename ValueType>
		size_t Write(ValueType && value)
		{ return WriteImpl(std::forward<ValueType>(value)); }

		template<typename ValueType>
		StringOutputStream & operator << (ValueType && value)
		{
			WriteImpl(std::forward<ValueType>(value));
			return *this;
		}

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
