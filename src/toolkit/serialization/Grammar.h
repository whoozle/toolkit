#ifndef TOOLKIT_SERIALIZATION_GRAMMAR_H
#define TOOLKIT_SERIALIZATION_GRAMMAR_H

#include <toolkit/serialization/ISerializationStream.h>
#include <toolkit/serialization/Serialization.h>
#include <toolkit/serialization/Serializator.h>
#include <unordered_map>
#include <vector>
#include <memory>

namespace TOOLKIT_NS { namespace serialization
{
	struct IObjectWriter
	{
		virtual ~IObjectWriter() = default;
		virtual void Write(IOutputStream & out) = 0;
	};
	TOOLKIT_DECLARE_PTR(IObjectWriter);

	struct IObjectReader
	{
		virtual ~IObjectReader() = default;
		virtual void Read(ConstBuffer data) = 0;
	};
	TOOLKIT_DECLARE_PTR(IObjectReader);

	template<typename ClassType>
	struct IDescriptor
	{
		virtual ~IDescriptor() = default;
		virtual void Write(IOutputStream & out, const ClassType & self) const = 0;
	};

	template<typename ClassType>
	class GrammarDescriptor;

	template<typename ClassType>
	class GrammarObjectFactory
	{
		std::string			_name;
		uint				_version;

	public:
		GrammarObjectFactory(const std::string & name, uint version):
			_name(name), _version(version)
		{ }
		void WriteTypeDescriptor(IOutputStream & out)
		{
			if (!_name.empty())
				Serialize(out, _name, _version);
		}
	};

	template<typename ClassType, typename MemberType>
	class GrammarMemberDescriptor final : public IDescriptor<ClassType>
	{
		using Pointer 		= MemberType ClassType::*;

		Pointer				_pointer;

	public:
		GrammarMemberDescriptor(Pointer pointer): _pointer(pointer)
		{ }

		void Write(IOutputStream & out, const ClassType & self) const override
		{ Serialize(out, Get(self)); }

		const MemberType & Get(const ClassType & self) const
		{ return (&self)->*_pointer; }

		void Set(ClassType & self, const MemberType & value)
		{ (&self)->*_pointer = value; }
	};

	template<typename ClassType>
	class GrammarDescriptor
	{
		using IDescriptorPtr 	= std::shared_ptr<IDescriptor<ClassType>>;
		using MemberMap 		= std::unordered_map<std::string, IDescriptorPtr>;
		using MemberList 		= std::vector<IDescriptorPtr>;
		using FactoryPtr    	= std::shared_ptr<GrammarObjectFactory<ClassType>>;

		FactoryPtr			_factory;
		MemberMap			_map;
		MemberList			_list;

	private:

		class ObjectWriter : public IObjectWriter
		{
		private:
			using Descriptor = GrammarDescriptor<ClassType>;

			const Descriptor &		_descriptor;
			const ClassType &		_object;

		public:
			ObjectWriter(const Descriptor & descriptor, const ClassType & object):
				_descriptor(descriptor), _object(object)
			{ }

			void Write(IOutputStream & out) override
			{
				out.BeginObject();
				{
					static std::string typeTag("t");
					Serialize(out, typeTag);
					_descriptor.WriteRecord(out, _object);
				}
				{
					static std::string objectTag("o");
					Serialize(out, objectTag);
					_descriptor.WriteObject(out, _object);
				}
				out.EndObject();
			}
		};

		class ObjectReader : public IObjectReader
		{
		private:
			using Descriptor = GrammarDescriptor<ClassType>;

			const Descriptor &		_descriptor;
			ClassType &				_object;

		public:
			ObjectReader(const Descriptor & descriptor, ClassType & object):
				_descriptor(descriptor), _object(object)
			{ }

			void Read(ConstBuffer data) override
			{ }
		};


		template<typename MemberType>
		void Add(const MemberDescriptor<ClassType, MemberType> & descriptor)
		{
			IDescriptorPtr grammarDesc = std::make_shared<GrammarMemberDescriptor<ClassType, MemberType>>(descriptor.Pointer);
			if (descriptor.Name.empty())
				_list.push_back(grammarDesc);
			else
				_map[descriptor.Name] = grammarDesc;
		}

		template<typename DescriptorType>
		void AddDescriptor(const DescriptorType & descriptor)
		{
			Add(descriptor);
		}

		template<std::size_t MemberCount, size_t Index = 0, typename DescriptorsType>
		typename std::enable_if<Index < MemberCount, void>::type
		AddDescriptors(const DescriptorsType & descriptors)
		{
			AddDescriptor(std::get<Index>(descriptors.Data));
			AddDescriptors<MemberCount, Index + 1>(descriptors);
		}

		template<std::size_t MemberCount, size_t Index = 0, typename DescriptorsType>
		static typename std::enable_if<Index == MemberCount, void>::type
		AddDescriptors(const DescriptorsType & descriptors)
		{ }

		void WriteRecord(IOutputStream & out, const ClassType & self) const
		{
			out.BeginList();
			_factory->WriteTypeDescriptor(out);
			for (auto it : _list)
			{
				it->Write(out, self);
			}
			out.EndList();
		}

		void WriteObject(IOutputStream & out, const ClassType & self) const
		{
			out.BeginObject();
			for (auto it : _map)
			{
				out.Write(it.first);
				it.second->Write(out, self);
			}
			out.EndObject();
		}

	public:
		template <typename DescriptorsType>
		GrammarDescriptor(const DescriptorsType & descriptor):
			_factory(std::make_shared<GrammarObjectFactory<ClassType>>(descriptor.Name, descriptor.Version))
		{ AddDescriptors<DescriptorsType::MemberCount>(descriptor); }

		IObjectWriterPtr CreateWriter(const ClassType & object) const
		{ return std::make_shared<ObjectWriter>(*this, object); }

		IObjectReaderPtr CreateReader(ClassType & object) const
		{ return std::make_shared<ObjectReader>(*this, object); }
	};

	template<typename ClassType>
	struct GrammarDescriptorHolder
	{
		static auto & Get()
		{
			static GrammarDescriptor<ClassType> descriptor(ClassDescriptorHolder<ClassType>::Get());
			return descriptor;
		}
	};


}}

#endif
