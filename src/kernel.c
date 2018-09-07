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
#include "include/debug.h"

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

void WriteDisk(const void *memory, size_t length, size_t disk_pos) {
    //
}

void InitSystemFromDisk(void *memory, size_t disk_pos) {
    // TODO: implement NAND controller
    // blink
    for (;;) {
        DelayMillisecond(500);
        GPIO_LED = 0xfffe;
        DelayMillisecond(500);
        GPIO_LED = 0xffff;
    }
}

void InitSystemFromMemory(const void *memory) {
    // memory must be a multiple of 4
    if (((size_t)memory & 0x3)) return;
    // reset GPIO
    GPIO_BILED0 = GPIO_BILED_OFF;
    GPIO_BILED1 = GPIO_BILED_OFF;
    GPIO_LED = 0xffff;
    GPIO_NUM = 0;
    // jump to memory address
    asm volatile ("jr $a1");
}

void OverrideSPI(const void *memory, size_t length) {
    //
    /*
        //
    */
}

void LoadMemoryFromUART(void *memory, size_t length) {
    // memory & length must be a multiple of 4
    if (((size_t)memory & 0x3) || (length & 0x3)) return;
    while (length) {
        *((uint32_t *)memory) = GetWordUART();
        memory += 4;
        length -= 4;
    }
}

void LoadMemoryFromXmodem(void *memory) {
    //
}

void KernelMain() {
    DEBUG(1);
    // initialize UART controller with 230400 baud rate
    InitUART(230400);
    DEBUG(2);
    // get GPIO switch status
    if (((~GPIO_SWITCH) & 0x01)) {
        DEBUG(3);
        InitSystemFromDisk((void *)0x80000000, 0);
    }
    else {
        DEBUG(4);
        InitShell();
    }
}

void ExceptionHandler() {
    size_t cause, epc;
    asm volatile ("mfc0 %0, $13" : "=r"(cause));
    asm volatile ("mfc0 %0, $14" : "=r"(epc));
    for (;;) {
        if (((~GPIO_SWITCH) & 0x01)) {
            // display CP0.EPC
            GPIO_NUM = epc;
        }
        else {
            // display CP0.Cause
            GPIO_NUM = cause;
        }
    }
}
