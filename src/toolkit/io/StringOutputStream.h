#ifndef TOOLKIT_IO_STRINGOUTPUTSTREAM_H
#define TOOLKIT_IO_STRINGOUTPUTSTREAM_H

#include <toolkit/io/MemoryOutputStream.h>

namespace TOOLKIT_NS { namespace io
{
	class StringOutputStream
	{
		MemoryOutputStream	_stream;

	public:
		template<typename ValueType>
		size_t Write(ValueType value);

		template<typename ValueType>
		StringOutputStream & operator << (ValueType && value)
		{
			Write(std::forward<ValueType>(value));
			return *this;
		}

		size_t GetSize() const
		{ return _stream.GetSize(); }
	};
}}

#endif
