#ifndef TOOLKIT_IO_DATASTREAM_H
#define TOOLKIT_IO_DATASTREAM_H

#include <toolkit/io/IStream.h>
#include <toolkit/core/Exception.h>
#include <array>

TOOLKIT_NS_BEGIN
namespace io
{

	namespace impl
	{
		template<typename T, uint N, uint Shift, int ShiftDelta>
		struct ByteConverter
		{
			static T Load(const u8 * data)
			{ return ((static_cast<T>(*data) << Shift) | ByteConverter<T, N - 1, Shift + ShiftDelta, ShiftDelta>::Load(data + 1)); }

			static void Store(u8 * data, T value)
			{
				*data = (value >> Shift) & u8(0xffu);
				ByteConverter<T, N - 1, Shift + ShiftDelta, ShiftDelta>::Store(data + 1, value);
			}
		};

		template<typename T, uint Shift, int ShiftDelta>
		struct ByteConverter<T, 0, Shift, ShiftDelta>
		{
			static T Load(const u8 * data)
			{ return 0; }

			static void Store(u8 * data, T value)
			{ }
		};

		template<bool BigEndian>
		struct DataConverter;

		template<>
		struct DataConverter<false>
		{
			template<typename T>
			static T Load(const u8 *data)
			{ return ByteConverter<T, sizeof(T), 0, 8>::Load(data); }

			template<typename T>
			static void Store(u8 *data, T value)
			{ ByteConverter<T, sizeof(T), 0, 8>::Store(data, value); }
		};

		template<>
		struct DataConverter<true>
		{
			template<typename T>
			static T Load(const u8 *data)
			{ return ByteConverter<T, sizeof(T), (sizeof(T) - 1) << 3, -8>::Load(data); }

			template<typename T>
			static void Store(u8 *data, T value)
			{ ByteConverter<T, sizeof(T), (sizeof(T) - 1) << 3, -8>::Store(data, value); }
		};
	}

	template<bool BigEndian>
	class DataInputStream
	{
		IInputStream &		_stream;

	public:
		DataInputStream(IInputStream & stream): _stream(stream)
		{ }

		template<typename T>
		T Read()
		{
			std::array<u8, sizeof(T)> data;
			if (_stream.Read(data) != data.size())
				throw Exception("short read");
			return impl::DataConverter<BigEndian>::template Load<T>(data.data());
		}

		u8 ReadU8()
		{ return Read<u8>(); }
		u16 ReadU16()
		{ return Read<u16>(); }
		u32 ReadU32()
		{ return Read<u32>(); }
		u64 ReadU64()
		{ return Read<u64>(); }

	};

	using LittleEndianDataInputStream = DataInputStream<false>;
	using BigEndianDataInputStream = DataInputStream<true>;

	template<bool BigEndian>
	class DataOutputStream
	{
		IOutputStream &		_stream;

	public:
		DataOutputStream(IOutputStream & stream): _stream(stream)
		{ }

		template<typename T>
		void Write(T value)
		{
			std::array<u8, sizeof(T)> data;
			impl::DataConverter<BigEndian>::Store(data.data(), value);
			if (_stream.Write(data) != data.size())
				throw Exception("short write");
		}

		void WriteU8(u8 value)
		{ Write<u8>(value); }

		void WriteU16(u16 value)
		{ Write<u16>(value); }

		void WriteU32(u32 value)
		{ Write<u32>(value); }

		void WriteU64(u64 value)
		{ Write<u64>(value); }

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

	using LittleEndianDataOutputStream = DataOutputStream<false>;
	using BigEndianDataOutputStream = DataOutputStream<true>;

}
TOOLKIT_NS_END

#endif
