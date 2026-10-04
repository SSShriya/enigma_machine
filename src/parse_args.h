#include <string>
#include <vector>

struct EnigmaSettings {
  std::vector<std::string> rotors;
  std::vector<char> positions;
  std::vector<int> rings;
  char reflector;
  std::vector<std::string> plugboardPairs;
  std::string text;
};

[[noreturn]] void usage(const char* program, const std::string& error);
bool isOption(const std::string& value);
char parseLetter(const std::string& value, const std::string& option);
int parseRing(const std::string& value);
std::vector<std::string> readValues(int argc, char* argv[], int& index, const std::string& option);
EnigmaSettings parseArguments(int argc, char* argv[]);