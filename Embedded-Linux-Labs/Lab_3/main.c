#define BCM2837_PERI_BASE 0x3F000000
#define GPIO_BASE         (BCM2837_PERI_BASE + 0x200000)

/* Register Pointers */
volatile unsigned int *GPFSEL2 = (unsigned int *)(GPIO_BASE + 0x08);
volatile unsigned int *GPSET0  = (unsigned int *)(GPIO_BASE + 0x1C);
volatile unsigned int *GPCLR0  = (unsigned int *)(GPIO_BASE + 0x28);

/* Simple busy-wait delay loop */
void delay(int count) {
    volatile int i;
    for (i = 0; i < count; i++) {
        asm volatile("nop");
    }
}

int main() {
    /* Configure GPIO 26 as an Output */
    /* GPIO 26 is controlled by bits 18-20 in the GPFSEL2 register */
    *GPFSEL2 &= ~(7 << 18);  // Clear bits 18, 19, 20
    *GPFSEL2 |= (1 << 18);   // Set bit 18 to 1 (Output mode)

    while (1) {
        *GPSET0 = (1 << 26); // Turn LED ON
        delay(5000000);      // Wait
        *GPCLR0 = (1 << 26); // Turn LED OFF
        delay(5000000);      // Wait
    }
    
    return 0; // The program must never crash or return
}