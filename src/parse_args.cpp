#include "parse_args.h"

#include <algorithm>
#include <iostream>
#include <set>

#include "utils.h"

const int MAX_PLUGBOARD_PAIRS = 10;
const int NUM_ROTORS = 3;
static const std::set<std::string> validRotors = {"I", "II", "III", "IV", "V"};
static const std::set<std::string> validReflectors = {"A", "B", "C"};

[[noreturn]] void usage(const char* program, const std::string& error = "") {
  if (!error.empty()) std::cerr << "Error: " << error << "\n\n";

  std::cerr << "Usage:\n " << program
            << " --rotors I II III --positions A A A --rings 1 1 1 "
               " --reflector B --plugboard AV BS CG --text \"HELLO WORLD\" \n\n"
            << "Required options:\n"
            << " --rotors     Rotor names, left to right. Valid names: I, II, III, IV, V\n"
            << " --positions  Starting window letters for each rotor\n"
            << " --rings      Ring settings for each rotor from 1 to 26\n"
            << " --reflector  Reflector name. Valid names: A, B, C\n"
            << " --text       Message to encrypt/decrypt\n"
            << "Optional:\n"
            << " --plugboard  Space-separated letter pairs for plugboard substitution\n";

  std::exit(EXIT_FAILURE);
}

bool isOption(const std::string& value) { return value.rfind("--", 0) == 0; }

char parseLetter(const std::string& value, const std::string& option) {
  if (value.size() != 1 || !std::isalpha(static_cast<unsigned char>(value[0]))) {
    throw std::runtime_error(option + " values must be single letters.");
  }
  return static_cast<char>(std::toupper(static_cast<unsigned char>(value[0])));
}

int parseRing(const std::string& value) {
  try {
    size_t used = 0;
    int ring = std::stoi(value, &used);
    if (used != value.size() || ring < 1 || ring > ALPHABET_SIZE) {
      throw std::runtime_error("Invalid ring");
    }
    return ring;
  } catch (...) {
    throw std::runtime_error("Ring settings must be integers from 1 to 26.");
  }
}

std::vector<std::string> readValues(int argc, char* argv[], int& index, const std::string& option) {
  std::vector<std::string> values;
  while (index + 1 < argc && !isOption(argv[index + 1])) {
    values.emplace_back(argv[++index]);
  }
  if (values.empty()) throw std::runtime_error(option + " needs at least one value");
  return values;
}

EnigmaSettings parseArguments(int argc, char* argv[]) {
  EnigmaSettings settings;

  for (int i = 1; i < argc; i++) {
    const std::string option = argv[i];

    if (option == "--help") usage(argv[0]);
    if (!isOption(option)) throw std::runtime_error("Unexpected value: " + option);

    if (option == "--rotors") {
      for (const auto& value : readValues(argc, argv, i, option)) {
        if (validRotors.find(value) == validRotors.end()) {
          throw std::runtime_error("Invalid rotor name: " + value);
        }
        settings.rotors.push_back(value);
      }
    } else if (option == "--positions") {
      for (const auto& value : readValues(argc, argv, i, option)) {
        settings.positions.push_back(parseLetter(value, option));
      }
    } else if (option == "--rings") {
      for (const auto& value : readValues(argc, argv, i, option)) {
        settings.rings.push_back(parseRing(value));
      }
    } else if (option == "--reflector") {
      auto values = readValues(argc, argv, i, option);
      if (values.size() != 1) throw std::runtime_error("--reflector needs exactly one value.");
      if (validReflectors.find(values[0]) == validReflectors.end()) {
        throw std::runtime_error("Invalid reflector name: " + values[0]);
      }
      settings.reflector = values[0];
    } else if (option == "--plugboard") {
      settings.plugboardPairs = readValues(argc, argv, i, option);
      for (const auto& pair : settings.plugboardPairs) {
        if (pair.size() != 2 || !std::isalpha(static_cast<unsigned char>(pair[0])) ||
            !std::isalpha(static_cast<unsigned char>(pair[1]))) {
          throw std::runtime_error("Each plugboard pair must have two letters");
        }
      }
    } else if (option == "--text") {
      if (i + 1 >= argc) throw std::runtime_error("--text needs a message.");
      std::string text = argv[++i];
      std::transform(text.begin(), text.end(), text.begin(), ::toupper);
      settings.text = text;
    } else {
      throw std::runtime_error("Unknown option: " + option);
    }
  }

  if (settings.rotors.empty() || settings.positions.empty() || settings.rings.empty() ||
      settings.reflector.empty() || settings.text.empty()) {
    throw std::runtime_error("Missing a required option. Use --help for usage.");
  }

  if (settings.rotors.size() != NUM_ROTORS || settings.positions.size() != NUM_ROTORS ||
      settings.rings.size() != NUM_ROTORS) {
    throw std::runtime_error("rotors=" + std::to_string(settings.rotors.size()) +
                             ", positions=" + std::to_string(settings.positions.size()) +
                             ", rings=" + std::to_string(settings.rings.size()) +
                             " must all equal " + std::to_string(NUM_ROTORS));
  }
  if (settings.plugboardPairs.size() > MAX_PLUGBOARD_PAIRS) {
    throw std::runtime_error("The plugboard can have at most 10 pairs");
  }
  return settings;
}