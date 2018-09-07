#ifndef UBW_INCLUDE_UART_H_
#define UBW_INCLUDE_UART_H_

#include "type.h"

void InitUART(uint32_t baud_rate);
uint8_t GetByteUART();
void PutByteUART(uint8_t byte);
uint32_t GetWordUART();
void PutWordUART(uint32_t word);

#endif // UBW_INCLUDE_UART_H_
