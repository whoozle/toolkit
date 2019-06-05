#ifndef TOOLKIT_SERIALIZATION_BSONTOKENIZER_H
#define TOOLKIT_SERIALIZATION_BSONTOKENIZER_H

#include <toolkit/core/types.h>
#include <toolkit/core/Optional.h>
#include <toolkit/serialization/BSON.h>
#include <toolkit/serialization/BinaryReader.h>

namespace TOOLKIT_NS { namespace serialization { namespace bson
{

	template<typename ParserType>
	class Reader
	{
	protected:
		ParserType *This()
		{ return static_cast<ParserType *>(this); }
		const ParserType *This() const
		{ return static_cast<const ParserType *>(this); }

		Tag _currentTag;

	private:
		IntegerReader<s64>			_intReader;
		StringReader<std::string>	_stringReader;

	public:
		Reader(): _currentTag(Tag::Undefined),
			_intReader([this](s64 && value)
			{ This()->Handle(value); _currentTag = Tag::Undefined; }),
			_stringReader([this](std::string && value)
			{ This()->Handle(value); _currentTag = Tag::Undefined; })
		{ }

		template<typename IteratorType>
		void Read(IteratorType begin, const IteratorType &end)
		{
			//continuation
			switch(_currentTag)
			{
			case Tag::String:
				_stringReader.Read(begin, end);
				break;
			case Tag::NegativeInteger:
			case Tag::PositiveInteger:
				_intReader.Read(begin, end);
				break;
			case Tag::Number:
				break;
			default:
				break;
			}

			while(begin != end)
			{
				_currentTag = static_cast<Tag>(*begin++);
				switch(_currentTag)
				{
				case Tag::Undefined:
					This()->Handle(Undefined());
					break;
				case Tag::Null:
					This()->Handle(nullptr);
					break;
				case Tag::BooleanFalse:
					This()->Handle(false);
					break;
				case Tag::BooleanTrue:
					This()->Handle(true);
					break;
				case Tag::Zero:
					This()->Handle((s64)0);
					break;
				case Tag::ListBegin:
					This()->HandleListBegin();
					break;
				case Tag::ListEnd:
					This()->HandleListEnd();
					break;
				case Tag::ObjectBegin:
					This()->HandleListBegin();
					break;
				case Tag::ObjectEnd:
					This()->HandleListEnd();
					break;
				case Tag::PositiveInteger:
				case Tag::NegativeInteger:
					_intReader.SetPositive(_currentTag == Tag::PositiveInteger);
					_intReader.Read(begin, end);
					break;
				case Tag::String:
					_stringReader.Read(begin, end);
					break;
				default:
					throw std::runtime_error("invalid tag");
				}
			}
		}
	};


}}}


#endif
