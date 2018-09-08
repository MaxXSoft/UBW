#include "../include/stdio.h"
#include "../include/uart.h"
#include "../include/type.h"
#include "../include/debug.h"

int putchar(int ch) {
    PutByteUART(ch);
    return ch;
}

int getchar() {
    return GetByteUART();
}

int puts(const char *str) {
    for (; *str; ++str) PutByteUART(*str);
#ifdef STDIO_UART
    PutByteUART('\r');
#endif
    PutByteUART('\n');
    return 0;
}

char *gets(char *str) {
    char *ptr = str - 1;
    do {
        *++ptr = GetByteUART();
    } while (*ptr && *ptr != '\n' && *ptr != '\r');
    *ptr = '\0';
    return str;
}

static void PrintInteger(int num, int base, int sign) {
    // get value
    unsigned int value;
    if (sign && num < 0) {
        value = -num;
        PutByteUART('-');
    }
    else {
        value = num;
    }
    // convert value
    char buffer[64];
    int i;
    for (i = 0; value; ++i) {
        buffer[i] = value % base;
        value /= base;
    }
    // print value
    for (--i; i >= 0; --i) {
        char c = buffer[i];
        PutByteUART(c <= 9 ? c + '0' : c - 10 + 'a');
    }
}

int printf(const char *format, ...) {
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    void **arg = ap;
    for (int i = 0; format[i]; ++i) {
        char c = format[i];
        if (c == '%') {
            // escape characters
            switch (format[i + 1]) {
                case 's': {
                    const char *str = *arg;
                    for (; *str; ++str) PutByteUART(*str);
                    ++arg;
                    ++i;
                    break;
                }
                case 'c': {
                    PutByteUART((int)*arg);
                    ++arg;
                    ++i;
                    break;
                }
                case 'u': {
                    PrintInteger((int)*arg, 10, 0);
                    ++arg;
                    ++i;
                    break;
                }
                case 'd': {
                    PrintInteger((int)*arg, 10, 1);
                    ++arg;
                    ++i;
                    break;
                }
                case 'o': {
                    PrintInteger((int)*arg, 8, 0);
                    ++arg;
                    ++i;
                    break;
                }
                case 'b': {
                    PrintInteger((int)*arg, 2, 0);
                    ++arg;
                    ++i;
                    break;
                }
                case 'p': case 'x': {
                    PrintInteger((int)*arg, 16, 0);
                    ++arg;
                    ++i;
                    break;
                }
                case '%': {
                    PutByteUART('%');
                    ++i;
                    break;
                }
            }
        }
        else {
            PutByteUART(c);
        }
    }
    return 0;
}
