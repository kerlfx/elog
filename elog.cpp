#include "elog.h"

#include <chrono>
#include <ctime>
#include <iostream>

ELog *ELog::elog_ptr = nullptr;
std::mutex ELog::init_mutex;

size_t ELog::elogOutGetQueueSize() { return tpl.getQueueSize(); }

LogLevel ELog::elogOutGetLevel() { return log_level; }

void ELog::elogOutSetLevel(LogLevel level)
{
    log_level = level;
    return;
}

std::shared_ptr<std::string> ELog::elogGetTimeStr()
{
    const auto tn =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    tm tmdata;
    ::localtime_s(&tmdata, &tn);
    char str[50];
    std::strftime(str, sizeof(str), "%F %T", &tmdata);

    return std::make_shared<std::string>(str);
}

ELog::ELog(LogLevel level) : tpl(1), log_level(level)
{
    log_level_head[static_cast<int>(LogLevel::LOG_ERROR)] = "ERR";
    log_level_head[static_cast<int>(LogLevel::LOG_WARM)] = "WAR";
    log_level_head[static_cast<int>(LogLevel::LOG_INFO)] = "INF";
    log_level_head[static_cast<int>(LogLevel::LOG_DEBUG)] = "DEB";
}

ELog::~ELog() {}
