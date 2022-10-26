#ifndef TOOLKIT_LOG_FILELOGGINGSINK_H
#define TOOLKIT_LOG_FILELOGGINGSINK_H

#include <toolkit/log/ILoggingSink.h>
#include <toolkit/io/File.h>

namespace TOOLKIT_NS { namespace log
{

	class FileLoggingSink final : public ILoggingSink
	{
		io::File _file;

	private:
		const char * GetColor(LogLevel level);

	public:
		FileLoggingSink(const std::string &filename, io::FileOpenMode mode = io::FileOpenMode::Overwrite);
		void Log(LogLevel level, const std::string& logger, const timespec& ts, const std::string& value) override;
	};

}}


#endif
