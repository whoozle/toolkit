#ifndef TOOLKIT_SERIALIZATION_JSON_H
#define TOOLKIT_SERIALIZATION_JSON_H

#include <toolkit/core/types.h>
#include <toolkit/text/String.h>
#include <toolkit/serialization/Serialization.h>
#include <toolkit/text/Formatters.h>
#include <string>
#include <tuple>

namespace TOOLKIT_NS { namespace serialization
{

	template<typename ClassType, typename ClassDescriptor>

	class JSONWriter
	{
		ClassDescriptor				_descriptor;

	public:
		struct State
		{
			bool				First;

			State(): First(true)
			{ }
		};

	protected:
		template<typename StreamType>
		static void MaybeComma(StreamType & stream, State & state)
		{
			if (!state.First)
				stream << ',';
			else
				state.First = false;
		}

	public:
		JSONWriter(ClassDescriptor && descriptor) : _descriptor(descriptor)
		{ }

		static State NewState()
		{ return State(); }

		template<typename StreamType>
		static void WriteString(StreamType & stream, const std::string &str)
		{ WriteString(stream, text::FromUtf8(str)); }

		template<typename StreamType>
		static void WriteString(StreamType & stream, const std::wstring &str)
		{
			stream << '"';
			for(auto uc : str)
			{
				if (uc >= 0x20 && uc < 0x7f)
					stream << static_cast<char>(uc);
				else
				{
					switch(uc)
					{
						case '"':
							stream << "\\\"";
							break;
						case '\\':
							stream << "\\\\";
							break;
						case '/':
							stream << "\\/";
							break;
						case '\b':
							stream << "\\b";
							break;
						case '\f':
							stream << "\\f";
							break;
						case '\n':
							stream << "\\n";
							break;
						case '\r':
							stream << "\\r";
							break;
						case '\t':
							stream << "\\t";
							break;
						default:
							stream << "\\u" << text::Hex(uc, 4);
					}
				}
			}
			stream << '"';
		}

		template<typename StreamType>
		void WriteProperty(StreamType & stream, State & state, const std::string &name, const std::string &value) const
		{
			MaybeComma(stream, state);
			WriteString(stream, name);
			stream << ':';
			WriteString(stream, value);
		}

		template<typename StreamType, typename ValueType>
		void WriteProperty(StreamType & stream, State & state, const std::string &name, ValueType && value) const
		{
			MaybeComma(stream, state);
			WriteString(stream, name);
			stream << ':' << std::forward<ValueType>(value);
		}


		template<typename StreamType>
		void Write(StreamType & stream, State & state, const ClassType & object) const
		{
			stream << "{";
			if (!_descriptor.Name.empty())
				WriteProperty(stream, state, "__classname", _descriptor.Name);
			if (_descriptor.Version != 0)
				WriteProperty(stream, state, "__version", _descriptor.Version);
			stream << "}";
		}
	};

}}


#endif

