#include "Rs232_Uart2_Driver.h"
#include "stm32f4xx.h"

/********************************************************************************
* 函数名 : Uart2_Init
* 功  能 : 串口2初始化
* 说  明 : none
* 入  参 : none
* 返  回 : none
* 设  计 : Shatang                    时  间 : 2020.06.23
* 修  改 : none                       时  间 : none
********************************************************************************/
void Uart2_Init(uint32_t BaudRate, uint16_t DataBits, uint16_t Parity, uint16_t StopBits)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 
	//串口5对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 
	
	//Rs232_Rx | Tx 端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    //UART2 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;      //波特率
	USART_InitStructure.USART_WordLength = DataBits;    //数据格式
	USART_InitStructure.USART_StopBits = StopBits;      //停止位
	USART_InitStructure.USART_Parity = Parity;          //校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART2, &USART_InitStructure);

	//Uart2 NVIC配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;        //串口5中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
    
    USART_Cmd(USART2, ENABLE);
}
/********************************************************************************
* 函数名 : Uart2_SendData
* 功  能 : 串口发送数据
* 说  明 : none
* 入  参 : none
* 返  回 : none
* 设  计 : Shatang                    时  间 : 2020.06.22
* 修  改 : none                       时  间 : none
********************************************************************************/
void Uart2_SendData(uint8_t *buf,uint8_t len)
{
	uint8_t i=0;
    
  	for(i=0;i<len;i++)		//循环发送数据
	{
        while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //等待发送结束		
        USART_SendData(USART2,buf[i]); //发送数据
	}
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //等待发送结束		
}
/********************************************************************************
* 函数名 : Uart2_PutChar
* 功  能 : 串口发送1byte数据
* 说  明 : none
* 入  参 : none
* 返  回 : none
* 设  计 : Shatang                    时  间 : 2020.06.22
* 修  改 : none                       时  间 : none
********************************************************************************/
void Uart2_PutChar(uint8_t c)
{
    while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //等待发送结束		
    USART_SendData(USART2,c); //发送数据
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //等待发送结束		 
}
/********************************************************************************
* 函数名 : Uart2_PutString
* 功  能 : 串口发送字符串数据
* 说  明 : 对输入要求苛刻。输入必须为字符串；如果是字符串数组，注意末尾'\0'防止程序跑飞
* 入  参 : none
* 返  回 : none
* 设  计 : Shatang                    时  间 : 2020.06.22
* 修  改 : none                       时  间 : none
********************************************************************************/
void Uart2_PutString(uint8_t *s)
{
    while (*s != '\0')
    {
        Uart2_PutChar(*s);
        s++;
    }
}
