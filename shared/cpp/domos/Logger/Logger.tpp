template <typename... Args>
void domos::Logger::logf(domos::Logger::LogLevel _level, std::string format, Args... args) {
  size_t size = snprintf(nullptr, 0, format.c_str(), args...);
  char buffer[size + 8];
  sprintf(buffer, format.c_str(), args...);
  domos::Logger::log(_level, buffer, "", "");
};

template <typename... Args> void domos::Logger::debugf(std::string format, Args... args) {
  domos::Logger::logf(domos::Logger::DEBUG, format, args...);
};

template <typename... Args> void domos::Logger::infof(std::string format, Args... args) {
  domos::Logger::logf(domos::Logger::INFO, format, args...);
};

template <typename... Args> void domos::Logger::warnf(std::string format, Args... args) {
  domos::Logger::logf(domos::Logger::WARN, format, args...);
};

template <typename... Args> void domos::Logger::errorf(std::string format, Args... args) {
  domos::Logger::logf(domos::Logger::ERROR, format, args...);
};