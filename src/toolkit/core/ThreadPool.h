#ifndef TOOLKIT_THREAD_POOL
#define TOOLKIT_THREAD_POOL

#include <toolkit/core/Noncopyable.h>
#include <toolkit/core/Task.h>
#include <toolkit/core/types.h>
#include <toolkit/log/Logger.h>
#include <condition_variable>
#include <future>
#include <memory>
#include <mutex>
#include <thread>
#include <queue>

namespace TOOLKIT_NS
{
	class ThreadPool : Noncopyable
	{
		class Thread : Noncopyable
		{
			static log::Logger	_log;
			ThreadPool *		_pool;
			std::thread			_thread;

		public:
			Thread(ThreadPool * pool):
				_pool(pool), _thread(&Thread::Run, this)
			{ }

			~Thread()
			{ _thread.join(); }

			void Run();
		};
		DECLARE_PTR(Thread);

		static log::Logger		_log;
		std::mutex				_mutex;
		bool					_running;
		std::condition_variable _condition;
		std::queue<Task>		_tasks;
		std::vector<ThreadPtr>	_threads;

	public:
		ThreadPool(size_t num);
		ThreadPool(): ThreadPool(std::thread::hardware_concurrency())
		{ }
		~ThreadPool();

		template<typename T>
		void AddTask(T && callable)
		{
			std::lock_guard<std::mutex> l(_mutex);
			_tasks.emplace(std::forward<T>(callable));
			_condition.notify_one();
		}
	};

}

#endif
