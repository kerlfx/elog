#ifndef ELOG_H_
#define ELOG_H_

// #pragma once

#include <iostream>
#include <mutex>

#include "threadpool.h"

enum class LogLevel : std::int8_t
{
    LOG_OFF = 0,
    LOG_ERROR,
    LOG_WARM,
    LOG_INFO,
    LOG_DEBUG,
};

class ELog
{
private:
    static ELog *elog_ptr;
    static std::mutex init_mutex;

    LogLevel log_level;

    ThreadPool tpl;

    ELog(LogLevel level = LogLevel::LOG_INFO);
    ~ELog();

    template <class OS> void osp(OS value);
    template <class OS, class... OSArgs> void osp(OS value, OSArgs... fargs);

public:
    size_t elogOutGetQueueSize();

    LogLevel elogOutGetLevel();
    void elogOutSetLevel(LogLevel level);

    template <class... OSArgs> void elogOut(OSArgs... fargs);

    static ELog *elgoPtr()
    {

        if (elog_ptr == nullptr)
        {
            std::unique_lock<std::mutex> lck(init_mutex);
            if (elog_ptr == nullptr)
            {
                elog_ptr = new ELog;
            }
        }
        return elog_ptr;
    }
};

template <class... OSArgs> void ELog::elogOut(OSArgs... fargs)
{

    tpl.toQueue(
        [fargs...]
        {
            ELog::elgoPtr()->osp(fargs...);
            return;
        });
}

template <class OS> void ELog::osp(OS value)
{
    std::cout << value << "\n";
    return;
}

template <class OS, class... OSArgs> void ELog::osp(OS value, OSArgs... args)
{

    std::cout << value;
    osp(args...);
    return;
};

#define LOG_SET_lEVEL(x) ELog::elgoPtr()->elogOutSetLevel(x)

#define LOG_I(...)                                                                                 \
    if (ELog::elgoPtr()->elogOutGetLevel() >= LogLevel::LOG_INFO)                                  \
    {                                                                                              \
        ELog::elgoPtr()->elogOut("INF| ", __FUNCTION__, "(", __LINE__, "):", __VA_ARGS__);         \
    }
#define LOG_W(...)                                                                                 \
    if (ELog::elgoPtr()->elogOutGetLevel() >= LogLevel::LOG_WARM)                                  \
    {                                                                                              \
        ELog::elgoPtr()->elogOut("WAR| ", __FUNCTION__, "(", __LINE__, "):", __VA_ARGS__);         \
    }
#define LOG_E(...)                                                                                 \
    if (ELog::elgoPtr()->elogOutGetLevel() >= LogLevel::LOG_ERROR)                                 \
    {                                                                                              \
        ELog::elgoPtr()->elogOut("ERR| ", __FUNCTION__, "(", __LINE__, "):", __VA_ARGS__);         \
    }
#define LOG_D(...)                                                                                 \
    if (ELog::elgoPtr()->elogOutGetLevel() >= LogLevel::LOG_DEBUG)                                 \
    {                                                                                              \
        ELog::elgoPtr()->elogOut("DEB| ", __FUNCTION__, "(", __LINE__, "):", __VA_ARGS__);         \
    }

#endif // ELOG_H_