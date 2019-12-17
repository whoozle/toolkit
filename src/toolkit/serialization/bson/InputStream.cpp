#include <toolkit/serialization/bson/InputStream.h>
#include <toolkit/text/Formatters.h>

namespace TOOLKIT_NS { namespace serialization { namespace bson
{
	bool IntegerStreamParser::Parse(ConstBuffer data, size_t & offset)
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

	bool StringStreamParser::Parse(ConstBuffer data, size_t & offset)
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

	bool ListStreamParser::Parse(ConstBuffer data, size_t & offset)
	{
		if (_index >= _parsers.size())
			return false;

		size_t n = data.size();
		while(offset < n)
		{
			if (!_parsers[_index]->Parse(data, offset))
			{
				if (++_index >= _parsers.size())
					return false;
			}
		}
		return true;
	}

	bool BaseInputStream::Parse(ConstBuffer data, size_t & offset)
	{
		if (_finished)
			return false;

		size_t size = data.size();
		while(offset < size)
		{
			while (_current)
			{
				if (!_current->Parse(data, offset))
				{
					auto current = _current;
					_current.reset();
					current->Set(*this);
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
				_current = std::make_shared<IntegerStreamParser>(false);
				break;
			case Tag::NegativeInteger:
				_current = std::make_shared<IntegerStreamParser>(true);
				break;
			case Tag::PositiveNumber:
				_current = std::make_shared<NumberStreamParser>(false);
				break;
			case Tag::NegativeNumber:
				_current = std::make_shared<NumberStreamParser>(true);
				break;
			case Tag::String:
				_current = std::make_shared<StringStreamParser>();
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
