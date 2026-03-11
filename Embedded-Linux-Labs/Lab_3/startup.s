.section ".text.boot"
.global _start

_start:
    /* Read the CPU Core ID. The RPi3 has 4 cores. */
    /* We only want Core 0 to run our C code. */
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, master

hang:  
    /* Put the other 3 cores to sleep infinitely */
    wfe
    b       hang

master:
    /* Set the Stack Pointer to a safe address (our load address) */
    ldr     x1, =_start
    mov     sp, x1

    /* Jump to our C code! */
    bl      main

end:
    /* If main() ever returns, trap the CPU in an infinite loop */
    b       end