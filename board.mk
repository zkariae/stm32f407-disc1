# SPDX-License-Identifier: MIT
# /*!
# ****************************
# \file       stm32f407-disc1/board.mk
# \brief      STM32F407G-DISC1 build: toolchain, board files, memory and recipes.
#
# \author     Z. BEN-AKKA
# \version    1.0
# \date       12/11/2026
#
# \details
#   Included by the Makefile of the repository root, which declares the
#   applications and calls the recipes below; nothing here depends on them.
#   That Makefile sets BOARD_DIR (this folder) and PROJECT_CFLAGS (warnings,
#   optimisation, include paths). Every board provides the same names:
#     BOARD_SRCS                                  sources linked into every image
#     BOARD_MEMORIES, BOARD_DEFAULT_MEMORY        where an image can live
#     MEMORY_<m>_LDSCRIPT, _ADDR, _OPENOCD        how it is linked and reached
#     board_compile, board_link, board_bin        build recipes, used with $(call)
#     board_flash, board_reset                    programming recipes
#     board_size, board_disasm, board_sections    inspection recipes
#
# \section board_mk_version_history Version history
# | Version | Date       | Author      | Description                                  |
# |:-------:|:----------:|:------------|:---------------------------------------------|
# | 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version, internal flash only.        |
# ****************************
# */

####################################################################
# Toolchain
####################################################################

CROSS   := arm-none-eabi-
CC      := $(CROSS)gcc
OBJCOPY := $(CROSS)objcopy
SIZE    := $(CROSS)size
OBJDUMP := $(CROSS)objdump

# Cortex-M4F. Hard-float ABI with the single-precision FPU: startup.c enables
# the coprocessor before any C code runs.
BOARD_ARCH := -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

# Freestanding code, no C library; one section per function so that the
# link drops what is unused. -I: mcu.h and board.h come from this folder.
BOARD_CFLAGS  := $(BOARD_ARCH) -ffunction-sections -fdata-sections
BOARD_CFLAGS  += -nostdlib -ffreestanding -I$(BOARD_DIR)
BOARD_LDFLAGS := $(BOARD_ARCH) -nostdlib -Wl,--gc-sections

# Linked into every image
BOARD_SRCS := $(BOARD_DIR)/startup.c $(BOARD_DIR)/board.c

####################################################################
# Memories
####################################################################

# Memories an image can be linked into
BOARD_MEMORIES       := flash
BOARD_DEFAULT_MEMORY := flash

# Internal flash, 1 MB, programmed through the on-board ST-LINK/V2-A
MEMORY_flash_LDSCRIPT := $(BOARD_DIR)/linker/internal-flash.ld
MEMORY_flash_ADDR     := 0x08000000
MEMORY_flash_OPENOCD  := -f interface/stlink.cfg -f target/stm32f4x.cfg

BOARD_OPENOCD := $(MEMORY_flash_OPENOCD)

####################################################################
# Recipes, called with $(call board_xxx,arguments)
####################################################################

# Compile one C file. $(1): object, $(2): source, $(3): extra flags
board_compile = $(CC) $(BOARD_CFLAGS) $(PROJECT_CFLAGS) $(3) -c $(2) -o $(1)

# Link objects for a memory. $(1): ELF, $(2): objects, $(3): memory, $(4): map file
board_link = $(CC) $(2) $(BOARD_LDFLAGS) -T $(MEMORY_$(3)_LDSCRIPT) -Wl,-Map=$(4) -o $(1)

# Raw binary from an ELF. $(1): binary, $(2): ELF
board_bin = $(OBJCOPY) -O binary $(2) $(1)

# OpenOCD runs as the user: the udev rule shipped with OpenOCD
# (60-openocd.rules) gives the ST-LINK to the plugdev group, so the user must
# be in plugdev.

# Program a binary into a memory with the ST-LINK. $(1): binary, $(2): memory
board_flash = openocd $(MEMORY_$(2)_OPENOCD) -c "program $(1) $(MEMORY_$(2)_ADDR) verify reset exit"

# Reset the board without reprogramming it
board_reset = openocd $(BOARD_OPENOCD) -c "init; reset; exit"

# Inspect an ELF. $(1): ELF
board_size     = $(SIZE) $(1)
board_disasm   = $(OBJDUMP) -d $(1)
board_sections = $(OBJDUMP) -h $(1)
