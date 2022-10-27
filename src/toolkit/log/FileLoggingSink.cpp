#include <toolkit/log/FileLoggingSink.h>
#include <toolkit/core/ByteArray.h>
#include <stdio.h>

namespace TOOLKIT_NS { namespace log {

FileLoggingSink::FileLoggingSink(const std::string &filename, io::FileOpenMode mode): _file(filename, mode)
{ }

void FileLoggingSink::Log(LogLevel level, const std::string& logger, const timespec& ts, const std::string& value)
{
	static constexpr ssize_t BufSize = 256;
	char line[BufSize];
	auto r = snprintf(line, sizeof(line), "%ld.%03ld: %s[%s]: ", static_cast<long>(ts.tv_sec), static_cast<long>(ts.tv_nsec / 1000000), logger.c_str(), to_string(level));
	ByteArray output(r + value.size() + 1);
	auto next = std::copy(line, line + r, output.begin());
	next = std::copy(value.begin(), value.end(), next);
	*next++ = '\n';
	_file.Write(output);
}

}}
