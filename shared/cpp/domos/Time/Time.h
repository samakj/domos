
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
static constexpr std::string TIMESTAMP_NULL_VALUE = "";
static constexpr uint16_t SECOND_IN_MS = 1000;
static constexpr uint16_t MINUTE_IN_MS = 60 * 1000;
static constexpr uint32_t HOUR_IN_MS = 60 * 60 * 1000;
static constexpr uint32_t DAY_IN_MS = 24 * 60 * 60 * 1000;

struct uptime_t {
  uint16_t days = 0;
  uint8_t hours = 0;
  uint8_t minutes = 0;
  uint8_t seconds = 0;
  uint16_t milliseconds = 0;
}

extern uptime_t uptime;
extern unsigned long lastUptimeUpdate;

unsigned long millisDiff(unsigned long start, unsigned long end);
unsigned long millisSince(unsigned long start);
std::string getIsoTimestamp();
std::string formatTime(const char *format);

void loop();

namespace NTP {
typedef std::function<void()> ConnectCallback_t;

extern std::string server;
extern uint16_t maxWait;
extern std::vector<ConnectCallback_t> connectCallbacks;
extern bool _isConnecting;

bool isConnecting();
bool isConnected();

void setServer(std::string server);
void setMaxWait(uint16_t maxWait);

void addConnectCallback(ConnectCallback_t callback);

void connect(bool force = false);
void loop();
} // namespace NTP

}; // namespace domos::Time

#endif