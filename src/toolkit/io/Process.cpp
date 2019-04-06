#include <toolkit/io/Process.h>
#include <toolkit/io/SystemException.h>

namespace TOOLKIT_NS { namespace io
{
	namespace
	{
		long MS(const timeval & tv)
		{
			return tv.tv_sec * 1000l + tv.tv_usec / 1000;
		}
	}
	std::string Process::Statistics::ToString() const
	{
		char buf[1024];

		const rusage &ru = ResourceUsage;
		size_t r = snprintf(buf, sizeof(buf), "CPU user: %ld, system: %ld; "
			"Memory maxrss: %ld, ixrss: %ld, idrss: %ld, isrss: %ld; "
			"PF hard: %ld, soft: %ld, swaps: %ld; "
			"Blocks in: %ld, out: %ld; "
			"IPC in: %ld, out: %ld; "
			"Signals: %ld; "
			"Context switches: %ld %ld;"
			,
			MS(ru.ru_utime), MS(ru.ru_stime),
			ru.ru_maxrss, ru.ru_ixrss, ru.ru_idrss, ru.ru_isrss,
			ru.ru_majflt, ru.ru_minflt, ru.ru_nswap,
			ru.ru_inblock, ru.ru_oublock,
			ru.ru_msgrcv, ru.ru_msgsnd,
			ru.ru_nsignals,
			ru.ru_nvcsw, ru.ru_nivcsw
		);
		return std::string(buf, r);
	}

	Process::Statistics Process::GetProcessStatistics()
	{
		rusage usage = { };
		SYSTEM_CALL(getrusage(RUSAGE_SELF, &usage));
		return { usage };
	}
}}

