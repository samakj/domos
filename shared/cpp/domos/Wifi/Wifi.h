#ifndef _domos_Wifi_h
#define _domos_Wifi_h

#include <Arduino.h>
#include <string>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <Logger/Logger.h>
#include <Time/Time.h>
#include <Utils/Utils.h>

namespace domos::Wifi {
struct credentials_t {
  std::string ssid;
  std::string password;
};

enum WifiStrength { VERY_STRONG, STRONG, GOOD, OKAY, WEAK, VERY_WEAK, NOT_CONNECTED };

typedef std::function<void(std::string ssid)> connect_callback_t;
typedef std::function<void()> disconnect_callback_t;
typedef std::function<void(std::string ssid)> ssid_change_callback_t;
typedef std::function<void(WifiStrength strength)> strength_change_callback_t;

extern connect_callback_t connectCallback;
extern disconnect_callback_t disconnectCallback;
extern ssid_change_callback_t ssidChangeCallback;
extern strength_change_callback_t strengthChangeCallback;

extern std::vector<credentials_t *> networks;
extern std::string targetHostname;
extern std::string targetIp;
extern uint16_t maxWait;
extern uint16_t strengthUpdatePeriod;

extern bool _isConnecting;
extern WifiStrength _strength;
extern uint32_t _connectionAttemptStart;
extern uint32_t _lastConnectionMessage;
extern uint32_t _lastStrengthUpdate;

bool isConnecting();
bool isConnected();
std::string getMACAddress();
std::string getIPAddress();
std::string getHostname();
std::string getSSID();
WifiStrength getStrength();

credentials_t *getStrongestNetwork(std::vector<credentials_t *> networks);

void setHostname(std::string hostname);
void setIPAddress(std::string ip);
void setMaxWait(uint16_t maxWait);
void setConnectCallback(connect_callback_t callback);
void setDisconnectCallback(disconnect_callback_t callback);
void setSsidChangeCallback(ssid_change_callback_t callback);
void setStrengthChangeCallback(strength_change_callback_t callback);
void setStrengthUpdatePeriod(uint16_t strengthUpdatePeriod);

WifiStrength categoriseRSSI(float rssi);

std::string serialise(credentials_t *credentials);
std::string serialise(WifiStrength strength);
std::string serialise(byte *mac);
std::string serialise(IPAddress ip);

void connect(credentials_t *network, std::string hostname = "", std::string ip = "");
void connect(std::vector<credentials_t *> networks, std::string hostname = "", std::string ip = "");
void reconnect();
void loop();

} // namespace domos::Wifi

#endif