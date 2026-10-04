#include <string>
#include <array>

class Rotor {
 public:
  Rotor(const std::string& wiring, char notch, char startPos);
  void step();
  bool atNotch();
  char encryptChar(char c);
  char decryptChar(char c);

 private:
  std::array<char, 26> wiring_;
  std::array<char, 26> inverseWiring_;
  int notchIdx_;
  int curIdx_;
  char transform(char c, const std::array<char, 26>& mapping);
  
};