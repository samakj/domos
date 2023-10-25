#ifndef _domos_DHT22_h
#define _domos_DHT22_h

#include "Sensor.h"
#include <Arduino.h>
#include <DHT.h>

class _DHT : public DHT {
public:
  _DHT(uint8_t pin, uint8_t type, uint8_t count = 6) : DHT(pin, type, count) {}
};

namespace domos::Sensors {

class DHT22 : domos::Sensors::Sensor {
public:
  typedef float temperature_t;
  typedef float humidity_t;

  typedef std::function<void(temperature_t temperature)> temperature_change_callback_t;
  typedef std::function<void(humidity_t humidity)> humidity_change_callback_t;

  DHT22(uint8_t pinNo)

  _DHT *client = nullptr;

  temperature_change_callback_t temperatureCallback = nullptr;
  humidity_change_callback_t humidityCallback = nullptr;

  temperature_t temperatureTolerance = 0.15;
  humidity_t humidityTolerance = 0.25;

  uint16_t readPeriod = 2000;
  unsigned long lastTemperatureRead = 0;
  unsigned long lastHumidityRead = 0;

  temperature_t getTemperature();
  humidity_t getHumidity();

  void setReadPeriod(uint16_t period);
  void setTemperatureTolerance(temperature_t tolerance);
  void setHumidityTolerance(humidity_t tolerance);
  void setTemperatureCallback(TemperatureCallback callback);
  void setHumidityCallback(HumidityCallback callback);

  void setup();
  void loop();
};
} // namespace domos::Sensors

#endif