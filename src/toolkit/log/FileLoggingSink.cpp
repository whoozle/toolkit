#include <toolkit/log/FileLoggingSink.h>
#include <stdio.h>

namespace TOOLKIT_NS { namespace log {

FileLoggingSink::FileLoggingSink(const std::string &filename, io::FileOpenMode mode): _file(filename, mode)
{ }

void FileLoggingSink::Log(LogLevel level, const std::string& logger, const timespec& ts, const std::string& value)
{
	static constexpr ssize_t BufSize = 4096;
	char line[BufSize];
	auto r = snprintf(line, sizeof(line) - 1, "%ld.%03ld: %s[%s]: %s", static_cast<long>(ts.tv_sec), static_cast<long>(ts.tv_nsec / 1000000), logger.c_str(), to_string(level), value.c_str());
	ASSERT(r > 0, io::SystemException, "snprintf");
	ASSERT(r < BufSize, Exception, "buffer overflow");
	line[r++] = '\n';
	_file.Write(ConstBuffer(reinterpret_cast<const u8 *>(line), r));
}

}}
