#ifndef TOOLKIT_SERIALIZATION_JSON_H
#define TOOLKIT_SERIALIZATION_JSON_H

#include <toolkit/core/types.h>
#include <toolkit/text/String.h>
#include <toolkit/serialization/Serialization.h>
#include <toolkit/text/Formatters.h>
#include <string>
#include <tuple>
#include <utility>

namespace TOOLKIT_NS { namespace serialization
{

	template<typename ClassType, typename ClassDescriptor>
	class JSONWriter
	{
		ClassDescriptor				_descriptor;

	public:
		struct State
		{
			const ClassType *	Object;
			bool				First;

			State(const ClassType * object): Object(object), First(true)
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

		template<typename StreamType, typename DescriptorType>
		void WriteDescriptor(StreamType & stream, State & state, const DescriptorType & desc) const
		{
			if (!desc.Name.empty())
				WriteProperty(stream, state, desc.Name, desc.Get(state.Object));
		}

		template<std::size_t MemberCount, size_t Index = 0, typename StreamType>
		typename std::enable_if<Index < MemberCount, void>::type
		WriteDescriptors(StreamType & stream, State & state) const
		{
			WriteDescriptor(stream, state, std::get<Index>(_descriptor.Data));
			WriteDescriptors<MemberCount, Index + 1>(stream, state);
		}

		template<std::size_t MemberCount, size_t Index = 0, typename StreamType>
		typename std::enable_if<Index == MemberCount, void>::type
		WriteDescriptors(StreamType & stream, State & state) const
		{ }

	public:
		JSONWriter(ClassDescriptor && descriptor) : _descriptor(descriptor)
		{ }

		static State NewState(const ClassType & object)
		{ return State(&object); }

		template<typename StreamType>
		static void WriteString(StreamType & stream, const std::string &str)
		{ WriteString(stream, text::FromUtf8(str)); }

		template<typename StreamType>
		static void WriteString(StreamType & stream, const std::wstring &str)
		{
			stream << '"';
			for(auto uc : str)
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
						if (uc >= 0x20 && uc < 0x7f)
							stream << static_cast<char>(uc);
						else
							stream << "\\u" << text::Hex(uc, 4);
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

		template<typename StreamType>
		void WriteProperty(StreamType & stream, State & state, const std::string &name, const std::wstring &value) const
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
		void Write(StreamType & stream, State & state) const
		{
			stream << "{";
			if (!_descriptor.Name.empty())
				WriteProperty(stream, state, "__classname", _descriptor.Name);
			if (_descriptor.Version != 0)
				WriteProperty(stream, state, "__version", _descriptor.Version);

			WriteDescriptors<ClassDescriptor::MemberCount>(stream, state);
			stream << "}";
		}
	};

	namespace json
	{
		class NumberReader
		{
		public:
			struct State
			{
				bool 	IsNumber;
				s64		Integer;
				double 	Number;
				bool	IsExponent;

				State(): IsNumber(false), Integer(0), Number(0), IsExponent(false)
				{ }
			};
		};

		class StringReader
		{
		public:
			struct State
			{
				std::string 	String;
				bool			Escape;

				State(): Escape(false)
				{ }
			};
		};
	}

	template<typename ClassType, typename ClassDescriptor>
	class JSONReader
	{
		ClassDescriptor				_descriptor;

	public:
		struct State
		{
			const ClassType *	Object;
			enum struct CurrentState
			{
				Initial
			};

			State(const ClassType * object): Object(object)
			{ }
		};

		JSONReader(ClassDescriptor && descriptor) : _descriptor(descriptor)
		{ }

		static State NewState(const ClassType & object)
		{ return State(&object); }

		void Handle(State & state, u8 ch) const
		{
			printf("HELLO: %u\n", ch);
		}
	};


}}


#endif

