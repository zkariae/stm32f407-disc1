// SPDX-License-Identifier: MIT
/*!
****************************
\file       stm32f407-disc1/mcu.h
\brief      STM32F407VG registers, bit fields and memory map.

\author     Z. BEN-AKKA
\version    1.0
\date       11/11/2026

\details
  Every board provides mcu.h; the drivers of dev/common include it. Addresses,
  offsets and bit positions follow RM0090 (STM32F405/407 reference manual) and
  PM0214 (Cortex-M4 programming manual). Only the STM32F407 is described: bits
  that exist only on the STM32F42x/43x are reserved fields.
  - Registers with named fields are bit-field structures, accessed as lvalues:
      RCC_AHB1ENR.bitfield.gpioaen = 1;
      GPIO_MODER(GPIOA_BASE).bitfield.moder5 = GPIO_MODE_OUTPUT;
  - Registers without fields are plain 32-bit lvalues: GPIO_ODR(GPIOD_BASE).
  - GPIO and USART macros take the instance base address (GPIOx_BASE, USARTx_BASE).
  - Registers marked WARNING must be written as a whole through .raw.

\section mcu_h_version_history Version history
| Version | Date       | Author      | Description                                 |
|:-------:|:----------:|:------------|:--------------------------------------------|
| 1.0     | 11/11/2026 | Z. BEN-AKKA | Initial version, bit-field register types.  |
****************************
*/

#ifndef MCU_H
#define MCU_H

#include <stdint.h>

/// Memory-mapped 32-bit register at a fixed address.
#define REG(addr) (*(volatile uint32_t *)(addr))

/// Memory-mapped register of a given bit-field type at a fixed address.
#define REG_T(type, addr) (*(volatile type *)(addr))

/* Memory map - RM0090 section 2.3 */
#define FLASH_MEM_BASE 0x08000000UL ///< Internal flash, 1 MB.
#define SYSMEM_BASE    0x1FFF0000UL ///< ST ROM bootloader (AN2606).
#define CCM_BASE       0x10000000UL ///< Core-coupled memory, 64 KB, CPU only.
#define CCM_END        0x10010000UL ///< First address past the CCM.
#define SRAM1_BASE     0x20000000UL ///< SRAM1, 112 KB.
#define SRAM2_BASE     0x2001C000UL ///< SRAM2, 16 KB.
#define RAM_BASE       0x20000000UL ///< SRAM1 + SRAM2, 128 KB contiguous.
#define RAM_END        0x20020000UL ///< First address past the SRAM.
#define HSI_HZ         16000000U    ///< Internal oscillator, clock source at reset.

/* ======================================================================== */
/* RCC - RM0090 section 7.3                                                 */
/* ======================================================================== */
#define RCC_BASE 0x40023800UL

/**
 * @brief RCC_CR (offset 0x00) - Reset value 0x0000 XX83
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t hsion : 1;     /* bit 0       */
      uint32_t hsirdy : 1;    /* bit 1       */
      uint32_t reserved0 : 1; /* bit 2       */
      uint32_t hsitrim : 5;   /* bits 3-7    */

      uint32_t hsical : 8; /* bits 8-15   */
      uint32_t hseon : 1;  /* bit 16      */
      uint32_t hserdy : 1; /* bit 17      */
      uint32_t hsebyp : 1; /* bit 18      */

      uint32_t csson : 1;     /* bit 19      */
      uint32_t reserved1 : 4; /* bits 20-23  */
      uint32_t pllon : 1;     /* bit 24      */
      uint32_t pllrdy : 1;    /* bit 25      */

      uint32_t plli2son : 1;  /* bit 26      */
      uint32_t plli2srdy : 1; /* bit 27      */
      uint32_t reserved2 : 4; /* bits 28-31: PLLSAI on F42x/43x only */
    } bitfield;
  };
} T_rcc_cr;

_Static_assert(sizeof(T_rcc_cr) == 4, "T_rcc_cr must be 32 bits");

/**
 * @brief RCC_PLLCFGR (offset 0x04) - Reset value 0x2400 3010
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t pllm : 6;      /* bits 0-5    */
      uint32_t plln : 9;      /* bits 6-14   */
      uint32_t reserved0 : 1; /* bit 15      */
      uint32_t pllp : 2;      /* bits 16-17  */

      uint32_t reserved1 : 4; /* bits 18-21  */
      uint32_t pllsrc : 1;    /* bit 22      */
      uint32_t reserved2 : 1; /* bit 23      */
      uint32_t pllq : 4;      /* bits 24-27  */

      uint32_t reserved3 : 4; /* bits 28-31  */
    } bitfield;
  };
} T_rcc_pllcfgr;

_Static_assert(sizeof(T_rcc_pllcfgr) == 4, "T_rcc_pllcfgr must be 32 bits");

/**
 * @brief RCC_CFGR (offset 0x08) - Reset value 0x0000 0000
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t sw : 2;        /* bits 0-1    */
      uint32_t sws : 2;       /* bits 2-3    */
      uint32_t hpre : 4;      /* bits 4-7    */
      uint32_t reserved0 : 2; /* bits 8-9    */

      uint32_t ppre1 : 3;  /* bits 10-12  */
      uint32_t ppre2 : 3;  /* bits 13-15  */
      uint32_t rtcpre : 5; /* bits 16-20  */
      uint32_t mco1 : 2;   /* bits 21-22  */

      uint32_t i2ssrc : 1;  /* bit 23      */
      uint32_t mco1pre : 3; /* bits 24-26  */
      uint32_t mco2pre : 3; /* bits 27-29  */
      uint32_t mco2 : 2;    /* bits 30-31  */
    } bitfield;
  };
} T_rcc_cfgr;

