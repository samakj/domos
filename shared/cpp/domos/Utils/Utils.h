#ifndef _domos_Utils_h
#define _domos_Utils_h

#include <ArduinoJson.h>

#include <sstream>
#include <string>
#include <vector>

namespace domos::Utils {
namespace string {
std::vector<std::string> split(std::string string, char delimeter = ',');
std::string join(std::vector<std::string> list, char delimeter = ',');
std::string formatFloat(std::string format, float value, float nullValue);
} // namespace string
} // namespace domos::Utils

#endif