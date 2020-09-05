#ifndef __RS485__UART4_DRIVER_H
#define __RS485__UART4_DRIVER_H
#include <stdint.h>
#include "stm32f4xx.h"

/*******************************************************************************
Rs485收发模式管脚定义
*******************************************************************************/
#define RS485_MODE_PORT          (GPIOA)
#define RS485_MODE_PIN           (GPIO_Pin_4)
//485模式控制
#define RS485_TX                GPIO_SetBits(RS485_MODE_PORT,RS485_MODE_PIN)
#define RS485_RX                GPIO_ResetBits(RS485_MODE_PORT,RS485_MODE_PIN)


extern void Uart4_Init(uint32_t BaudRate, uint16_t DataBits, uint16_t Parity, uint16_t StopBits);
extern void Uart4_SendData(uint8_t *buf,uint8_t len);
extern void Uart4_PutChar(uint8_t c);
extern void Uart4_PutString(uint8_t *s);

#endif
