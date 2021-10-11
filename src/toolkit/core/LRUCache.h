#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <toolkit/core/IntrusiveList.h>
#include <toolkit/core/Counter.h>
#include <unordered_map>

namespace TOOLKIT_NS
{

	template<typename KeyType, typename ValueType>
	struct LRUCacheNode :
		Counter<ValueType>,
		IntrusiveListNode<LRUCacheNode<KeyType, ValueType>>
	{
		KeyType 		Key;
		ValueType		Value;

		LRUCacheNode(const KeyType & key, ValueType && value):
			Key(std::move(key)), Value(std::move(value))
		{ }
	};

	template<typename KeyType, typename ValueType, typename Hash = std::hash<KeyType>, typename Equal = std::equal_to<KeyType>>
	class LRUCache
	{
	public:
		using Node = LRUCacheNode<KeyType, ValueType>;
		using NodePtr = CounterPtr<Node>;

	private:
		std::unordered_map<KeyType, NodePtr, Hash, Equal> 	_cache;
		IntrusiveList<Node>									_list;

	public:
		bool Lookup(const KeyType & key, NodePtr & ptr)
		{
			auto it = _cache.find(key);
			if (it != _cache.end())
			{
				ptr = it->second;
				Touch(ptr);
				return true;
			}
			else
				return false;
		}

		NodePtr Insert(const KeyType & key, ValueType && value)
		{
			NodePtr ptr(new Node(key, std::move(value)));
			_cache[key] = ptr;
			Touch(ptr);
			return ptr;
		}

		bool IsEmpty() const
		{ return _list.IsEmpty(); }

		void RemoveOldest()
		{
			if (IsEmpty())
				return;

			auto& oldest = _list.GetFirstValue();
			_cache.erase(oldest.Key);
		}

		void Touch(const NodePtr & ptr)
		{
			_list.Append(ptr.get()); //move node to the end
		}
	};

}

#endif

