#ifndef LOG_H
#define LOG_H

#include <Arduino.h>
#include <stdarg.h> 

enum LogLevel{

  LOG_NONE = 0,
  LOG_ERROR = 1,
  LOG_WARN = 2,
  LOG_INFO = 3,
  LOG_DEBUG = 4

};

extern LogLevel CURRENT_LOG_LEVEL;

void logInit(LogLevel level);

void log(LogLevel level, const char* fmt, ...);

#endif