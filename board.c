// SPDX-License-Identifier: MIT
/*!
****************************
\file       stm32f407-disc1/board.c
\brief      Board instances of the STM32F407G-DISC1.

\author     Z. BEN-AKKA
\version    1.1
\date       12/11/2026

\details
  Descriptors declared in board.h, passed by the applications to the
  generic drivers of dev/common.

\section board_c_version_history Version history
| Version | Date       | Author      | Description                                  |
|:-------:|:----------:|:------------|:---------------------------------------------|
| 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version, no descriptors yet.         |
| 1.1     | 12/11/2026 | Z. BEN-AKKA | Define the LD4 descriptor.                   |
****************************
*/

#include "board.h"

const Led BOARD_LED_LD4 = {
    .port = GPIOD_BASE,
    .pin = LED_PIN,
    .active_high = 1,
};
