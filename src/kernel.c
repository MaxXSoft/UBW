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

#include "kernel.h"

#include "include/soc.h"
#include "include/uart.h"
#include "include/time.h"
#include "include/mem.h"
#include "include/debug.h"

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

void ReadDisk(void *memory, size_t length, size_t disk_pos) {
    //
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

static void InitialMemory(size_t gp, size_t bss_start, size_t bss_end) {
    // move global memory to new address space
    gp -= 32768;
    memcpy((void *)MEM_GLOBAL_BASE, (void *)gp, bss_start - gp);
    // initialize static memory
    DEBUG(bss_start);
    size_t bss_base = MEM_GLOBAL_BASE + (bss_start - gp);
    memset((void *)bss_base, 0, bss_end - bss_start);
    // set new global pointer
    DEBUG(bss_end);
    asm volatile ("la $gp, %0" : : "i"(MEM_GLOBAL_BASE + 32768));
}

void KernelMain(size_t gp, size_t bss_start, size_t bss_end) {
    // initialize memory
    InitialMemory(gp, bss_start, bss_end);
    // initialize UART controller with 230400 baud rate
    InitUART(230400);
    // get GPIO switch status
    if (((~GPIO_SWITCH) & 0x01)) {
        InitSystemFromDisk((void *)0x80000000, 0);
    }
    else {
        InitShell();
    }
}

void ExceptionHandler() {
    size_t debug, gp, sp, ra, badv, status, cause, epc;
    debug = GPIO_NUM;
    asm volatile ("move %0, $gp" : "=r"(gp));
    asm volatile ("move %0, $sp" : "=r"(sp));
    asm volatile ("move %0, $ra" : "=r"(ra));
    asm volatile ("mfc0 %0, $8" : "=r"(badv));
    asm volatile ("mfc0 %0, $12" : "=r"(status));
    asm volatile ("mfc0 %0, $13" : "=r"(cause));
    asm volatile ("mfc0 %0, $14" : "=r"(epc));
    for (;;) {
        if (((~GPIO_SWITCH) & (1 << 6))) {
            // display last debug info
            GPIO_NUM = debug;
        }
        else if (((~GPIO_SWITCH) & (1 << 5))) {
            // display global pointer
            GPIO_NUM = gp;
        }
        else if (((~GPIO_SWITCH) & (1 << 4))) {
            // display stack pointer
            GPIO_NUM = sp;
        }
        else if (((~GPIO_SWITCH) & (1 << 3))) {
            // display return address
            GPIO_NUM = ra;
        }
        else if (((~GPIO_SWITCH) & (1 << 2))) {
            // display CP0.BadVAddr
            GPIO_NUM = badv;
        }
        else if (((~GPIO_SWITCH) & (1 << 1))) {
            // display CP0.Status
            GPIO_NUM = status;
        }
        else if (((~GPIO_SWITCH) & (1 << 0))) {
            // display CP0.EPC
            GPIO_NUM = epc;
        }
        else {
            // display CP0.Cause
            GPIO_NUM = cause;
        }
    }
}
