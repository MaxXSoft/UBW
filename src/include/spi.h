#ifndef UBW_INCLUDE_SPI_H_
#define UBW_INCLUDE_SPI_H_

#include "type.h"

void EraseSPI();
void EraseAreaSPI(uint32_t addr_start, uint32_t addr_end);
void WriteAreaSPI(uint32_t addr, uint8_t *buffer, size_t length, int page);
void ReadAreaSPI(uint32_t addr, uint8_t *buffer, size_t length);

#endif // UBW_INCLUDE_SPI_H_