_Static_assert(sizeof(T_rcc_cfgr) == 4, "T_rcc_cfgr must be 32 bits");

/**
 * @brief RCC_AHB1ENR (offset 0x30) - Reset value 0x0010 0000
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t gpioaen : 1; /* bit 0       */
      uint32_t gpioben : 1; /* bit 1       */
      uint32_t gpiocen : 1; /* bit 2       */
      uint32_t gpioden : 1; /* bit 3       */

      uint32_t gpioeen : 1; /* bit 4       */
      uint32_t gpiofen : 1; /* bit 5       */
      uint32_t gpiogen : 1; /* bit 6       */
      uint32_t gpiohen : 1; /* bit 7       */

      uint32_t gpioien : 1;   /* bit 8       */
      uint32_t reserved0 : 3; /* bits 9-11: GPIOJ/GPIOK on F42x/43x only */
      uint32_t crcen : 1;     /* bit 12      */
      uint32_t reserved1 : 5; /* bits 13-17  */

      uint32_t bkpsramen : 1;    /* bit 18      */
      uint32_t reserved2 : 1;    /* bit 19      */
      uint32_t ccmdataramen : 1; /* bit 20      */
      uint32_t dma1en : 1;       /* bit 21      */

      uint32_t dma2en : 1;     /* bit 22      */
      uint32_t reserved3 : 2;  /* bits 23-24: DMA2D on F42x/43x only */
      uint32_t ethmacen : 1;   /* bit 25      */
      uint32_t ethmactxen : 1; /* bit 26      */

      uint32_t ethmacrxen : 1;  /* bit 27      */
      uint32_t ethmacptpen : 1; /* bit 28      */
      uint32_t otghsen : 1;     /* bit 29      */
      uint32_t otghsulpien : 1; /* bit 30      */

      uint32_t reserved4 : 1; /* bit 31      */
    } bitfield;
  };
} T_rcc_ahb1enr;

_Static_assert(sizeof(T_rcc_ahb1enr) == 4, "T_rcc_ahb1enr must be 32 bits");

/**
 * @brief RCC_APB1ENR (offset 0x40) - Reset value 0x0000 0000
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t tim2en : 1; /* bit 0       */
      uint32_t tim3en : 1; /* bit 1       */
      uint32_t tim4en : 1; /* bit 2       */
      uint32_t tim5en : 1; /* bit 3       */

      uint32_t tim6en : 1;  /* bit 4       */
      uint32_t tim7en : 1;  /* bit 5       */
      uint32_t tim12en : 1; /* bit 6       */
      uint32_t tim13en : 1; /* bit 7       */

      uint32_t tim14en : 1;   /* bit 8       */
      uint32_t reserved0 : 2; /* bits 9-10   */
      uint32_t wwdgen : 1;    /* bit 11      */
      uint32_t reserved1 : 2; /* bits 12-13  */

      uint32_t spi2en : 1;    /* bit 14      */
      uint32_t spi3en : 1;    /* bit 15      */
      uint32_t reserved2 : 1; /* bit 16      */
      uint32_t usart2en : 1;  /* bit 17      */

      uint32_t usart3en : 1; /* bit 18      */
      uint32_t uart4en : 1;  /* bit 19      */
      uint32_t uart5en : 1;  /* bit 20      */
      uint32_t i2c1en : 1;   /* bit 21      */

      uint32_t i2c2en : 1;    /* bit 22      */
      uint32_t i2c3en : 1;    /* bit 23      */
      uint32_t reserved3 : 1; /* bit 24      */
      uint32_t can1en : 1;    /* bit 25      */

      uint32_t can2en : 1;    /* bit 26      */
      uint32_t reserved4 : 1; /* bit 27      */
      uint32_t pwren : 1;     /* bit 28      */
      uint32_t dacen : 1;     /* bit 29      */

      uint32_t reserved5 : 2; /* bits 30-31: UART7/UART8 on F42x/43x only */
    } bitfield;
  };
} T_rcc_apb1enr;

_Static_assert(sizeof(T_rcc_apb1enr) == 4, "T_rcc_apb1enr must be 32 bits");

/**
 * @brief RCC_APB2ENR (offset 0x44) - Reset value 0x0000 0000
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t tim1en : 1;    /* bit 0       */
      uint32_t tim8en : 1;    /* bit 1       */
      uint32_t reserved0 : 2; /* bits 2-3    */
      uint32_t usart1en : 1;  /* bit 4       */

      uint32_t usart6en : 1;  /* bit 5       */
      uint32_t reserved1 : 2; /* bits 6-7    */
      uint32_t adc1en : 1;    /* bit 8       */
      uint32_t adc2en : 1;    /* bit 9       */

      uint32_t adc3en : 1;    /* bit 10      */
      uint32_t sdioen : 1;    /* bit 11      */
      uint32_t spi1en : 1;    /* bit 12      */
      uint32_t reserved2 : 1; /* bit 13: SPI4 on F42x/43x only */

      uint32_t syscfgen : 1;  /* bit 14      */
      uint32_t reserved3 : 1; /* bit 15      */
      uint32_t tim9en : 1;    /* bit 16      */
      uint32_t tim10en : 1;   /* bit 17      */

      uint32_t tim11en : 1;    /* bit 18      */
      uint32_t reserved4 : 13; /* bits 19-31: SPI5/SPI6/SAI1/LTDC on F42x/43x only */
    } bitfield;
  };
} T_rcc_apb2enr;

