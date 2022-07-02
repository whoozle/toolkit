#ifndef TOOLKIT_IO_BYTEARRAY_H
#define TOOLKIT_IO_BYTEARRAY_H

#include <toolkit/core/Buffer.h>
#include <toolkit/core/Noncopyable.h>
#include <toolkit/core/types.h>
#include <vector>
#include <memory>

namespace TOOLKIT_NS
{

	class ByteArray : public Noncopyable
	{
	public:
		using Storage = std::vector<u8>;

	private:
		Storage			_data;

	public:
		using const_iterator	= std::vector<u8>::const_iterator;
		using iterator 			= std::vector<u8>::iterator;

		ByteArray(): _data() { }
		ByteArray(size_t size): _data(size) { }
		ByteArray(std::vector<u8> && data): _data(data) { }
		ByteArray(ByteArray && data): ByteArray(std::move(data._data)) { }
		template<typename U>
		ByteArray(BasicBuffer<U> buffer): _data(buffer.begin(), buffer.end()) { }
		ByteArray & operator = (const ByteArray && data)
		{ _data = std::move(data._data); return *this; }

		u8 * GetData()
		{ return _data.data(); }
		const u8 * GetData() const
		{ return _data.data(); }
		size_t GetSize() const
		{ return _data.size(); }

		u8 * data()
		{ return _data.data(); }
		const u8 * data() const
		{ return _data.data(); }
		size_t size() const
		{ return _data.size(); }
		void clear()
		{ _data.clear(); }

		void Append(ConstBuffer data)
		{
			_data.reserve(_data.size() + data.size());
			std::copy(data.begin(), data.end(), std::back_inserter(_data));
		}

		void Resize(size_t size)
		{ return _data.resize(size); }
		void Reserve(size_t size)
		{ _data.reserve(size); }
		void Clear()
		{ _data.clear(); }

		Storage & GetStorage()
		{ return _data; }
		public:
		const Storage & GetStorage() const
		{ return _data; }

		iterator begin() 				{ return _data.begin(); }
		iterator end() 					{ return _data.end(); }
		const_iterator begin() const	{ return _data.begin(); }
		const_iterator end() const		{ return _data.end(); }

		u8 & operator[] (size_t index)
		{ return _data.at(index); }
		u8 operator[] (size_t index) const
		{ return _data.at(index); }

		static ByteArray FromHex(const std::string & hex)
		{
			ASSERT((hex.size() & 1) == 0, Exception, "Invalid hex string");

			ByteArray data(hex.size() >> 1);
			u8 * dst = data.data();
			for(size_t i = 0; i != hex.size(); i += 2)
				*dst++ = (FromChar(hex[i]) << 4) | FromChar(hex[i + 1]);
			return data;
		}

	private:
		static u8 FromChar(char ch)
		{
			if (ch >= '0' && ch <= '9')
				return ch - '0';
			else if (ch >= 'a' && ch <= 'f')
				return 10u + ch - 'a';
			else if (ch >= 'A' && ch <= 'F')
				return 10u + ch - 'A';
			else
				THROW(Exception, std::string("invalid hex char ") + ch);
		}
	};
	TOOLKIT_DECLARE_PTR(ByteArray);

}

#endif
