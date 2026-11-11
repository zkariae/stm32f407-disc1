# stm32f407-disc1

Board support for the **STM32F407G-DISC1** discovery kit (STM32F407VG, Cortex-M4F),
for register-level, bare-metal C firmware: no HAL, no CMSIS.

This repository is the board side of
[stm32f407-disc1-baremetal](https://github.com/zkariae/stm32f407-disc1-baremetal),
where it lives as a Git submodule at `stm32f407-disc1/`. It describes the
hardware and how to build for it and program it; what to build (the
applications, the generic drivers) is in the main repository (`../dev`,
`../Makefile`).

## Contents

| File            | Role                                                                 |
|:----------------|:---------------------------------------------------------------------|
| `mcu.h`         | STM32F407VG registers, bit fields and memory map (RM0090)            |
| `board.h/.c`    | Pins (LEDs LD3-LD6, USER button) and the descriptors passed to the drivers (`BOARD_LED_LD4`, `BOARD_UART_VCP`...) |
| `startup.c`     | Vector table, reset handler, default exception handlers              |
| `linker/`       | Memory layout: internal flash (1 MB), SRAM (128 KB) and CCM (64 KB)  |
| `board.mk`      | Toolchain, board sources, memories and the recipes the root Makefile calls: `board_compile`, `board_link`, `board_flash`, `board_reset`... |
| `.clang-format` | Code style; keep identical to `dev/.clang-format`                    |

## Contract with dev/

Every board provides `mcu.h` (registers, included by the drivers),
`board.h` (wiring, included by the applications) and `board.mk` (build
description, included by the root Makefile). A new board is a new
repository with the same three files.

## References

- RM0090: STM32F405/415, STM32F407/417, STM32F427/437 and STM32F429/439 reference manual
- UM1472: Discovery kit with STM32F407VG MCU, user manual (MB997 schematic)
- DS8626: STM32F405xx/STM32F407xx datasheet (pinout, alternate functions)

## Usage

From the root of the main repository:

```sh
git submodule update --init      # after cloning the main repository
make 01-blink-flash             # build and flash with the ST-LINK
make help                       # every target
```

## License

MIT, see [LICENSE](LICENSE).
