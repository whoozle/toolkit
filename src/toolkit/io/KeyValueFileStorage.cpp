#include <toolkit/io/KeyValueFileStorage.h>
#include <toolkit/io/Directory.h>
#include <toolkit/io/File.h>
#include <toolkit/text/Formatters.h>
#include <toolkit/core/Exception.h>

namespace TOOLKIT_NS { namespace io
{

	ByteArray KeyValueStorage::Load(Key key) const
	{
		ByteArray result;
		ASSERT(Load(key, result), Exception, "no value for key " + text::Hex(key).ToString());
		return result;
	}

	bool KeyValueStorage::Load(Key key, ByteArray & data) const
	{
		size_t level = GetLevel(key, _level);
		for(unsigned i = 0; i != level; ++i)
		{
			auto fname = GetFilename(key, level - i - 1);
			if (io::File::Access(fname))
			{
				auto stat = io::File::GetStatus(fname);
				ByteArray buf(stat.st_size);
				ASSERT(io::File(fname).Read(buf) == buf.size(), Exception, "short read");
				data = std::move(buf);
				return true;
			}
		}
		return false;
	}

	void KeyValueStorage::Save(Key key, ConstBuffer data) const
	{
		size_t level = GetLevel(key, _level);
		auto dst = GetFilename(key, level);
		auto dstTemp = dst + ".tmp";

		{
			io::File f(dstTemp, io::FileOpenMode::Overwrite);
			f.Write(data);
		}
		io::File::Rename(dstTemp, dst);
	}

	std::string KeyValueStorage::GetFilename(Key key, size_t level) const
	{
		level = GetLevel(key, level);
		text::StringOutputStream ss;
		ss << _root << "/";
		for(size_t i = 0; i != level; ++i)
			ss << text::Hex(key[i]) << "/";

		io::Directory::MakePath(ss.Get());
		ss << text::Hex(ConstBuffer(key, level));
		return ss.Get();
	}

}}
