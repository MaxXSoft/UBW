#ifndef UBW_KERNEL_H_
#define UBW_KERNEL_H_

#include "include/type.h"

const char *GetCurrentSeg();

void ReadDisk(void *memory, size_t length, size_t disk_pos);
void WriteDisk(const void *memory, size_t length, size_t disk_pos);
void InitSystemFromMemory(const void *memory);
void OverrideSPI(const void *memory, size_t length);
void LoadMemoryFromUART(void *memory, size_t length);
void LoadMemoryFromXmodem(void *memory);

#endif // UBW_KERNEL_H_
