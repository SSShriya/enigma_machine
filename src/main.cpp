#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "parse_args.h"
#include "machine.h"

/* Rotor + Reflector Wiring strings */
const std::string WIRING_RTR_I   = "EKMFLGDQVZNTOWYHXUSPAIBRCJ";
const std::string WIRING_RTR_II  = "AJDKSIRUXBLHWTMCQGZNPYFVOE";
const std::string WIRING_RTR_III = "BDFHJLCPRTXVZNYEIWGAKMUSQO";
const std::string WIRING_RTR_IV  = "ESOVPZJAYQUIRHXLNFTGKDCMWB";
const std::string WIRING_RTR_V   = "VZBRGITYUPSDNHLXAWMJQOFECK";
const std::string WIRING_RFL_A   = "EJMZALYXVBWFCRQUONTSPIKHGD";
const std::string WIRING_RFL_B   = "YRUHQSLDPXNGOKMIEBFZCWVJAT";
const std::string WIRING_RFL_C   = "FVPJIAOYEDRZXWGCTKUQSBNMHL";

/* Rotor Notches */
const char NOTCH_RTR_I   = 'Q';
const char NOTCH_RTR_II  = 'E';
const char NOTCH_RTR_III = 'V';
const char NOTCH_RTR_IV  = 'J';
const char NOTCH_RTR_V   = 'Z';

/* Maps for rotors and reflectors */
struct RotorDefinition {
  std::string wiring;
  char notch;
}; 

const std::map<std::string, RotorDefinition> rotorDefinitions = {
  {"I",   {WIRING_RTR_I,   NOTCH_RTR_I}},
  {"II",  {WIRING_RTR_II,  NOTCH_RTR_II}},
  {"III", {WIRING_RTR_III, NOTCH_RTR_III}},
  {"IV",  {WIRING_RTR_IV,  NOTCH_RTR_IV}},
  {"V",   {WIRING_RTR_V,   NOTCH_RTR_V}}
};

const std::map<char, std::string> reflectorWiring = {
  {'A', WIRING_RFL_A}, 
  {'B', WIRING_RFL_B}, 
  {'C', WIRING_RFL_C}
};

std::string getReflectorWiring(char reflectorName) {
  auto it = reflectorWiring.find(reflectorName);
  if (it == reflectorWiring.end()) {
    throw std::runtime_error(std::string("Unknown reflector: ") + reflectorName);
  }
  return it->second;
}

RotorDefinition getRotorDefinition(std::string rotorName) {
  auto it = rotorDefinitions.find(rotorName);
  if (it == rotorDefinitions.end()) {
    throw std::runtime_error(std::string("Unknown rotor: ") + rotorName);
  }
  return it->second;
}

int main(int argc, char* argv[]) {
  try {
    EnigmaSettings settings = parseArguments(argc, argv);

    std::cout << "Rotors: ";
    for (const auto& rotor : settings.rotors) std::cout << rotor << ' ';
    std::cout << "\nPositions: ";
    for (char position : settings.positions) std::cout << position << ' ';
    std::cout << "\nRings: ";
    for (int ring : settings.rings) std::cout << ring << ' ';
    std::cout << "\nReflector: " << settings.reflector;
    std::cout << "\nPlugboard: ";
    for (const auto& pair : settings.plugboardPairs) std::cout << pair << ' ';
    std::cout << "\nText: " << settings.text << '\n';

    Plugboard plugboard = Plugboard(settings.plugboardPairs);

    RotorDefinition leftRotorDef = getRotorDefinition(settings.rotors[0]);
    RotorDefinition middleRotorDef = getRotorDefinition(settings.rotors[1]);
    RotorDefinition rightRotorDef = getRotorDefinition(settings.rotors[2]);
    Rotor leftRotor = Rotor(leftRotorDef.wiring, leftRotorDef.notch, settings.positions[0], settings.rings[0]);
    Rotor middleRotor = Rotor(middleRotorDef.wiring, middleRotorDef.notch, settings.positions[1], settings.rings[0]);
    Rotor rightRotor = Rotor(rightRotorDef.wiring, rightRotorDef.notch, settings.positions[2], settings.rings[0]);

    Reflector reflector = Reflector(getReflectorWiring(settings.reflector));

    EnigmaMachine enigma = EnigmaMachine(plugboard, rightRotor, middleRotor, leftRotor, reflector);
    std::cout << "Encrypted text: " << enigma.encrypt(settings.text) << '\n';
  } catch (const std::exception& e) {
    usage(argv[0], e.what());
  }
  return 0;
}
