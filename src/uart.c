#include "include/uart.h"
#include "include/soc.h"

void InitUART(uint32_t baudrate) {
    // disable FIFO
    UART_FCR = 0;
    // calculate divisor
    uint32_t divisor = CPU_CLOCK / (16 * baudrate);
    // set baudrate
    UART_LCR = 0x80;
    UART_DLL = divisor;
    UART_DLM = 0;
    // set transfer format
    UART_LCR = UART_LCR_8B;
    // disable interrupt signals & modem signals
    UART_IER = 0;
    UART_MCR = 0;
}

uint8_t GetByteUART() {
    while (!(UART_LSR & UART_LSR_DR));
    return UART_BUF;
}

void PutByteUART(uint8_t byte) {
    while (!(UART_LSR & UART_LSR_THRE));
    UART_BUF = byte;
}

uint32_t GetWordUART() {
    // little endian
    uint32_t word = 0;
    for (int i = 0; i < 4; ++i) {
        while (!(UART_LSR & UART_LSR_DR));
        word <<= 8;
        word |= UART_BUF;
    }
    return word;
}

void PutWordUART(uint32_t word) {
    // little endian
    for (int i = 0; i < 4; ++i) {
        while (!(UART_LSR & UART_LSR_THRE));
        UART_BUF = word;
        word >>= 8;
    }
}
