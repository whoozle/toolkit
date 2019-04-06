#include <toolkit/io/Event.h>
#include <toolkit/io/SystemException.h>

#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

namespace TOOLKIT_NS { namespace io
{
	Event::Event(): _active(false)
	{
		int fd[2];
		if (pipe(fd) == -1)
			throw SystemException("pipe");
		_rd = fd[0];
		_wd = fd[1];
	}

	Event::~Event()
	{
		close(_rd);
		close(_wd);
	}

	void Event::Signal()
	{
		std::lock_guard<decltype(_mutex)> l(_mutex);
		if (_active)
			return;

		u8 value = 0;
		if (write(_wd, &value, 1) != 1)
			throw SystemException("write");
		_active = true;
	}

	bool Event::Active() const
	{
		std::lock_guard<decltype(_mutex)> l(_mutex);
		return _active;
	}

	void Event::Cancel()
	{
		std::lock_guard<decltype(_mutex)> l(_mutex);
		if (!_active)
			return;
		u8 buf[PIPE_BUF];
		ssize_t r = read(_rd, buf, sizeof(buf));
		if (r == -1)
			throw SystemException("read");
		if (r != 1)
			throw std::logic_error("more than one byte in pipe");
		_active = false;
	}
}}

