#include "Sensor.h"

domos::Sensors::pin_t domos::Sensors::pinGND = {PIN_GND, GROUND};
domos::Sensors::pin_t domos::Sensors::pin3V3 = {PIN_3V3, POWER_3V3};
domos::Sensors::pin_t domos::Sensors::pin5V = {PIN_5V, POWER_5V};
domos::Sensors::pin_t domos::Sensors::pinRST = {PIN_RST, OUTPUT};

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

void domos::Sensors::Sensor::setPin(uint8_t number, uint8_t mode) {
  domos::Sensors::pin_t *pin = new domos::Sensors::pin_t;
  pin->number = number;
  pin->mode = mode;
  domos::Sensors::Sensor::setPin(pin);
};

void domos::Sensors::Sensor::setPin(domos::Sensors::pin_t *pin) { this->pins[pin->number] = pin; };

void domos::Sensors::Sensor::applyPinModes() {
  for (auto &it : this->pins)
    if (domos::Sensors::isValidPinMode(it.second->mode))
      pinMode(it.second->number, it.second->mode);
};

bool domos::Sensors::isValidPinMode(uint8_t mode) {
  if (mode == OUTPUT) {
    return true;
  } else if (mode == INPUT) {
    return true;
#ifdef OUTPUT_OPEN_DRAIN
  } else if (mode == OUTPUT_OPEN_DRAIN) {
    return true;
#endif
#ifdef INPUT_PULLUP
  } else if (mode == INPUT_PULLUP) {
    return true;
#endif
#ifdef INPUT_PULLDOWN
  } else if (mode == INPUT_PULLDOWN) {
    return true;
#endif
#ifdef PULLUP
  } else if (mode == PULLUP) {
    return true;
#endif
#ifdef PULLDOWN
  } else if (mode == PULLDOWN) {
    return true;
#endif
#ifdef OPEN_DRAIN
  } else if (mode == OPEN_DRAIN) {
    return true;
#endif
#ifdef OUTPUT_OPEN_DRAIN
  } else if (mode == OUTPUT_OPEN_DRAIN) {
    return true;
#endif
#ifdef ANALOG
  } else if (mode == ANALOG) {
    return true;
#endif
  }
  return false;
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

  _pin += "\"number\":\"";
  if (pin->number == PIN_GND)
    _pin += "gnd";
  if (pin->number == PIN_3V3)
    _pin += "3v3";
  if (pin->number == PIN_5V)
    _pin += "5v";
  if (pin->number == PIN_RST)
    _pin += "rst";
  else
    _pin += pin->number;
  _pin += "\",";

  _pin += "\"value\":";

  if (pin->mode == OUTPUT) {
    _pin += "\"output\"";
  } else if (pin->mode == INPUT) {
    _pin += "\"input\"";
  } else if (pin->mode == GROUND) {
    _pin += "\"ground\"";
  } else if (pin->mode == POWER_3V3) {
    _pin += "\"3v3\"";
  } else if (pin->mode == POWER_5V) {
    _pin += "\"5v\"";
  } else if (pin->mode == INPUT_OUTPUT) {
    _pin += "\"input output\"";
#ifdef OUTPUT_OPEN_DRAIN
  } else if (pin->mode == OUTPUT_OPEN_DRAIN) {
    _pin += "\"output open drain\"";
#endif
#ifdef INPUT_PULLUP
  } else if (pin->mode == INPUT_PULLUP) {
    _pin += "\"input pull up\"";
#endif
#ifdef INPUT_PULLDOWN
  } else if (pin->mode == INPUT_PULLDOWN) {
    _pin += "\"input pull down\"";
#endif
#ifdef PULLUP
  } else if (pin->mode == PULLUP) {
    _pin += "\"pullup\"";
#endif
#ifdef PULLDOWN
  } else if (pin->mode == PULLDOWN) {
    _pin += "\"pulldown\"";
#endif
#ifdef OPEN_DRAIN
  } else if (pin->mode == OPEN_DRAIN) {
    _pin += "\"open drain\"";
#endif
#ifdef OUTPUT_OPEN_DRAIN
  } else if (pin->mode == OUTPUT_OPEN_DRAIN) {
    _pin += "\"output open drain\"";
#endif
#ifdef ANALOG
  } else if (pin->mode == ANALOG) {
    _pin += "\"analog\"";
#endif
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
