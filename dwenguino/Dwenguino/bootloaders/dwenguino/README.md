# CDC Bootloader for Dwenguino board
Bootloader for the Dwenguino board to easily upload and run sketches with Arduino IDE.

## Features and use
TODO
also see https://github.com/aweatherguy/Katiana/blob/master/Katiana-Bootloader.pdf

## Build instructions
- download the [LUFA library - v170418](https://github.com/abcminiuser/lufa/releases/tag/LUFA-170418) in this folder
- run `make`


## Credits
The bootloader code is developed by [Jelle Roets](mailto:jelle@dwengo.org) commisioned by [Dwengo vzw](http://www.dwengo.org/).

This bootloader is based on the [Katiana bootloader](https://github.com/aweatherguy/Katiana).
Which on his turn is based on the Caterina bootloader, as distributed with the Arduino releases starting from 1.8.4.
The Caterina CDC bootloader was originally developed by Dean Camera and distrubeted with the [LUFA library](https://github.com/abcminiuser/lufa).