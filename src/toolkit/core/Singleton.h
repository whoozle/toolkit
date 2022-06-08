#ifndef TOOLKIT_LOG_SINGLETON_H
#define TOOLKIT_LOG_SINGLETON_H

#include <atomic>
#include <type_traits>

namespace TOOLKIT_NS
{
	template <typename ClassName>
	class Singleton
	{
	private:
		using Storage = typename std::aligned_storage<sizeof(ClassName), alignof(ClassName)>::type;
		Storage 			_storage;
		std::atomic<bool> 	_alive;

	private:
		ClassName& Ref()
		{ return reinterpret_cast<ClassName &>(_storage); }

		void Ctor()
		{
			bool expected = false;
			if (_alive.compare_exchange_weak(expected, true))
				new (&_storage) ClassName();
		}

		void Dtor()
		{
			bool expected = true;
			if (_alive.compare_exchange_weak(expected, false))
				Ref().ClassName::~ClassName();
		}

	public:
		ClassName& Get()
		{ Ctor(); return Ref(); }

		Singleton()
		{ }

		~Singleton()
		{ Dtor(); }
	};


}

#endif
