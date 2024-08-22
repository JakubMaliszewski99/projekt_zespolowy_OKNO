#pragma once
#include <fstream>
#include <iostream>
#include <string>

#define GET_OBJ_CLASSNAME(A) typeid(A).name()
#define CLASS_LOGGER(LEVEL, MSG) Logger::getInstance()->log(LEVEL, GET_OBJ_CLASSNAME(*this), MSG)

enum LoggingLevel { eFatal, eError, eWarn, eInfo, eDebug };

class Logger {
public:
  static Logger *getInstance();
  void setLoggingLevel(LoggingLevel level);
  void log(LoggingLevel level, std::string caller, std::string message);

private:
  static Logger *m_instance;
  LoggingLevel m_currentLevel;
};