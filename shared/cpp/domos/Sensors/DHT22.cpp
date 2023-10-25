#include "DHT22.h"

domos::Sensors::DHT22::DHT22(uint8_t id, uint8_t pinNo)
    : domos::Sensors(id, domos::Sensors::Model::DHT22) {
  this->setPin(&domos::Sensors::pinGND);
  this->setPin(&domos::Sensors::pin5V);
  this->setPin(pinNo, INPUT_OUTPUT);
}