_Static_assert(sizeof(T_rcc_apb2enr) == 4, "T_rcc_apb2enr must be 32 bits");

/**
 * @brief RCC_CSR (offset 0x74) - Reset value 0x0E00 0000
 * @details Reset flags are set by hardware and kept across resets until RMVF
 *          clears them. Clear them with a whole write through .raw.
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t lsion : 1;      /* bit 0       */
      uint32_t lsirdy : 1;     /* bit 1       */
      uint32_t reserved0 : 22; /* bits 2-23   */
      uint32_t rmvf : 1;       /* bit 24: WARNING: write 1 clears every reset flag */

      uint32_t borrstf : 1; /* bit 25      */
      uint32_t pinrstf : 1; /* bit 26      */
      uint32_t porrstf : 1; /* bit 27      */
      uint32_t sftrstf : 1; /* bit 28      */

      uint32_t iwdgrstf : 1; /* bit 29      */
      uint32_t wwdgrstf : 1; /* bit 30      */
      uint32_t lpwrrstf : 1; /* bit 31      */
    } bitfield;
  };
} T_rcc_csr;

_Static_assert(sizeof(T_rcc_csr) == 4, "T_rcc_csr must be 32 bits");

#define RCC_CR      REG_T(T_rcc_cr, RCC_BASE + 0x00U)
#define RCC_PLLCFGR REG_T(T_rcc_pllcfgr, RCC_BASE + 0x04U)
#define RCC_CFGR    REG_T(T_rcc_cfgr, RCC_BASE + 0x08U)
#define RCC_AHB1ENR REG_T(T_rcc_ahb1enr, RCC_BASE + 0x30U)
#define RCC_APB1ENR REG_T(T_rcc_apb1enr, RCC_BASE + 0x40U)
#define RCC_APB2ENR REG_T(T_rcc_apb2enr, RCC_BASE + 0x44U)
#define RCC_CSR     REG_T(T_rcc_csr, RCC_BASE + 0x74U) ///< WARNING: write through .raw.

/// RCC_CFGR SW / SWS: system clock source.
#define RCC_SYSCLK_HSI 0U
#define RCC_SYSCLK_HSE 1U
#define RCC_SYSCLK_PLL 2U

/// RCC_PLLCFGR PLLSRC: PLL input.
#define RCC_PLLSRC_HSI 0U
#define RCC_PLLSRC_HSE 1U

/// RCC_PLLCFGR PLLP: main PLL output division.
#define RCC_PLLP_DIV2 0U
#define RCC_PLLP_DIV4 1U
#define RCC_PLLP_DIV6 2U
#define RCC_PLLP_DIV8 3U

/// RCC_CFGR HPRE: AHB prescaler (values 0 to 7: not divided).
#define RCC_HPRE_DIV1 0U

/// RCC_CFGR PPRE1 / PPRE2: APB prescalers (values 0 to 3: not divided).
#define RCC_PPRE_DIV1 0U
#define RCC_PPRE_DIV2 4U
#define RCC_PPRE_DIV4 5U

/* ======================================================================== */
/* PWR - RM0090 section 5.4                                                 */
/* ======================================================================== */
#define PWR_BASE 0x40007000UL

/**
 * @brief PWR_CR (offset 0x00) - Reset value 0x0000 4000
 * @details CWUF and CSBF are rc_w1: write the register through .raw to clear them.
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t lpds : 1; /* bit 0       */
      uint32_t pdds : 1; /* bit 1       */
      uint32_t cwuf : 1; /* bit 2: WARNING: rc_w1, write 1 clears WUF */
      uint32_t csbf : 1; /* bit 3: WARNING: rc_w1, write 1 clears SBF */

      uint32_t pvde : 1; /* bit 4       */
      uint32_t pls : 3;  /* bits 5-7    */
      uint32_t dbp : 1;  /* bit 8       */
      uint32_t fpds : 1; /* bit 9       */

      uint32_t reserved0 : 4;  /* bits 10-13: LPLVDS/MRLVDS/ADCDC1 on F42x/43x only */
      uint32_t vos : 1;        /* bit 14      */
      uint32_t reserved1 : 17; /* bits 15-31: VOS[1]/ODEN/ODSWEN/UDEN on F42x/43x only */
    } bitfield;
  };
} T_pwr_cr;

_Static_assert(sizeof(T_pwr_cr) == 4, "T_pwr_cr must be 32 bits");

/**
 * @brief PWR_CSR (offset 0x04) - Reset value 0x0000 0000
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t wuf : 1;  /* bit 0       */
      uint32_t sbf : 1;  /* bit 1       */
      uint32_t pvdo : 1; /* bit 2       */
      uint32_t brr : 1;  /* bit 3       */

      uint32_t reserved0 : 4; /* bits 4-7    */
      uint32_t ewup : 1;      /* bit 8       */
      uint32_t bre : 1;       /* bit 9       */
      uint32_t reserved1 : 4; /* bits 10-13  */

      uint32_t vosrdy : 1;     /* bit 14      */
      uint32_t reserved2 : 17; /* bits 15-31: ODRDY/ODSWRDY/UDRDY on F42x/43x only */
    } bitfield;
  };
} T_pwr_csr;

