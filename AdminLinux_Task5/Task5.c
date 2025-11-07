void my_write(int fd, const char *buf, unsigned long count) {
    __asm__ volatile (
        "movq $1, %%rax\n\t"      // syscall number for write rax = 0x01
        "movq %0, %%rdi\n\t"      // first argument fd        rdi = fd
        "movq %1, %%rsi\n\t"      // second argument          rsi = buffer pointer
        "movq %2, %%rdx\n\t"      // third argument           rdx = count             
        "syscall\n\t"             // perform syscall
        :                                           // No output operands (we don't capture any return value)
        : "r"((long)fd), "r"(buf), "r"(count)       // map arguments to %0, %1, %2
        : "%rax", "%rdi", "%rsi", "%rdx"            // tell compiler these are modified
    );
}

void my_printf(const char *str) {
    unsigned long len = 0;
    while (str[len] != '\0'){
        len++;
    }
    my_write(1, str, len);
}

int main() {
    my_printf("Hello from my_printf using syscall!\n");
    return 0;
}