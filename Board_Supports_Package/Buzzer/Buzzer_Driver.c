#include "Buzzer_Driver.h"


void Buzzer_PortInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        //下拉
    GPIO_Init(BEEP_PORT, &GPIO_InitStructure);         //初始化对应GPIO
    
    //初始化为高电平
    GPIO_ResetBits(BEEP_PORT,BEEP_PIN); 
}

