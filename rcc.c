#include "rcc.h"

#ifdef __cplusplus
    namespace rcc{
#endif



#define RCC_BASE_ADDRESS 0x40023800
#define RCC ((RCC_registers_t *)RCC_BASE_ADDRESS)

typedef union
{
    volatile uint32 REG;
    struct
    {
        volatile uint32 HSION : 1;
        volatile uint32 HSIRDY : 1;
        volatile uint32 : 1;
        volatile uint32 HSITRIM : 5;
        volatile uint32 HSICAL : 8;
        volatile uint32 HSEON : 1;
        volatile uint32 HSERDY : 1;
        volatile uint32 HSEBYP : 1;
        volatile uint32 CSSON : 1;
        volatile uint32 : 4;
        volatile uint32 PLLON : 1;
        volatile uint32 PLLRDY : 1;
        volatile uint32 PLLI2SON : 1;
        volatile uint32 PLLI2SRDY : 1;
        volatile uint32 : 4;
    } BIT;
} RCC_CR_t;

typedef union
{
    volatile uint32 REG;
    struct
    {
        volatile uint32 SW : 2;
        volatile uint32 SWS : 2;
        volatile uint32 HPRE : 4;
        volatile uint32 : 2;
        volatile uint32 PPRE1 : 3;
        volatile uint32 PPRE2 : 3;
        volatile uint32 RTCPRE : 5;
        volatile uint32 MCO1 : 2;
        volatile uint32 I2SSRC : 1;
        volatile uint32 MCO1PRE : 3;
        volatile uint32 MCO2PRE : 3;
        volatile uint32 MCO2 : 2;
    } BIT;
} RCC_CFGR_t;

typedef union
{
    volatile uint32 REG;
    struct
    {
        volatile uint32 PLLM : 6;
        volatile uint32 PLLN : 9;
        volatile uint32 : 1;
        volatile uint32 PLLP : 2;
        volatile uint32 : 4;
        volatile uint32 PLLSRC : 1;
        volatile uint32 : 1;
        volatile uint32 PLLQ : 4;
        volatile uint32 : 4;
    } BIT;
} RCC_PLLCFGR_t;

typedef struct
{
    RCC_CR_t CR;
    RCC_PLLCFGR_t PLLCFGR;
    RCC_CFGR_t CFGR;
    volatile uint32 CIR;
    volatile uint32 AHB1RSTR;
    volatile uint32 AHB2RSTR;
    uint32 RESERVED0[2];
    volatile uint32 APB1RSTR;
    volatile uint32 APB2RSTR;
    uint32 RESERVED1[2];
    volatile uint32 AHB1ENR;
    volatile uint32 AHB2ENR;
    uint32 RESERVED2[2];
    volatile uint32 APB1ENR;
    volatile uint32 APB2ENR;
    uint32 RESERVED3[2];
    volatile uint32 AHB1LPENR;
    volatile uint32 AHB2LPENR;
    uint32 RESERVED4[2];
    volatile uint32 APB1LPENR;
    volatile uint32 APB2LPENR;
    uint32 RESERVED5[2];
    volatile uint32 BDCR;
    volatile uint32 CSR;
    uint32 RESERVED6[2];
    volatile uint32 SSCGR;
    volatile uint32 PLLI2SCFGR;
} RCC_registers_t;

uint8 RCC_setClk(RCC_Clk_t clk)
{
    switch (clk)
    {
    case RCC_CLK_HSI:
        RCC->CFGR.BIT.SW = RCC_CLK_HSI;
        while (RCC->CFGR.BIT.SWS != RCC_CLK_HSI)
            ;
        break;
    case RCC_CLK_HSE:
        RCC->CFGR.BIT.SW = RCC_CLK_HSE;
        while (RCC->CFGR.BIT.SWS != RCC_CLK_HSE)
            ;
        break;
    case RCC_CLK_PLL:
        //*(uint32 *)((0x40000000UL + 0x00020000UL) + 0x3C00UL) &= ~0b111;
        //*(uint32 *)((0x40000000UL + 0x00020000UL) + 0x3C00UL) |= (2 & 0b111);
        RCC->CFGR.BIT.SW = RCC_CLK_PLL;
        while (RCC->CFGR.BIT.SWS != RCC_CLK_PLL)
            ;
        break;
    default:
        return EXIT_FAILURE;
        break;
    }
    return EXIT_SUCCESS;
}

uint8 RCC_clkCtrl(RCC_Clk_t clk, uint8 enable)
{
    switch (clk)
    {
    case RCC_CLK_HSI:
        if (enable == TRUE)
        {
            RCC->CR.BIT.HSION = 1;
            uint32 timeout = 1000000;
            while (RCC->CR.BIT.HSIRDY != 1)
            {
                if (--timeout == 0)
                {
                    return EXIT_FAILURE; // Timeout occurred
                }
            }
        }
        else
        {
            RCC->CR.BIT.HSION = 0;
        }
        break;
    case RCC_CLK_HSE:
        if (enable == TRUE)
        {
            RCC->CR.BIT.HSEON = 1;
            uint32 timeout = 1000000;
            while (RCC->CR.BIT.HSERDY != 1) // wait until HSE is ready
            {
                if (--timeout == 0)
                {
                    return EXIT_FAILURE; // Timeout occurred
                }
            }
        }
        else
        {
            RCC->CR.BIT.HSEON = 0;
        }
        break;
    case RCC_CLK_PLL:
        if (enable == TRUE)
        {
            RCC->CR.BIT.PLLON = 1;
            uint32 timeout = 1000000;
            while (RCC->CR.BIT.PLLRDY != 1) // wait until PLL is ready
            {
                if (--timeout == 0)
                {
                    return EXIT_FAILURE; // Timeout occurred
                }
            }
        }
        else
        {
            RCC->CR.BIT.PLLON = 0;
        }
        break;
    default:
        break;
    }
    return EXIT_SUCCESS;
}

uint8 RCC_PllCfg(PLL_CFG_T cfg)
{
    // Validate PLL_M (division factor: 2-63)
    if (cfg.PLL_M < 2 || cfg.PLL_M > 63)
    {
        return EXIT_FAILURE;
    }
    // Validate PLL_N (multiplication factor: 192-432)
    if (cfg.PLL_N < 192 || cfg.PLL_N > 432)
    {
        return EXIT_FAILURE;
    }
    // Validate PLL_P (only 2, 4, 6, or 8 allowed)
    if (cfg.PLL_P != 2 && cfg.PLL_P != 4 && cfg.PLL_P != 6 && cfg.PLL_P != 8)
    {
        return EXIT_FAILURE;
    }
    // Validate PLL_Q (division factor: 2-15)
    if (cfg.PLL_Q < 2 || cfg.PLL_Q > 15)
    {
        return EXIT_FAILURE;
    }
    // Validate PLLSRC (0: HSI, 1: HSE)
    if (cfg.PLLSRC != 0 && cfg.PLLSRC != 1)
    {
        return EXIT_FAILURE;
    }

    // Configure PLL parameters
    RCC->PLLCFGR.BIT.PLLM = cfg.PLL_M;
    RCC->PLLCFGR.BIT.PLLN = cfg.PLL_N;
    // PLLP (2->0, 4->1, 6->2, 8->3)
    RCC->PLLCFGR.BIT.PLLP = (cfg.PLL_P / 2) - 1;
    RCC->PLLCFGR.BIT.PLLQ = cfg.PLL_Q;
    RCC->PLLCFGR.BIT.PLLSRC = cfg.PLLSRC;

    return EXIT_SUCCESS;
}

uint8 RCC_setCfg(RCC_CFG_t cfg)
{
    // Configure AHB prescaler
    RCC->CFGR.BIT.HPRE = cfg.AHB_Prescaler;

    // Configure APB1 prescaler
    RCC->CFGR.BIT.PPRE1 = cfg.APB1_Prescaler;

    // Configure APB2 prescaler
    RCC->CFGR.BIT.PPRE2 = cfg.APB2_Prescaler;

    return EXIT_SUCCESS;
}

uint8 RCC_EnablePeripherals(Bus_t bus, const uint8 *peripherals, uint8 count, uint8 enable)
{
    uint8 i;
    volatile uint32 *bus_register;

    // Validate input parameters
    if (peripherals == NULL_PTR || count == 0)
    {
        return EXIT_FAILURE;
    }

    // Select the appropriate bus register
    switch (bus)
    {
    case AHB1:
        bus_register = &(RCC->AHB1ENR);
        break;

    case AHB2:
        bus_register = &(RCC->AHB2ENR);
        break;

    case APB1:
        bus_register = &(RCC->APB1ENR);
        break;

    case APB2:
        bus_register = &(RCC->APB2ENR);
        break;

    default:
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; i++)
    {
        if (peripherals[i] > 31)
        {
            return EXIT_FAILURE;
        }

        if (enable == TRUE)
        {
            *bus_register |= (1 << peripherals[i]);
        }
        else
        {
            *bus_register &= ~(1 << peripherals[i]);
        }
    }

    return EXIT_SUCCESS;
}

#ifdef __cplusplus
}
#endif
