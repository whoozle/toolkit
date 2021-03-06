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
		virtual void Write(ISerializationStream & out) = 0;
	};
	TOOLKIT_DECLARE_PTR(IObjectWriter);

	struct IObjectReader
	{
		virtual ~IObjectReader() = default;
		virtual size_t Read(IInputSerializationStream & in, ConstBuffer data) = 0;
	};
	TOOLKIT_DECLARE_PTR(IObjectReader);

	struct IGrammarDescriptor
	{
		virtual ~IGrammarDescriptor() = default;
	};

	template<typename ClassType>
	struct IDescriptor
	{
		virtual ~IDescriptor() = default;
		virtual void Write(ISerializationStream & out, const ClassType & self) const = 0;
	};

	template<typename ClassType>
	class GrammarDescriptor;

	template<typename ClassType>
	class GrammarObjectFactory
	{
		TypeDescriptor		_type;

	public:
		GrammarObjectFactory(const TypeDescriptor & type):
			_type(type)
		{ }

		void WriteTypeDescriptor(ISerializationStream & out)
		{
			if (!_type.Name.empty())
				Serialize(out, _type.Name, _type.Version);
		}

		bool IsEmpty() const
		{ return _type.Name.empty() && _type.Version == 0; }
	};

	template<typename ClassType, typename MemberType>
	class GrammarMemberDescriptor final : public IDescriptor<ClassType>
	{
		using Pointer 		= MemberType ClassType::*;

		Pointer				_pointer;

	public:
		GrammarMemberDescriptor(Pointer pointer): _pointer(pointer)
		{ }

		void Write(ISerializationStream & out, const ClassType & self) const override
		{ Serialize(out, Get(self)); }

		const MemberType & Get(const ClassType & self) const
		{ return (&self)->*_pointer; }

		void Set(ClassType & self, const MemberType & value)
		{ (&self)->*_pointer = value; }
	};

	template<typename ClassType>
	class GrammarDescriptor final : public IGrammarDescriptor
	{
		using IDescriptorPtr 	= std::shared_ptr<IDescriptor<ClassType>>;
		using MemberMap 		= std::unordered_map<std::string, IDescriptorPtr>;
		using MemberList 		= std::vector<IDescriptorPtr>;
		using FactoryPtr    	= std::shared_ptr<GrammarObjectFactory<ClassType>>;

		FactoryPtr			_factory;
		MemberList			_list;
		MemberMap			_map;

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

			void Write(ISerializationStream & out) override
			{ _descriptor.Write(out, _object); }
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

			size_t Read(IInputSerializationStream & in, ConstBuffer data) override
			{
				size_t offset = 0;
				size_t size = data.size();
				while(offset < size && !in.Finished())
					in.Parse(data, offset); //loop through all data, even for partial matches
				return offset;
			}
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

		void Write(ISerializationStream & out, const ClassType & self) const
		{
			out.BeginObject();

			if (!_factory->IsEmpty())
			{
				static const std::string metaTag("m");
				out.Write(metaTag);
				out.BeginList();
				_factory->WriteTypeDescriptor(out);
				out.EndList();
			}

			{
				static const std::string recordTag("r");
				out.Write(recordTag);
				out.BeginList();
				for (auto it : _list)
				{
					it->Write(out, self);
				}
				out.EndList();
			}

			{
				static const std::string objectTag("d");
				out.Write(objectTag);
				out.BeginObject();
				for (auto it : _map)
				{
					out.Write(it.first);
					it.second->Write(out, self);
				}
				out.EndObject();
			}
		}

		const auto & GetList() const
		{ return _list; }

		const auto & GetMap() const
		{ return _map; }

	public:
		template <typename DescriptorsType>
		GrammarDescriptor(const DescriptorsType & descriptor):
			_factory(std::make_shared<GrammarObjectFactory<ClassType>>(descriptor.Type))
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
			auto & descriptor = ClassDescriptorHolder<ClassType>::Get();
			static GrammarDescriptor<ClassType> grammarDescriptor(descriptor);
			return grammarDescriptor;
		}
	};


}}

#endif
