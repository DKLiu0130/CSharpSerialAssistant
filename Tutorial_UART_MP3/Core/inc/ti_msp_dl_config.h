/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)



#define CPUCLK_FREQ                                                     80000000



/* Defines for PWM_0 */
#define PWM_0_INST                                                         TIMA0
#define PWM_0_INST_IRQHandler                                   TIMA0_IRQHandler
#define PWM_0_INST_INT_IRQN                                     (TIMA0_INT_IRQn)
#define PWM_0_INST_CLK_FREQ                                               800000
/* GPIO defines for channel 0 */
#define GPIO_PWM_0_C0_PORT                                                 GPIOA
#define GPIO_PWM_0_C0_PIN                                         DL_GPIO_PIN_21
#define GPIO_PWM_0_C0_IOMUX                                      (IOMUX_PINCM46)
#define GPIO_PWM_0_C0_IOMUX_FUNC                     IOMUX_PINCM46_PF_TIMA0_CCP0
#define GPIO_PWM_0_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_0_C1_PORT                                                 GPIOA
#define GPIO_PWM_0_C1_PIN                                         DL_GPIO_PIN_22
#define GPIO_PWM_0_C1_IOMUX                                      (IOMUX_PINCM47)
#define GPIO_PWM_0_C1_IOMUX_FUNC                     IOMUX_PINCM47_PF_TIMA0_CCP1
#define GPIO_PWM_0_C1_IDX                                    DL_TIMER_CC_1_INDEX



/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                        DL_GPIO_PIN_11
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_10
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM22)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM21)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM21_PF_UART0_TX
#define UART_0_BAUD_RATE                                               (1000000)
#define UART_0_IBRD_40_MHZ_1000000_BAUD                                      (2)
#define UART_0_FBRD_40_MHZ_1000000_BAUD                                     (32)




/* Defines for SPI_OLED */
#define SPI_OLED_INST                                                      SPI1
#define SPI_OLED_INST_IRQHandler                                SPI1_IRQHandler
#define SPI_OLED_INST_INT_IRQN                                    SPI1_INT_IRQn
#define GPIO_SPI_OLED_PICO_PORT                                           GPIOB
#define GPIO_SPI_OLED_PICO_PIN                                    DL_GPIO_PIN_8
#define GPIO_SPI_OLED_IOMUX_PICO                                (IOMUX_PINCM25)
#define GPIO_SPI_OLED_IOMUX_PICO_FUNC                IOMUX_PINCM25_PF_SPI1_PICO
/* GPIO configuration for SPI_OLED */
#define GPIO_SPI_OLED_SCLK_PORT                                           GPIOB
#define GPIO_SPI_OLED_SCLK_PIN                                    DL_GPIO_PIN_9
#define GPIO_SPI_OLED_IOMUX_SCLK                                (IOMUX_PINCM26)
#define GPIO_SPI_OLED_IOMUX_SCLK_FUNC                IOMUX_PINCM26_PF_SPI1_SCLK
#define GPIO_SPI_OLED_CS0_PORT                                            GPIOB
#define GPIO_SPI_OLED_CS0_PIN                                     DL_GPIO_PIN_6
#define GPIO_SPI_OLED_IOMUX_CS0                                 (IOMUX_PINCM23)
#define GPIO_SPI_OLED_IOMUX_CS0_FUNC                  IOMUX_PINCM23_PF_SPI1_CS0
/* Defines for SPI_Flash */
#define SPI_Flash_INST                                                     SPI0
#define SPI_Flash_INST_IRQHandler                               SPI0_IRQHandler
#define SPI_Flash_INST_INT_IRQN                                   SPI0_INT_IRQn
#define GPIO_SPI_Flash_PICO_PORT                                          GPIOA
#define GPIO_SPI_Flash_PICO_PIN                                   DL_GPIO_PIN_5
#define GPIO_SPI_Flash_IOMUX_PICO                               (IOMUX_PINCM10)
#define GPIO_SPI_Flash_IOMUX_PICO_FUNC               IOMUX_PINCM10_PF_SPI0_PICO
#define GPIO_SPI_Flash_POCI_PORT                                          GPIOA
#define GPIO_SPI_Flash_POCI_PIN                                   DL_GPIO_PIN_4
#define GPIO_SPI_Flash_IOMUX_POCI                                (IOMUX_PINCM9)
#define GPIO_SPI_Flash_IOMUX_POCI_FUNC                IOMUX_PINCM9_PF_SPI0_POCI
/* GPIO configuration for SPI_Flash */
#define GPIO_SPI_Flash_SCLK_PORT                                          GPIOA
#define GPIO_SPI_Flash_SCLK_PIN                                   DL_GPIO_PIN_6
#define GPIO_SPI_Flash_IOMUX_SCLK                               (IOMUX_PINCM11)
#define GPIO_SPI_Flash_IOMUX_SCLK_FUNC               IOMUX_PINCM11_PF_SPI0_SCLK
#define GPIO_SPI_Flash_CS1_PORT                                           GPIOA
#define GPIO_SPI_Flash_CS1_PIN                                    DL_GPIO_PIN_3
#define GPIO_SPI_Flash_IOMUX_CS1                                 (IOMUX_PINCM8)
#define GPIO_SPI_Flash_IOMUX_CS1_FUNC            IOMUX_PINCM8_PF_SPI0_CS1_POCI1



