#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "reflector.h"

void expectEqual(const std::string& actual, const std::string& expected,
                 const std::string& message) {
    if (actual != expected) {
        throw std::runtime_error(message + " Expected: \"" + expected +
                                 "\", actual: \"" + actual + "\".");
    }
}

void testReflectsLettersUsingWiring() {
    Reflector reflector("ZYXWVUTSRQPONMLKJIHGFEDCBA");

    expectEqual(reflector.reflect("ABCMZ"), "ZYXNA",
                "Reflector should use the supplied wiring for every letter.");
}

void testEnigmaReflectorBKnownMapping() {
    Reflector reflector("YRUHQSLDPXNGOKMIEBFZCWVJAT");

    expectEqual(reflector.reflect("ABCDE"), "YRUHQ",
                "Reflector B should produce its known mappings.");
}

void testPunctuationDigitsAndWhitespaceAreUnchanged() {
    Reflector reflector("ZYXWVUTSRQPONMLKJIHGFEDCBA");

    expectEqual(reflector.reflect("A! B? 7\tC\n#Z."), "Z! Y? 7\tX\n#A.",
                "Only letters should be reflected; punctuation, digits, and whitespace "
                "must remain exactly unchanged.");
}

void testEmptyTextStaysEmpty() {
    Reflector reflector("YRUHQSLDPXNGOKMIEBFZCWVJAT");

    expectEqual(reflector.reflect(""), "",
                "Reflecting an empty message should return an empty message.");
}

void testReflectionIsItsOwnInverseWithNonLetters() {
    Reflector reflector("YRUHQSLDPXNGOKMIEBFZCWVJAT");
    const std::string input = "ENIGMA, 1942!\nTEST #7.";

    const std::string once = reflector.reflect(input);
    const std::string twice = reflector.reflect(once);

    expectEqual(twice, input,
                "A valid Enigma reflector must return the original text when used twice, "
                "including non-letter characters.");
}

int main() {
    int passed = 0;
    int failed = 0;

    const std::vector<std::pair<std::string, std::function<void()>>> tests = {
        {"uses supplied wiring", testReflectsLettersUsingWiring},
        {"reflector B known mapping", testEnigmaReflectorBKnownMapping},
        {"punctuation, digits, and whitespace", testPunctuationDigitsAndWhitespaceAreUnchanged},
        {"empty text", testEmptyTextStaysEmpty},
        {"reflection is its own inverse", testReflectionIsItsOwnInverseWithNonLetters},
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

    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
