#include "elog.h"

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

ELog::ELog(LogLevel level) : tpl(1), log_level(level)
{
    // std::cout << "ELog init"
    //           << "\n";
}

ELog::~ELog()
{
    // std::cout << "ELog delete"
    //           << "\n";
}
