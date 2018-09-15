#ifndef UBW_KERNEL_H_
#define UBW_KERNEL_H_

#include "include/type.h"

#define FLASH_START_ADDR   0xbfc00000
#define BOOT_SECTOR_SIZE   (18 * 1024)   // 18KB
#define DISK_START_ADDR    (FLASH_START_ADDR + BOOT_SECTOR_SIZE)

const char *GetCurrentSeg();

void WriteDisk(const void *memory, size_t length, size_t disk_pos);
void InitSystemFromMemory(const void *memory);
void OverrideSPI(const void *memory, size_t length, uint32_t step);
void LoadMemoryFromUART(void *memory, size_t length);
int LoadMemoryFromXmodem(void *memory);

#endif // UBW_KERNEL_H_
