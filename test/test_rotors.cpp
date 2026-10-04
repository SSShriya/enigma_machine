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

void testRotorIForwardWiringAtPositionAAndRing1() {
  Rotor rotor(rotorIWiring, rotorINotch, 'A', 1);

  expectEqual(rotor.encryptChar('A'), 'E', "Rotor I at ring 1 should map A forward");
  expectEqual(rotor.encryptChar('B'), 'K', "Rotor I at ring 1 should map B forward");
  expectEqual(rotor.encryptChar('C'), 'M', "Rotor I at ring 1 should map C forward");
}

void testRotorIBackwardWiringAtPositionAAndRing1() {
  Rotor rotor(rotorIWiring, rotorINotch, 'A', 1);

  expectEqual(rotor.decryptChar('E'), 'A', "Rotor I should map E backward at ring 1");
  expectEqual(rotor.decryptChar('K'), 'B', "Rotor I should map K backward at ring 1");
  expectEqual(rotor.decryptChar('M'), 'C', "Rotor I should map M backward at ring 1");
}

void testForwardThenBackwardRestoresEveryLetterWithRingSetting() {
  Rotor rotor(rotorIWiring, rotorINotch, 'G', 12);

  for (char c = 'A'; c <= 'Z'; ++c) {
    const char forward = rotor.encryptChar(c);
    expectEqual(rotor.decryptChar(forward), c,
                "Backward traversal must undo forward traversal for every ring setting");
  }
}

void testRingSettingChangesMapping() {
  Rotor ring1(rotorIWiring, rotorINotch, 'A', 1);
  Rotor ring2(rotorIWiring, rotorINotch, 'A', 2);

  expectEqual(ring1.encryptChar('A'), 'E', "Rotor I at A/ring 1 should map A to E.");

  expectEqual(ring2.encryptChar('A'), 'K', "Rotor I at A/ring 2 should map A to K.");
  expect(ring1.encryptChar('A') != ring2.encryptChar('A'),
         "Changing the ring setting must change the mapping.");
}

void testStepChangesMappingWithRingSetting() {
  Rotor rotor(rotorIWiring, rotorINotch, 'A', 1);

  rotor.step();  

  expectEqual(rotor.encryptChar('A'), 'J',
              "Rotor position must still affect forward mapping when rings are present.");
  expectEqual(rotor.decryptChar('J'), 'A', "Backward mapping must use the same stepped position.");
}

void testRingSettingShiftsNotchWindowPosition() {
  Rotor rotor(rotorIWiring, rotorINotch, 'P', 2);

  expect(rotor.atNotch(), "Rotor I ring 2 should be at its turnover notch at P.");

  rotor.step();
  expect(!rotor.atNotch(), "Rotor should leave its notch after stepping.");
}

void testStepWrapsFromZToA() {
  Rotor rotor(rotorIWiring, rotorINotch, 'Z', 1);

  rotor.step();

  expectEqual(rotor.encryptChar('A'), 'E', "A rotor stepping from Z must wrap to position A.");
}

void testNonLettersRemainUnchanged() {
  Rotor rotor(rotorIWiring, rotorINotch, 'A', 7);

  expectEqual(rotor.encryptChar('!'), '!', "Forward path should preserve punctuation.");
  expectEqual(rotor.decryptChar('7'), '7', "Backward path should preserve digits.");
  expectEqual(rotor.encryptChar(' '), ' ', "Forward path should preserve spaces.");
  expectEqual(rotor.decryptChar('\n'), '\n', "Backward path should preserve newlines.");
}

}  // namespace

int main() {
  const std::vector<std::pair<std::string, std::function<void()>>> tests = {
      {"Rotor I forward wiring at A/ring 1", testRotorIForwardWiringAtPositionAAndRing1},
      {"Rotor I backward wiring at A/ring 1", testRotorIBackwardWiringAtPositionAAndRing1},
      {"forward then backward restores every letter",
       testForwardThenBackwardRestoresEveryLetterWithRingSetting},
      {"ring setting changes mapping", testRingSettingChangesMapping},
      {"step changes mapping", testStepChangesMappingWithRingSetting},
      {"ring setting shifts notch", testRingSettingShiftsNotchWindowPosition},
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

  std::cout << '\n' << passed << " passed, " << failed << " failed\n\n";
  return failed == 0 ? 0 : 1;
}
