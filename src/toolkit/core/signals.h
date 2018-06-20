#ifndef TOOLKIT_SIGNALS_H
#define TOOLKIT_SIGNALS_H

#include <pure/core/FunctionTraits.h>
#include <pure/core/IntrusiveList.h>
#include <pure/core/NonCopyable.h>
#include <pure/core/Token.h>
#include <functional>
#include <memory>

namespace pure
{
	template<typename Signature>
	class signal;

	namespace impl
	{
		template <typename SlotType>
		class SlotStorage : NonCopyable
		{
			IntrusiveList<SlotType> _slots;

		public:
			using Node = typename IntrusiveList<SlotType>::Node;

			template<typename Slot>
			Node * Append(Slot slot)
			{ return _slots.Append(slot); }

			template<typename ... Args>
			void InvokeAll(const Args & ... args)
			{
				for (const auto & slot : _slots)
				{ try { slot(args ...); } catch(const std::exception &ex) { /*pass exception to exception handler*/} }
			}
		};
	}

	template <typename ... Args>
	class signal<void (Args ...)> : NonCopyable
	{
	public:
		using Signature			= void (Args ...);
		using SlotType			= std::function<Signature>;

	private:
		using Storage = impl::SlotStorage<SlotType>;
		using StorageNode = typename Storage::Node;

		Storage _slots;

		class Connection : public IToken
		{
			std::unique_ptr<StorageNode> _slot; //fixme: remove it

		public:
			Connection(StorageNode *slot): _slot(slot) { }
		};

	public:

		void operator () (const Args & ... args)
		{ _slots.template InvokeAll(args...); }

		template<typename Slot>
		Token Connect(Slot slot)
		{ return Token(ITokenPtr(new Connection(_slots.Append(slot)))); }
	};
}

#endif
