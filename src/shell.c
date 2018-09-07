#include "include/soc.h"
#include "include/uart.h"
#include "include/stdio.h"
#include "include/debug.h"

#include "kernel.h"
#include "shell.h"

static void PrintAbout() {
    puts("Unlimited Boot Works, version 0.01");
    DEBUG(5);   // TODO: add more debug point
    puts("Copyright (C) 2010-2018 MaxXSoft MaxXing.");
    puts("Copyright (C) 2018 USTB NSCSCC Team.\n");
    puts("System Information:");
    SoCInfo info = GetSoCInfo();
    printf("CPU %s @ %u MHz / Bus @ %u MHz\n",
            info.cpu_name, info.cpu_clock, info.bus_clock);
    printf("Memory size %u MB, current segment is ", info.mem_size);
    puts(GetCurrentSeg());
}

static void SwitchToREPL() {
    char input_buffer[128];
    for (;;) {
        printf("UBW> ");
        gets(input_buffer);
        puts(input_buffer);
    }
}

void InitShell() {
    // green light
    GPIO_BILED0 = GPIO_BILED_GREEN;
    GPIO_BILED1 = GPIO_BILED_GREEN;
    // display init information
    PrintAbout();
    // switch to REPL
    SwitchToREPL();
}
