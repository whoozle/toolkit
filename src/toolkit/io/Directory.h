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
	};

}}

#endif
