#include "Wifi.h"

std::vector<domos::Wifi::credentials_t *> domos::Wifi::networks = {};
std::string targetHostname = "";
std::string targetIp = "";
uint16_t maxWait = domos::Time::MINUTE_IN_MS;
uint16_t strengthUpdatePeriod = 15 * domos::Time::SECOND_IN_MS;

bool domos::Wifi::_isConnecting = false;
domos::Wifi::WifiStrength domos::Wifi::_strength = domos::Wifi::WifiStrength::NOT_CONNECTED;
uint32_t domos::Wifi::_connectionAttemptStart = 0;
uint32_t domos::Wifi::_lastStrengthUpdate = 0;
std::vector<std::string> domos::Wifi::_logTags = {"Wifi"};

bool domos::Wifi::isConnecting() { return domos::Wifi::_isConnecting; }

bool domos::Wifi::isConnected() { return WiFi.status() == WL_CONNECTED; }

std::string domos::Wifi::getMACAddress() {
  byte mac[6];
  WiFi.macAddress(mac);
  return domos::Wifi::serialise(mac);
}

std::string domos::Wifi::getIPAddress() {
  IPAddress ip = WiFi.localIP();
  return domos::Wifi::serialise(ip);
}

std::string domos::Wifi::getHostname() {
  if (!domos::Wifi::isConnected())
    return "";
  return (std::string)WiFi.getHostname();
}

std::string domos::Wifi::getSSID() {
  if (!domos::Wifi::isConnected())
    return "";
  return (std::string)WiFi.SSID().c_str();
}

domos::Wifi::WifiStrength domos::Wifi::getStrength() {
  if (!domos::Wifi::isConnected())
    return domos::Wifi::WifiStrength::NOT_CONNECTED;
  return domos::Wifi::categoriseRSSI(WiFi.RSSI());
}

domos::Wifi::credentials_t *
domos::Wifi::getStrongestNetwork(std::vector<domos::Wifi::credentials_t *> _networks) {
  if (!_networks.size()) {
    domos::Logger::error(
        domos::Wifi::_logTags, "Empty list passed to domos::Wifi::getStrongestNetwork"
    );
    return nullptr;
  }

  domos::Logger::infof(
      domos::Wifi::_logTags, "Finding strongest of %d networks...\n", _networks.size()
  );
  domos::Logger::debug(domos::Wifi::_logTags, "Scanning local networks...");

  uint8_t networkCount = WiFi.scanNetworks();

  domos::Logger::debugf(domos::Wifi::_logTags, "%d networks found in range.\n", networkCount);

  domos::Wifi::credentials_t *strongest = nullptr;
  float strengthOfStrongest = 0.0f;

  for (uint8_t i = 0; i < networkCount; i++)
    for (domos::Wifi::credentials_t *_network : _networks)
      if (_network->ssid == WiFi.SSID(i).c_str()) {
        float _strength = WiFi.RSSI(i);
        domos::Logger::debugf(
            domos::Wifi::_logTags, "    '%s' network found with RSSI %.1f - %s.\n",
            _network->ssid.c_str(), _strength,
            domos::Wifi::serialise(domos::Wifi::categoriseRSSI(_strength)).c_str()
        );
        if (_strength > strengthOfStrongest) {
          strongest = _network;
          strengthOfStrongest = _strength;
        }
      }

  if (strongest == nullptr)
    domos::Logger::warn(
        domos::Wifi::_logTags, "None of the provided credentials_t matched found local networks."
    );
  else
    domos::Logger::infof(
        domos::Wifi::_logTags, "'%s' found as the strongest.\n", strongest->ssid.c_str()
    );

  return strongest;
}

void domos::Wifi::setHostname(std::string hostname) {
  domos::Wifi::targetHostname = hostname;
  if (!domos::Wifi::isConnected()) {
    domos::Logger::infof(
        domos::Wifi::_logTags, "Setting hostname to: ", domos::Wifi::targetHostname.c_str()
    );
    WiFi.setHostname(domos::Wifi::targetHostname.c_str());
  } else
    domos::Logger::warn(
        domos::Wifi::_logTags, "Hostname changes will only take effect on wifi re-connect."
    );
};