_Static_assert(sizeof(T_pwr_csr) == 4, "T_pwr_csr must be 32 bits");

#define PWR_CR  REG_T(T_pwr_cr, PWR_BASE + 0x00U) ///< WARNING: CWUF/CSBF, see T_pwr_cr.
#define PWR_CSR REG_T(T_pwr_csr, PWR_BASE + 0x04U)

/// PWR_CR VOS: regulator voltage scale.
#define PWR_VOS_SCALE2 0U ///< HCLK up to 144 MHz.
#define PWR_VOS_SCALE1 1U ///< HCLK up to 168 MHz, reset value.

/* ======================================================================== */
/* FLASH interface - RM0090 section 3.9                                     */
/* ======================================================================== */
#define FLASH_R_BASE 0x40023C00UL

/**
 * @brief FLASH_ACR (offset 0x00) - Reset value 0x0000 0000
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t latency : 3;   /* bits 0-2    */
      uint32_t reserved0 : 5; /* bits 3-7: LATENCY[3] on F42x/43x only */
      uint32_t prften : 1;    /* bit 8       */
      uint32_t icen : 1;      /* bit 9       */

      uint32_t dcen : 1;       /* bit 10      */
      uint32_t icrst : 1;      /* bit 11      */
      uint32_t dcrst : 1;      /* bit 12      */
      uint32_t reserved1 : 19; /* bits 13-31  */
    } bitfield;
  };
} T_flash_acr;

_Static_assert(sizeof(T_flash_acr) == 4, "T_flash_acr must be 32 bits");

#define FLASH_ACR REG_T(T_flash_acr, FLASH_R_BASE + 0x00U)

/// FLASH_ACR LATENCY: wait states (RM0090 table 10, 2.7-3.6 V).
#define FLASH_LATENCY_0WS 0U ///< HCLK up to 30 MHz.
#define FLASH_LATENCY_5WS 5U ///< HCLK up to 168 MHz.

/* ======================================================================== */
/* GPIO - RM0090 section 8.4. Ports are 0x400 apart from 0x40020000.        */
/* ======================================================================== */
#define GPIOA_BASE 0x40020000UL
#define GPIOB_BASE 0x40020400UL
#define GPIOC_BASE 0x40020800UL
#define GPIOD_BASE 0x40020C00UL
#define GPIOE_BASE 0x40021000UL
#define GPIOF_BASE 0x40021400UL
#define GPIOG_BASE 0x40021800UL
#define GPIOH_BASE 0x40021C00UL
#define GPIOI_BASE 0x40022000UL

/// Port base address from a port index, for tables storing ports as one byte.
#define GPIO_PORT(n) (GPIOA_BASE + 0x400UL * (n))
#define PORT_A       0
#define PORT_B       1
#define PORT_C       2
#define PORT_D       3
#define PORT_E       4
#define PORT_F       5
#define PORT_G       6
#define PORT_H       7
#define PORT_I       8

/**
 * @brief GPIOx_MODER (offset 0x00) - Reset value 0xA800 0000 (port A),
 *        0x0000 0280 (port B), 0x0000 0000 (other ports)
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t moder0 : 2; /* bits 0-1    */
      uint32_t moder1 : 2; /* bits 2-3    */
      uint32_t moder2 : 2; /* bits 4-5    */
      uint32_t moder3 : 2; /* bits 6-7    */

      uint32_t moder4 : 2; /* bits 8-9    */
      uint32_t moder5 : 2; /* bits 10-11  */
      uint32_t moder6 : 2; /* bits 12-13  */
      uint32_t moder7 : 2; /* bits 14-15  */

      uint32_t moder8 : 2;  /* bits 16-17  */
      uint32_t moder9 : 2;  /* bits 18-19  */
      uint32_t moder10 : 2; /* bits 20-21  */
      uint32_t moder11 : 2; /* bits 22-23  */

      uint32_t moder12 : 2; /* bits 24-25  */
      uint32_t moder13 : 2; /* bits 26-27  */
      uint32_t moder14 : 2; /* bits 28-29  */
      uint32_t moder15 : 2; /* bits 30-31  */
    } bitfield;
  };
} T_gpio_moder;

_Static_assert(sizeof(T_gpio_moder) == 4, "T_gpio_moder must be 32 bits");

/**
 * @brief GPIOx_OTYPER (offset 0x04) - Reset value 0x0000 0000
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t ot0 : 1; /* bit 0       */
      uint32_t ot1 : 1; /* bit 1       */
      uint32_t ot2 : 1; /* bit 2       */
      uint32_t ot3 : 1; /* bit 3       */

      uint32_t ot4 : 1; /* bit 4       */
      uint32_t ot5 : 1; /* bit 5       */
      uint32_t ot6 : 1; /* bit 6       */
      uint32_t ot7 : 1; /* bit 7       */

      uint32_t ot8 : 1;  /* bit 8       */
      uint32_t ot9 : 1;  /* bit 9       */
      uint32_t ot10 : 1; /* bit 10      */
      uint32_t ot11 : 1; /* bit 11      */

      uint32_t ot12 : 1; /* bit 12      */
      uint32_t ot13 : 1; /* bit 13      */
      uint32_t ot14 : 1; /* bit 14      */
      uint32_t ot15 : 1; /* bit 15      */

      uint32_t reserved0 : 16; /* bits 16-31  */
    } bitfield;
  };
} T_gpio_otyper;

