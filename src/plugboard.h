#include <array>
#include <string>
#include <vector>

#include "utils.h"

class Plugboard {
 public:
  Plugboard(const std::vector<std::string>& plugboardPairs);
  std::string swapLetters(std::string text) const;
  char swapLetter(char c) const;

 private:
  std::array<int, ALPHABET_SIZE> mapping_;
  std::vector<std::string> plugboardPairs_;
  void connect();
};
