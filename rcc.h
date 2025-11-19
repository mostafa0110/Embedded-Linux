#ifndef RCC_H
#define RCC_H
#include "std_types.h"
#ifdef __cplusplus
    namespace rcc{
#endif



// use masks for clock set

#define HSI_ENABLE_MASK 0b00000001

/* Clock Types */
typedef enum
{
    RCC_CLK_HSI = 0, // High-Speed Internal clock
    RCC_CLK_HSE = 1, // High-Speed External clock
    RCC_CLK_PLL = 2  // PLL clock
} RCC_Clk_t;

/* PLL Configuration Structure */
typedef struct
{
    uint8 PLL_M;  // Division factor for the main PLL (2-63)
    uint16 PLL_N; // Multiplication factor for the main PLL (192-432)
    uint8 PLL_P;  // Division factor for main system clock (2, 4, 6, or 8)
    uint8 PLL_Q;  // Division factor for USB OTG FS, SDIO (2-15)
    uint8 PLLSRC; // PLL clock source (0: HSI, 1: HSE)
} PLL_CFG_T;

/* Bus Types */
typedef enum
{
    AHB1,
    AHB2,
    APB1,
    APB2
} Bus_t;

/* AHB1 Peripherals */
typedef enum
{
    RCC_GPIOA = 0,
    RCC_GPIOB = 1,
    RCC_GPIOC = 2,
    RCC_GPIOD = 3,
    RCC_GPIOE = 4,
    RCC_GPIOH = 7,
    RCC_CRC = 12,
    RCC_DMA1 = 21,
    RCC_DMA2 = 22
} RCC_AHB1_Peripheral_t;

/* AHB2 Peripherals */
typedef enum
{
    RCC_OTGFS = 7
} RCC_AHB2_Peripheral_t;

/* APB1 Peripherals */
typedef enum
{
    RCC_TIM2 = 0,
    RCC_TIM3 = 1,
    RCC_TIM4 = 2,
    RCC_TIM5 = 3,
    RCC_WWDG = 11,
    RCC_SPI2 = 14,
    RCC_SPI3 = 15,
    RCC_USART2 = 17,
    RCC_I2C1 = 21,
    RCC_I2C2 = 22,
    RCC_I2C3 = 23,
    RCC_PWR = 28
} RCC_APB1_Peripheral_t;

/* APB2 Peripherals */
typedef enum
{
    RCC_TIM1 = 0,
    RCC_USART1 = 4,
    RCC_USART6 = 5,
    RCC_ADC1 = 8,
    RCC_SDIO = 11,
    RCC_SPI1 = 12,
    RCC_SPI4 = 13,
    RCC_SYSCFG = 14,
    RCC_TIM9 = 16,
    RCC_TIM10 = 17,
    RCC_TIM11 = 18
} RCC_APB2_Peripheral_t;

/* AHB Prescaler */
typedef enum
{
    AHB_NO_DIV = 0,
    AHB_DIV_2 = 8,
    AHB_DIV_4 = 9,
    AHB_DIV_8 = 10,
    AHB_DIV_16 = 11,
    AHB_DIV_64 = 12,
    AHB_DIV_128 = 13,
    AHB_DIV_256 = 14,
    AHB_DIV_512 = 15
} RCC_AHB_Prescaler_t;

/* APB Prescaler */
typedef enum
{
    APB_NO_DIV = 0,
    APB_DIV_2 = 4,
    APB_DIV_4 = 5,
    APB_DIV_8 = 6,
    APB_DIV_16 = 7
} RCC_APB_Prescaler_t;

/* System Configuration Structure */
typedef struct
{
    RCC_AHB_Prescaler_t AHB_Prescaler;
    RCC_APB_Prescaler_t APB1_Prescaler;
    RCC_APB_Prescaler_t APB2_Prescaler;
} RCC_CFG_t;

uint8 RCC_setClk(RCC_Clk_t clk);

uint8 RCC_clkCtrl(RCC_Clk_t clk, uint8 enable);

uint8 RCC_PllCfg(PLL_CFG_T cfg);

uint8 RCC_EnablePeripherals(Bus_t bus, const uint8 *peripherals, uint8 count, uint8 enable);

uint8 RCC_setCfg(RCC_CFG_t cfg);

#ifdef __cplusplus
}
#endif

#endif