_Static_assert(sizeof(T_gpio_otyper) == 4, "T_gpio_otyper must be 32 bits");

/**
 * @brief GPIOx_OSPEEDR (offset 0x08) - Reset value 0x0000 00C0 (port B),
 *        0x0000 0000 (other ports)
 */
/* TODO: vérifier RM0090 §8.4.3 (reset value of port A) */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t ospeedr0 : 2; /* bits 0-1    */
      uint32_t ospeedr1 : 2; /* bits 2-3    */
      uint32_t ospeedr2 : 2; /* bits 4-5    */
      uint32_t ospeedr3 : 2; /* bits 6-7    */

      uint32_t ospeedr4 : 2; /* bits 8-9    */
      uint32_t ospeedr5 : 2; /* bits 10-11  */
      uint32_t ospeedr6 : 2; /* bits 12-13  */
      uint32_t ospeedr7 : 2; /* bits 14-15  */

      uint32_t ospeedr8 : 2;  /* bits 16-17  */
      uint32_t ospeedr9 : 2;  /* bits 18-19  */
      uint32_t ospeedr10 : 2; /* bits 20-21  */
      uint32_t ospeedr11 : 2; /* bits 22-23  */

      uint32_t ospeedr12 : 2; /* bits 24-25  */
      uint32_t ospeedr13 : 2; /* bits 26-27  */
      uint32_t ospeedr14 : 2; /* bits 28-29  */
      uint32_t ospeedr15 : 2; /* bits 30-31  */
    } bitfield;
  };
} T_gpio_ospeedr;

_Static_assert(sizeof(T_gpio_ospeedr) == 4, "T_gpio_ospeedr must be 32 bits");

/**
 * @brief GPIOx_PUPDR (offset 0x0C) - Reset value 0x6400 0000 (port A),
 *        0x0000 0100 (port B), 0x0000 0000 (other ports)
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t pupdr0 : 2; /* bits 0-1    */
      uint32_t pupdr1 : 2; /* bits 2-3    */
      uint32_t pupdr2 : 2; /* bits 4-5    */
      uint32_t pupdr3 : 2; /* bits 6-7    */

      uint32_t pupdr4 : 2; /* bits 8-9    */
      uint32_t pupdr5 : 2; /* bits 10-11  */
      uint32_t pupdr6 : 2; /* bits 12-13  */
      uint32_t pupdr7 : 2; /* bits 14-15  */

      uint32_t pupdr8 : 2;  /* bits 16-17  */
      uint32_t pupdr9 : 2;  /* bits 18-19  */
      uint32_t pupdr10 : 2; /* bits 20-21  */
      uint32_t pupdr11 : 2; /* bits 22-23  */

      uint32_t pupdr12 : 2; /* bits 24-25  */
      uint32_t pupdr13 : 2; /* bits 26-27  */
      uint32_t pupdr14 : 2; /* bits 28-29  */
      uint32_t pupdr15 : 2; /* bits 30-31  */
    } bitfield;
  };
} T_gpio_pupdr;

_Static_assert(sizeof(T_gpio_pupdr) == 4, "T_gpio_pupdr must be 32 bits");

/**
 * @brief GPIOx_AFRL (offset 0x20) - Reset value 0x0000 0000
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t afrl0 : 4; /* bits 0-3    */
      uint32_t afrl1 : 4; /* bits 4-7    */
      uint32_t afrl2 : 4; /* bits 8-11   */
      uint32_t afrl3 : 4; /* bits 12-15  */

      uint32_t afrl4 : 4; /* bits 16-19  */
      uint32_t afrl5 : 4; /* bits 20-23  */
      uint32_t afrl6 : 4; /* bits 24-27  */
      uint32_t afrl7 : 4; /* bits 28-31  */
    } bitfield;
  };
} T_gpio_afrl;

_Static_assert(sizeof(T_gpio_afrl) == 4, "T_gpio_afrl must be 32 bits");

/**
 * @brief GPIOx_AFRH (offset 0x24) - Reset value 0x0000 0000
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t afrh8 : 4;  /* bits 0-3    */
      uint32_t afrh9 : 4;  /* bits 4-7    */
      uint32_t afrh10 : 4; /* bits 8-11   */
      uint32_t afrh11 : 4; /* bits 12-15  */

      uint32_t afrh12 : 4; /* bits 16-19  */
      uint32_t afrh13 : 4; /* bits 20-23  */
      uint32_t afrh14 : 4; /* bits 24-27  */
      uint32_t afrh15 : 4; /* bits 28-31  */
    } bitfield;
  };
} T_gpio_afrh;

_Static_assert(sizeof(T_gpio_afrh) == 4, "T_gpio_afrh must be 32 bits");

