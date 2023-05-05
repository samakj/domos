#include "Time.h"

domos::Time::uptime_t domos::Time::uptime = {};
unsigned long domos::Time::lastUptimeUpdate = 0;

unsigned long domos::Time::millisDiff(unsigned long start, unsigned long end) {
  if (end >= start)
    return end - start;
  unsigned long startOffset = std::numeric_limits<unsigned long>::max() - start;
  return end + startOffset;
};

unsigned long domos::Time::millisSince(unsigned long start) {
  return domos::Time::millisDiff(start, millis());
};

std::string domos::Time::getIsoTimestamp() {
  if (!domos::Time::NTP::isConnected())
    return domos::Time::TIMESTAMP_NULL_VALUE;

  char buffer[23];
  time_t tm = time(nullptr);
  strftime(buffer, 23, "%FT%TZ", gmtime(&tm));
  return std::string(buffer);
};

std::string domos::Time::formatTime(const char *format) {
  char buffer[64];
  time_t tm = ::time(nullptr);
  strftime(buffer, sizeof(buffer), format, gmtime(&tm));
  return std::string(buffer);
};

void domos::Time::loop() {
  unsigned long now = millis();
  unsigned long diff = domos::Time::millisDiff(domos::Time::lastUptimeUpdate, now);

  if (diff > 0) {
    uint16_t days = diff / domos::Time::DAY_IN_MS;
    diff = diff % domos::Time::DAY_IN_MS;
    uint8_t hours = diff / domos::Time::HOUR_IN_MS;
    diff = diff % domos::Time::HOUR_IN_MS;
    uint8_t minutes = diff / domos::Time::MINUTE_IN_MS;
    diff = diff % domos::Time::MINUTE_IN_MS;
    uint8_t seconds = diff / domos::Time::SECOND_IN_MS;
    uint16_t milliseconds = diff % domos::Time::SECOND_IN_MS;

    domos::Time::uptime.milliseconds += milliseconds;
    if (domos::Time::uptime.milliseconds >= 1000) {
      seconds += domos::Time::uptime.milliseconds / 1000;
      domos::Time::uptime.milliseconds = domos::Time::uptime.milliseconds % 1000;
    }
    domos::Time::uptime.seconds += seconds;
    if (domos::Time::uptime.seconds >= 60) {
      minutes += domos::Time::uptime.seconds / 60;
      domos::Time::uptime.seconds = domos::Time::uptime.seconds % 60;
    }
    domos::Time::uptime.minutes += minutes;
    if (domos::Time::uptime.minutes >= 60) {
      hours += domos::Time::uptime.minutes / 60;
      domos::Time::uptime.minutes = domos::Time::uptime.minutes % 60;
    }
    domos::Time::uptime.hours += hours;
    if (domos::Time::uptime.hours >= 24) {
      days += domos::Time::uptime.hours / 24;
      domos::Time::uptime.hours = domos::Time::uptime.hours % 24;
    }
    domos::Time::uptime.hours += days;

    domos::Time::lastUptimeUpdate = now;
  }

  domos::Time::NTPL::loop();
}

// NTP

std::string domos::Time::NTP::server = "uk.pool.ntp.org";
uint16_t domos::Time::NTP::maxWait = 20000;
std::vector<domos::Time::NTP::ConnectCallback> domos::Time::NTP::connectCallbacks = {};
bool domos::Time::NTP::_isConnecting = false;

bool domos::Time::NTP::isConnecting() { return domos::Time::NTP::_isConnecting; };

bool domos::Time::NTP::isConnected() { return time(nullptr) > 1616000000; };

void domos::Time::NTP::setServer(std::string server) { domos::Time::NTP::server = server; };

void domos::Time::NTP::setMaxWait(uint16_t maxWait) { domos::Time::NTP::maxWait = maxWait; };

void domos::Time::NTP::addConnectCallback(domos::Time::NTP::ConnectCallback callback) {
  domos::Time::NTP::connectCallbacks.push_back(callback);
};

void domos::Time::NTP::connect(bool force) {
  if (!domos::Wifi::isConnected())
    domos::Logger::warn("No internet connection, skiping NTP.");
  else if (force || (!domos::Time::NTP::isConnecting() && !domos::Time::NTP::isConnected())) {
    domos::Logger::infof("Connecting NTP to %s\n", domos::Time::NTP::server.c_str());
    domos::Time::NTP::_isConnecting = true;
    configTime(0, 0, domos::Time::NTP::server.c_str());
  }
};

void domos::Time::NTP::loop() {
  if (!domos::Time::NTP::isConnected() && !domos::Time::NTP::isConnecting() &&
      domos::Wifi::isConnected())
    connect();

  if (domos::Time::NTP::isConnected() && domos::Time::NTP::isConnecting()) {
    domos::Logger::infof(
        "Synced with '%s'. Time: %s.\n", domos::Time::NTP::server.c_str(),
        domos::Time::getIsoTimestamp().c_str()
    );

    for (domos::Time::NTP::ConnectCallback callback : domos::Time::NTP::connectCallbacks)
      callback();

    domos::Time::NTP::_isConnecting = false;
  }
};