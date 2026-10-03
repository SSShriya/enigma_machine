#include "plugboard.h"

#include <array>
#include <iostream>

/* Constructor method: initialise the mappings array using the provided plugboard pairs s*/
Plugboard::Plugboard(std::vector<std::string> plugboardPairs) {
  plugboardPairs_ = plugboardPairs;
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    mapping_[i] = static_cast<unsigned char>('A' + i);
  }
  connect();
}

/* Swaps letters according to the stored mappings array */
std::string Plugboard::swapLetters(std::string text) {
  for (char& c : text) {
    if (std::isalpha(static_cast<unsigned char>(c))) {
      c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
      c = mapping_[c - 'A'];
    }
  }
  return text;
}

/* Connects characters in plugboard - fails if either letter already connected to something else */
void Plugboard::connect() {
  for (std::string pair : plugboardPairs_) {
    char fst = pair[0];
    char snd = pair[1];
    int fstIdx = fst - 'A';
    int sndIdx = snd - 'A';
    if (mapping_[fstIdx] != fst || mapping_[sndIdx] != snd) {
      throw std::runtime_error("A letter on the plugboard cannot be connected to two things");
    }
    mapping_[fst - 'A'] = snd;
    mapping_[snd - 'A'] = fst;
  }
}