#define GPIO_MODER(port)   REG_T(T_gpio_moder, (port) + 0x00U)
#define GPIO_OTYPER(port)  REG_T(T_gpio_otyper, (port) + 0x04U)
#define GPIO_OSPEEDR(port) REG_T(T_gpio_ospeedr, (port) + 0x08U)
#define GPIO_PUPDR(port)   REG_T(T_gpio_pupdr, (port) + 0x0CU)
#define GPIO_IDR(port)     REG((port) + 0x10U) ///< Input data, bits 0-15, read-only.
#define GPIO_ODR(port)     REG((port) + 0x14U) ///< Output data, bits 0-15.
/// WARNING: write-only. Atomic set (bits 0-15) / reset (bits 16-31); never read-modify-write.
#define GPIO_BSRR(port) REG((port) + 0x18U)
#define GPIO_AFRL(port) REG_T(T_gpio_afrl, (port) + 0x20U)
#define GPIO_AFRH(port) REG_T(T_gpio_afrh, (port) + 0x24U)

/// GPIOx_MODER field values.
#define GPIO_MODE_INPUT  0U
#define GPIO_MODE_OUTPUT 1U
#define GPIO_MODE_AF     2U
#define GPIO_MODE_ANALOG 3U

/// GPIOx_OTYPER field values.
#define GPIO_OTYPE_PUSH_PULL  0U
#define GPIO_OTYPE_OPEN_DRAIN 1U

/// GPIOx_OSPEEDR field values.
#define GPIO_SPEED_LOW       0U
#define GPIO_SPEED_MEDIUM    1U
#define GPIO_SPEED_HIGH      2U
#define GPIO_SPEED_VERY_HIGH 3U

/// GPIOx_PUPDR field values.
#define GPIO_PULL_NONE 0U
#define GPIO_PULL_UP   1U
#define GPIO_PULL_DOWN 2U

/// GPIOx_AFRL / GPIOx_AFRH field values (DS8626, alternate function mapping).
#define GPIO_AF_USART1 7U
#define GPIO_AF_USART2 7U

/* ======================================================================== */
/* USART - RM0090 section 30.6 (F4 layout: SR and DR, differs from the F7)  */
/* ======================================================================== */
#define USART1_BASE 0x40011000UL ///< APB2.
#define USART2_BASE 0x40004400UL ///< APB1.

/**
 * @brief USART_SR (offset 0x00) - Reset value 0x0000 00C0
 * @details RXNE, TC, LBD and CTS are rc_w0: a read-modify-write can clear a
 *          flag set in between. Clear them with a whole write through .raw.
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t pe : 1;  /* bit 0       */
      uint32_t fe : 1;  /* bit 1       */
      uint32_t nf : 1;  /* bit 2       */
      uint32_t ore : 1; /* bit 3       */

      uint32_t idle : 1; /* bit 4       */
      uint32_t rxne : 1; /* bit 5: WARNING: rc_w0 */
      uint32_t tc : 1;   /* bit 6: WARNING: rc_w0 */
      uint32_t txe : 1;  /* bit 7       */

      uint32_t lbd : 1;        /* bit 8: WARNING: rc_w0 */
      uint32_t cts : 1;        /* bit 9: WARNING: rc_w0 */
      uint32_t reserved0 : 22; /* bits 10-31  */
    } bitfield;
  };
} T_usart_sr;

_Static_assert(sizeof(T_usart_sr) == 4, "T_usart_sr must be 32 bits");

/**
 * @brief USART_BRR (offset 0x08) - Reset value 0x0000 0000
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t div_fraction : 4;  /* bits 0-3    */
      uint32_t div_mantissa : 12; /* bits 4-15   */
      uint32_t reserved0 : 16;    /* bits 16-31  */
    } bitfield;
  };
} T_usart_brr;

_Static_assert(sizeof(T_usart_brr) == 4, "T_usart_brr must be 32 bits");

/**
 * @brief USART_CR1 (offset 0x0C) - Reset value 0x0000 0000
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t sbk : 1; /* bit 0       */
      uint32_t rwu : 1; /* bit 1       */
      uint32_t re : 1;  /* bit 2       */
      uint32_t te : 1;  /* bit 3       */

      uint32_t idleie : 1; /* bit 4       */
      uint32_t rxneie : 1; /* bit 5       */
      uint32_t tcie : 1;   /* bit 6       */
      uint32_t txeie : 1;  /* bit 7       */

      uint32_t peie : 1; /* bit 8       */
      uint32_t ps : 1;   /* bit 9       */
      uint32_t pce : 1;  /* bit 10      */
      uint32_t wake : 1; /* bit 11      */

      uint32_t m : 1;         /* bit 12      */
      uint32_t ue : 1;        /* bit 13      */
      uint32_t reserved0 : 1; /* bit 14      */
      uint32_t over8 : 1;     /* bit 15      */

      uint32_t reserved1 : 16; /* bits 16-31  */
    } bitfield;
  };
} T_usart_cr1;

_Static_assert(sizeof(T_usart_cr1) == 4, "T_usart_cr1 must be 32 bits");

