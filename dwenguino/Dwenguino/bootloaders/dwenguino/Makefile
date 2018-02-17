#
#             LUFA Library
#     Copyright (C) Dean Camera, 2017.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# 
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

# Modified on Dec 23 2017 by Jelle Roets from Dwengo vzw (www.dwengo.org)
# 	Bootloader for BootloaderDwenguino	TODO: automatically make a new bootloader that includes welcome sketch as well!
#
#
# =========== USER EDIT SECTION ====================
#
VERSION		 = 2.2.0

MCU          = at90usb646
ARCH         = AVR8
BOARD        = USER
F_CPU        = 16000000
F_USB        = $(F_CPU)

#
# Flash and bootloader section sizes of the target, in KB. 
# There's no easy way to import these into this makefile based on the MCU macro,
# so they must be manually set to match the selected MCU's flash size and also
# the boot sector size programmed into the MCU's fuses.
#
FLASH_SIZE_KB		 = 64
BOOT_SECTION_SIZE_KB = 4
OPTIMIZATION		 = s
TARGET				 = BootloaderDwenguino
SRC					 = $(TARGET).c Board/Board.c Descriptors.c SignatureTable.S $(LUFA_SRC_USB)

APPINTEGRITY_WORD    = 0x4B4F

# Welcome application
WELCOMEAPP_SRC = ../../libraries/Dwenguino/examples/Welcome/Welcome.ino
COMBINED = $(TARGET)-Welcome
#
# ============== END USER EDIT SECTION ==================
#
# not normally required to edit lines below this point
#
LUFA_PATH    = ./LUFA-170418/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/ -DBOOT_START_ADDR=$(BOOT_START_OFFSET) -DAPPINTEGRITY_WORD=$(APPINTEGRITY_WORD) -DAPPINTEGRITY_ADDRESS=$(APPINTEGRITY_ADDRESS)
LD_FLAGS     = -Wl,--section-start=.text=$(BOOT_START_OFFSET) $(BOOT_API_LD_FLAGS)
# Bootloader address calculation formulas
# Do not modify these macros, but rather modify the dependent values above.
CALC_ADDRESS_IN_HEX   = $(shell printf "0x%X" $$(( $(1) )) )
BOOT_START_OFFSET     = $(call CALC_ADDRESS_IN_HEX, ($(FLASH_SIZE_KB) - $(BOOT_SECTION_SIZE_KB)) * 1024 )
BOOT_SEC_OFFSET       = $(call CALC_ADDRESS_IN_HEX, ($(FLASH_SIZE_KB) * 1024) - ($(strip $(1))) )

FLASH_END_OFFSET      = $(call CALC_ADDRESS_IN_HEX, $(FLASH_SIZE_KB) * 1024 - 1)
APPINTEGRITY_ADDRESS  = $(call CALC_ADDRESS_IN_HEX, ($(FLASH_SIZE_KB) - $(BOOT_SECTION_SIZE_KB)) * 1024 - 2)

# Bootloader linker section flags for relocating the API table sections to
# known FLASH addresses - these should not normally be user-edited.
BOOT_SECTION_LD_FLAG  = -Wl,--section-start=$(strip $(1))=$(call BOOT_SEC_OFFSET, $(3)) -Wl,--undefined=$(strip $(2))
BOOT_API_LD_FLAGS    += $(call BOOT_SECTION_LD_FLAG, .apitable_signatures,  BootloaderAPI_Signatures,  4)

# Welcome application
WELCOME_OBJDIR = $(OBJDIR)/sketch/
WELCOME_HARDWAREDIR = $(OBJDIR)/hardware/
WELCOME_BOARD = dwenguino:avr:Dwenguino
WELCOME_HEX = $(WELCOME_OBJDIR)Welcome.ino

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
ArduinoAppDir = /Applications/Arduino.app/Contents/Java/
endif
ifeq ($(UNAME), Linux)
ArduinoAppDir = ~/Downloads/arduino-1.8.5/
endif

