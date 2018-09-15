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
#include "include/spi.h"
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

void WriteDisk(const void *memory, size_t length, size_t disk_pos) {
    // memory & length must be a multiple of 4
    if (((size_t)memory & 0x3) || (length & 0x3)) return;
    // erase area
    uint32_t start_addr = DISK_START_ADDR + disk_pos;
    EraseAreaSPI(start_addr, start_addr + length);
    // write area
    WriteAreaSPI(start_addr, (uint8_t *)memory, length, 1);
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
    typedef void (*SystemEntry)();
    SystemEntry entry = (SystemEntry)memory;
    entry();
}

void OverrideSPI(const void *memory, size_t length, uint32_t step) {
    if (!step) {   // step 1
        // only can override boot sector
        if (length > BOOT_SECTOR_SIZE) return;
        // memory & length must be a multiple of 4
        if (((size_t)memory & 0x3) || (length & 0x3)) return;
        // copy UBW to new address
        uint8_t *buffer = (uint8_t *)malloc(BOOT_SECTOR_SIZE);
        memcpy(buffer, (void *)FLASH_START_ADDR, BOOT_SECTOR_SIZE);
        // caculate the address of new entry
        uint32_t entry_addr = (uint32_t)&OverrideSPI;
        entry_addr -= FLASH_START_ADDR;
        entry_addr += (uint32_t)buffer;
        // jump to next step
        typedef void (*OverrideEntry)(const void *, size_t, uint32_t);
        OverrideEntry entry = (OverrideEntry)entry_addr;
        entry(memory, length, (uint32_t)buffer);
    }
    else {   // step 2
        // override
        EraseAreaSPI(FLASH_START_ADDR, DISK_START_ADDR);
        WriteAreaSPI(FLASH_START_ADDR, (uint8_t *)memory,
                BOOT_SECTOR_SIZE, 1);
        // free the buffer
        free((void *)step);
        // do not return
        for (;;);
    }
}

void LoadMemoryFromUART(void *memory, size_t length) {
    // memory & length must be a multiple of 4
    if (((size_t)memory & 0x3) || (length & 0x3)) return;
    uint32_t *ptr = memory, word;
    for (int i = 0; i < (length >> 2); ++i) {
        word = GetWordUART();
        DEBUG(word);
        ptr[i] = word;
    }
}

// calculate CRC for Xmodem protocol
static uint16_t CalcCRC(uint8_t *buffer, size_t length) {
    uint16_t crc = 0;
    while (length--) {
        crc = crc ^ ((uint16_t)*buffer++ << 8);
        for (int i = 0; i < 8; ++i) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            }
            else {
                crc = crc << 1;
            }
        }
    }
    return crc;
}

int LoadMemoryFromXmodem(void *memory) {
    size_t status = 0, len, crc;
    uint8_t byte, data[1024], *p = memory;
    const uint8_t kSOH = '\x01', kSTX = '\x02', kEOT = '\x04',
            kACK = '\x06', kCAN = '\x18';
    // send request
    for (;;) {
        PutByteUART('C');   // CRC mode
        DelayMillisecond(500);
        if ((UART_LSR & UART_LSR_DR)) {
            byte = UART_BUF;
            break;
        }
    }
    // process data blocks
    for (;;) {
        // check block header
        if (byte == kEOT) {
            break;   // end of transmission
        }
        else if (byte == kSOH) {
            len = 128;   // 128B mode
        }
        else if (byte == kSTX) {
            len = 1024;   // 1K mode
        }
        else {
            DEBUG(byte);
            status = -1;   // error
            break;
        }
        // get & check package number
        byte = GetByteUART();
        if ((GetByteUART() & 0xff) != ((~byte) & 0xff)) {
            status = -2;   // error
            break;
        }
        // receive data block
        for (int i = 0; i < len; ++i) data[i] = GetByteUART();
        // receive CRC
        crc = (GetByteUART() << 8) | GetByteUART();
        // check data block
        if (CalcCRC(data, len) != crc) {
            DEBUG(crc);
            status = -3;   // error
            break;
        }
        // write memory
        for (int i = 0; i < len; ++i) p[i] = data[i];
        p += len;
        status += len;
        // accept & get next header
        PutByteUART(kACK);
        byte = GetByteUART();
    }
    if (status < 0) {
        // reject
        PutByteUART(kCAN);
    }
    else {
        // accept
        PutByteUART(kACK);
        // delay
        DelayMillisecond(500);
    }
    return status;
}

static void InitSystemFromDisk() {
    // jump to disk address (offset = 0)
    InitSystemFromMemory((void *)DISK_START_ADDR);
}

static void InitialMemory(size_t gp, size_t bss_start, size_t bss_end) {
    // move global memory to new address space
    gp -= 32768;
    memcpy((void *)MEM_GLOBAL_BASE, (void *)gp, bss_start - gp);
    // initialize static memory
    size_t bss_base = MEM_GLOBAL_BASE + (bss_start - gp);
    memset((void *)bss_base, 0, bss_end - bss_start);
    // set new global pointer
    asm volatile ("la $gp, %0" : : "i"(MEM_GLOBAL_BASE + 32768));
}

void KernelMain(size_t gp, size_t bss_start, size_t bss_end) {
    // initialize memory
    InitialMemory(gp, bss_start, bss_end);
    // initialize UART controller
    InitUART((~GPIO_SWITCH) & 0x80 ? 115200 : 230400);
    // get GPIO switch status
    if (((~GPIO_SWITCH) & 0x01)) {
        // boot from disk
        InitSystemFromDisk();
    }
    else if (((~GPIO_SWITCH) & 0x02)) {
        // boot from UART
        GPIO_BILED0 = GPIO_BILED_GREEN;
        GPIO_BILED1 = GPIO_BILED_OFF;
        size_t length = GetWordUART();
        DEBUG(length);
        LoadMemoryFromUART((void *)0x80000000, length);
        GPIO_BILED0 = GPIO_BILED_OFF;
        GPIO_NUM = 0;
        InitSystemFromMemory((void *)0x80000000);
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
