#define SYSCALL_PRINT_AT 3

void main(void) {
    const char *msg = "hello from init!";

    asm volatile (
        "movl %0, %%esi\n"
        "movl $%c1, %%eax\n"
        "movl $100, %%ecx\n"
        "movl $100, %%edx\n"
        "movl $0xFFFF00, %%EBX\n"
        "int $0x80\n"
        :
        : "r"(msg), "i"(SYSCALL_PRINT_AT)
    );

    asm volatile(
        "mov $0x2, %eax\n"
        "int $0x80"
    );

    while (1) {
        asm volatile ("hlt"); // stop the cpu from going crazy
    }
}
