#include <toolkit/io/Directory.h>
#include <toolkit/io/File.h>
#include <toolkit/io/SystemException.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <unistd.h>

namespace TOOLKIT_NS { namespace io
{

	std::string Directory::GetHomeDirectory()
	{
		const char * home = getenv("HOME");
		return home? home: "/";
	}

	std::string Directory::GetTemporaryDirectory()
	{
		const char * temp = getenv("TMPDIR");
		return temp? temp: "/tmp";
	}

	std::string Directory::GetCurrentDirectory()
	{
		char cwd[PATH_MAX];
		ASSERT(getcwd(cwd, sizeof(cwd)), io::SystemException, "getcwd");
		return cwd;
	}

	void Directory::SetCurrentDirectory(const std::string &dir)
	{ SYSTEM_CALL(chdir(dir.c_str())); }

}}
