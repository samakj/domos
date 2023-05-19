#include "Sensor.h"

domos::Sensors::Sensor::Sensor(uint8_t _id, domos::Sensors::Model _model)
    : id(_id), model(_model), measurements({}), pins({}) {}

domos::Sensors::measurement_t<void *> *domos::Sensors::Sensor::getMeasurement(uint8_t id) {
  return this->measurements[id];
};

std::unordered_map<uint8_t, domos::Sensors::measurement_t<void *> *>
domos::Sensors::Sensor::getMeasurements() {
  return this->measurements;
};

domos::Sensors::pin_t *domos::Sensors::Sensor::getPin(uint8_t id) { return this->pins[id]; };

std::unordered_map<uint8_t, domos::Sensors::pin_t *> domos::Sensors::Sensor::getPins() {
  return this->pins;
};

void domos::Sensors::Sensor::setMeasurement(
    uint8_t id, domos::Sensors::measurement_t<void *> *measurement
) {
  this->measurements[id] = measurement;
};

void domos::Sensors::Sensor::setPin(uint8_t id, domos::Sensors::pin_t *pin) {
  this->pins[id] = pin;
};

void domos::Sensors::Sensor::applyPinModes() {
  for (auto &it : this->pins) {
    pinMode(it.second->number, it.second->mode);
  }
};

std::string domos::Sensors::serialise(Metric metric) {
  if (metric == domos::Sensors::Metric::TEMPERATURE)
    return "temperature";
  if (metric == domos::Sensors::Metric::HUMIDITY)
    return "humidity";
  if (metric == domos::Sensors::Metric::OPEN)
    return "open";
  if (metric == domos::Sensors::Metric::POWER)
    return "power";
  if (metric == domos::Sensors::Metric::VOLTAGE)
    return "voltage";
  if (metric == domos::Sensors::Metric::CURRENT)
    return "current";
  if (metric == domos::Sensors::Metric::ON)
    return "on";
  return nullptr;
};

std::string domos::Sensors::serialise(std::vector<std::string> tags) {
  std::string _tags = "[";
  _tags += domos::Utils::string::join(tags, ',');
  _tags += "]";
  return _tags;
};

std::string domos::Sensors::serialise(domos::Sensors::pin_t *pin) {
  std::string _pin = "{";

  _pin += "\"number\":";
  _pin += pin->number;
  _pin += ",";

  _pin += "\"value\":";

  if (pin->mode == OUTPUT) {
    _pin += "\"output\"";
  } else if (pin->mode == INPUT) {
    _pin += "\"input\"";
  } else if (pin->mode == INPUT_PULLUP) {
    _pin += "\"input_pullup\"";
  } else {
    _pin += "null";
  }

  _pin += "}";
  return _pin;
};

std::string domos::Sensors::serialise(domos::Sensors::Model model) {
  if (model == domos::Sensors::Model::DHT22)
    return "dht22";
  if (model == domos::Sensors::Model::DS18B20)
    return "ds18b20";
  if (model == domos::Sensors::Model::SWITCH)
    return "switch";
  return nullptr;
};

std::string domos::Sensors::Sensor::serialise() {
  std::string _sensor = "{";

  _sensor += "\"id\":";
  _sensor += this->id;
  _sensor += ",";

  _sensor += "\"model\":";
  _sensor += domos::Sensors::serialise(this->model);
  _sensor += ",";

  _sensor += "\"measurements\":{";

  for (auto &it : this->measurements) {
    _sensor += "\"";
    _sensor += it.first;
    _sensor += "\":";
    _sensor += "null";
    _sensor += ",";
  }

  _sensor += "},";

  _sensor += "\"pins\":{";

  for (auto &it : this->pins) {
    _sensor += "\"";
    _sensor += it.first;
    _sensor += "\":";
    _sensor += domos::Sensors::serialise(it.second);
    _sensor += ",";
  }

  _sensor += "}";

  _sensor += "}";
  return _sensor;
};

std::string domos::Sensors::serialise(int value) {
  char buffer[64];
  sprintf(buffer, "%d", value);
  return buffer;
};

std::string domos::Sensors::serialise(float value) {
  char buffer[64];
  sprintf(buffer, "%.3f", value);
  return buffer;
};

std::string domos::Sensors::serialise(bool value) { return value ? "true" : "false"; };

std::string domos::Sensors::serialise(std::string value) {
  std::string _value = "\"";
  _value = value;
  _value += "\"";
  return _value;
};
