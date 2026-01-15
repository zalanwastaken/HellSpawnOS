__attribute__((section(".start")))
void kernel_main(void){
    while (1){
        asm volatile("hlt"); // just STOP
    }
}
