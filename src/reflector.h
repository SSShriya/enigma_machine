#include <string>

class Reflector {
 public:
  Reflector(const std::string& wiring);
  std::string reflect(std::string text) const;
  char reflect(char c) const;
 private:
  std::string wiring_;
};