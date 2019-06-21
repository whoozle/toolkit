#include <toolkit/io/DirectoryReader.h>
#include <toolkit/io/SystemException.h>

namespace TOOLKIT_NS { namespace io
{

	DirectoryReader::DirectoryReader(const std::string & path): _dir(opendir(path.c_str())), _entry { }
	{
		if (!_dir)
			throw SystemException("opendir");
	}

	DirectoryReader::~DirectoryReader()
	{
		closedir(_dir);
	}

	bool DirectoryReader::Read(struct Entry & dir)
	{
		struct dirent * result = readdir(_dir);
		if (!result)
			return false;

		dir.Inode = result->d_ino;
		dir.Type = result->d_type;
		dir.Name = result->d_name;
		return true;
	}

}}
