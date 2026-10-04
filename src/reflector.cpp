#include "reflector.h"

#include "utils.h"

/* Constructor takes in the wiring string for the reflector */
Reflector::Reflector(const std::string& wiring): wiring_(wiring) {}

/* Replaces all characters with their corresponding chars in the wiring table */
std::string Reflector::reflect(std::string text) {
  for (char& c : text) {
    c = reflect(c);
  }
  return text;
}

/* Replaces one character with its corresponding char in the wiring table */
char Reflector::reflect(char c) {
  if (!std::isalpha(c)) {
    return c;
  }

  c = static_cast<char>(
    std::toupper(static_cast<unsigned char>(c))
  );
  return wiring_[toIdx(c)];
}
