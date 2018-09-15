// reference: pmon_archlab/Targets/LS1B/dev/spi_w.c

#include "../include/spi.h"

// base address of SPI controller
#define SPI_ADDR_BASE   0xbfcf0000

// address offset of control registers
#define SPI_CR_SPCR     0x0
#define SPI_CR_SPSR     0x1
#define SPI_CR_TXFIFO   0x2
#define SPI_CR_RXFIFO   0x2
#define SPI_CR_FIFO     0x2
#define SPI_CR_SPER     0x3
#define SPI_CR_PARAM    0x4
#define SPI_CR_SOFTCS   0x5
#define SPI_CR_PARAM2   0x6

// constants
#define SPI_RFEMPTY     1
#define SPI_PAGE_SIZE   256

// send/receive command
#define SPI_SET(a, v)   *((volatile uint8_t *)(SPI_ADDR_BASE + a)) = (v)
#define SPI_GET(a)      *((volatile uint8_t *)(SPI_ADDR_BASE + a))

// wrap of common complex operations
#define SPI_CS_L()      SPI_SET(SPI_CR_SOFTCS, 0x01)
#define SPI_CS_H()      SPI_SET(SPI_CR_SOFTCS, 0x11)

#define SPI_SEND_DATA(x)                                             \
    do {                                                             \
        SPI_SET(SPI_CR_TXFIFO, (x));                                 \
        while ((SPI_GET(SPI_CR_SPSR) & SPI_RFEMPTY) == SPI_RFEMPTY); \
        SPI_GET(SPI_CR_RXFIFO);                                      \
    } while (0)

#define SPI_RECV_DATA(x)                                             \
    do {                                                             \
        SPI_SET(SPI_CR_TXFIFO, 0xff);                                \
        while ((SPI_GET(SPI_CR_SPSR) & SPI_RFEMPTY) == SPI_RFEMPTY); \
        (x) = SPI_GET(SPI_CR_RXFIFO);                                \
    } while (0)

// internal functions
static void InitWrite() {
  	SPI_SET(SPI_CR_SPSR, 0xc0);
  	SPI_SET(SPI_CR_PARAM, 0x40);
 	SPI_SET(SPI_CR_SPER, 0x05);
  	SPI_SET(SPI_CR_PARAM2,0x01);
  	SPI_SET(SPI_CR_SPCR, 0x50);
}

static void InitRead() {
  	SPI_SET(SPI_CR_PARAM, 0x47);
}

static int WaitStatusRegister() {
    uint8_t value;
    SPI_CS_L();
    SPI_SEND_DATA(0x05);
    do {
        SPI_RECV_DATA(value);
    } while (value & 0x01);
    SPI_CS_H();
    return value;
}

static void SetWriteEnable() {
    uint8_t value;
    WaitStatusRegister();
    SPI_CS_L();
    SPI_SEND_DATA(0x6);
    SPI_CS_H();
    SPI_CS_L();
    SPI_SEND_DATA(0x05);
    do {
        SPI_RECV_DATA(value);
    } while (!(value & 0x02));
    SPI_CS_H();
}

static void WriteStatusRegister(uint8_t status) {
    SetWriteEnable();
    WaitStatusRegister();
    SPI_CS_L();
    SPI_SEND_DATA(0x1);
    SPI_SEND_DATA(status);
    SPI_CS_H();
}

static int ReadStatusRegister() {
    uint8_t value;
    SPI_CS_L();
    SPI_SEND_DATA(0x05);
    SPI_RECV_DATA(value);
    SPI_CS_H();
    return value;
}

