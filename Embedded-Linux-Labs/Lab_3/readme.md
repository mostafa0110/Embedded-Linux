# Bare-Metal AArch64 LED Blinky on Raspberry Pi 3B+


### 1. Explanation of the Chosen Load Address

The application is linked and loaded at memory address `0x02000000`.

When executing a bare-metal application via U-Boot, the program must be placed in a safe physical RAM address. It cannot overlap with the memory space where U-Boot currently resides, nor can it overwrite U-Boot's exception vector tables located at the bottom of memory. The address `0x02000000` provides a safe, empty buffer space within the Raspberry Pi's DRAM that prevents memory corruption and perfectly matches the `. = 0x02000000;` directive in the linker script.

### 2. Why do we need a `startup.s` file? Can’t we just write everything in C?

We cannot write the entire application exclusively in C because C code requires a foundational "C Runtime" environment to execute properly.

When the CPU first receives power and hands control over to our application, it has no concept of a Stack Pointer (`sp`). Without a stack, C functions cannot allocate local variables or manage function calls. Furthermore, the Raspberry Pi 3B+ has a quad-core processor. Upon boot, all four cores wake up simultaneously. If we branch straight into `main()`, all four cores will attempt to execute the GPIO blinking logic at the same time, resulting in race conditions and unpredictable behavior.

The `startup.s` assembly file is strictly required to:

1. Read the CPU Core ID.
2. Park the secondary slave cores (Cores 1, 2, and 3) in an infinite `wfe` (Wait For Event) sleep loop.
3. Initialize the Stack Pointer for the primary master core (Core 0).
4. Safely branch (`bl`) into the `main()` C function.

