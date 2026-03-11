ENTRY(_start)
SECTIONS
{
    /* We will tell U-Boot to load us exactly here in RAM */
    . = 0x02000000;
    
    .text : {
        *(.text.boot)
        *(.text*)
    }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : {
        . = ALIGN(8);
        __bss_start = .;
        *(.bss*)
        __bss_end = .;
    }
}