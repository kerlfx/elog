#include "elog.h"
#include "threadpool.h"

#include <chrono>
#include <iostream>
#include <thread>

auto add(int a, int b)
{
    auto n = a + b;
    // std::cout << a << " + " << b << " = " << n << "\n";
    LOG_I(a, " + ", b, " = ", n)
    return n;
}
auto addd(double a, double b)
{
    auto n = a + b;
    // std::cout << a << " + " << b << " = " << n << "\n";
    LOG_I(a, " + ", b, " = ", n)
    return n;
}

int main(int argc, char const *argv[])
{

    ThreadPool thread(5);

    {
        auto ret = thread.toQueue(addd, 1.1, 2.5);
        std::cout << __FILE__ << "\\" << __FUNCTION__ << "(" << __LINE__ << ")"
                  << " "
                  << std::chrono::duration_cast<std::chrono::hours>(
                         std::chrono::system_clock::now().time_since_epoch())
                         .count()
                  << " "
                  << "ret = " << ret.get() << "\n";
    }
    auto start = std::chrono::system_clock::now();

    LOG_I("hello world")

    LOG_SET_lEVEL(LogLevel::LOG_WARM);
    for (int i = 0; i < 100; i++)
    {
        thread.toQueue([i]
                       { LOG_I("hello", " world ", "log ", i, "+", i * 2, "=", add(i, i * 2)) });
        thread.toQueue([i]
                       { LOG_W("hello", " world ", "log ", i, "+", i * 2, "=", add(i, i * 2)) });
        thread.toQueue([i]
                       { LOG_E("hello", " world ", "log ", i, "+", i * 2, "=", add(i, i * 2)) });
        thread.toQueue([i]
                       { LOG_D("hello", " world ", "log ", i, "+", i * 2, "=", add(i, i * 2)) });
    }

    LOG_I("queue size : ", ELog::elgoPtr()->elogOutGetQueueSize() + 1)

    while (ELog::elgoPtr()->elogOutGetQueueSize() > 0)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
    return 0;
}