/**
 * @brief USART_CR2 (offset 0x10) - Reset value 0x0000 0000
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t add : 4;       /* bits 0-3    */
      uint32_t reserved0 : 1; /* bit 4       */
      uint32_t lbdl : 1;      /* bit 5       */
      uint32_t lbdie : 1;     /* bit 6       */

      uint32_t reserved1 : 1; /* bit 7       */
      uint32_t lbcl : 1;      /* bit 8       */
      uint32_t cpha : 1;      /* bit 9       */
      uint32_t cpol : 1;      /* bit 10      */

      uint32_t clken : 1;      /* bit 11      */
      uint32_t stop : 2;       /* bits 12-13  */
      uint32_t linen : 1;      /* bit 14      */
      uint32_t reserved2 : 17; /* bits 15-31  */
    } bitfield;
  };
} T_usart_cr2;

_Static_assert(sizeof(T_usart_cr2) == 4, "T_usart_cr2 must be 32 bits");

/**
 * @brief USART_CR3 (offset 0x14) - Reset value 0x0000 0000
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t eie : 1;   /* bit 0       */
      uint32_t iren : 1;  /* bit 1       */
      uint32_t irlp : 1;  /* bit 2       */
      uint32_t hdsel : 1; /* bit 3       */

      uint32_t nack : 1; /* bit 4       */
      uint32_t scen : 1; /* bit 5       */
      uint32_t dmar : 1; /* bit 6       */
      uint32_t dmat : 1; /* bit 7       */

      uint32_t rtse : 1;   /* bit 8       */
      uint32_t ctse : 1;   /* bit 9       */
      uint32_t ctsie : 1;  /* bit 10      */
      uint32_t onebit : 1; /* bit 11      */

      uint32_t reserved0 : 20; /* bits 12-31  */
    } bitfield;
  };
} T_usart_cr3;

_Static_assert(sizeof(T_usart_cr3) == 4, "T_usart_cr3 must be 32 bits");

/// WARNING: rc_w0 flags, see T_usart_sr.
#define USART_SR(usart)  REG_T(T_usart_sr, (usart) + 0x00U)
#define USART_DR(usart)  REG((usart) + 0x04U) ///< Data, bits 0-8; reading it clears RXNE.
#define USART_BRR(usart) REG_T(T_usart_brr, (usart) + 0x08U)
#define USART_CR1(usart) REG_T(T_usart_cr1, (usart) + 0x0CU)
#define USART_CR2(usart) REG_T(T_usart_cr2, (usart) + 0x10U)
#define USART_CR3(usart) REG_T(T_usart_cr3, (usart) + 0x14U)

/// USART_CR2 STOP field values.
#define USART_STOP_1   0U
#define USART_STOP_0_5 1U
#define USART_STOP_2   2U
#define USART_STOP_1_5 3U

/* ======================================================================== */
/* NVIC - Cortex-M4 system control space (PM0214 section 4.3)               */
/* ======================================================================== */
#define NVIC_ISER_BASE 0xE000E100UL
#define NVIC_IPR_BASE  0xE000E400UL
/// WARNING: write-1-to-set; write a single bit, never read-modify-write.
#define NVIC_ISER(n) REG(NVIC_ISER_BASE + (((n) / 32U) * 4UL))
#define NVIC_IPR(n)  (*(volatile uint8_t *)(NVIC_IPR_BASE + (n))) ///< One byte per IRQ.

/* STM32F407 external interrupt numbers (RM0090 table 61). */
#define USART1_IRQn 37U
#define USART2_IRQn 38U

/* ======================================================================== */
/* SysTick - PM0214 section 4.5                                             */
/* ======================================================================== */

/**
 * @brief STK_CTRL (0xE000E010) - Reset value 0x0000 0000
 * @details COUNTFLAG is cleared by any read of the register.
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t enable : 1;     /* bit 0       */
      uint32_t tickint : 1;    /* bit 1       */
      uint32_t clksource : 1;  /* bit 2       */
      uint32_t reserved0 : 13; /* bits 3-15   */

      uint32_t countflag : 1;  /* bit 16      */
      uint32_t reserved1 : 15; /* bits 17-31  */
    } bitfield;
  };
} T_stk_ctrl;

_Static_assert(sizeof(T_stk_ctrl) == 4, "T_stk_ctrl must be 32 bits");

#define STK_CTRL REG_T(T_stk_ctrl, 0xE000E010UL)
#define STK_LOAD REG(0xE000E014UL) ///< Reload value, bits 0-23.
#define STK_VAL  REG(0xE000E018UL) ///< Current value, bits 0-23; any write clears it.

/// STK_CTRL CLKSOURCE field values.
#define STK_CLKSOURCE_AHB_DIV8 0U
#define STK_CLKSOURCE_AHB      1U

/* ======================================================================== */
/* SCB - PM0214 section 4.4, CPACR section 4.6.1                            */
/* ======================================================================== */
#define SCB_VTOR REG(0xE000ED08UL) ///< Vector table offset.

/**
 * @brief SCB_AIRCR (0xE000ED0C) - Reset value 0xFA05 0000
 * @details VECTKEY reads as 0xFA05 and every write must carry 0x05FA, so the
 *          register is never written field by field: build the value, then
 *          write it through .raw.
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t vectreset : 1;     /* bit 0       */
      uint32_t vectclractive : 1; /* bit 1       */
      uint32_t sysresetreq : 1;   /* bit 2       */
      uint32_t reserved0 : 5;     /* bits 3-7    */

      uint32_t prigroup : 3;  /* bits 8-10   */
      uint32_t reserved1 : 4; /* bits 11-14  */
      uint32_t endianess : 1; /* bit 15      */
      uint32_t vectkey : 16;  /* bits 16-31: WARNING: write key 0x05FA, reads 0xFA05 */
    } bitfield;
  };
} T_scb_aircr;

