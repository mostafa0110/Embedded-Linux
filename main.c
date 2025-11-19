#include "rcc.h"
#include "std_types.h"
#include <stdio.h>


int main(void)
{
    #ifdef __cplusplus
    rcc::RCC_CFG_t rcc_config;
    printf("RCC configuration set successfully with C++.\n");
    #else
    RCC_CFG_t rcc_config;
    printf("RCC configuration set successfully with C.\n");
    #endif


    while (1)
    {
        // Main loop
    }

    return 0;
}