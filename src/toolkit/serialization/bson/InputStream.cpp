#include <toolkit/serialization/bson/InputStream.h>
#include <toolkit/text/Formatters.h>

namespace TOOLKIT_NS { namespace serialization { namespace bson
{
	namespace
	{
		class IntegerParser : public BaseInputStream
		{
			s64		_value;
			bool	_negative;
			bool	_loaded;

		public:
			IntegerParser(bool neg): _value(0), _negative(neg), _loaded(false) { }

			bool Parse(ConstBuffer data, size_t & offset) override
			{
				if (_loaded)
					return false;

				size_t n = data.size();
				int shift = 0;
				while(offset < n)
				{
					u8 byte = data[offset++];
					_value |= (static_cast<s64>(byte & 0x7f) << shift);
					shift += 7;
					if ((byte & 0x80) == 0)
					{
						_loaded = true;
						return false; //loaded
					}
				}
				return true;
			}

			void Set(ISerializationStream & target) override
			{ target.Write(_value); }
		};

		class NumberParser : public BaseInputStream
		{
			double	_value;
			bool	_negative;

		public:
			NumberParser(bool neg): _value(0), _negative(neg) { }

			bool Parse(ConstBuffer data, size_t & offset) override
			{ return 0; }

			void Write(s64 value) override //handling zero
			{ _value = value; }

			void Set(ISerializationStream & target) override
			{ target.Write(_value); }
		};

		class StringParser : public BaseInputStream
		{
			IntegerParser 	_lengthParser;
			bool			_lengthParsed;
			size_t			_length;
			std::string 	_value;

		public:
			StringParser(): _lengthParser(false), _lengthParsed(false), _length(0)
			{ }

			bool Parse(ConstBuffer data, size_t & offset) override
			{
				if (!_lengthParsed) {
					if (_lengthParser.Parse(data, offset))
						return true;
					_lengthParsed = true;
					_lengthParser.Set(*this);
				}
				size_t remain = std::min(data.size() - offset, _length);
				std::copy(data.data() + offset, data.data() + offset + remain, std::back_inserter(_value));
				offset += remain;
				_length -= remain;
				return _length != 0;
			}
			void Write(s64 value) override
			{ _length = value; }

			void Set(ISerializationStream & target) override
			{ target.Write(_value); }
		};

	}

	bool BaseInputStream::Parse(ConstBuffer data, size_t & offset)
	{
		size_t size = data.size();

		while(offset < size)
		{
			if (_current)
			{
				if (!_current->Parse(data, offset))
				{
					_current->Set(*this);
					_current.reset();
				}
			}
			Tag tag = static_cast<Tag>(data[offset++]);
			switch(tag)
			{
			case Tag::Undefined:
				Write(Undefined());
				break;
			case Tag::BooleanTrue:
				Write(true);
				break;
			case Tag::BooleanFalse:
				Write(false);
				break;
			case Tag::Zero:
				Write(s64(0));
				break;
			case Tag::PositiveInteger:
				_current = std::make_shared<IntegerParser>(false);
				break;
			case Tag::NegativeInteger:
				_current = std::make_shared<IntegerParser>(true);
				break;
			case Tag::PositiveNumber:
				_current = std::make_shared<NumberParser>(false);
				break;
			case Tag::NegativeNumber:
				_current = std::make_shared<NumberParser>(true);
				break;
			case Tag::String:
				_current = std::make_shared<StringParser>();
				break;
			case Tag::ListBegin:
				BeginList();
				break;
			case Tag::ListEnd:
				EndList();
				break;
			case Tag::Null:
				Write(nullptr);
				break;
			case Tag::ObjectBegin:
				BeginObject();
				break;
			case Tag::ObjectEnd:
				EndObject();
				break;
			default:
				throw Exception("unknown tag 0x" + text::Hex(static_cast<u8>(tag)).ToString() + ", corrupted stream");
			}
		}
		return offset;
	}

	void BaseInputStream::Write(const Undefined &)
	{ throw Exception("unexpected undefined"); }

	void BaseInputStream::Write(std::nullptr_t)
	{ throw Exception("unexpected null"); }

	void BaseInputStream::Write(bool value)
	{ throw Exception("unexpected bool"); }

	void BaseInputStream::Write(s64 value)
	{ throw Exception("unexpected integer"); }

	void BaseInputStream::Write(double value)
	{ throw Exception("unexpected number"); }

	void BaseInputStream::Write(const std::string & value)
	{ throw Exception("unexpected string"); }

	void BaseInputStream::BeginList()
	{ throw Exception("unexpected list begin"); }

	void BaseInputStream::EndList()
	{ throw Exception("unexpected list end"); }

	void BaseInputStream::BeginObject()
	{ throw Exception("unexpected object begin"); }

	void BaseInputStream::EndObject()
	{ throw Exception("unexpected object end"); }

	void BaseInputStream::Set(ISerializationStream & target)
	{ throw Exception("no set implemented"); }
}}}
