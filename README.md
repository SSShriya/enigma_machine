# Enigma Machine Command Line Simulator

## Usage
### Required Arguments 
#### Rotors
Takes in 3 rotor names, from left to right. Valid rotors are: I, II, III, IV, V \
`--rotors I II III`

#### Positiona
Takes in 3 rotor starting positions as letters A - Z \
`--positions A B C`

#### Rings
Takes in 3 rotor ring settings as numbers 1-26 \
`--rings 1 2 3`

#### Reflector
Takes in 1 reflector name. Valid reflector names: A, B, C \
`--reflector B`

#### Text
Takes in the text to encrypt/decrypt \
`--text "Hello World"`

### Optional Arguments
#### Plugboard
Takes in pairs of characters for plugboard letter swapping. \
`--plugboard AB CD EF GH`

### Example
`make run 'ARGS=--rotors I II III --positions A A A --rings 1 1 1 --reflector B --plugboard AV BS CG --text "HELLO WORLD"'`