ArduinoBuilder_flags =	-hardware $(ArduinoAppDir)hardware -hardware $(WELCOME_HARDWAREDIR) \
						-tools $(ArduinoAppDir)tools-builder -tools $(ArduinoAppDir)hardware/tools/avr \
						-libraries $(ArduinoAppDir)libraries \
						-fqbn $(WELCOME_BOARD) \
						-build-path $(CURDIR)/$(WELCOME_OBJDIR)

# Combine application with bootloader
APPINTEGRITYgenerator = -gen $(APPINTEGRITY_ADDRESS) $(BOOT_START_OFFSET) -constant-l-e $(APPINTEGRITY_WORD) 2

PackageExludes = "*.DS_Store" "Dwenguino/bootloaders/dwenguino/LUFA*" "Dwenguino/bootloaders/dwenguino/obj/*" \
				 "*/$(TARGET).elf" "*/$(TARGET).bin" "*/$(TARGET).eep" "*/$(TARGET).map" "*/$(TARGET).lss" "*/$(TARGET).sym" "*/lib$(TARGET).a"

# Default target
all: $(COMBINED).hex

clean: cleanCombinedWelcome

welcome: $(WELCOME_HEX).hex

combined: $(COMBINED).hex 

$(WELCOME_HEX).hex: $(WELCOMEAPP_SRC)
	@echo $(MSG_INFO_MESSAGE) Building Welcome sketch with arduino builder
	@if [ ! -d "$(WELCOME_HARDWAREDIR)dwenguino/avr" ]; then \
		mkdir -p $(WELCOME_HARDWAREDIR)dwenguino; \
		ln -s ../../../../.. $(WELCOME_HARDWAREDIR)dwenguino/avr; \
		mkdir -p $(WELCOME_OBJDIR); \
	fi
	$(ArduinoAppDir)arduino-builder $(ArduinoBuilder_flags) $(WELCOMEAPP_SRC)

$(COMBINED).hex: $(WELCOME_HEX).hex $(TARGET).hex 
	@echo $(MSG_INFO_MESSAGE) Combine Bootloader with Welcome app into one hex file
	srec_cat '(' $(WELCOME_HEX).hex -Intel $(TARGET).hex -Intel $(APPINTEGRITYgenerator) ')' -fill 0xff 0x0000 $(FLASH_END_OFFSET) -o $(COMBINED).hex -Intel -address-length=2

cleanCombinedWelcome:
	@echo $(MSG_REMOVE_CMD) Removing output files of Welcome sketch and combined sketch and bootloader
	rm -rf $(WELCOME_OBJDIR) $(WELCOME_HARDWAREDIR)
	rm -f $(COMBINED).hex

package:
	@echo $(MSG_INFO_MESSAGE) Creating final zip-package \(without intermediate build output\)
	rm -f $(WELCOME_HARDWAREDIR)dwenguino/avr
	cd ../../.. && \
	zip -q -FS Dwenguino-$(VERSION).zip -r Dwenguino -x $(PackageExludes) && \
	shasum -a 256 Dwenguino-$(VERSION).zip && \
	wc -c < Dwenguino-$(VERSION).zip

# Include LUFA-specific DMBS extension modules
DMBS_LUFA_PATH ?= $(LUFA_PATH)/Build/LUFA
include $(DMBS_LUFA_PATH)/lufa-sources.mk
include $(DMBS_LUFA_PATH)/lufa-gcc.mk

# Include common DMBS build system modules
DMBS_PATH      ?= $(LUFA_PATH)/Build/DMBS/DMBS
include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/cppcheck.mk
include $(DMBS_PATH)/doxygen.mk
include $(DMBS_PATH)/dfu.mk
include $(DMBS_PATH)/gcc.mk
include $(DMBS_PATH)/hid.mk
include $(DMBS_PATH)/avrdude.mk
include $(DMBS_PATH)/atprogram.mk
