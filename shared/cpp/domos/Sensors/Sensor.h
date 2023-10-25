#ifndef _domos_Sensor_h
#define _domos_Sensor_h

#include <Arduino.h>

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include <Utils/Utils.h>

#define GROUND 0xF0
#define POWER_3V3 0xF1
#define POWER_5V 0xF2
#define INPUT_OUTPUT 0xF3

#define PIN_GND 0xF0
#define PIN_3V3 0xF1
#define PIN_5V 0xF2
#define PIN_RST 0xF3

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

extern pin_t pinGND;
extern pin_t pin3V3;
extern pin_t pin5V;
extern pin_t pinRST;

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
  void setPin(uint8_t number, uint8_t mode);
  void setPin(pin_t *pin);

  void applyPinModes();

  std::string serialise();
};

bool isValidPinMode(uint8_t mode);

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