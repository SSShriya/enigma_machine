#include "reflector.h"

#include "utils.h"

/* Constructor takes in the wiring string for the reflector */
Reflector::Reflector(std::string wiring) { wiring_ = wiring; }

/* Replaces all characters with their corresponding chars in the wiring table */
std::string Reflector::reflect(std::string text) {
  for (char& c : text) {
    if (std::isalpha(c)) {
      c = wiring_[toIdx(c)];
    }
  }
  return text;
}
