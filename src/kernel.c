/*
    I am the bone of my processor.
    Silicon is my body, and current is by blood.
    I have created over a thousand bootloaders.
    Unknown to Crash.
    Nor known to Work.
    Have withstood pain to initialize many stacks.
    Yet, those pointers will never hold anything.
    So I pray, Unlimited Boot Works.
*/

#include "include/uart.h"

void KernelMain() {
    // initialize UART controller with 230400 baud rate
    InitUART(230400);
}
