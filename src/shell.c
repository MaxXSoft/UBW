#include "shell.h"

#include "include/soc.h"
#include "include/uart.h"
#include "include/stdio.h"
#include "include/string.h"
#include "include/mem.h"
#include "include/debug.h"

#include "kernel.h"

typedef void (*CommandFuncPtr)(const char *, const char *,
        const char *, const char *);

typedef struct CommandPrototype {
    const char *name;
    size_t name_len;
    size_t arg_len;
    CommandFuncPtr handler_pointer;
} Command;

typedef enum LoadTypePrototype {
    kLoadUART, kLoadXmodem
} LoadType;

static void PrintAbout();
static void Echo(const char *str);
static void Clear();
static void LoadData(const char *type, const char *base_addr,
        const char *length);
static void WriteFlash(const char *base_addr, const char *length,
        const char *disk_pos);
static void InitSystem(const char *base_addr);
static void Override(const char *base_addr, const char *length);
static void Peep(const char *base_addr, const char *length);

Command command_list[] = {
    {"about", 5, 0, (CommandFuncPtr)&PrintAbout},
    {"echo", 4, 1, (CommandFuncPtr)&Echo},
    {"clear", 5, 0, (CommandFuncPtr)&Clear},
    {"load", 4, 3, (CommandFuncPtr)&LoadData},
    {"write", 5, 3, (CommandFuncPtr)&WriteFlash},
    {"init", 4, 1, (CommandFuncPtr)&InitSystem},
    {"override", 8, 2, (CommandFuncPtr)&Override},
    {"peep", 4, 2, (CommandFuncPtr)&Peep},
};

static void PrintAbout() {
    puts("Unlimited Boot Works, version 0.01");
    puts("Copyright (C) 2010-2018 MaxXSoft MaxXing.");
    puts("Copyright (C) 2018 USTB NSCSCC Team.\n");
    puts("System Information:");
    SoCInfo info = GetSoCInfo();
    printf("CPU %s @ %u MHz / Bus @ %u MHz\r\n",
            info.cpu_name, info.cpu_clock, info.bus_clock);
    printf("Memory size %u MB, current segment is ", info.mem_size);
    puts(GetCurrentSeg());
}

static void Echo(const char *str) {
    if (str) puts(str);
}

static void Clear() {
    printf("\033[H\033[J");
}

static void LoadData(const char *type, const char *base_addr,
        const char *length) {
    LoadType t = (LoadType)strtol(type, NULL, 10);
    void *base = (void *)strtoul(base_addr, NULL, 16);
    size_t len = length ? (size_t)strtoul(length, NULL, 10) : 0;
    if (t == kLoadUART) {
        LoadMemoryFromUART(base, len);
        puts("done");
    }
    else {   // kLoadXmodem
        int ret = LoadMemoryFromXmodem(base);
        puts("");
        puts("");
        if (ret < 0) {
            printf("data transfer error (code %d)\r\n", ret);
        }
        else {
            printf("accepted %d bytes of data\r\n", ret);
        }
    }
}

static void WriteFlash(const char *base_addr, const char *length,
        const char *disk_pos) {
    void *base = (void *)strtoul(base_addr, NULL, 16);
    size_t len = (size_t)strtoul(length, NULL, 10);
    size_t pos = disk_pos ? (size_t)strtoul(disk_pos, NULL, 10) : 0;
    WriteDisk(base, len, pos);
}

static void InitSystem(const char *base_addr) {
    void *base = (void *)strtoul(base_addr, NULL, 16);
    Clear();
    InitSystemFromMemory(base);
}

static void Override(const char *base_addr, const char *length) {
    void *base = (void *)strtoul(base_addr, NULL, 16);
    size_t len = (size_t)strtoul(length, NULL, 10);
    char input_buffer[16];
    puts("this operation may cause the SoC to fail to initialize");
    printf("continue? (y/n) ");
    if (!gets(input_buffer) || tolower(input_buffer[0]) != 'y') {
        puts("");
        return;
    }
    Clear();
    OverrideSPI(base, len, 0);
}

static void Peep(const char *base_addr, const char *length) {
    void *base = (void *)strtoul(base_addr, NULL, 16);
    size_t len = length ? (size_t)strtoul(length, NULL, 10) : 4;
    for (int i = 0; i < len; i += 4) {
        printf("%u: %u\r\n", base + i, *(uint32_t *)(base + i));
    }
}

static void REPL() {
    char input_buffer[128];
    for (;;) {
        // print prompt
        printf("UBW> ");
        // get user input
        if (!gets(input_buffer) || !input_buffer[0]) {
            puts("");
            continue;
        }
        // newline
        putchar('\n');
        // run command
        int bad = 1;
        for (int i = 0; i < sizeof(command_list) / sizeof(Command); ++i) {
            if (!strncmp(input_buffer, command_list[i].name,
                    command_list[i].name_len)) {
                char *p = input_buffer + command_list[i].name_len;
                if (*p && *p != ' ') break;
                char *alloc_ptrs[4] = {NULL};
                // read argument list
                for (int n = 0; n < command_list[i].arg_len && *p; ++n) {
                    // skip spaces
                    while (isspace(*p)) ++p;
                    int j;
                    // get next argument string
                    for (j = 0; p[j] && !isspace(p[j]); ++j);
                    // make pointer 'p' pointed a normal string
                    char c = p[j];
                    p[j] = '\0';
                    // allocate space
                    alloc_ptrs[n] = (char *)malloc(j + 1);
                    strcpy(alloc_ptrs[n], p);
                    // restore the content of string
                    p[j] = c;
                    p += j;
                }
                // call command
                command_list[i].handler_pointer(alloc_ptrs[0],
                        alloc_ptrs[1], alloc_ptrs[2], alloc_ptrs[3]);
                // free allocated pointers
                for (int i = 0; i < 4; ++i) {
                    if (alloc_ptrs[i]) free(alloc_ptrs[i]);
                }
                bad = 0;
                break;
            }
        }
        // specific command does not exist, print error message
        if (bad) printf("bad command: %s\r\n", input_buffer);
    }
}

void InitShell() {
    // green light
    GPIO_BILED0 = GPIO_BILED_GREEN;
    GPIO_BILED1 = GPIO_BILED_GREEN;
    // display init information
    PrintAbout();
    // switch to REPL
    REPL();
}
