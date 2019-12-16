#include <toolkit/serialization/bson/InputStream.h>
#include <toolkit/text/Formatters.h>

namespace TOOLKIT_NS { namespace serialization { namespace bson
{
	namespace
	{
		class IntegerParser : public IInputStreamParser
		{
			s64		_value;
			bool	_negative;

		public:
			IntegerParser(bool neg): _value(0), _negative(neg) { }

			size_t Parse(ConstBuffer data) override
			{ return 0; }

			void Set(ISerializationStream & target) override
			{ target.Write(_value); }
		};

		class NumberParser : public IInputStreamParser
		{
			double	_value;
			bool	_negative;

		public:
			NumberParser(bool neg): _value(0), _negative(neg) { }

			size_t Parse(ConstBuffer data) override
			{ return 0; }

			void Set(ISerializationStream & target) override
			{ target.Write(_value); }
		};

		class StringParser : public IInputStreamParser
		{
			std::string 	_value;

		public:
			size_t Parse(ConstBuffer data) override
			{ return 0; }

			void Set(ISerializationStream & target) override
			{ target.Write(_value); }
		};

	}

	size_t BaseInputStream::Parse(ConstBuffer data)
	{
		size_t offset = 0, size = data.size();

		while(offset < size)
		{
			if (_current)
			{
				auto r = _current->Parse(ConstBuffer(data, offset));
				offset += r;
				if (r == 0)
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

}}}
