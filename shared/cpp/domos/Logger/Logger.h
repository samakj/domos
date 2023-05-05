#ifndef _domos_Logger_h
#define _domos_Logger_h

#include <Arduino.h>

#include <algorithm>
#include <string>
#include <vector>

namespace domos::Time {
std::string getIsoTimestamp();
}

namespace domos::Logger {
enum LogLevel { DEBUG, INFO, WARN, ERROR };
typedef std::function<void(domos::Logger::LogLevel level, std::string message)> LogCallback_t;

extern LogLevel level;
extern bool showTimestamp;
extern bool formatWithColour;
extern std::vector<LogCallback_t> logCallbacks;

void setLogLevel(LogLevel level);
void setShowTimestamp(bool showTimestamp);
void setFormatWithColour(bool formatWithColour);
void addLogCallback(LogCallback_t callback);

std::string levelName(LogLevel level);
const char *levelColour(LogLevel level);
std::string levelLogPrefix(LogLevel level);

void log(LogLevel level, std::string message, std::string start = "", std::string end = "\n");
template <typename... Args> void logf(LogLevel level, std::string format, Args... args);

void debug(std::string message, std::string start = "", std::string end = "\n");
template <typename... Args> void debugf(std::string format, Args... args);

void info(std::string message, std::string start = "", std::string end = "\n");
template <typename... Args> void infof(std::string format, Args... args);

void warn(std::string message, std::string start = "", std::string end = "\n");
template <typename... Args> void warnf(std::string format, Args... args);

void error(std::string message, std::string start = "", std::string end = "\n");
template <typename... Args> void errorf(std::string format, Args... args);
}; // namespace domos::Logger

#include "Logger.tpp"

#endif