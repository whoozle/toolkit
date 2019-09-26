#ifndef TOOLKIT_IO_NAMEDFILE_H
#define TOOLKIT_IO_NAMEDFILE_H

#include <toolkit/io/File.h>
#include <limits.h>


namespace TOOLKIT_NS { namespace io
{

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
	TOOLKIT_DECLARE_PTR(TemporaryFile);

}}


#endif /* NAMEDFILE_H */

