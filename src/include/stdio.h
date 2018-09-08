#ifndef UBW_INCLUDE_STDIO_H_
#define UBW_INCLUDE_STDIO_H_

#define EOF (-1)

#define STDIO_UART

int putchar(int ch);
int getchar();
int puts(const char *str);
char *gets(char *str);
int printf(const char *format, ...);

#endif // UBW_INCLUDE_STDIO_H_
