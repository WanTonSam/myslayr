#include "log.h"

namespace sylar {

const char* LogLevel::ToString(LogLevel::Level level) {
    switch(level) {
#define XX(name) \
    case LogLevel::name: \
        return #name; \
        break;

    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(ERROR);
    XX(FATAL);
#undef XX
    default:
        return "UNKNOW";        
    }
    return "UNKNOW";
}

LogLevel::Level LogLevel::FromString(const std::string& str) {
    #define XX(level, v) \
        if (str == #v) { \
            return LogLevel::level; \
        }
        XX(DEBUG, debug);
        XX(INFO, info);
        XX(WARN, warn);
        XX(ERROR, error);
        XX(FATAL, fatal);
    #undef XX
}

Logger::Logger(const std::string & name) 
    :m_name(name) {

}

void Logger::addAppender(LogAppender::ptr appender) {
    m_appender.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender) {
    for (auto it = m_appender.begin();
            it != m_appender.end(); ++it) {
        if (*it == appender) {
            m_appender.erase(it);
            break;
        }
    }
}

void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        for (auto &i : m_appender) {
            i->log(level, event);
        }
    }
}

void Logger::debug(LogEvent::ptr event) {
    log(LogLevel::DEBUG, event);
}

void Logger::info(LogEvent::ptr event) {
    log(LogLevel::INFO, event);
}

void Logger::warn(LogEvent::ptr event) {
    log(LogLevel::WARN, event);
}

void Logger::error(LogEvent::ptr event) {
    log(LogLevel::ERROR, event);
}

void Logger::fatal(LogEvent::ptr event) {
    log(LogLevel::FATAL, event);
}

void FileLogAppender::log(LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        m_filestream << m_formatter->format(event);
    }
}

bool FileLogAppender::reopen() {
    if (m_filestream) {
        m_filestream.close();
    }
    m_filestream.open(m_filename);
    return !!m_filestream;
}

void StdoutLogAppender::log(LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        std::cout << m_formatter->format(event);
    }
}

}; // namespace sylar

