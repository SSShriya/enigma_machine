#include "plugboard.h"

#include <array>
#include <iostream>

/* Constructor method: initialise the mappings array using the provided plugboard pairs s*/
Plugboard::Plugboard(const std::vector<std::string>& plugboardPairs) : plugboardPairs_(plugboardPairs) {
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    mapping_[i] = static_cast<unsigned char>('A' + i);
  }
  connect();
}

/* Swaps letters according to the stored mappings array */
std::string Plugboard::swapLetters(std::string text) {
  for (char& c : text) {
    c = swapLetter(c);
  }
  return text;
}

/* Swap one letter according to stored mappings */
char Plugboard::swapLetter(char c) {
  if (std::isalpha(static_cast<unsigned char>(c))) {
    c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    c = mapping_[toIdx(c)];
  }
  return c;
}

/* Connects characters in plugboard - fails if either letter already connected to something else */
void Plugboard::connect() {
  for (std::string pair : plugboardPairs_) {
    char fst = static_cast<char>(std::toupper(static_cast<unsigned char>(pair[0])));
    char snd = static_cast<char>(std::toupper(static_cast<unsigned char>(pair[1])));

    if (fst == snd) {
      throw std::runtime_error("Letters on a plugboard cannot map to themselves");
    }

    int fstIdx = toIdx(fst);
    int sndIdx = toIdx(snd);
    if (mapping_[fstIdx] != fst || mapping_[sndIdx] != snd) {
      throw std::runtime_error("One letter on the plugboard cannot be connected to two things");
    }
    
    mapping_[fstIdx] = snd;
    mapping_[sndIdx] = fst;
  }
}