_Static_assert(sizeof(T_scb_aircr) == 4, "T_scb_aircr must be 32 bits");

/**
 * @brief SCB_CPACR (0xE000ED88) - Reset value 0x0000 0000
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t reserved0 : 20; /* bits 0-19   */
      uint32_t cp10 : 2;       /* bits 20-21  */
      uint32_t cp11 : 2;       /* bits 22-23  */
      uint32_t reserved1 : 8;  /* bits 24-31  */
    } bitfield;
  };
} T_scb_cpacr;

_Static_assert(sizeof(T_scb_cpacr) == 4, "T_scb_cpacr must be 32 bits");

#define SCB_AIRCR REG_T(T_scb_aircr, 0xE000ED0CUL) ///< WARNING: write through .raw.
#define SCB_CPACR REG_T(T_scb_cpacr, 0xE000ED88UL)

#define SCB_AIRCR_VECTKEY 0x05FAU ///< Value of VECTKEY required by every AIRCR write.

/// SCB_CPACR CP10 / CP11 field values.
#define CPACR_ACCESS_DENIED     0U
#define CPACR_ACCESS_PRIVILEGED 1U
#define CPACR_ACCESS_FULL       3U

/* ======================================================================== */
/* Debug: DEMCR and DWT - ARMv7-M Architecture Reference Manual C1.6, C1.8  */
/* ======================================================================== */

/**
 * @brief DEMCR (0xE000EDFC) - Reset value 0x0000 0000
 */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t vc_corereset : 1; /* bit 0       */
      uint32_t reserved0 : 3;    /* bits 1-3    */
      uint32_t vc_mmerr : 1;     /* bit 4       */
      uint32_t vc_nocperr : 1;   /* bit 5       */

      uint32_t vc_chkerr : 1;  /* bit 6       */
      uint32_t vc_staterr : 1; /* bit 7       */
      uint32_t vc_buserr : 1;  /* bit 8       */
      uint32_t vc_interr : 1;  /* bit 9       */

      uint32_t vc_harderr : 1; /* bit 10      */
      uint32_t reserved1 : 5;  /* bits 11-15  */
      uint32_t mon_en : 1;     /* bit 16      */
      uint32_t mon_pend : 1;   /* bit 17      */

      uint32_t mon_step : 1;  /* bit 18      */
      uint32_t mon_req : 1;   /* bit 19      */
      uint32_t reserved2 : 4; /* bits 20-23  */
      uint32_t trcena : 1;    /* bit 24      */

      uint32_t reserved3 : 7; /* bits 25-31  */
    } bitfield;
  };
} T_coredebug_demcr;

_Static_assert(sizeof(T_coredebug_demcr) == 4, "T_coredebug_demcr must be 32 bits");

/**
 * @brief DWT_CTRL (0xE0001000) - Reset value 0x4000 0000
 */
/* TODO: vérifier ARMv7-M ARM §C1.8.7 (reset value, NUMCOMP on the Cortex-M4) */
typedef struct {
  union {
    uint32_t raw;
    struct {
      uint32_t cyccntena : 1;  /* bit 0       */
      uint32_t postpreset : 4; /* bits 1-4    */
      uint32_t postinit : 4;   /* bits 5-8    */
      uint32_t cyctap : 1;     /* bit 9       */

      uint32_t synctap : 2;    /* bits 10-11  */
      uint32_t pcsamplena : 1; /* bit 12      */
      uint32_t reserved0 : 3;  /* bits 13-15  */
      uint32_t exctrcena : 1;  /* bit 16      */

      uint32_t cpievtena : 1;   /* bit 17      */
      uint32_t excevtena : 1;   /* bit 18      */
      uint32_t sleepevtena : 1; /* bit 19      */
      uint32_t lsuevtena : 1;   /* bit 20      */

      uint32_t foldevtena : 1; /* bit 21      */
      uint32_t cycevtena : 1;  /* bit 22      */
      uint32_t reserved1 : 1;  /* bit 23      */
      uint32_t noprfcnt : 1;   /* bit 24      */

      uint32_t nocyccnt : 1;  /* bit 25      */
      uint32_t noexttrig : 1; /* bit 26      */
      uint32_t notrcpkt : 1;  /* bit 27      */
      uint32_t numcomp : 4;   /* bits 28-31  */
    } bitfield;
  };
} T_dwt_ctrl;

_Static_assert(sizeof(T_dwt_ctrl) == 4, "T_dwt_ctrl must be 32 bits");

#define DEMCR      REG_T(T_coredebug_demcr, 0xE000EDFCUL)
#define DWT_CTRL   REG_T(T_dwt_ctrl, 0xE0001000UL)
#define DWT_CYCCNT REG(0xE0001004UL) ///< Cycle counter, counts core clocks.

/* ======================================================================== */
/* Interrupt masking                                                        */
/* ======================================================================== */

/// Mask all configurable interrupts (PRIMASK = 1).
static inline void IrqDisable(void)
{
  __asm__ volatile("cpsid i" ::: "memory");
}

/// Unmask interrupts (PRIMASK = 0).
static inline void IrqEnable(void)
{
  __asm__ volatile("cpsie i" ::: "memory");
}

#endif
