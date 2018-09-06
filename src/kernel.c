#include "include/uart.h"

void KernelMain() {
    // initialize UART controller with 230400 baudrate
    InitUART(230400);
}
