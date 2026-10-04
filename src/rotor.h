#include <string>

class Rotor {
 public:
  Rotor(const std::string wiring, char notch, char startPos);
  void step();

 private:
  std::string wiring_;
  int notchIdx_;
  int curIdx_;
};