#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "machine.h"

namespace {

constexpr char rotorIWiring[] = "EKMFLGDQVZNTOWYHXUSPAIBRCJ";
constexpr char rotorIIWiring[] = "AJDKSIRUXBLHWTMCQGZNPYFVOE";
constexpr char rotorIIIWiring[] = "BDFHJLCPRTXVZNYEIWGAKMUSQO";
constexpr char reflectorBWiring[] = "YRUHQSLDPXNGOKMIEBFZCWVJAT";

void expect(bool condition, const std::string& message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

void expectEqual(const std::string& actual, const std::string& expected,
                 const std::string& message) {
  if (actual != expected) {
    throw std::runtime_error(message + " Expected: \"" + expected + "\", actual: \"" + actual +
                             "\".");
  }
}

EnigmaMachine makeMachine(const std::vector<std::string>& plugboardPairs = {},
                          char leftPosition = 'A', char middlePosition = 'A',
                          char rightPosition = 'A', int leftRing = 1, int middleRing = 1,
                          int rightRing = 1) {
  Plugboard plugboard(plugboardPairs);

  Rotor right(rotorIIIWiring, 'V', rightPosition, rightRing);
  Rotor middle(rotorIIWiring, 'E', middlePosition, middleRing);
  Rotor left(rotorIWiring, 'Q', leftPosition, leftRing);
  Reflector reflector(reflectorBWiring);

  return EnigmaMachine(plugboard, right, middle, left, reflector);
}

void testHistoricalKnownAnswer() {
  EnigmaMachine machine = makeMachine();

  expectEqual(machine.encrypt("AAAAA"), "BDZGO",
              "Known Enigma-I example should encrypt AAAAA to BDZGO.");
}

void testEncryptionDecryptsAfterReset() {
  const std::string plainText = "ENIGMA TEST MESSAGE";

  EnigmaMachine encryptor = makeMachine({"AV", "BS", "CG"}, 'M', 'C', 'K', 3, 7, 12);
  const std::string cipherText = encryptor.encrypt(plainText);

  EnigmaMachine decryptor = makeMachine({"AV", "BS", "CG"}, 'M', 'C', 'K', 3, 7, 12);
  expectEqual(decryptor.encrypt(cipherText), plainText,
              "Encrypting again after resetting must recover the plaintext.");
}

void testPlugboardIsPartOfFullMachinePath() {
  const std::string plainText = "HELLOWORLD";

  EnigmaMachine withoutPlugboard = makeMachine();
  EnigmaMachine withPlugboard = makeMachine({"AV", "BS", "CG"});

  const std::string withoutPairs = withoutPlugboard.encrypt(plainText);
  const std::string withPairs = withPlugboard.encrypt(plainText);

  expect(withoutPairs != withPairs,
         "Adding plugboard pairs should change the full-machine ciphertext.");

  EnigmaMachine resetWithPlugboard = makeMachine({"AV", "BS", "CG"});
  expectEqual(resetWithPlugboard.encrypt(withPairs), plainText,
              "The full machine should still decrypt correctly with a plugboard.");
}

void testRotorStateAdvancesBetweenCalls() {
  EnigmaMachine machine = makeMachine();

  const std::string first = machine.encrypt("A");
  const std::string second = machine.encrypt("A");

  expect(first != second,
         "Two consecutive identical letters should differ because the right rotor steps.");
}

void testRingSettingsAffectFullMachineResult() {
  EnigmaMachine ringOne = makeMachine();
  EnigmaMachine changedRings = makeMachine({}, 'A', 'A', 'A', 2, 5, 9);

  const std::string input = "AAAAA";
  expect(ringOne.encrypt(input) != changedRings.encrypt(input),
         "Changing ring settings must change the full-machine ciphertext.");
}

void testNonLettersArePreservedAndDoNotStepRotors() {
  EnigmaMachine withPunctuation = makeMachine();
  EnigmaMachine lettersOnly = makeMachine();

  const std::string result = withPunctuation.encrypt("A! B?\nC7");
  const std::string letters = lettersOnly.encrypt("ABC");

  expect(result[1] == '!' && result[2] == ' ' && result[4] == '?' && result[5] == '\n' &&
             result[7] == '7',
         "Punctuation, whitespace, and digits must be unchanged.");

  expectEqual(std::string{result[0], result[3], result[6]}, letters,
              "Non-letters must not cause the rotors to step.");
}

void testEmptyTextStaysEmpty() {
  EnigmaMachine machine = makeMachine();

  expectEqual(machine.encrypt(""), "",
              "Encrypting an empty message should produce an empty message.");
}

}  

int main() {
  const std::vector<std::pair<std::string, std::function<void()>>> tests = {
      {"historical known-answer example", testHistoricalKnownAnswer},
      {"encryption decrypts after reset", testEncryptionDecryptsAfterReset},
      {"plugboard full-machine path", testPlugboardIsPartOfFullMachinePath},
      {"rotor state advances", testRotorStateAdvancesBetweenCalls},
      {"ring settings affect full machine", testRingSettingsAffectFullMachineResult},
      {"non-letters are preserved without stepping", testNonLettersArePreservedAndDoNotStepRotors},
      {"empty text", testEmptyTextStaysEmpty},
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
