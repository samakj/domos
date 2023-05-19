
#ifndef _domos_Time_h
#define _domos_Time_h

#include <Arduino.h>
#include <functional>
#include <string>
#include <time.h>

#include <Logger/Logger.h>

namespace domos::Wifi {
bool isConnected();
};

namespace domos::Time {
static constexpr uint8_t MILLISECOND_IN_MS = 1;
static constexpr uint16_t SECOND_IN_MS = 1000 * MILLISECOND_IN_MS;
static constexpr uint16_t MINUTE_IN_MS = 60 * SECOND_IN_MS;
static constexpr uint32_t HOUR_IN_MS = 60 * MINUTE_IN_MS;
static constexpr uint32_t DAY_IN_MS = 24 * HOUR_IN_MS;
static constexpr uint32_t WEEK_IN_MS = 7 * DAY_IN_MS;
static constexpr uint32_t FORTNIGHT_IN_MS = 14 * DAY_IN_MS;
static constexpr uint32_t YEAR_IN_MS = 365 * DAY_IN_MS;
static constexpr uint32_t MONTH_IN_MS = YEAR_IN_MS / 12;

struct uptime_t {
  uint16_t days = 0;
  uint8_t hours = 0;
  uint8_t minutes = 0;
  uint8_t seconds = 0;
  uint16_t milliseconds = 0;
};

extern uptime_t uptime;
extern unsigned long lastUptimeUpdate;

unsigned long millisDiff(unsigned long start, unsigned long end);
unsigned long millisSince(unsigned long start);
std::string getIsoTimestamp();
std::string formatTime(const char *format);

void loop();

namespace NTP {
typedef std::function<void()> connect_callback_t;

extern std::string server;
extern uint16_t maxWait;
extern connect_callback_t connectCallback;
extern bool _isConnecting;
extern std::vector<std::string> _logTags;

bool isConnecting();
bool isConnected();

void setServer(std::string server);
void setMaxWait(uint16_t maxWait);
void setConnectCallback(connect_callback_t callback);

void connect(bool force = false);
void loop();
} // namespace NTP

}; // namespace domos::Time

#endif