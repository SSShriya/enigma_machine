#include "plugboard.h"
#include "reflector.h"
#include "rotor.h"

class EnigmaMachine {
  public:
    EnigmaMachine(
      Plugboard plugboard, 
      Rotor rightRotor, 
      Rotor middleRotor, 
      Rotor leftRotor, 
      Reflector reflector
    );

    std::string encrypt(std::string text);
  
  private:
    Plugboard plugboard_;
    Rotor rightRotor_;
    Rotor middleRotor_;
    Rotor leftRotor_;
    Reflector reflector_;
    void stepRotors();
};