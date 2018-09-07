#ifndef UBW_STRING_H_
#define UBW_STRING_H_

#include "type.h"

char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t count);
char *strcat(char *dest, const char *src);
size_t strlen(const char *str);
int strcmp(const char *lhs, const char *rhs);
int strncmp(const char *lhs, const char *rhs, size_t count);
char *strchr(const char *str, int ch);
char *strrchr(const char *str, int ch);

#define RANGE(v, l, r) ((v) >= (l) && (v) <= (r))

static inline int isalnum(int ch) {
    return RANGE(ch, '0', '9') || RANGE(ch, 'a', 'z')
            || RANGE(ch, 'A', 'Z');
}

static inline int isalpha(int ch) {
    return RANGE(ch, 'a', 'z') || RANGE(ch, 'A', 'Z');
}

static inline int islower(int ch) {
    return RANGE(ch, 'a', 'z');
}

static inline int isupper(int ch) {
    return RANGE(ch, 'A', 'Z');
}

static inline int isdigit(int ch) {
    return RANGE(ch, '0', '9');
}

static inline int isxdigit(int ch) {
    return RANGE(ch, '0', '9') || RANGE(ch, 'a', 'f')
            || RANGE(ch, 'A', 'F');
}

static inline int iscntrl(int ch) {
    return RANGE(ch, '\0', '\x1F') || ch == '\x7F';
}

static inline int isgraph(int ch) {
    return RANGE(ch, '!', '~');
}

static inline int isspace(int ch) {
    return RANGE(ch, '\t', '\r') || ch == ' ';
}

static inline int isblank(int ch) {
    return ch == '\t' || ch == ' ';
}

static inline int isprint(int ch) {
    return RANGE(ch, ' ', '~');
}

static inline int ispunct(int ch) {
    return RANGE(ch, '!', '/') || RANGE(ch, ':', '@')
            || RANGE(ch, '[', '`') || RANGE(ch, '{', '~');
}

static inline int tolower(int ch) {
    return RANGE(ch, 'A', 'Z') ? ch - 'A' + 'a' : ch;
}

static inline int toupper(int ch) {
    return RANGE(ch, 'a', 'z') ? ch - 'a' + 'A' : ch;
}

#undef RANGE

int atoi(const char *str);
long atol(const char *str);
long strtol(const char *str, char **str_end, int base);
unsigned long strtoul(const char *str, char **str_end, int base);

#endif // UBW_STRING_H_
