#include "../include/string.h"

char *strcpy(char *dest, const char *src) {
    if (!dest) return NULL;
    char *ptr = dest;
    *ptr = '\0';
    if (!src) return dest;
    while ((*ptr++ = *src++));
    return dest;
}

char *strncpy(char *dest, const char *src, size_t count) {
    if (!dest || !src) return NULL;
    char *ptr = dest;
    for (; *src && count; ++ptr, ++src, --count) *ptr = *src;
    while (count--) *ptr++ = '\0';
    return dest;
}

char *strcat(char *dest, const char *src) {
    if (!dest || !src) return NULL;
    char *ptr = dest;
    for (; *ptr; ++ptr);
    for (; *src; ++src) *ptr++ = *src;
    *ptr = '\0';
    return dest;
}

size_t strlen(const char *str) {
    size_t len = 0;
    while (*str++) ++len;
    return len;
}

int strcmp(const char *lhs, const char *rhs) {
    while (*lhs == *rhs++) {
        if (*lhs++ == '\0') return 0;
    }
    return (*(const uint8_t *)lhs - *(const uint8_t *)--rhs);
}

int strncmp(const char *lhs, const char *rhs, size_t count) {
    if (!lhs || !rhs) return 0;
    while (count && *lhs == *rhs) {
        if (*lhs == '\0') return 0;
        ++lhs;
        ++rhs;
        --count;
    }
    return count ? *lhs - *rhs : 0;
}

char *strchr(const char *str, int ch) {
    if (!str) return NULL;
    for (; *str; ++str) {
        if (*str == ch) return str;
    }
    return NULL;
}

char *strrchr(const char *str, int ch) {
    if (!str) return NULL;
    char *ptr = NULL;
    for (; *str; ++str) {
        if (*str == ch) ptr = str;
    }
    return ptr;
}
