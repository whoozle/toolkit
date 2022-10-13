#ifndef TOOLKIT_IO_PIPE_H
#define TOOLKIT_IO_PIPE_H

#include <toolkit/io/File.h>

namespace TOOLKIT_NS { namespace io
{

	class Pipe
	{
	private:
		struct Descriptors
		{
			int ReadFd;
			int WriteFd;
		};
		Descriptors _desc;

		static Descriptors Create(int flags)
		{
			Descriptors desc;
			io::File::CreatePipe(desc.ReadFd, desc.WriteFd, flags);
			return desc;
		}

	public:
		io::File ReadPipe;
		io::File WritePipe;

		Pipe(int flags = 0): _desc(Create(flags)), ReadPipe(_desc.ReadFd), WritePipe(_desc.WriteFd)
		{ }
	};

}}

#endif
