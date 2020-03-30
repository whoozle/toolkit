#ifndef TOOLKIT_IO_DIRECTORYREADER_H
#define TOOLKIT_IO_DIRECTORYREADER_H

#include <toolkit/core/types.h>
#include <string>
#include <sys/types.h>
#include <dirent.h>

namespace TOOLKIT_NS { namespace io
{
	class DirectoryReader
	{
	private:
		DIR * 			_dir;

	public:
		struct Entry
		{
			std::string 	Name;
			u8				Type;
			ino_t			Inode;
		};
		DirectoryReader(const std::string & path);
		~DirectoryReader();

		bool Read(struct Entry & dir);
	};
}}

#endif
