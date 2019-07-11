#ifndef TOOLKIT_TASK_QUEUE
#define TOOLKIT_TASK_QUEUE

#include <toolkit/core/Task.h>
#include <toolkit/log/Logger.h>
#include <mutex>
#include <queue>

namespace TOOLKIT_NS
{
	class TaskQueue
	{
		static log::Logger		_log;
		std::mutex 				_lock;
		std::queue<Task>		_queue;

	public:
		TaskQueue() { }

		template<typename Callable>
		void AddTask(Callable && task)
		{
			std::lock_guard<decltype(_lock)> l(_lock);
			_queue.emplace(std::forward<Callable>(task));
		}

		void ProcessTasks();

		void operator()()
		{ ProcessTasks(); }
	};
}

#endif
