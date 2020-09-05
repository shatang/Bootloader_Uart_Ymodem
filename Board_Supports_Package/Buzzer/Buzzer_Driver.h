#ifndef __BUZZER_DRIVER_H
#define __BUZZER_DRIVER_H
#include "stm32f4xx.h" 


/*******************************************************************************
·äÃùÆ÷¶¨Òå
*******************************************************************************/
#define BEEP_PORT           (GPIOA)
#define BEEP_PIN            (GPIO_Pin_10)

#define BEEP_ON             GPIO_SetBits(BEEP_PORT,BEEP_PIN)
#define BEEP_OFF            GPIO_ResetBits(BEEP_PORT,BEEP_PIN)


void Buzzer_PortInit(void);


#endif
