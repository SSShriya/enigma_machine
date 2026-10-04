#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#define main enigma_program_main
#include "parse_args.h"
#undef main

EnigmaSettings parse(std::vector<std::string> arguments) {
  std::vector<char*> argv;
  argv.reserve(arguments.size());

  for (auto& argument : arguments) {
    argv.push_back(argument.data());
  }

  return parseArguments(static_cast<int>(argv.size()), argv.data());
}

void expect(bool condition, const std::string& message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

void expectThrows(const std::function<void()>& action, const std::string& expectedText) {
  try {
    action();
  } catch (const std::runtime_error& error) {
    expect(std::string(error.what()).find(expectedText) != std::string::npos,
           "Wrong error message: " + std::string(error.what()));
    return;
  }

  throw std::runtime_error("Expected an exception containing: " + expectedText);
}

void testValidThreeRotorInput() {
  const EnigmaSettings settings =
      parse({"enigma", "--rotors",    "I",       "II", "III",    "--positions", "A",
             "B",      "C",           "--rings", "1",  "2",      "3",           "--reflector",
             "B",      "--plugboard", "AV",      "BS", "--text", "HELLO WORLD"});

  expect(settings.rotors.size() == 3, "Expected three rotors.");
  expect(settings.rotors[0] == "I", "Wrong left rotor.");
  expect(settings.positions[0] == 'A' && settings.positions[2] == 'C',
         "Positions were not parsed correctly.");
  expect(settings.rings[0] == 1 && settings.rings[2] == 3,
         "Ring settings were not parsed correctly.");
  expect(settings.reflector == 'B', "Reflector was not parsed correctly.");
  expect(settings.plugboardPairs.size() == 2, "Expected two plugboard pairs.");
  expect(settings.text == "HELLO WORLD", "Text was not parsed correctly.");
}

void testLowercasePositionIsNormalized() {
  const EnigmaSettings settings =
      parse({"enigma", "--rotors", "I", "I", "I", "--positions", "z", "a", "b", "--rings", "26",
             "20", "1", "--reflector", "C", "--text", "X"});

  expect(settings.positions[0] == 'Z', "Lowercase position should become uppercase.");
}

void testMismatchedRotorCountsFail() {
  expectThrows(
      [] {
        parse({"enigma", "--rotors", "I", "II", "III", "--positions", "A", "A", "--rings", "1", "1",
               "1", "--reflector", "B", "--text", "TEST"});
      },
      "must all equal");
}

void testNotEnoughRotorsFail() {
  expectThrows(
      [] {
        parse({"enigma", "--rotors", "I", "II", "--positions", "A", "A", "--rings", "1", "1",
               "--reflector", "B", "--text", "TEST"});
      },
      "must all equal");
}

void testInvalidRotorsFail() {
  expectThrows(
      [] {
        parse({"enigma", "--rotors", "I", "II", "X", "--positions", "A", "A", "--rings", "1", "1",
               "1", "--reflector", "B", "--text", "TEST"});
      },
      "Invalid rotor");
}

void testInvalidRingFails() {
  expectThrows(
      [] {
        parse({"enigma", "--rotors", "I", "II", "III", "--positions", "A", "A", "A", "--rings",
               "27", "24", "1", "--reflector", "B", "--text", "TEST"});
      },
      "Ring settings");
}

void testInvalidReflectorFail() {
  expectThrows(
      [] {
        parse({"enigma", "--rotors", "I", "II", "III", "--positions", "A", "A", "--rings", "1", "1",
               "1", "--reflector", "Y", "--text", "TEST"});
      },
      "Invalid reflector");
}

void testInvalidPlugboardPairFails() {
  expectThrows(
      [] {
        parse({"enigma", "--rotors", "I", "I", "I", "--positions", "A", "A", "A", "--rings", "1",
               "2", "3", "--reflector", "B", "--plugboard", "ABC", "--text", "TEST"});
      },
      "plugboard pair");
}

int main() {
  int passed = 0;
  int failed = 0;

  const std::vector<std::pair<std::string, std::function<void()>>> tests = {
      {"valid three-rotor input", testValidThreeRotorInput},
      {"lowercase position normalization", testLowercasePositionIsNormalized},
      {"mismatched rotor counts", testMismatchedRotorCountsFail},
      {"not enough rotor counts", testNotEnoughRotorsFail},
      {"invalid rotor", testInvalidRotorsFail},
      {"invalid reflector", testInvalidReflectorFail},
      {"invalid ring", testInvalidRingFails},
      {"invalid plugboard pair", testInvalidPlugboardPairFails},
  };

  for (const auto& [name, test] : tests) {
    try {
      test();
      ++passed;
      std::cout << "PASS: " << name << '\n';
    } catch (const std::exception& error) {
      ++failed;
      std::cerr << "FAIL: " << name << " -- " << error.what() << '\n';
    }
  }

  std::cout << "\n" << passed << " passed, " << failed << " failed\n\n";
  return failed == 0 ? 0 : 1;
}
