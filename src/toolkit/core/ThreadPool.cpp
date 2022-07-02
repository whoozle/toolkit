#include <toolkit/core/ThreadPool.h>

namespace TOOLKIT_NS
{
	log::Logger	ThreadPool::Thread::_log("thread");
	log::Logger	ThreadPool::_log("thread-pool");

	void ThreadPool::Thread::Run()
	{
		_log.Debug() << "pooled thread started";
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
#ifdef __EXCEPTIONS
				try { task(); } catch(const std::exception &ex)
				{ _log.Warning() << "error in thread pool task: " << ex.what(); }
#endif
				l.lock();
			}
		}
	}

	ThreadPool::ThreadPool(size_t num): _running(true)
	{
		_log.Debug() << "starting thread pool of " << num << " threads";
		while(num--)
			_threads.push_back(std::make_shared<Thread>(this));
	}

	ThreadPool::~ThreadPool()
	{
		{
			std::lock_guard<std::mutex> l(_mutex);
			_running = false;
			_condition.notify_all();
		}
		_threads.clear();
	}

}
