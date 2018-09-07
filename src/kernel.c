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

#include "include/soc.h"
#include "include/uart.h"
#include "include/time.h"

#include "kernel.h"
#include "shell.h"

const char *GetCurrentSeg() {
    void *pc;
    asm volatile ("move %0, $ra" : "=r"(pc));
    switch ((((uint32_t)pc) >> 28) & 0xf) {
        case 0x8: case 0x9: return "kseg0";
        case 0xa: case 0xb: return "kseg1";
        case 0xc: case 0xd: return "kseg2";
        case 0xe: case 0xf: return "kseg3";
        default: return "kuseg";
    }
}

void WriteDisk(void *memory, size_t length, size_t disk_pos) {
    //
}

void InitSystemFromDisk(size_t disk_pos) {
    // TODO: implement NAND controller
    // blink
    for (;;) {
        DelayMillisecond(500);
        GPIO_LED = 0xfffe;
        DelayMillisecond(500);
        GPIO_LED = 0xffff;
    }
}

void OverrideSPI(const void *memory, size_t length) {
    //
    /*
        //
    */
}

void LoadMemoryFromUART(void *memory, size_t length) {
    // length must be a multiple of 4
    if ((length & 0x3)) return;
    while (length) {
        *((uint32_t *)memory) = GetWordUART();
        memory += 4;
        length -= 4;
    }
}

void KernelMain() {
    // initialize UART controller with 230400 baud rate
    InitUART(230400);
    // get GPIO switch status
    if (((~GPIO_SWITCH) & 0x01)) {
        InitSystemFromDisk(0);
    }
    else {
        InitShell();
    }
}
