#ifndef UBW_STRING_H_
#define UBW_STRING_H_

#include "type.h"

char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t count);
char *strcat(char *dest, const char *src);
char *strncat(char *dest, const char *src, size_t count);
size_t strlen(const char *str);
int strcmp(const char *lhs, const char *rhs);
int strncmp(const char *lhs, const char *rhs, size_t count);
char *strchr(const char *str, int ch);
char *strrchr(const char *str, int ch);

#endif // UBW_STRING_H_
