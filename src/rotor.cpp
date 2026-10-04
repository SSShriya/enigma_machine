#include "rotor.h"
#include "utils.h"

/* Constructor: takes in the wiring string, the position of the notch, and the starting position: 
   the character that appears in the rotor window at the start. */
Rotor::Rotor(const std::string& wiring, char notch, char startPos) :
  notchIdx_(toIdx(notch)),
  curIdx_(toIdx(startPos)) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
      wiring_[i] = wiring[i];
      inverseWiring_[toIdx(wiring[i])] = static_cast<char>('A' + i);
    }
  }

/* Step the rotor - increase the current index by 1 */
void Rotor::step() {
  curIdx_ = (curIdx_ + 1) % ALPHABET_SIZE;
}

/* Returns true if the rotor is at its notch */
bool Rotor::atNotch() {
  return curIdx_ == notchIdx_;
}

/* Uses the wiring table to encrypt a character */
char Rotor::transform(char c, const std::array<char, 26>& mapping) {
  if (!std::isalpha(static_cast<unsigned char>(c))) {
     return c;
  }  
   
  c = static_cast<char>(
    std::toupper(static_cast<unsigned char>(c))
  );

  const int inputIndex = toIdx(c);
  const int shiftedInput = (inputIndex + curIdx_) % ALPHABET_SIZE;
  const int wiredOutput = toIdx(mapping[shiftedInput]);
  const int outputIndex = (wiredOutput - curIdx_ + ALPHABET_SIZE) % ALPHABET_SIZE;
  
  return static_cast<char>('A' + outputIndex);
}

char Rotor::encryptChar(char c) {
    return transform(c, wiring_);
}

char Rotor::decryptChar(char c) {
    return transform(c, inverseWiring_);
}