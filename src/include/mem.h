#ifndef UBW_MEM_H_
#define UBW_MEM_H_

#include "type.h"

#ifndef MEM_HEAP_BASE
#define MEM_HEAP_BASE  0x04000000
#endif

void *malloc(size_t length);
void free(void *ptr);
void *memset(void *dest, uint8_t byte, size_t length);
void *memcpy(void *dest, const void *src, size_t length);
int memcmp(const void *lhs, const void *rhs, size_t length);

#endif // UBW_MEM_H_
