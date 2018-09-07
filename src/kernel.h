#ifndef UBW_KERNEL_H_
#define UBW_KERNEL_H_

#include "include/type.h"

const char *GetCurrentSeg();

void WriteDisk(void *memory, size_t length, size_t disk_pos);
void InitSystemFromDisk(size_t disk_pos);
void OverrideSPI(const void *memory, size_t length);
void LoadMemoryFromUART(void *memory, size_t length);

#endif // UBW_KERNEL_H_
