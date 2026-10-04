#include "machine.h"
#include <string>

EnigmaMachine::EnigmaMachine(
    Plugboard plugboard,
    Rotor rightRotor,
    Rotor middleRotor,
    Rotor leftRotor,
    Reflector reflector)
    : plugboard_(std::move(plugboard)),
      rightRotor_(std::move(rightRotor)),
      middleRotor_(std::move(middleRotor)),
      leftRotor_(std::move(leftRotor)),
      reflector_(std::move(reflector)) {}

std::string EnigmaMachine::encrypt(std::string text) {
  /* Encrypt in order: plugboard, right rotor, middle rotor, left rotor, reflector,
     left rotor, middle rotor, right rotor, plugboard */
  std::string encryptedText;
  for (char c : text) {
    // don't dod anything for non alpha chars
    if (!std::isalpha(static_cast<unsigned char>(c))) {
      encryptedText += c;
      continue;
    }  

    stepRotors();

    c = plugboard_.swapLetter(c);

    c = rightRotor_.encryptChar(c);
    c = middleRotor_.encryptChar(c);
    c = leftRotor_.encryptChar(c);

    c = reflector_.reflect(c);

    c = leftRotor_.decryptChar(c);
    c = middleRotor_.decryptChar(c);
    c = rightRotor_.decryptChar(c);

    c = plugboard_.swapLetter(c);

    encryptedText += c;
  }

  return encryptedText;
}

void EnigmaMachine::stepRotors() {

  // check the old positions before any rotor moves.
  const bool middleAtNotch = middleRotor_.atNotch();
  const bool rightAtNotch = rightRotor_.atNotch();  

  // step rotors
  if (middleAtNotch) {
    leftRotor_.step();
  }  

  if (rightAtNotch) {
    middleRotor_.step();
  }  

  rightRotor_.step();
}