#ifndef TOOLKIT_IO_NAMEDFILE_H
#define TOOLKIT_IO_NAMEDFILE_H

#include <toolkit/io/File.h>
#include <limits.h>


TOOLKIT_SUBNS_BEGIN(io)

	class TemporaryFile : public File
	{
		char _storagePath[PATH_MAX]; //fixme: optimize me

	public:
		TemporaryFile(const std::string &filenameTemplate);
		~TemporaryFile();

		void Remove();
	private:
		int CreateTemporaryFileDescriptor(const std::string &filenameTemplate, char *storagePath);
	};
	DECLARE_PTR(TemporaryFile);

TOOLKIT_SUBNS_END

#endif /* NAMEDFILE_H */

