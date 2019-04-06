#ifndef TOOLKIT_IO_STRINGOUTPUTSTREAM_H
#define TOOLKIT_IO_STRINGOUTPUTSTREAM_H

#include <toolkit/core/Buffer.h>
#include <toolkit/io/MemoryOutputStream.h>

namespace TOOLKIT_NS { namespace io
{
	class StringOutputStream
	{
		MemoryOutputStream	_stream;

		template<typename ValueType>
		size_t WriteImpl(ValueType value);

	public:
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

}}

#endif
