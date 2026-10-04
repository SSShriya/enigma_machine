#include <string>

class Reflector {
 public:
  Reflector(const std::string& wiring);
  std::string reflect(std::string text);
  char reflect(char c);
 private:
  std::string wiring_;
};