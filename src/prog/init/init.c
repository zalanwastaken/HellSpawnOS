void main(void) {
    const char *msg = "hello from init!";
    asm volatile ( // write to screen and render it
        "movl %0, %%esi\n"
        "movl $3, %%eax\n"
        "movl $0, %%ecx\n"
        "movl $80, %%edx\n"
        "movl $0xFFFF00, %%EBX\n"
        "int $0x80\n"
        :
        : "r"(msg)
    );

    const char *file = "root/kernel";
    asm volatile( // read file
        "movl %0, %%esi\n"
        "movl $4, %%eax\n"
        "movl $0x900, %%ebx\n"
        "mov $90, %%ecx\n"
        "int $0x80"
        :
        : "r"(file)
    );

    asm volatile( // put pixel
        "movl $5, %%eax\n"
        "movl $400, %%ebx\n"
        "movl $400, %%ecx\n"
        "mov $0xFFFFFF, %%edx\n"
        "int $0x80"
        :
        :
    );

    asm volatile( // kernel panic
        "mov $0x2, %eax\n"
        "mov $0x0000, %ebx\n"
        "int $0x80"
    );

    while (1) {
        asm volatile ("hlt"); // stop the cpu from going crazy
    }
}
