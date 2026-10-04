#include <string>

class Reflector {
 public:
  Reflector(std::string wiring);
  std::string reflect(std::string text);
 private:
  std::string wiring_;
};