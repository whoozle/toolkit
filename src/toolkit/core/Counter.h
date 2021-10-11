#ifndef TOOLKIT_CORE_COUNTER_H
#define TOOLKIT_CORE_COUNTER_H

#include <toolkit/core/Exception.h>
#include <toolkit/core/Noncopyable.h>
#include <toolkit/core/types.h>

#include <atomic>
#include <utility>

namespace TOOLKIT_NS
{

	template < typename Type >
	class CounterPtr
	{
		template<typename U> friend class CounterPtr;

	private:
		Type *			_ptr;

	private:
		void CheckNullPointer() const
		{ if (!_ptr) throw NullPointerException(); }

	public:
		typedef Type ValueType;

		CounterPtr() : _ptr()
		{ }

		CounterPtr(std::nullptr_t) : CounterPtr()
		{ }

		explicit CounterPtr(Type * ptr, bool addRef = false) : _ptr(ptr)
		{
			if (ptr && addRef)
				ptr->AddRef();
		}

		CounterPtr(const CounterPtr<Type> & other) : _ptr(other._ptr)
		{ if (_ptr) _ptr->AddRef(); }

		~CounterPtr()
		{
			if (_ptr && _ptr->ReleaseRef())
				delete _ptr;
		}

		CounterPtr & operator = (const CounterPtr & other)
		{
			CounterPtr tmp(other);
			swap(tmp);
			return *this;
		}

		void reset(Type* ptr = nullptr) //mimic std pointers.
		{
			CounterPtr<Type> tmp(ptr);
			swap(tmp);
		}

		void swap(CounterPtr<Type> & other)
		{ std::swap(_ptr, other._ptr); }

		Type * get() const				{ return _ptr; }
		Type * operator -> () const		{ CheckNullPointer(); return _ptr; }
		Type & operator * () const		{ CheckNullPointer(); return *_ptr; }
		bool unique() const				{ return _ptr? _ptr->GetReferenceCount() == 1: true; }

		bool before(const CounterPtr & other) const
		{ return get() < other.get(); }

		explicit operator bool () const							{ return _ptr; }

		bool operator == (Type * ptr) const						{ return _ptr == ptr; }
		bool operator != (Type * ptr) const						{ return !(*this == ptr); }
		bool operator == (const CounterPtr<Type> & other) const	{ return other == _ptr; }
		bool operator != (const CounterPtr<Type> & other) const	{ return !(*this == other); }
	};

	template<typename Type, typename ValueType_ = s32>
	class Counter : public Noncopyable
	{
		typedef std::atomic<ValueType_>		CounterType;
		mutable CounterType					_counter;

	public:
		typedef ValueType_					ValueType;
		typedef CounterPtr<Type>			PointerType;

		Counter(): _counter(static_cast<ValueType>(1)) { }

		ValueType GetReferenceCounter() const
		{ return _counter; }

		void AddRef() const
		{ ++_counter; }

		bool ReleaseRef() const
		{ return --_counter == 0; }
	};

#define DECLARE_COUNTER_PTR(CLASS) typedef TOOLKIT_NS :: CounterPtr< CLASS > CLASS##Ptr

}

#endif
