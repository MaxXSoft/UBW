#ifndef UBW_INCLUDE_MEM_H_
#define UBW_INCLUDE_MEM_H_

#include "type.h"

#ifndef MEM_HEAP_BASE
#define MEM_HEAP_BASE 0x84000000
#endif

#ifndef MEM_HEAP_SIZE
#define MEM_HEAP_SIZE 0x100000
#endif

void *malloc(size_t length);
void free(void *ptr);
void *memset(void *dest, uint8_t byte, size_t length);
void *memcpy(void *dest, const void *src, size_t length);
int memcmp(const void *lhs, const void *rhs, size_t length);

#endif // UBW_INCLUDE_MEM_H_
