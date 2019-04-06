#ifndef TOOLKIT_IO_PROCESSSTATISTICS_H
#define TOOLKIT_IO_PROCESSSTATISTICS_H

#include <toolkit/core/core.h>
#include <string>

#include <sys/time.h>
#include <sys/resource.h>

namespace TOOLKIT_NS { namespace io
{
	class Process
	{
	public:
		struct Statistics
		{
			rusage ResourceUsage;

			std::string ToString() const;
		};

		static Statistics GetProcessStatistics();
	};
}}


#endif

