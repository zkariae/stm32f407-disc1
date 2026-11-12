// SPDX-License-Identifier: MIT
/*!
****************************
\file       stm32f407-disc1/startup.c
\brief      Reset handler, vector table and default exception handlers.

\author     Z. BEN-AKKA
\version    1.1
\date       12/11/2026

\details
  Shared by every application. On reset:
  1) Enables the FPU (CP10/CP11 full access), before any C code may use it.
  2) Copies .data from flash to RAM and zeroes .bss.
  3) Calls main().
  Exception handlers are weak aliases of Default_Handler, which traps.

\section startup_c_version_history Version history
| Version | Date       | Author      | Description                                  |
|:-------:|:----------:|:------------|:---------------------------------------------|
| 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version, enables the FPU at reset.   |
| 1.1     | 12/11/2026 | Z. BEN-AKKA | Console on USART2: IRQ 38 replaces USART1.   |
****************************
*/

#include <stdint.h>

#include "mcu.h"

// Provided by the linker script
extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss, _estack;

int main(void);

/*!
\brief  Give the CPU full access to the FPU (coprocessors CP10 and CP11).

\details
  The barriers make sure the new access rights are in place before the next
  instruction, which may already be a floating-point one (PM0214 section 4.6.1).
*/
static void fpu_enable(void)
{
  SCB_CPACR.bitfield.cp10 = CPACR_ACCESS_FULL;
  SCB_CPACR.bitfield.cp11 = CPACR_ACCESS_FULL;
  __asm__ volatile("dsb\n\tisb" ::: "memory");
}

/*!
\brief  Entry point after reset: enable the FPU, initialise memory, then run main().
*/
void Reset_Handler(void)
{
  fpu_enable();

  // Copy .data from flash into RAM
  uint32_t *src = &_sidata;
  uint32_t *dst = &_sdata;
  while (dst < &_edata) {
    *dst++ = *src++;
  }

  // Zero .bss
  dst = &_sbss;
  while (dst < &_ebss) {
    *dst++ = 0;
  }

  main();

  while (1) {} // main must never return
}

/*!
\brief  Trap for every exception without its own handler.
*/
void Default_Handler(void)
{
  while (1) {}
}

// Weak aliases: an application or driver definition overrides these.
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USART2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

// Not formatted: clang-format would pack several vectors on one line
// clang-format off
/// Cortex-M vector table, placed first in flash by the linker script.
__attribute__((section(".isr_vector"), used)) void (*const vector_table[])(void) = {
    (void (*)(void))&_estack, // 0x00: initial stack pointer
    Reset_Handler,            // 0x04: reset
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0, // reserved
    0, // reserved
    0, // reserved
    0, // reserved
    SVC_Handler,
    DebugMon_Handler,
    0, // reserved
    PendSV_Handler,
    SysTick_Handler,

    // External interrupt vectors: IRQ 0 through IRQ 38 (STM32F407, RM0090 table 61).
    Default_Handler, // 0: WWDG
    Default_Handler, // 1: PVD
    Default_Handler, // 2: TAMP_STAMP
    Default_Handler, // 3: RTC_WKUP
    Default_Handler, // 4: FLASH
    Default_Handler, // 5: RCC
    Default_Handler, // 6: EXTI0
    Default_Handler, // 7: EXTI1
    Default_Handler, // 8: EXTI2
    Default_Handler, // 9: EXTI3
    Default_Handler, // 10: EXTI4
    Default_Handler, // 11: DMA1_Stream0
    Default_Handler, // 12: DMA1_Stream1
    Default_Handler, // 13: DMA1_Stream2
    Default_Handler, // 14: DMA1_Stream3
    Default_Handler, // 15: DMA1_Stream4
    Default_Handler, // 16: DMA1_Stream5
    Default_Handler, // 17: DMA1_Stream6
    Default_Handler, // 18: ADC
    Default_Handler, // 19: CAN1_TX
    Default_Handler, // 20: CAN1_RX0
    Default_Handler, // 21: CAN1_RX1
    Default_Handler, // 22: CAN1_SCE
    Default_Handler, // 23: EXTI9_5
    Default_Handler, // 24: TIM1_BRK_TIM9
    Default_Handler, // 25: TIM1_UP_TIM10
    Default_Handler, // 26: TIM1_TRG_COM_TIM11
    Default_Handler, // 27: TIM1_CC
    Default_Handler, // 28: TIM2
    Default_Handler, // 29: TIM3
    Default_Handler, // 30: TIM4
    Default_Handler, // 31: I2C1_EV
    Default_Handler, // 32: I2C1_ER
    Default_Handler, // 33: I2C2_EV
    Default_Handler, // 34: I2C2_ER
    Default_Handler, // 35: SPI1
    Default_Handler, // 36: SPI2
    Default_Handler, // 37: USART1
    USART2_IRQHandler, // 38: USART2
};
// clang-format on
