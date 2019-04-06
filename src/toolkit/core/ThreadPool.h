#ifndef PURE_CORE_THREAD_POOL
#define PURE_CORE_THREAD_POOL

#include <toolkit/core/Noncopyable.h>
#include <thread>
#include <condition_variable>
#include <future>
#include <queue>
#include <mutex>
#include <stdio.h>

namespace TOOLKIT_NS
{

	class ThreadPool : Noncopyable
	{
		using Task = std::function<void ()>;

		struct Thread : Noncopyable
		{
			ThreadPool *	_pool;
			std::thread		_thread;

			Thread(ThreadPool * pool): _pool(pool), _thread(&Thread::Run, this) { }
			void Run()
			{
				printf("thread pool thread started\n");
				std::unique_lock<std::mutex> l(_pool->_mutex);
				while(_pool->_running)
				{
					_pool->_condition.wait(l);
					while (!_pool->_tasks.empty())
					{
						Task task(std::move(_pool->_tasks.front()));
						_pool->_tasks.pop();
						if (!_pool->_tasks.empty())
							_pool->_condition.notify_all();
						l.unlock();
						try { task(); } catch(const std::exception &ex)
						{ fprintf(stderr, "error in thread pool task: %s\n", ex.what()); }
						l.lock();
					}
				}
			}

			~Thread()
			{
				_thread.join();
			}
		};
		DECLARE_PTR(Thread);

		std::mutex				_mutex;
		bool					_running;
		std::condition_variable _condition;
		std::queue<Task>		_tasks;
		std::vector<ThreadPtr>	_threads;

	public:
		ThreadPool(): ThreadPool(std::thread::hardware_concurrency()) { }
		ThreadPool(unsigned num): _running(true)
		{
			printf("starting thread pool of %u threads\n", num);
			while(num--)
				_threads.push_back(std::make_shared<Thread>(this));
		}
		~ThreadPool()
		{
			{
				std::lock_guard<std::mutex> l(_mutex);
				_running = false;
				_condition.notify_all();
			}
			_threads.clear();
		}

		template<typename T>
		void AddTask(T callable)
		{
			std::lock_guard<std::mutex> l(_mutex);
			_tasks.emplace(callable);
			_condition.notify_one();
		}
	};

}

#endif
