#include <toolkit/serialization/bson/InputStream.h>
#include <toolkit/text/Formatters.h>

namespace TOOLKIT_NS { namespace serialization { namespace bson
{

	void Tokenizer::ParseGeneric(ConstBuffer data, size_t & offset)
	{
		if (_finished)
			return;

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




	void IntegerStreamParser::Parse(ConstBuffer data, size_t & offset)
	{
		if (_finished)
			return;

		size_t n = data.size();
		int shift = 0;
		while(offset < n)
		{
			u8 byte = data[offset++];
			_value |= (static_cast<s64>(byte & 0x7f) << shift);
			shift += 7;
			if ((byte & 0x80) == 0)
			{
				_finished = true;
				return;
			}
		}
	}

	void StringStreamParser::Parse(ConstBuffer data, size_t & offset)
	{
		if (_finished)
			return;

		if (!_lengthParsed) {
			if (!_lengthParser.Finished())
				_lengthParser.Parse(data, offset);
			if (!_lengthParser.Finished())
				return;
			_lengthParsed = true;
			_lengthParser.Set(*this);
		}
		size_t remain = std::min(data.size() - offset, _length);
		std::copy(data.data() + offset, data.data() + offset + remain, std::back_inserter(_value));
		offset += remain;
		_length -= remain;
		_finished = _length == 0;
	}

	void Tokenizer::Parse(ConstBuffer data, size_t & offset)
	{
		if (_finished)
			return;

		size_t size = data.size();
		if (!_current && !_finished && offset < size)
			ParseGeneric(data, offset);

		while (_current && offset < size)
		{
			auto current = _current;
			if (!current->Finished())
				current->Parse(data, offset);
			if (current->Finished())
			{
				if (_current == current)
					_current.reset();
				current->Set(*this);
				break;
			}
		}
	}

	void Tokenizer::Write(const Undefined &)
	{ throw Exception("unexpected undefined"); }

	void Tokenizer::Write(std::nullptr_t)
	{ throw Exception("unexpected null"); }

	void Tokenizer::Write(bool value)
	{ throw Exception("unexpected bool"); }

	void Tokenizer::Write(s64 value)
	{ throw Exception("unexpected integer"); }

	void Tokenizer::Write(double value)
	{ throw Exception("unexpected number"); }

	void Tokenizer::Write(const std::string & value)
	{ throw Exception("unexpected string"); }

	void Tokenizer::BeginList()
	{ throw Exception("unexpected list begin"); }

	void Tokenizer::EndList()
	{ throw Exception("unexpected list end"); }

	void Tokenizer::BeginObject()
	{ throw Exception("unexpected object begin"); }

	void Tokenizer::EndObject()
	{ throw Exception("unexpected object end"); }

	void Tokenizer::Set(ISerializationStream & target)
	{ throw Exception("no set implemented"); }
}}}
