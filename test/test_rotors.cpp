#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "rotor.h"

namespace {

constexpr char rotorIWiring[] = "EKMFLGDQVZNTOWYHXUSPAIBRCJ";
constexpr char rotorINotch = 'Q';

void expect(bool condition, const std::string& message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

void expectEqual(char actual, char expected, const std::string& message) {
  if (actual != expected) {
    throw std::runtime_error(message + " Expected '" + expected + "', actual '" + actual + "'.");
  }
}

void testRotorIForwardWiringAtPositionA() {
  Rotor rotor(rotorIWiring, rotorINotch, 'A');

  expectEqual(rotor.encryptChar('A'), 'E', "Rotor I should map A forward at A.");
  expectEqual(rotor.encryptChar('B'), 'K', "Rotor I should map B forward at A.");
  expectEqual(rotor.encryptChar('C'), 'M', "Rotor I should map C forward at A.");
  expectEqual(rotor.encryptChar('D'), 'F', "Rotor I should map D forward at A.");
  expectEqual(rotor.encryptChar('E'), 'L', "Rotor I should map E forward at A.");
}

void testRotorIBackwardWiringAtPositionA() {
  Rotor rotor(rotorIWiring, rotorINotch, 'A');

  // Reverse mappings of the forward examples above
  expectEqual(rotor.decryptChar('E'), 'A', "Rotor I should map E backward at A.");
  expectEqual(rotor.decryptChar('K'), 'B', "Rotor I should map K backward at A.");
  expectEqual(rotor.decryptChar('M'), 'C', "Rotor I should map M backward at A.");
  expectEqual(rotor.decryptChar('F'), 'D', "Rotor I should map F backward at A.");
  expectEqual(rotor.decryptChar('L'), 'E', "Rotor I should map L backward at A.");
}

void testForwardThenBackwardRestoresEveryLetter() {
  Rotor rotor(rotorIWiring, rotorINotch, 'A');

  for (char c = 'A'; c <= 'Z'; ++c) {
    const char forward = rotor.encryptChar(c);
    expectEqual(rotor.decryptChar(forward), c, "Backward traversal must undo forward traversal");
  }
}

void testStepAdvancesWindowAndChangesMapping() {
  Rotor rotor(rotorIWiring, rotorINotch, 'A');

  expectEqual(rotor.encryptChar('A'), 'E', "Rotor I at A should map A to E.");

  rotor.step(); 

  expectEqual(rotor.encryptChar('A'), 'J',
              "Rotor position must affect the forward mapping after stepping");
  expectEqual(rotor.decryptChar('J'), 'A', "Backward mapping must use the same stepped position");
}

void testNotchIsDetectedAtQ() {
  Rotor rotor(rotorIWiring, rotorINotch, 'A');

  expect(!rotor.atNotch(), "Rotor I should not start at its Q notch when at A");

  for (int i = 0; i < 16; ++i) {
    rotor.step();
  }

  expect(rotor.atNotch(), "Rotor I should be at its notch after stepping A to Q");

  rotor.step();
  expect(!rotor.atNotch(), "Rotor I should leave its notch after one more step");
}

void testStepWrapsFromZToA() {
  Rotor rotor(rotorIWiring, rotorINotch, 'Z');

  rotor.step();

  expectEqual(rotor.encryptChar('A'), 'E', "A rotor stepping from Z must wrap to position A");
}

void testNonLettersRemainUnchanged() {
  Rotor rotor(rotorIWiring, rotorINotch, 'A');

  expectEqual(rotor.encryptChar('!'), '!', "Forward path should preserve punctuation");
  expectEqual(rotor.decryptChar('7'), '7', "Backward path should preserve digits");
  expectEqual(rotor.encryptChar(' '), ' ', "Forward path should preserve spaces");
  expectEqual(rotor.decryptChar('\n'), '\n', "Backward path should preserve newlines");
}

}  

int main() {
  const std::vector<std::pair<std::string, std::function<void()>>> tests = {
      {"Rotor I forward wiring at A", testRotorIForwardWiringAtPositionA},
      {"Rotor I backward wiring at A", testRotorIBackwardWiringAtPositionA},
      {"forward then backward restores every letter", testForwardThenBackwardRestoresEveryLetter},
      {"step changes mapping", testStepAdvancesWindowAndChangesMapping},
      {"notch detection", testNotchIsDetectedAtQ},
      {"step wraps from Z to A", testStepWrapsFromZToA},
      {"non-letters remain unchanged", testNonLettersRemainUnchanged},
  };

  int passed = 0;
  int failed = 0;

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

  std::cout << '\n' << passed << " passed, " << failed << " failed\n";
  return failed == 0 ? 0 : 1;
}
