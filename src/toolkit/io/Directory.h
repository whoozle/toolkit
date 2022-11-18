#ifndef TOOLKIT_IO_DIRECTORY_H
#define TOOLKIT_IO_DIRECTORY_H

#include <string>

namespace TOOLKIT_NS { namespace io
{

	class Directory
	{
	public:
		static std::string GetHomeDirectory();
		static std::string GetTemporaryDirectory();

		static std::string GetCurrentDirectory();
		static void SetCurrentDirectory(const std::string &dir);

		// XDG
		static std::string GetConfigHome();
		static std::string GetCacheHome();
		static std::string GetDataHome();
		static std::string GetStateHome();
		static std::string GetRuntimeDirectory();

	private:
		static std::string GetXdgDirectory(const std::string &env, const std::string &homePath);
	};

}}

#endif
