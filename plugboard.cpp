#include "plugboard.h"

#include <array>

#include "utils.h"

std::string plugboard(std::string& text, std::vector<std::string> plugboardPairs) {
  std::array<char, ALPHABET_SIZE> mapping;
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    mapping[i] = static_cast<unsigned char>('A' + i);
  }

  for (std::string pair : plugboardPairs) {
    char fst = pair[0];
    char snd = pair[1];
    mapping[fst - 'A'] = snd;
    mapping[snd - 'A'] = fst;
  }

  for (char& c : text) {
    if (std::isalpha(static_cast<unsigned char>(c))) {
      c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
      c = mapping[c - 'A'];
    }
  }

  return text;
}