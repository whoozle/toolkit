#ifndef TOOLKIT_SERIALIZATION_BINARYREADER_H
#define TOOLKIT_SERIALIZATION_BINARYREADER_H

#include <toolkit/core/types.h>
#include <toolkit/core/ByteArray.h>
#include <toolkit/core/Optional.h>
#include <toolkit/core/Noncopyable.h>
#include <toolkit/serialization/BinaryReaderBase.h>
#include <string>
#include <algorithm>
#include <tuple>

namespace TOOLKIT_NS { namespace serialization
{

	template<typename ValueType>
	class IntegerReader : public BaseBinaryReader<ValueType>
	{
		ValueType	_value;
		s8			_sign;

		using super = BaseBinaryReader<ValueType>;

	public:
		IntegerReader(const typename super::Callback & callback): super(callback), _value(0), _sign(1)
		{ }

		IntegerReader(typename super::Callback && callback): super(callback), _value(0), _sign(1)
		{ }

		void SetPositive(bool positive)
		{ _sign = positive? 1: -1; }

		template<typename IteratorType>
		void Read(IteratorType & begin, const IteratorType &end)
		{
			while(begin != end)
			{
				u8 byte = *begin++;
				_value = (_value << 7) | (byte & 0x7f);
				if (!(byte & 0x80))
				{
					super::_callback(std::move(_sign * _value));
					_value = 0;
					break;
				}
			}
		}
	};

	template<typename StringType>
	class StringReader : public BaseBinaryReader<StringType>
	{
		using super = BaseBinaryReader<StringType>;

		bool					_readSize;
		size_t					_offset;
		StringType				_value;
		IntegerReader<size_t>	_sizeReader;

	public:
		StringReader(typename super::Callback && callback): super(callback),
			_readSize(false), _offset(),
			_sizeReader([this](size_t size)
			{ _readSize = true; _value.resize(size); _offset = 0; })
		{ }

		template<typename IteratorType>
		void Read(IteratorType & begin, const IteratorType & end)
		{
			if (!_readSize)
			{
				_sizeReader.Read(begin, end);
				if (!_readSize)
					return;
			}
			ReadData(begin, end);
			if (_offset >= _value.size())
			{
				super::_callback(std::move(_value));
				_readSize = false;
				_value.clear();
				_offset = 0;
			}
		}

	private:
		template<typename IteratorType>
		void ReadData(IteratorType & begin, const IteratorType & end)
		{
			size_t n = std::min<size_t>(std::distance(begin, end), _value.size() - _offset);
			std::copy_n(begin, n, _value.begin() + _offset);
			_offset += n;
			std::advance(begin, n);
		}

	};

	template<>
	struct BinaryReader<bool> : public BaseBinaryReader<bool>
	{
		using super = BaseBinaryReader<bool>;
		using super::super;

		template<typename IteratorType>
		void Read(IteratorType & begin, const IteratorType & end)
		{
			u8 value = *begin++;
			switch(value)
			{
			case 0:		this->_callback(false); break;
			case 1:		this->_callback(true); break;
			default:	throw std::runtime_error("read invalid bool value");
			}
		}
	};

	template<>
	struct BinaryReader<std::nullptr_t> : BaseBinaryReader<std::nullptr_t>
	{
		using super = BaseBinaryReader<std::nullptr_t>;
		using super::super;

		template<typename IteratorType>
		void Read(IteratorType & begin, const IteratorType & end)
		{ _callback(nullptr); }
	};

	template<>
	struct BinaryReader<std::string> : StringReader<std::string>
	{ using super = StringReader<std::string>; using super::super; };

	template<>
	struct BinaryReader<ByteArray> : StringReader<ByteArray>
	{ using super = StringReader<ByteArray>; using super::super; };

#define DECLARE_INT_READER(T) \
	template<> struct BinaryReader< T > : IntegerReader< T > \
	{ using super = IntegerReader< T >; using super::super; };

	DECLARE_INT_READER(s8);
	DECLARE_INT_READER(s16);
	DECLARE_INT_READER(s32);
	DECLARE_INT_READER(s64);
	DECLARE_INT_READER(u8);
	DECLARE_INT_READER(u16);
	DECLARE_INT_READER(u32);
	DECLARE_INT_READER(u64);

#undef DECLARE_INT_READER
}}

#endif
