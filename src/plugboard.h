#include <array>
#include <string>
#include <vector>

#include "utils.h"

class Plugboard {
 public:
  Plugboard(const std::vector<std::string>& plugboardPairs);
  std::string swapLetters(std::string text);
  char swapLetter(char c);

 private:
  std::array<int, ALPHABET_SIZE> mapping_;
  std::vector<std::string> plugboardPairs_;
  void connect();
};
