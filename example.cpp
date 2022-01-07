#include "src/elog.h"
#include "src/threadpool.h"

#include <chrono>
#include <iostream>
#include <ratio>
#include <thread>

template <class A, class B> auto add(A &&a, B &&b)
{
    auto n = a + b;
    return n;
}

int main(int argc, char const *argv[])
{

    ThreadPool thread(5);

    {
        auto ret = thread.toQueue([] { return add(1.1, 2); });
        std::cout << __FILE__ << "\\" << __FUNCTION__ << "(" << __LINE__ << ")"
                  << " "
                  << std::chrono::duration_cast<std::chrono::duration<
                         int, std::ratio_multiply<std::ratio<24>,
                                                  std::chrono::hours::period>>>(
                         std::chrono::system_clock::now().time_since_epoch())
                         .count()
                  << " "
                  << "ret = " << ret.get() << "\n";
    }
    auto start = std::chrono::system_clock::now();

    LOG_I("hello world")
    LOG_SET_lEVEL(LogLevel::LOG_WARM);
    double n = 2.3;
    for (int i = 0; i < 100; i++)
    {
        thread.toQueue(
            [i, n] {
                LOG_I("hello", " world ", "log ", i, "+", i * n, "=",
                      add(i, i * n))
            });
        thread.toQueue(
            [i, n] {
                LOG_W("hello", " world ", "log ", i, "+", i * n, "=",
                      add(i, i * n))
            });
        thread.toQueue(
            [i, n] {
                LOG_E("hello", " world ", "log ", i, "+", i * n, "=",
                      add(i, i * n))
            });
    }

    while (ELog::elgoPtr()->elogOutGetQueueSize() > 0)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
    std::cout << "time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::system_clock::now() - start)
                     .count()
              << " ms\n";
    return 0;
}