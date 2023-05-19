template <typename T>
std::string domos::Sensors::serialise(domos::Sensors::measurement_t<T> *measurement) {
  std::string _measurement = "{";

  _measurement += "\"id\":";
  _measurement += measurement->id;
  _measurement += ",";

  _measurement += "\"value\":";
  _measurement += domos::Sensors::serialise(measurement->value);
  _measurement += ",";

  _measurement += "\"tags\":\"";
  _measurement += domos::Sensors::serialise(measurement->tags);
  _measurement += "\",";

  _measurement += "\"metric\":\"";
  _measurement += domos::Sensors::serialise(measurement->metric);
  _measurement += "\"";

  _measurement += "}";
  return _measurement;
}