void domos::Wifi::setIPAddress(std::string ip) {
  domos::Wifi::targetIp = ip;

  if (!domos::Wifi::isConnected()) {
    std::vector<std::string> ipSplit = domos::Utils::string::split(domos::Wifi::targetIp, '.');

    if (ipSplit.size() != 4) {
      domos::Logger::errorf(
          domos::Wifi::_logTags, "Invalid IP set for wifi: %s\n", domos::Wifi::targetIp.c_str()
      );
      return;
    }

    IPAddress localIp(stoi(ipSplit[0]), stoi(ipSplit[1]), stoi(ipSplit[2]), stoi(ipSplit[3]));
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 0, 0);
    IPAddress dns1(8, 8, 8, 8);
    IPAddress dns2(4, 4, 4, 4);

    domos::Logger::infof(
        domos::Wifi::_logTags, "Setting IP to: %s\n", domos::Wifi::targetIp.c_str()
    );
    WiFi.config(localIp, gateway, subnet, dns1, dns2);
  } else
    domos::Logger::warn(
        domos::Wifi::_logTags, "IP changes will only take effect on wifi re-connect."
    );
};

void domos::Wifi::setMaxWait(uint16_t maxWait) { domos::Wifi::maxWait = maxWait; };

void domos::Wifi::setConnectCallback(connect_callback_t callback) {
  domos::Wifi::connectCallback = callback;
};

void domos::Wifi::setDisconnectCallback(disconnect_callback_t callback) {
  domos::Wifi::disconnectCallback = callback;
};

void domos::Wifi::setSsidChangeCallback(ssid_change_callback_t callback) {
  domos::Wifi::ssidChangeCallback = callback;
};

void domos::Wifi::setStrengthChangeCallback(strength_change_callback_t callback) {
  domos::Wifi::strengthChangeCallback = callback;
};

void domos::Wifi::setStrengthUpdatePeriod(uint16_t strengthUpdatePeriod) {
  domos::Wifi::strengthUpdatePeriod = strengthUpdatePeriod;
};

domos::Wifi::WifiStrength domos::Wifi::categoriseRSSI(float rssi) {
  if (rssi > -30.0f)
    return domos::Wifi::WifiStrength::VERY_STRONG;
  if (rssi > -55.0f)
    return domos::Wifi::WifiStrength::STRONG;
  if (rssi > -67.0f)
    return domos::Wifi::WifiStrength::GOOD;
  if (rssi > -80.0f)
    return domos::Wifi::WifiStrength::OKAY;
  if (rssi > -90.0f)
    return domos::Wifi::WifiStrength::WEAK;

  return domos::Wifi::WifiStrength::VERY_WEAK;
};

std::string domos::Wifi::serialise(credentials_t *credentials) { return credentials->ssid; };

std::string domos::Wifi::serialise(WifiStrength strength) {
  if (strength == domos::Wifi::WifiStrength::VERY_STRONG)
    return "very strong";
  if (strength == domos::Wifi::WifiStrength::STRONG)
    return "strong";
  if (strength == domos::Wifi::WifiStrength::GOOD)
    return "good";
  if (strength == domos::Wifi::WifiStrength::OKAY)
    return "okay";
  if (strength == domos::Wifi::WifiStrength::WEAK)
    return "weak";
  if (strength == domos::Wifi::WifiStrength::VERY_WEAK)
    return "very weak";
  if (strength == domos::Wifi::WifiStrength::NOT_CONNECTED)
    return "not connected";
  return "";
};

