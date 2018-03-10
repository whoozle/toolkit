#ifndef TOOLKIT_IO_DATASTREAM_H
#define TOOLKIT_IO_DATASTREAM_H

#include <toolkit/io/IStream.h>
#include <toolkit/core/Exception.h>

TOOLKIT_NS_BEGIN
namespace io
{

	class DataInputStream
	{
		IStreamPtr		_stream;

	public:
		DataInputStream(IStreamPtr stream): _stream(stream)
		{ }

		u8 ReadU8()
		{
			u8 value;
			_stream->Read(ByteData(&value, sizeof(value)));
			return value;
		}

		u16 ReadU16()
		{
			u8 value[2];
			ByteData data(value, sizeof(value));
			return value[0] | ((u16)value[1] << 8);
		}
	};

	class DataOutputStream
	{
		IStreamPtr		_stream;

	public:
		DataOutputStream(IStreamPtr stream): _stream(stream)
		{ }

		void WriteU8(u8 value)
		{
			u8 data[] = { value };
			if (_stream->Write(ConstByteData(data, sizeof(data))) != 1)
				throw Exception("Could not write 1 byte to stream");
		}

		void WriteU16(u16 value)
		{
			u8 data[] = { static_cast<u8>(value), static_cast<u8>(value >> 8) };
			if (_stream->Write(ConstByteData(data, sizeof(data))) != 2)
				throw Exception("Could not write 2 bytes to stream");
		}

		void WriteU32(u32 value)
		{
			u8 data[] = { static_cast<u8>(value), static_cast<u8>(value >> 8), static_cast<u8>(value >> 16), static_cast<u8>(value >> 24) };
			if (_stream->Write(ConstByteData(data, sizeof(data))) != 4)
				throw Exception("Could not write 4 bytes to stream");
		}

		void WriteU64(u64 value)
		{
			u8 data[] = {
				static_cast<u8>(value), static_cast<u8>(value >> 8),
				static_cast<u8>(value >> 16), static_cast<u8>(value >> 24),
				static_cast<u8>(value >> 32), static_cast<u8>(value >> 40),
				static_cast<u8>(value >> 48), static_cast<u8>(value >> 56),
			};
			if (_stream->Write(ConstByteData(data, sizeof(data))) != 8)
				throw Exception("Could not write 8 bytes to stream");
		}

		template<typename T>
		void WriteVariableLengthInt(T _value)
		{
			bool sign = _value < 0;
			u8 size;
			u64 value = sign? -_value: _value;
			if (value == 0)
				size = 0;
			else
			{
				u8 i;
				for(i = 1; i < sizeof(_value); ++i)
				{
					if (value < (1Lu << (i << 3)))
					{
						size = i;
						break;
					}
				}
				if (i == sizeof(_value))
					size = i;
			}

			WriteU8(sign? size | 0x80: size);
			while(size--)
			{
				WriteU8(static_cast<u8>(value));
				value >>= 8;
			}
		}
	};

}
TOOLKIT_NS_END

#endif
