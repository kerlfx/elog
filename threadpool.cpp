#include "threadpool.h"

size_t ThreadPool::getQueueSize() { return funcs.size(); }

ThreadPool::ThreadPool(size_t threadn) : stop(false)
{

    for (size_t i = 0; i < threadn; i++)
    {
        threads.emplace_back(
            [this]
            {
                while (true)
                {
                    std::function<void()> func;
                    {

                        std::unique_lock<std::mutex> lock(func_queue);
                        this->condition.wait(lock, [this]
                                             { return (!this->funcs.empty() || this->stop); });
                        if (this->stop && this->funcs.empty())
                        {
                            return;
                        }
                        func = std::move(this->funcs.front());
                        this->funcs.pop();
                    }
                    func();
                }
            });
    }
}

ThreadPool::~ThreadPool()
{
    stop = true;
    condition.notify_all();

    for (std::thread &threadp : threads)
        threadp.join();
}
