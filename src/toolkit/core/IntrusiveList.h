#ifndef TOOLKIT_CORE_INTRUSIVELIST_H
#define TOOLKIT_CORE_INTRUSIVELIST_H

#include <iterator>

TOOLKIT_NS_BEGIN

	template<typename ValueType>
	class IntrusiveListNode
	{
		template<typename, size_t>
		friend class IntrusiveList;
		template<typename, typename>
		friend class IntrusiveListIterator;

		IntrusiveListNode *_prev, *_next;

	public:
		IntrusiveListNode(): _prev(this), _next(this)
		{ }

		IntrusiveListNode(const IntrusiveListNode & src) = delete;
		IntrusiveListNode& operator = (IntrusiveListNode & src) = delete;

		void Unlink()
		{
			_prev->_next = _next;
			_next->_prev = _prev;
			_prev = _next = this;
		}

		void Insert(IntrusiveListNode &before)
		{
			//unlink
			_prev->_next = _next;
			_next->_prev = _prev;

			//link
			_prev = before._prev;
			_next = &before;
			_prev->_next = _next->_prev = this;
		}

		bool Linked() const
		{ return _prev != this; }

		ValueType *GetValuePointer()
		{ return static_cast<ValueType *>(this); }

		const ValueType *GetValuePointer() const
		{ return static_cast<const ValueType *>(this); }

	protected:
		~IntrusiveListNode()
		{ Unlink(); }
	};

	template<typename ValueType, typename NodeType>
	class IntrusiveListIterator :
		public std::iterator<std::bidirectional_iterator_tag, ValueType>
	{
		using Node = NodeType;
		Node *_node;

	public:
		IntrusiveListIterator(Node * node) : _node(node) { }
		bool operator == (const IntrusiveListIterator &it) { return _node == it._node; }
		bool operator != (const IntrusiveListIterator &it) { return _node != it._node; }
		explicit operator bool() const { return _node; }

		ValueType * operator * ()
		{ return _node->GetValuePointer(); }

		const ValueType * operator * () const
		{ return _node->GetValuePointer(); }

		ValueType * operator -> ()
		{ return _node->GetValuePointer(); }

		const ValueType * operator -> () const
		{ return _node->GetValuePointer(); }

		IntrusiveListIterator& operator++ ()
		{
			_node = _node->_next;
			return *this;
		}

		IntrusiveListIterator& operator-- ()
		{
			_node = _node->_prev;
			return *this;
		}

		IntrusiveListIterator operator++ (int)
		{ IntrusiveListIterator prev(*this); ++(*this); return prev; }

		IntrusiveListIterator operator-- (int)
		{ IntrusiveListIterator prev(*this); --(*this); return prev; }
	};


	template<typename ValueType, size_t Index = 0L>
	class IntrusiveList
	{
	public:
		using Node		= IntrusiveListNode<ValueType>;

		using iterator			= IntrusiveListIterator<ValueType, IntrusiveListNode<ValueType>>;
		using const_iterator	= IntrusiveListIterator<const ValueType, const IntrusiveListNode<ValueType>>;

//		struct NodeStorage : public ValueType, public Node
//		{
//			NodeStorage(const ValueType & value) : ValueType(value) { }
//			NodeStorage(ValueType && value) noexcept : ValueType(std::move(value)) { }
//		};
//
		iterator begin() noexcept
		{ return _root._next; }

		const_iterator begin() const noexcept
		{ return _root._next; }

		const_iterator cbegin() const noexcept
		{ return _root._next; }

		iterator end() noexcept
		{ return &_root; }

		const_iterator end() const noexcept
		{ return &_root; }

		const_iterator cend() const noexcept
		{ return &_root; }

		Node & GetRoot()
		{ return _root; }

		const Node & GetRoot() const
		{ return _root; }

		void Append(Node * node)
		{ node->Insert(_root); }

		void push_back(Node *node)
		{ Append(node); }

		void DeleteAll()
		{
			auto i = begin(), e = end();
			while(i != e)
				delete *i++;
		}

		bool IsEmpty() const
		{ return !_root.Linked(); }

		ValueType & GetFirstValue()
		{ return *_root._next->GetValuePointer(); }
		const ValueType & GetFirstValue() const
		{ return *_root._next->GetValuePointer(); }

		ValueType & GetLastValue()
		{ return *_root._prev->GetValuePointer(); }
		const ValueType & GetLastValue() const
		{ return *_root._prev->GetValuePointer(); }

	private:
		Node _root;
	};
TOOLKIT_NS_END

#endif
