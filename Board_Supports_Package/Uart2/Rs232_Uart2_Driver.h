#ifndef __RS232__UART2_DRIVER_H
#define __RS232__UART2_DRIVER_H
#include <stdint.h>

extern void Uart2_Init(uint32_t BaudRate, uint16_t DataBits, uint16_t Parity, uint16_t StopBits);
extern void Uart2_SendData(uint8_t *buf,uint8_t len);
extern void Uart2_PutChar(uint8_t c);
extern void Uart2_PutString(uint8_t *s);
#endif



