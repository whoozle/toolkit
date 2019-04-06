#include <toolkit/io/TemporaryFile.h>
#include <toolkit/io/SystemException.h>
#include <unistd.h>
#include <string.h>

namespace TOOLKIT_NS { namespace io
{

	int TemporaryFile::CreateTemporaryFileDescriptor(const std::string &filenameTemplate, char *storagePath)
	{
		snprintf(_storagePath, PATH_MAX, "%s", filenameTemplate.c_str());
		int fd = mkstemp(_storagePath);
		if (fd < 0)
			throw io::SystemException("mkstemp failed");	
		return fd;
	}

	TemporaryFile::TemporaryFile(const std::string& filenameTemplate):
		File(CreateTemporaryFileDescriptor(filenameTemplate, _storagePath))
	{ }

	TemporaryFile::~TemporaryFile()
	{ Remove(); }

	void TemporaryFile::Remove()
	{
		unlink(_storagePath);
	}

}}

