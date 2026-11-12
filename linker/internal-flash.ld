/* SPDX-License-Identifier: MIT */
/*!
****************************
\file       linker/internal-flash.ld
\brief      Linker script for images in the internal flash.

\author     Z. BEN-AKKA
\version    1.0
\date       12/11/2026

\details
  STM32F407VG: 1 MB flash at 0x08000000. The RAM is split by use:
  - SRAM, 128 KB at 0x20000000 (SRAM1 112 KB + SRAM2 16 KB): .data and
    .bss, reachable by the CPU and the DMA.
  - CCM, 64 KB at 0x10000000, CPU only: the .ccm section at the bottom,
    neither copied nor zeroed at reset, and the stack, which grows down
    from the top of the CCM and so can never overwrite .data or .bss.
  The link fails if less than _min_stack is left for the stack.

\section internal_flash_ld_version_history Version history
| Version | Date       | Author      | Description                                  |
|:-------:|:----------:|:------------|:---------------------------------------------|
| 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version: data in SRAM, stack in CCM. |
****************************
*/

ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
    SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 128K   /* SRAM1 112K + SRAM2 16K, reached by the DMA */
    CCM   (rw)  : ORIGIN = 0x10000000, LENGTH = 64K    /* CPU only (D-bus), no DMA, no code */
}

_estack = ORIGIN(CCM) + LENGTH(CCM);   /* stack at the top of the CCM, never over .data / .bss */
_min_stack = 16K;

SECTIONS
{
    .isr_vector :
    {
        KEEP(*(.isr_vector))           /* must be first, at 0x08000000 */
    } > FLASH

    .text :
    {
        *(.text*)
        *(.rodata*)
    } > FLASH

    /* Initialised data: stored in flash, addressed in SRAM */
    _sidata = LOADADDR(.data);
    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);                  /* startup.c copies whole words */
        _edata = .;
    } > SRAM AT > FLASH

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);                  /* startup.c zeroes whole words */
        _ebss = .;
    } > SRAM

    /* CPU-only buffers: not loaded, not zeroed, the application initialises them */
    .ccm (NOLOAD) :
    {
        . = ALIGN(8);
        *(.ccm*)
        _eccm = .;
    } > CCM

    ASSERT(_eccm + _min_stack <= _estack, "less than _min_stack left for the stack in CCM")
}
