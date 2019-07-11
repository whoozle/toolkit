#include <toolkit/core/TaskQueue.h>

namespace TOOLKIT_NS
{
	log::Logger TaskQueue::_log("task-queue");

	void TaskQueue::ProcessTasks()
	{
		std::unique_lock<decltype(_lock)> l(_lock);
		while(!_queue.empty())
		{
			Task task(std::move(_queue.front())); //allow move ctor to pass exception
			_queue.pop();
			l.unlock();

			try
			{
				task();
			}
			catch(const std::exception & ex)
			{
				_log.Warning() << "task queue failure: " << ex.what();
			}

			l.lock();
		}
	}
}