static void WriteByteSPI(uint32_t addr, uint8_t data) {
    // get different segment of flash address
    uint8_t addr2 = (addr & 0xff0000) >> 16;
    uint8_t addr1 = (addr & 0x00ff00) >> 8;
    uint8_t addr0 = addr & 0x0000ff;
    // send command
    SetWriteEnable();
    WaitStatusRegister();
    SPI_CS_L();
    SPI_SEND_DATA(0x2);
    SPI_SEND_DATA(addr2);
    SPI_SEND_DATA(addr1);
    SPI_SEND_DATA(addr0);
    SPI_SEND_DATA(data);
    SPI_CS_H();
}

static void WritePageSPI(uint32_t addr, uint8_t *buffer, uint32_t count) {
    // get different segment of flash address
    uint8_t addr2 = (addr & 0xff0000) >> 16;
    uint8_t addr1 = (addr & 0x00ff00) >> 8;
    uint8_t addr0 = addr & 0x0000ff;
    uint32_t real_count = count < SPI_PAGE_SIZE ? count : SPI_PAGE_SIZE;
    // sned command
    SetWriteEnable();
    WaitStatusRegister();
    SPI_CS_L();
    SPI_SEND_DATA(0x2);
    SPI_SEND_DATA(addr2);
    SPI_SEND_DATA(addr1);
    SPI_SEND_DATA(addr0);
    for (int i = 0; i < real_count; ++i) {
        SPI_SEND_DATA(buffer[i]);
    }
    SPI_CS_H();
}

// external functions
void EraseSPI() {
    // send command
    InitWrite();
    WriteStatusRegister(0);
    SetWriteEnable();
    WaitStatusRegister();
    SPI_CS_L();
    SPI_SEND_DATA(0xc7);
    SPI_CS_H();
    // wait until finish
    while (ReadStatusRegister() & 0x3);
    InitRead();
}

void EraseAreaSPI(uint32_t addr_start, uint32_t addr_end) {
    const uint32_t sector_size = 0x10000;
    InitWrite();
    for(uint32_t i = addr_start; i < addr_end; i += sector_size) {
        SetWriteEnable();
        WaitStatusRegister();
        SPI_CS_L();
        SPI_SEND_DATA(0xd8);
        SPI_SEND_DATA(i >> 16);
        SPI_SEND_DATA(i >> 8);
        SPI_SEND_DATA(i);
        SPI_CS_H();
    }
    WaitStatusRegister();
    InitRead();
}

void WriteAreaSPI(uint32_t addr, uint8_t *buffer, size_t length, int page) {
    InitWrite();
    WriteStatusRegister(0x00);
    if (page) {   // per page
        // write extra bytes (unable to align by page)
        for (int i = 0; length > 0 && (addr & (SPI_PAGE_SIZE - 1));
                ++addr, --length, ++i) {
            WriteByteSPI(addr, *buffer++);
        }
        // write pages
        while (length >= SPI_PAGE_SIZE) {
            WritePageSPI(addr, buffer, SPI_PAGE_SIZE);
            buffer += SPI_PAGE_SIZE;
            addr += SPI_PAGE_SIZE;
            length -= SPI_PAGE_SIZE;
        }
        // write remaining bytes
        WritePageSPI(addr, buffer, length);
    }
    else {   // per byte
        for(int i = 0; length > 0; ++addr, --length, ++i) {
            WriteByteSPI(addr, buffer[i]);
        }
    }
    WaitStatusRegister();
    InitRead();
}

void ReadAreaSPI(uint32_t addr, uint8_t *buffer, size_t length) {
    // get different segment of flash address
    uint8_t addr2 = (addr & 0xff0000) >> 16;
    uint8_t addr1 = (addr & 0x00ff00) >> 8;
    uint8_t addr0 = addr & 0x0000ff;
    // send command
    InitWrite();
    SPI_CS_L();
    SPI_SEND_DATA(0x03);
    SPI_SEND_DATA(addr2);
    SPI_SEND_DATA(addr1);
    SPI_SEND_DATA(addr0);
    for(int i = 0; i < length; ++i) {
        SPI_RECV_DATA(buffer[i]);
    }
    SPI_CS_H();
    InitRead();
}
