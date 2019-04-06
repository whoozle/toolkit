#ifndef TOOLKIT_CORE_OPTIONAL_H
#define TOOLKIT_CORE_OPTIONAL_H

#include <type_traits>

namespace TOOLKIT_NS
{

	struct EmptyOptionalException : Exception
	{
		EmptyOptionalException(const std::string &what = std::string()): Exception(what + ": empty optional")
		{ }
	};


	template<typename Type>
	class Optional
	{
		using StorageType = typename std::aligned_storage<sizeof(Type)>::type;

		bool			_initialized;
		StorageType		_storage;

		Type & Ref()
		{ return reinterpret_cast<Type &>(_storage); }

		const Type & Ref() const
		{ return reinterpret_cast<const Type &>(_storage); }

	public:
		Optional(std::nullptr_t = nullptr): _initialized(false) { }

		template<typename ...Args>
		Optional(Args ... args)
		{
			new (&Ref()) Type (args ... );
			_initialized = true;
		}

		template<typename ...Args>
		void Construct(Args ... args)
		{
			Clear();
			new (&Ref()) Type (args ... );
			_initialized = true;
		}

		Optional(const Optional &o): _initialized(o._initialized)
		{ if (o._initialized) new (&Ref()) Type(o.Ref()); }

		Optional(Optional && o): _initialized(o._initialized)
		{
			if (o._initialized)
			{
				new (&Ref()) Type(std::move(o.Ref()));
				o.Clear();
			}
		}

		~Optional()
		{ Clear(); }

		void Clear()
		{
			if (_initialized)
			{
				Type & value = Ref();
				value.~Type();
				_initialized = false;
			}
		}

		template<typename ValueType>
		Optional & operator = (ValueType value)
		{
			if (_initialized)
				Ref() = value;
			else
			{
				new (&Ref()) Type (value);
				_initialized = true;
			}
			return *this;
		}

		Type & operator *()
		{
			if (!_initialized)
				throw EmptyOptionalException();
			return Ref();
		}

		const Type & operator *() const
		{
			if (!_initialized)
				throw EmptyOptionalException();
			return Ref();
		}

		explicit operator bool() const
		{ return _initialized; }
	};

}

#endif
