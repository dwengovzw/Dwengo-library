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
#
# not set here (see Descriptors.c), but VID is 0x14BF and PID is 0x9207
# the boot loader reports this and the sketch will report PID of 0x9208
# so the boot loader shows up on a different COM port than the sketch.
#
# USB hdwr serial numbers are 10 bytes or 20 nibbles, and each nibble
# is encoded as a 16-bit ASCII unicode character. Suggest only hex chars be used.
# See LUFA/Drivers/USB/Core/AVR8/Device_AVR8.h for the function we need to 
# override to implement this. Also see DeviceStandardReq.c function USB_Device_GetInternalSerialDescriptor.
# this is not defined, or called from USB_Device_GetDescriptor based on this macro:
#	!defined(NO_INTERNAL_SERIAL) && (USE_INTERNAL_SERIAL != NO_DESCRIPTOR)
# to insert our own function we must edit DeviceStandardReq.c and modify USB_Device_GetInternalSerialDescriptor
# to call our own serial number extraction function. Easiest thing perhaps is store 20 hex chars in 40 bytes
# of eeprom as unicode and just copy out.
# Summary edits: 
#   Add CUSTOM_USB_SERIAL to LUFAConfig.h
#   DeviceStandardReq.c:    Enable code in two places
#   Device_AVR8.h	    Enable declaration of USB_Device_GetSerialString based on CUSTOM_USB_SERIAL
#			    Remove static inline from declaration.
#
# =========== USER EDIT SECTION ====================
#
MCU          = atmega32u4
ARCH         = AVR8
BOARD        = USER
F_CPU        = 8000000
F_USB        = 8000000
#
# Flash and bootloader section sizes of the target, in KB. 
# There's no easy way to import these into this makefile based on the MCU macro,
# so they must be manually set to match the selected MCU's flash size and also
# the boot sector size programmed into the MCU's fuses.
#
FLASH_SIZE_KB		= 32
BOOT_SECTION_SIZE_KB	= 4
OPTIMIZATION		= s
TARGET			= Katiana
SRC			= $(TARGET).c Descriptors.c SignatureTable.S $(LUFA_SRC_USB)
#
# ============== END USER EDIT SECTION ==================
#
# not normally required to edit lines below this point
#
LUFA_PATH    = ../../LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/ -DBOOT_START_ADDR=$(BOOT_START_OFFSET)
LD_FLAGS     = -Wl,--section-start=.text=$(BOOT_START_OFFSET) $(BOOT_API_LD_FLAGS)
# Bootloader address calculation formulas
# Do not modify these macros, but rather modify the dependent values above.
CALC_ADDRESS_IN_HEX   = $(shell printf "0x%X" $$(( $(1) )) )
BOOT_START_OFFSET     = $(call CALC_ADDRESS_IN_HEX, ($(FLASH_SIZE_KB) - $(BOOT_SECTION_SIZE_KB)) * 1024 )
BOOT_SEC_OFFSET       = $(call CALC_ADDRESS_IN_HEX, ($(FLASH_SIZE_KB) * 1024) - ($(strip $(1))) )

# Bootloader linker section flags for relocating the API table sections to
# known FLASH addresses - these should not normally be user-edited.
BOOT_SECTION_LD_FLAG  = -Wl,--section-start=$(strip $(1))=$(call BOOT_SEC_OFFSET, $(3)) -Wl,--undefined=$(strip $(2))
BOOT_API_LD_FLAGS    += $(call BOOT_SECTION_LD_FLAG, .apitable_signatures,  BootloaderAPI_Signatures,  8)

# Default target
all:

all:	CheckUsbSerial

clean:	RemoveUsbSerialCheck

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

CheckUsbSerial: ValidateUsbHdwrSerial.c
	cc ValidateUsbHdwrSerial.c -o TmpExec.exe
	./TmpExec.exe
	@rm -f TmpExec.exe

RemoveUsbSerialCheck:
	rm -f TmpExec.exe
