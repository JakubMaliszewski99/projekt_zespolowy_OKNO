#include "core/logger/Logger.h"

Logger *Logger::m_instance = nullptr;

std::string getTagFromLevel(LoggingLevel level) {
  switch (level) {
  case LoggingLevel::eError:
    return "[ERROR] ";
  case LoggingLevel::eWarn:
    return "[WARNING] ";
  case LoggingLevel::eInfo:
    return "[INFO] ";
  case LoggingLevel::eDebug:
    return "[DEBUG] ";
  }

  return "[UNKNOWN]";
}

void Logger::setLoggingLevel(LoggingLevel level) { m_currentLevel = level; }

Logger *Logger::getInstance() {
  if (!m_instance) {
    m_instance = new Logger();
  }

  return m_instance;
}

void Logger::log(LoggingLevel level, std::string caller, std::string message) {
  if (m_currentLevel < level) {
    return;
  }

  std::string log = "";
  log += getTagFromLevel(level);
  log += caller;
  log += ": ";
  log += message;

  std::cout << log << std::endl;
}
