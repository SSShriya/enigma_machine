#include "reflector.h"

#include "utils.h"

Reflector::Reflector(std::string wiring) { wiring_ = wiring; }

std::string Reflector::reflect(std::string text) {
  for (char& c : text) {
    if (std::isalpha(c)) {
      c = wiring_[toIdx(c)];
    }
  }
  return text;
}
