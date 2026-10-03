#include <iostream>
#include <string>
#include <vector>

#include "parse_args.h"
#include "plugboard.h"

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

    std::cout << plugboard(settings.text, settings.plugboardPairs) << '\n';
  } catch (const std::exception& e) {
    usage(argv[0], e.what());
  }
  return 0;
}
