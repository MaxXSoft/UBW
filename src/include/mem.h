#ifndef UBW_INCLUDE_MEM_H_
#define UBW_INCLUDE_MEM_H_

#include "type.h"

/*
        Memory Map of UBW (kseg0)

        +-------------------+
  9FFF  |                   |
        |    unallocated    |
  9FE0  |                   |
        +-------------------+
  9FDF  |                   |
        |    peripherals    |
  9FD0  |                   |
        +-------------------+
  9FCF  |                   |
        |        ROM        |
        |  (.text & .data)  |
  9FC0  |                   |
        +-------------------+
  9FBF  |                   |
        |    unallocated    |
  8600  |                   |
        +-------------------+
  85FF  |                   |
        |     video mem     |
  8500  |                   |
        +-------------------+
  84FF  |                   |
        |       heap        |
  8400  |                   |
        +-------------------+
  83FF  |                   |
        |       stack       |
  8300  |                   |
        +-------------------+
  82FF  |    global mem     |
        +-------------------+
  82FE  |                   |
        |       free        |
  8000  |                   |
        +-------------------+
*/

#ifndef MEM_HEAP_BASE
#define MEM_HEAP_BASE 0x84000000
#endif

#ifndef MEM_HEAP_SIZE
#define MEM_HEAP_SIZE 0x100000
#endif

#ifndef MEM_GLOBAL_BASE
#define MEM_GLOBAL_BASE 0x82FF0000
#endif

#define MEM_GLOBAL_SIZE 0x0000FFFF

void *malloc(size_t length);
void free(void *ptr);
void *memset(void *dest, uint8_t byte, size_t length);
void *memcpy(void *dest, const void *src, size_t length);
int memcmp(const void *lhs, const void *rhs, size_t length);

#endif // UBW_INCLUDE_MEM_H_
