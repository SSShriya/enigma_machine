#include "rotor.h"

#include "utils.h"

/* Constructor: takes in the wiring string, the position of the notch, and the starting position: 
   the character that appears in the rotor window at the start. */
Rotor::Rotor(const std::string wiring, char notch, char startPos) {
  wiring_ = wiring;
  notchIdx_ = toIdx(notch);
  curIdx_ = toIdx(startPos);
}