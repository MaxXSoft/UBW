#ifndef UBW_INCLUDE_SOC_H_
#define UBW_INCLUDE_SOC_H_

// CPU
#define CPU_CLOCK   60000000

// GPIO
#define GPIO_SWITCH (*(volatile unsigned *)0xbfd05000)
#define GPIO_KEYPAD (*(volatile unsigned *)0xbfd05004)
#define GPIO_BILED0 (*(volatile unsigned *)0xbfd05008)
#define GPIO_BILED1 (*(volatile unsigned *)0xbfd0500c)
#define GPIO_LED    (*(volatile unsigned *)0xbfd05010)
#define GPIO_NUM    (*(volatile unsigned *)0xbfd05014)
#define GPIO_TIMER  (*(volatile unsigned *)0xbfd05018)
#define GPIO_BILED_OFF      0
#define GPIO_BILED_RED      1
#define GPIO_BILED_GREEN    2
#define GPIO_BILED_REDGREEN 3

// VGA
// base address of video memory
#define VGA_AR      (*(volatile unsigned *)0xbfd03000)
// control the scanning mode & display on/off
#define VGA_CR      (*(volatile unsigned *)0xbfd03004)
#define VGA_CR_DPS  (1 << 1)   // rotate screen 180 degrees
#define VGA_CR_TDE  (1 << 0)   // enable display

// UART
// buffer register (receive/transmit data)
#define UART_BUF    (*(volatile unsigned *)0xbfd01000)
// interrupt enable register
#define UART_IER    (*(volatile unsigned *)0xbfd01004)
// FIFO control register
// write only when !UART.LCR[7], read only when UART.LCR[7]
#define UART_FCR    (*(volatile unsigned *)0xbfd01008)
// line control register
#define UART_LCR    (*(volatile unsigned *)0xbfd0100c)
// modem control register
#define UART_MCR    (*(volatile unsigned *)0xbfd01010)
// line status register
#define UART_LSR    (*(volatile unsigned *)0xbfd01014)
// divisor latch register (LSB), enable when UART.LCR[7]
#define UART_DLL    (*(volatile unsigned *)0xbfd01000)
// divisor latch register (MSB), enable when UART.LCR[7]
#define UART_DLM    (*(volatile unsigned *)0xbfd01004)
#define UART_LCR_8B   3          // word length: 8 bits per character
#define UART_LSR_DR   (1 << 0)   // data_ready
#define UART_LSR_THRE (1 << 5)   // data_ready

typedef struct SoCInfoPrototype {
    const char *cpu_name;
    size_t cpu_clock;       // MHz
    size_t bus_clock;       // MHz
    size_t mem_size;        // MiB
} SoCInfo;

inline SoCInfo GetSoCInfo() {
    SoCInfo soc_info;
    soc_info.cpu_name = "Uranus Zero";
    soc_info.cpu_clock = 60;
    soc_info.bus_clock = 60;
    soc_info.mem_size = 128;
}

#endif // UBW_INCLUDE_SOC_H_
