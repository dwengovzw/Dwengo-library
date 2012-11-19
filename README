Requeriments
============
To use the Dwengo library to compile your programs and program your Dwengo board
you need to have the Microchip propietary software tools mcc18, mplink and
pk2cmd. If you also want to generate dwngo.lib from sources you need mplab as
well.

In order for the script ./configure to work correctly, you have to define the
following varibales :
* MCC_BIN
The PATH wherein mcc18 and mplib can be found
* MCC_INCLUDE
The PATH wherein p18f4550.h can be found
* PK2_BIN
The PATH wherein pk2cmd can be found

You can do this before calling the configure script with commands like

export MCC_BIN=path/to/mcc18/folder
export MCC_INCLUDE=path/to/p18f4550/folder
export PK2_BIN=path/to/pk2cmd/folder

Compiling and Programming your projects
========================================
You will find the following three files in the folder ${prefix}/share/libdwengo

* common.mk
 Contains all the variables required to compile and program the board (this values are generated at configure time).
Makefile.topprojdir : A file that can be placed in the folder containing all the project folders (common.mk should be placed there too) and renamed to Makefile
Makefile.proj : A file that can be placed in the folder of **each** project and renamed to Makefile.

So a possible tree structure looks like

projects/
|-- common.mk
|-- Makefile                 # This was Makefile.topprojdir
|-- blinkingLEDS/
     |-- blinkingLEDS.c
     `-- Makefile                 # This was Makefile.proj
|-- newsTicker/
     |-- newsTicker.c
     `-- Makefile                 # This was Makefile.proj
|-- usartCounter/
     |-- usartCounter.c
     `-- Makefile                 # This was Makefile.proj
|-- lightMeasurement/
     |-- lightMeasurement.c
     `-- Makefile                 # This was Makefile.proj
|-- usartNewsticker/
     |-- usartNewsticker.c
     `-- Makefile                 # This was Makefile.proj
`-- usartEcho/
     |-- usartEcho.c
     `-- Makefile                 # This was Makefile.proj

The files provide, at top level, the commands
make   #defualts to make help
make help
make clean
make all
make PROJ=<dir> all   #(where <dir> is the folder name of one of the projects, explained in "make help")
make PROJ=<dir> program

Inside each project folder they provide the same commands
make   #defaults to make all
make clean
make all
make program

Compiling out of source tree
============================
If you are compiling dwengo.lib from source, you may want to compile it outside
of the foulder with the sources. Lets assume you have the sources at
~/libdwengo-version, to compile in a folder ~/libdwengo-build do the following

cd ~/libdwengo-version
./bootstrap.sh
cd ~/libdwengo-build
< export the required varibles here >
~/libdwengo-version/configure
make

This will make the library inside the folder ~/libdwengo-build and keep
~/libdwengo-version clean.

Additionally, if you want to install to a local folder (recommended) call the
configure script with the option --prefix=path/to/install
If you do so, running

make install

will install the library at path/to/install/lib/libdwengo
The header files will be in path/to/install/include/libdwengo
And the files that help you compile your projects and program the board will be
in path/to/install/share/libdwengo