/* Defines for ADC12_0 */
#define ADC12_0_INST                                                        ADC0
#define ADC12_0_INST_IRQHandler                                  ADC0_IRQHandler
#define ADC12_0_INST_INT_IRQN                                    (ADC0_INT_IRQn)
#define ADC12_0_ADCMEM_0                                      DL_ADC12_MEM_IDX_0
#define ADC12_0_ADCMEM_0_REF                     DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC12_0_ADCMEM_0_REF_VOLTAGE                                          -1 // VDDA cannot be determined
#define GPIO_ADC12_0_C0_PORT                                               GPIOA
#define GPIO_ADC12_0_C0_PIN                                       DL_GPIO_PIN_27

/* Defines for ADC12_1 */
#define ADC12_1_INST                                                        ADC1
#define ADC12_1_INST_IRQHandler                                  ADC1_IRQHandler
#define ADC12_1_INST_INT_IRQN                                    (ADC1_INT_IRQn)
#define ADC12_1_ADCMEM_1                                      DL_ADC12_MEM_IDX_1
#define ADC12_1_ADCMEM_1_REF                     DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC12_1_ADCMEM_1_REF_VOLTAGE                                          -1 // VDDA cannot be determined
#define GPIO_ADC12_1_C1_PORT                                               GPIOA
#define GPIO_ADC12_1_C1_PIN                                       DL_GPIO_PIN_16



/* Defines for DMA_CH0 */
#define DMA_CH0_CHAN_ID                                                      (1)
#define ADC12_0_INST_DMA_TRIGGER                      (DMA_ADC0_EVT_GEN_BD_TRIG)

/* Defines for DMA_CH1 */
#define DMA_CH1_CHAN_ID                                                      (0)
#define ADC12_1_INST_DMA_TRIGGER                      (DMA_ADC1_EVT_GEN_BD_TRIG)



/* Port definition for Pin Group OLED */
#define OLED_PORT                                                        (GPIOB)

/* Defines for DC: GPIOB.7 with pinCMx 24 on package pin 59 */
#define OLED_DC_PIN                                              (DL_GPIO_PIN_7)
#define OLED_DC_IOMUX                                            (IOMUX_PINCM24)
/* Port definition for Pin Group Keyboard */
#define Keyboard_PORT                                                    (GPIOA)

/* Defines for KE_1: GPIOA.13 with pinCMx 35 on package pin 6 */
#define Keyboard_KE_1_PIN                                       (DL_GPIO_PIN_13)
#define Keyboard_KE_1_IOMUX                                      (IOMUX_PINCM35)
/* Defines for KE_2: GPIOA.14 with pinCMx 36 on package pin 7 */
#define Keyboard_KE_2_PIN                                       (DL_GPIO_PIN_14)
#define Keyboard_KE_2_IOMUX                                      (IOMUX_PINCM36)
/* Defines for KE_3: GPIOA.17 with pinCMx 39 on package pin 10 */
#define Keyboard_KE_3_PIN                                       (DL_GPIO_PIN_17)
#define Keyboard_KE_3_IOMUX                                      (IOMUX_PINCM39)
/* Defines for KE_4: GPIOA.18 with pinCMx 40 on package pin 11 */
#define Keyboard_KE_4_PIN                                       (DL_GPIO_PIN_18)
#define Keyboard_KE_4_IOMUX                                      (IOMUX_PINCM40)
/* Defines for KR_1: GPIOA.0 with pinCMx 1 on package pin 33 */
#define Keyboard_KR_1_PIN                                        (DL_GPIO_PIN_0)
#define Keyboard_KR_1_IOMUX                                       (IOMUX_PINCM1)
/* Defines for KR_2: GPIOA.1 with pinCMx 2 on package pin 34 */
#define Keyboard_KR_2_PIN                                        (DL_GPIO_PIN_1)
#define Keyboard_KR_2_IOMUX                                       (IOMUX_PINCM2)
/* Defines for KR_3: GPIOA.7 with pinCMx 14 on package pin 49 */
#define Keyboard_KR_3_PIN                                        (DL_GPIO_PIN_7)
#define Keyboard_KR_3_IOMUX                                      (IOMUX_PINCM14)
/* Defines for KR_4: GPIOA.12 with pinCMx 34 on package pin 5 */
#define Keyboard_KR_4_PIN                                       (DL_GPIO_PIN_12)
#define Keyboard_KR_4_IOMUX                                      (IOMUX_PINCM34)





/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_0_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_SPI_OLED_init(void);
void SYSCFG_DL_SPI_Flash_init(void);
void SYSCFG_DL_ADC12_0_init(void);
void SYSCFG_DL_ADC12_1_init(void);
void SYSCFG_DL_DMA_init(void);

void SYSCFG_DL_SYSTICK_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
