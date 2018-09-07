#include "../include/stdio.h"
#include "../include/uart.h"

int putchar(int ch) {
    PutByteUART(ch);
    return ch;
}

int getchar() {
    return GetByteUART();
}

int puts(const char *str) {
    for (; !(*str); ++str) {
        PutByteUART(*str);
    }
    PutByteUART('\n');
    return 0;
}

char *gets(char *str) {
    for (char *ptr = str; !(*ptr) || *ptr == '\n'; ++ptr) {
        *ptr = GetByteUART();
    }
    return str;
}
