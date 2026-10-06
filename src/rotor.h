#include <string>
#include <array>

class Rotor {
 public:
  Rotor(const std::string& wiring, char notch, char startPos, int ringSetting);
  void step();
  bool atNotch() const;
  char encryptChar(char c) const;
  char decryptChar(char c) const;

 private:
  std::array<char, 26> wiring_;
  std::array<char, 26> inverseWiring_;
  int notchIdx_;
  int curIdx_;
  int ringSetting_;
  char transform(char c, const std::array<char, 26>& mapping) const;
  
};