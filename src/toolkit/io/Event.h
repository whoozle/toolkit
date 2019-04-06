#ifndef TOOLKIT_EVENT_H
#define TOOLKIT_EVENT_H

#include <toolkit/io/IPollable.h>
#include <toolkit/core/Noncopyable.h>
#include <toolkit/core/types.h>
#include <memory>
#include <mutex>

namespace TOOLKIT_NS { namespace io
{

	class Event : public IPollable, public Noncopyable
	{
		mutable std::mutex	_mutex;
		bool				_active;
		int					_rd, _wd;

	public:
		Event();
		~Event();

		int GetFileDescriptor() const override
		{ return _rd; }

		void Signal();
		void Cancel();
		bool Active() const;
	};
	DECLARE_PTR(Event);

}}


#endif
