#ifndef TOOLKIT_IO_KEY_VALUE_STORAGE_H__
#define TOOLKIT_IO_KEY_VALUE_STORAGE_H__

#include <toolkit/core/Buffer.h>
#include <toolkit/core/ByteArray.h>
#include <string>

namespace TOOLKIT_NS { namespace io
{

	class KeyValueStorage
	{
		std::string 		_root;
		size_t				_level;

	public:
		using Key = ConstBuffer;

		KeyValueStorage(std::string root, unsigned level = 1):
			_root(std::move(root)), _level(level)
		{ }

		ByteArray Load(Key key) const;
		bool Load(Key key, ByteArray & data) const;
		void Save(Key key, ConstBuffer data) const;

		size_t GetLevel(Key key, size_t level) const
		{ return (key.size() <= level)? key.size() - 1: level; }
		std::string GetFilename(Key key, size_t level) const;
	};

}}

#endif
