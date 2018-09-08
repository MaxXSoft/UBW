#include "../include/stdio.h"
#include "../include/type.h"
#include "../include/debug.h"

#if defined(STDIO_UART)
#include "../include/uart.h"
#define PUT_BYTE(b) PutByteUART(b)
#define GET_BYTE() GetByteUART()
#elif defined(STDIO_VGAPS2)
#define PUT_BYTE(b) ((void)0)
#define GET_BYTE(b) ((void)0)
#endif

int putchar(int ch) {
    PUT_BYTE(ch);
    return ch;
}

int getchar() {
    return GET_BYTE();
}

int puts(const char *str) {
    for (; *str; ++str) PUT_BYTE(*str);
    PUT_BYTE('\r');
    PUT_BYTE('\n');
    return 0;
}

char *gets(char *str) {
    char *ptr = str, c;
    do {
        c = GET_BYTE();
        *ptr++ = c;
        switch (c) {
            case '\x03': case '\x04': {
                // '^C' or '^D'
                return NULL;
            }
            case '\b': {
                if (ptr != str) --ptr;
                break;
            }
            default: {
                // echo input
                PUT_BYTE(c);
                break;
            }
        }
    } while (c && c != '\n' && c != '\r');
    *--ptr = '\0';
    return str;
}

static void PrintInteger(int num, int base, int sign) {
    // get value
    unsigned int value;
    if (sign && num < 0) {
        value = -num;
        PUT_BYTE('-');
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
        PUT_BYTE(c <= 9 ? c + '0' : c - 10 + 'a');
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
                    for (; *str; ++str) PUT_BYTE(*str);
                    ++arg;
                    ++i;
                    break;
                }
                case 'c': {
                    PUT_BYTE((int)*arg);
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
                    PUT_BYTE('%');
                    ++i;
                    break;
                }
            }
        }
        else {
            PUT_BYTE(c);
        }
    }
    return 0;
}
