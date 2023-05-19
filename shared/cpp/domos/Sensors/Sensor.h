#ifndef _domos_Sensor_h
#define _domos_Sensor_h

#include <Arduino.h>

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include <Utils/Utils.h>

namespace domos::Sensors {
enum Metric { TEMPERATURE, HUMIDITY, OPEN, POWER, VOLTAGE, CURRENT, ON };

template <typename T> struct measurement_t {
  uint8_t id;
  T value;
  std::vector<std::string> tags;
  Metric metric;
};

struct pin_t {
  uint8_t number;
  uint8_t mode;
};

enum Model { DHT22, DS18B20, SWITCH };

class Sensor {
public:
  uint8_t id;
  Model model;
  std::unordered_map<uint8_t, measurement_t<void *> *> measurements;
  std::unordered_map<uint8_t, pin_t *> pins;

  Sensor(uint8_t id, Model model);

  measurement_t<void *> *getMeasurement(uint8_t id);
  std::unordered_map<uint8_t, measurement_t<void *> *> getMeasurements();
  pin_t *getPin(uint8_t id);
  std::unordered_map<uint8_t, pin_t *> getPins();

  void setMeasurement(uint8_t id, measurement_t<void *> *measurement);
  void setPin(uint8_t id, pin_t *pin);

  void applyPinModes();

  std::string serialise();
};

std::string serialise(Metric metric);
std::string serialise(std::vector<std::string> tags);
template <typename T> std::string serialise(measurement_t<T> *measurement);
std::string serialise(pin_t *pin);
std::string serialise(Model model);
std::string serialise(int value);
std::string serialise(float value);
std::string serialise(bool value);
std::string serialise(std::string value);
} // namespace domos::Sensors

#include "Sensor.tpp"

#endif