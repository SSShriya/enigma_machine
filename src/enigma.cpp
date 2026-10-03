#include <iostream>
#include <string>
#include <vector>

#include "parse_args.h"
#include "plugboard.h"

/* Rotor + Reflector Wiring strings */
const std::string WIRING_RTR_I   = "EKMFLGDQVZNTOWYHXUSPAIBRCJ";
const std::string WIRING_RTR_II  = "AJDKSIRUXBLHWTMCQGZNPYFVOE";
const std::string WIRITN_RTR_III = "BDFHJLCPRTXVZNYEIWGAKMUSQO";
const std::string WIRING_RTR_IV  = "ESOVPZJAYQUIRHXLNFTGKDCMWB";
const std::string WIRING_RTR_V   = "VZBRGITYUPSDNHLXAWMJQOFECK";
const std::string WIRING_RFL_A   = "EJMZALYXVBWFCRQUONTSPIKHGD";
const std::string WIRING_RFL_B   = "YRUHQSLDPXNGOKMIEBFZCWVJAT";
const std::string WIRING_RFL_C   = "FVPJIAOYEDRZXWGCTKUQSBNMHL";

/* Rotor Notches */
const char NOTCH_RTR_I   = 'Y';
const char NOTCH_RTR_II  = 'M';
const char NOTCH_RTR_III = 'D';
const char NOTCH_RTR_IV  = 'R';
const char NOTCH_RTR_V   = 'H';

int main(int argc, char* argv[]) {
  try {
    EnigmaSettings settings = parseArguments(argc, argv);

    // currently just print all settings
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
    std::cout << plugboard.swapLetters(settings.text) << '\n';
  } catch (const std::exception& e) {
    usage(argv[0], e.what());
  }
  return 0;
}
