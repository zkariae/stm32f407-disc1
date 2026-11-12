// SPDX-License-Identifier: MIT
/*!
****************************
\file       stm32f407-disc1/board.h
\brief      STM32F407G-DISC1 wiring: clock, LED, user button, console UART pins.

\author     Z. BEN-AKKA
\version    1.0
\date       12/11/2026

\details
  Every board provides board.h; the applications include it. It describes
  the hardware around the MCU (UM1472, MB997 schematic).
  The console UART is USART2 on PA2/PA3: the ST-LINK/V2-A virtual COM port
  is not wired to the MCU by default, so a 3.3 V USB-serial adapter is
  connected to these pins.

\section board_board_h_version_history Version history
| Version | Date       | Author      | Description                                  |
|:-------:|:----------:|:------------|:---------------------------------------------|
| 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version: clock, LD4, B1, USART2.     |
****************************
*/

#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

#include "mcu.h"

/* Board - STM32F407G-DISC1 (UM1472, MB997 schematic) */
#define HSE_HZ 8000000UL ///< External crystal X2.

#define LED_PIN         12 ///< PD12, LD4 green, active high.
#define USER_BUTTON_PIN 0  ///< PA0, B1 blue, high when pressed.
#define CONSOLE_TX_PIN  2  ///< PA2, USART2_TX (AF7), to the USB-serial adapter RX.
#define CONSOLE_RX_PIN  3  ///< PA3, USART2_RX (AF7), from the USB-serial adapter TX.

#endif
