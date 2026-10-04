#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "plugboard.h"

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

void testNoPairsLeaveLettersUnchanged() {
  Plugboard plugboard({});

  expectEqual(plugboard.swapLetters("ENIGMA"), "ENIGMA",
              "An empty plugboard should not change letters.");
}

void testOnePairSwapsInBothDirections() {
  Plugboard plugboard({"AV"});

  expectEqual(plugboard.swapLetters("AVVAZ"), "VAAVZ",
              "A plugboard connection must work in both directions.");
}

void testSeveralPairsAndUnconnectedLetters() {
  Plugboard plugboard({"AV", "BS", "CG"});

  expectEqual(plugboard.swapLetters("ABC VSG XYZ"), "VSG ABC XYZ",
              "Only connected letters should be swapped.");
}

void testLowercaseIsNormalizedAndPunctuationSurvives() {
  Plugboard plugboard({"AV", "BS"});

  expectEqual(plugboard.swapLetters("a big, vase!"), "V SIG, AVBE!",
              "Letters should be uppercased and swapped; punctuation stays unchanged.");
}

void testARepeatedLetterIsRejected() {
  expectThrows([] { Plugboard plugboard({"AV", "AB"}); }, "cannot be connected to two things");
}

void testReversedDuplicatePairIsRejected() {
  expectThrows([] { Plugboard plugboard({"AV", "VA"}); }, "cannot be connected to two things");
}

int main() {
  int passed = 0;
  int failed = 0;

  const std::vector<std::pair<std::string, std::function<void()>>> tests = {
      {"no pairs leave letters unchanged", testNoPairsLeaveLettersUnchanged},
      {"one pair swaps both directions", testOnePairSwapsInBothDirections},
      {"several pairs and unconnected letters", testSeveralPairsAndUnconnectedLetters},
      {"lowercase and punctuation", testLowercaseIsNormalizedAndPunctuationSurvives},
      {"repeated letter is rejected", testARepeatedLetterIsRejected},
      {"reversed duplicate pair is rejected", testReversedDuplicatePairIsRejected},
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
