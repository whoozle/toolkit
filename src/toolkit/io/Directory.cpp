#include <toolkit/io/Directory.h>
#include <toolkit/io/File.h>
#include <toolkit/io/SystemException.h>
#include <toolkit/text/StringOutputStream.h>
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

	std::string Directory::GetXdgDirectory(const std::string &env, const std::string &homePath)
	{
		const char * path = getenv(env.c_str());
		return path? path: GetHomeDirectory() + homePath;
	}

	std::string Directory::GetConfigHome()
	{ return GetXdgDirectory("XDG_CONFIG_HOME", "/.config"); }

	std::string Directory::GetCacheHome()
	{ return GetXdgDirectory("XDG_CACHE_HOME", "/.cache"); }

	std::string Directory::GetDataHome()
	{ return GetXdgDirectory("XDG_DATA_HOME", "/.local/share"); }

	std::string Directory::GetStateHome()
	{ return GetXdgDirectory("XDG_STATE_HOME", "/.local/state"); }

	std::string Directory::GetRuntimeDirectory()
	{
		const char * path = getenv("XDG_RUNTIME_DIR");
		if (path)
			return path;

		text::StringOutputStream ss;
		ss << "/run/user/" << getuid();
		return ss.Get();
	}

	void Directory::MakePath(const std::string & path, mode_t mode)
	{
		size_t pos = 0;
		while(pos < path.size())
		{
			auto next = path.find('/', pos);
			if (next == path.npos)
				next = path.size();

			if (next != pos)
			{
				auto sub = path.substr(0, next);
				mkdir(sub.c_str(), mode);
			}
			pos = next + 1;
		}
	}


}}
