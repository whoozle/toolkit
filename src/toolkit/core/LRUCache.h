#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <toolkit/core/IntrusiveList.h>
#include <toolkit/core/Counter.h>
#include <unordered_map>

TOOLKIT_NS_BEGIN

	template<typename ValueType>
	struct LRUCacheNode : public ValueType, Counter<ValueType>, IntrusiveListNode<LRUCacheNode<ValueType>>
	{
		LRUCacheNode(ValueType && value): ValueType(std::move(value)) { }
	};

	template<typename KeyType, typename ValueType, typename Hash = std::hash<KeyType>, typename Equal = std::equal_to<KeyType>>
	class LRUCache
	{
	public:
		using Node = LRUCacheNode<ValueType>;
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
				_list.Append(ptr.get()); //move node to the end
				return true;
			}
			else
				return false;
		}

		NodePtr Insert(const KeyType & key, ValueType && value)
		{
			NodePtr ptr(new Node(std::move(value)), true);
			_cache[key] = ptr;
			_list.Append(ptr.get());
			return ptr;
		}

		void Touch(const NodePtr & ptr)
		{
			_list.Append(ptr.get());
		}
	};

TOOLKIT_NS_END

#endif

