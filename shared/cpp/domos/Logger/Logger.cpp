
#include "Logger.h"

domos::Logger::LogLevel domos::Logger::level = domos::Logger::LogLevel::DEBUG;
bool domos::Logger::showTimestamp = true;
bool domos::Logger::formatWithColour = true;
domos::Logger::log_callback_t domos::Logger::logCallback = nullptr;

void domos::Logger::setLogLevel(domos::Logger::LogLevel _level) { domos::Logger::level = _level; };

void domos::Logger::setShowTimestamp(bool _showTimestamp) {
  domos::Logger::showTimestamp = _showTimestamp;
};

void domos::Logger::setFormatWithColour(bool _formatWithColour) {
  domos::Logger::formatWithColour = _formatWithColour;
};

void domos::Logger::setLogCallback(domos::Logger::log_callback_t callback) {
  domos::Logger::logCallback = callback;
}

std::string domos::Logger::serialise(domos::Logger::LogLevel level) {
  if (level == domos::Logger::DEBUG)
    return "debug";
  if (level == domos::Logger::INFO)
    return "info";
  if (level == domos::Logger::WARN)
    return "warn";
  if (level == domos::Logger::ERROR)
    return "error";
  return nullptr;
};

std::string domos::Logger::serialise(std::vector<std::string> tags) {
  if (!tags.size())
    return "";

  std::string _tags = "[";
  _tags += domos::Utils::string::join(tags, ',');
  _tags += "]";
  return _tags;
};

const char *domos::Logger::levelColour(domos::Logger::LogLevel level) {
  if (level == domos::Logger::DEBUG)
    return "\033[36m";
  if (level == domos::Logger::INFO)
    return "";
  if (level == domos::Logger::WARN)
    return "\033[33m";
  if (level == domos::Logger::ERROR)
    return "\033[31m";
  return "";
};

std::string domos::Logger::levelLogPrefix(domos::Logger::LogLevel level) {
  std::string levelName = domos::Logger::serialise(level);
  std::transform(levelName.begin(), levelName.end(), levelName.begin(), ::toupper);
  std::string prefix = "[";
  prefix += levelName;
  prefix += "]";
  while (prefix.size() < 8)
    prefix += " ";
  return prefix;
};

void domos::Logger::log(
    domos::Logger::LogLevel _level, std::string message, std::string start, std::string end
) {
  domos::Logger::log(_level, (std::vector<std::string>){}, message, start, end);
}

void domos::Logger::log(
    domos::Logger::LogLevel _level, std::vector<std::string> tags, std::string message,
    std::string start, std::string end
) {
  if (!Serial) {
    Serial.begin(115200);
    while (!Serial) {
      yield();
      delay(10);
    }
  }

  std::string timestamp = !domos::Logger::showTimestamp ? "" : domos::Time::getIsoTimestamp();

  if (_level >= domos::Logger::level) {
    Serial.printf(
        "%s%s%s %s %s %s%s%s", "", start.c_str(), timestamp.c_str(), levelLogPrefix(level).c_str(),
        domos::Logger::serialise(tags).c_str(), message.c_str(), end.c_str(), ""
    );
  }

  if (domos::Logger::logCallback != nullptr)
    domos::Logger::logCallback(_level, tags, message);
};

void domos::Logger::debug(std::string message, std::string start, std::string end) {
  domos::Logger::log(domos::Logger::DEBUG, message, start, end);
};

void domos::Logger::debug(
    std::vector<std::string> tags, std::string message, std::string start, std::string end
) {
  domos::Logger::log(domos::Logger::DEBUG, tags, message, start, end);
};

void domos::Logger::info(std::string message, std::string start, std::string end) {
  domos::Logger::log(domos::Logger::INFO, message, start, end);
};

void domos::Logger::info(
    std::vector<std::string> tags, std::string message, std::string start, std::string end
) {
  domos::Logger::log(domos::Logger::INFO, tags, message, start, end);
};

void domos::Logger::warn(std::string message, std::string start, std::string end) {
  domos::Logger::log(domos::Logger::WARN, message, start, end);
};

void domos::Logger::warn(
    std::vector<std::string> tags, std::string message, std::string start, std::string end
) {
  domos::Logger::log(domos::Logger::WARN, tags, message, start, end);
};

void domos::Logger::error(std::string message, std::string start, std::string end) {
  domos::Logger::log(domos::Logger::ERROR, message, start, end);
};

void domos::Logger::error(
    std::vector<std::string> tags, std::string message, std::string start, std::string end
) {
  domos::Logger::log(domos::Logger::ERROR, tags, message, start, end);
};