std::string domos::Wifi::serialise(byte *mac) {
  char buffer[32];
  sprintf(buffer, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return (std::string)buffer;
};

std::string domos::Wifi::serialise(IPAddress ip) {
  char buffer[32];
  sprintf(buffer, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  return (std::string)buffer;
};

void domos::Wifi::connect(
    domos::Wifi::credentials_t *_network, std::string hostname, std::string ip
) {
  return domos::Wifi::connect({_network}, hostname, ip);
}

void domos::Wifi::connect(
    std::vector<domos::Wifi::credentials_t *> _networks, std::string hostname, std::string ip
) {
  domos::Wifi::_strength = domos::Wifi::WifiStrength::NOT_CONNECTED;
  domos::Wifi::networks = _networks;
  domos::Wifi::credentials_t *strongest = domos::Wifi::getStrongestNetwork(domos::Wifi::networks);

  if (strongest == nullptr)
    return;

  if (domos::Wifi::isConnected())
    WiFi.disconnect();

  if (hostname.size())
    domos::Wifi::setHostname(hostname);
  if (ip.size())
    domos::Wifi::setIPAddress(ip);

  domos::Logger::infof(domos::Wifi::_logTags, "Wifi connecting to: %s\n", strongest->ssid.c_str());
  domos::Wifi::_isConnecting = true;
  domos::Wifi::_connectionAttemptStart = millis();
  domos::Wifi::_lastConnectionMessage = millis();
  WiFi.begin(strongest->ssid.c_str(), strongest->password.c_str());
}

void domos::Wifi::reconnect() {
  if (domos::Wifi::networks.size())
    domos::Wifi::connect(domos::Wifi::networks);
  else
    domos::Logger::error(domos::Wifi::_logTags, "No wifi networks to reconnect to.");
}

void domos::Wifi::loop() {
  if (domos::Wifi::isConnecting()) {
    if (domos::Wifi::isConnected()) {
      domos::Wifi::_isConnecting = false;
      WifiStrength _strength = domos::Wifi::getStrength();
      std::string ip = domos::Wifi::getIPAddress();
      std::string mac = domos::Wifi::getMACAddress();
      std::string ssid = domos::Wifi::getSSID();

      domos::Logger::infof(
          domos::Wifi::_logTags, "Wifi connected to %s, with a %s connection.\n", ssid.c_str(),
          domos::Wifi::serialise(_strength).c_str()
      );
      domos::Logger::infof(domos::Wifi::_logTags, "IP:   %s\n", ip.c_str());
      domos::Logger::infof(domos::Wifi::_logTags, "MAC:  %s\n", mac.c_str());

      if (domos::Wifi::connectCallback != nullptr)
        domos::Wifi::connectCallback(ssid);
      if (domos::Wifi::ssidChangeCallback != nullptr)
        domos::Wifi::ssidChangeCallback(ssid);
      if (domos::Wifi::strengthChangeCallback != nullptr)
        domos::Wifi::strengthChangeCallback(_strength);
    } else if (domos::Time::millisSince(domos::Wifi::_connectionAttemptStart) > domos::Wifi::maxWait) {
      domos::Logger::warn(
          domos::Wifi::_logTags,
          "Max wait exceeded trying to connect wifi, aborting and trying again."
      );
      domos::Wifi::reconnect();
    } else if (domos::Time::millisSince(domos::Wifi::_lastConnectionMessage) > 10000) {
      float dt = domos::Time::millisSince(domos::Wifi::_lastConnectionMessage) / 1000.0f;
      domos::Logger::infof(domos::Wifi::_logTags, "Wifi connecting %.1fs...\n", dt);
      domos::Wifi::_lastConnectionMessage = millis();
    }
  } else if (!domos::Wifi::isConnected()) {
    domos::Logger::warnf(domos::Wifi::_logTags, "Wifi disconnected, trying to reconnect");
    if (domos::Wifi::disconnectCallback != nullptr)
      domos::Wifi::disconnectCallback();
    domos::Wifi::reconnect();
  } else {
    if (domos::Time::millisSince(domos::Wifi::_lastStrengthUpdate) >
        domos::Wifi::strengthUpdatePeriod) {
      domos::Wifi::WifiStrength strength = domos::Wifi::getStrength();
      if (strength != domos::Wifi::_strength) {
        domos::Logger::infof(
            domos::Wifi::_logTags, "Wifi strength changed from %s to %s\n", domos::Wifi::_strength,
            strength
        );
        domos::Wifi::_strength = strength;
        domos::Wifi::strengthChangeCallback(domos::Wifi::_strength);
      }
      domos::Wifi::_lastStrengthUpdate = millis();
    }
  }
}