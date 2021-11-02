#include "elog.h"
#include "threadpool.h"

#include <chrono>
#include <ctime>
#include <iostream>
#include <ratio>
#include <thread>

template <class A, class B> auto add(A &&a, B &&b)
{
    auto n = a + b;
    LOG_I(a, " + ", b, " = ", n)
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
    for (int i = 0; i < 10; i++)
    {
        thread.toQueue(
            [i] {
                LOG_I("hello", " world ", "log ", i, "+", i * 2, "=",
                      add(i, i * 2))
            });
        thread.toQueue(
            [i] {
                LOG_W("hello", " world ", "log ", i, "+", i * 2, "=",
                      add(i, i * 2))
            });
        thread.toQueue(
            [i] {
                LOG_E("hello", " world ", "log ", i, "+", i * 2, "=",
                      add(i, i * 2))
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