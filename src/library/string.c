#include "../include/string.h"
#include "../include/limits.h"

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

int atoi(const char *str) {
    return (int)strtol(str, NULL, 10);
}

long atol(const char *str) {
    return strtol(str, NULL, 10);
}

long strtol(const char *str, char **str_end, int base) {
    char *ptr = str;
    int ch, neg = 0;
    // skip spaces
    do {
        ch = *ptr++;
    } while (isspace(ch));
    // handle the sign
    if (ch == '-') {
        neg = 1;
        ch = *ptr++;
    }
    else if (ch == '+') {
        ch = *ptr++;
    }
    // handle zero 
    if ((base == 0 || base == 16) && ch == '0'
            && (*ptr == 'x' || *ptr == 'X')) {
        // skip "0x"
        ch = ptr[1];
        ptr += 2;
        // hexadecimal string
        base = 16;
    }
    // auto-detect the base
    if (base == '0') {
        base = ch == '0' ? 8 : 10;
    }
    /*
     * Compute the cutoff value between legal numbers and illegal
     * numbers.  That is the largest legal value, divided by the
     * base.  An input number that is greater than this value, if
     * followed by a legal input character, is too big.  One that
     * is equal to this value may be valid or not; the limit
     * between valid and invalid numbers is then based on the last
     * digit.  For instance, if the range for longs is
     * [-2147483648..2147483647] and the input base is 10,
     * cutoff will be set to 214748364 and cutlim to either
     * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
     * a value > 214748364, or equal but the next digit is > 7 (or 8),
     * the number is too big, and we will return a range error.
     *
     * Set any if any `digits' consumed; make it negative to indicate
     * overflow.
     */
    unsigned long cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
    int cutlim = cutoff % (unsigned long)base;
    cutoff /= (unsigned long)base;
    int acc, any;
    for (acc = 0, any = 0; ; ch = *ptr++) {
        if (isdigit(ch)) {
            ch -= '0';
        }
        else if (isalpha(ch)) {
            ch -= isupper(ch) ? 'A' - 10 : 'a' - 10;
        }
        else {
            break;
        }
        if (ch >= base) break;
        if (any < 0 || acc > cutoff || (acc == cutoff && ch > cutlim)) {
            any = -1;
        }
        else {
            any = 1;
            acc *= base;
            acc += ch;
        }
    }
    if (any < 0) {
        acc = neg ? LONG_MIN : LONG_MAX;
    }
    else if (neg) {
        acc = -acc;
    }
    if (str_end) *str_end = (char *)(any ? ptr - 1 : str);
    return acc;
}

unsigned long strtoul(const char *str, char **str_end, int base) {
    // similar to the above
    char *ptr = str;
    int ch, neg = 0;
    do {
        ch = *ptr++;
    } while (isspace(ch));
    if (ch == '-') {
        neg = 1;
        ch = *ptr++;
    }
    else if (ch == '+') {
        ch = *ptr++;
    }
    if ((base == 0 || base == 16) && ch == '0'
            && (*ptr == 'x' || *ptr == 'X')) {
        ch = ptr[1];
        ptr += 2;
        base = 16;
    }
    if (base == '0') {
        base = ch == '0' ? 8 : 10;
    }
    unsigned long cutoff = (unsigned long)ULONG_MAX / (unsigned long)base;
    int cutlim = (unsigned long)ULONG_MAX % (unsigned long)base;
    int acc, any;
    for (acc = 0, any = 0; ; ch = *ptr++) {
        if (isdigit(ch)) {
            ch -= '0';
        }
        else if (isalpha(ch)) {
            ch -= isupper(ch) ? 'A' - 10 : 'a' - 10;
        }
        else {
            break;
        }
        if (ch >= base) break;
        if (any < 0 || acc > cutoff || (acc == cutoff && ch > cutlim)) {
            any = -1;
        }
        else {
            any = 1;
            acc *= base;
            acc += ch;
        }
    }
    if (any < 0) {
        acc = ULONG_MAX;
    }
    else if (neg) {
        acc = -acc;
    }
    if (str_end) *str_end = (char *)(any ? ptr - 1 : str);
    return acc;
}
