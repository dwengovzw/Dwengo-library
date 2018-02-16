# CDC Bootloader for Dwenguino board
Bootloader for the Dwenguino board to easily upload and run sketches with Arduino IDE.

## Features and use
TODO
also see https://github.com/aweatherguy/Katiana/blob/master/Katiana-Bootloader.pdf

## Build instructions
- download the [LUFA library - v170418](https://github.com/abcminiuser/lufa/releases/tag/LUFA-170418) in this folder
- make sure arduino IDE is installed, download from [arduino.cc](https://www.arduino.cc/en/Main/Software)
- install Srecord `brew install Srecord` or `sudo apt-get install srecord`
- run `make` to build bootloader, Welcome sketch and combine both into a final bootloader hex file
- run `make package` to create a final zip package for arduino IDE board manager

Current version built with 
- make v 3.81
CrossPack 2013-12-16:
- avr-gcc: 4.8.1
- avrdude 6.3

Warning: known issues with avr-gcc 7.3, use latest CrossPack instead


## Credits
The bootloader code is developed by [Jelle Roets](mailto:jelle@dwengo.org) commisioned by [Dwengo vzw](http://www.dwengo.org/).

This bootloader is based on the [Katiana bootloader](https://github.com/aweatherguy/Katiana).
Which on his turn is based on the Caterina bootloader, as distributed with the Arduino releases starting from 1.8.4.
The Caterina CDC bootloader was originally developed by Dean Camera and distrubeted with the [LUFA library](https://github.com/abcminiuser/lufa).