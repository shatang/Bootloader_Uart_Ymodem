#include "Rs232_Uart2_Driver.h"
#include "stm32f4xx.h"

/********************************************************************************
* ������ : Uart2_Init
* ��  �� : ����2��ʼ��
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.23
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void Uart2_Init(uint32_t BaudRate, uint16_t DataBits, uint16_t Parity, uint16_t StopBits)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 
	//����5��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 
	
	//Rs232_Rx | Tx �˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    //UART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = BaudRate;      //������
	USART_InitStructure.USART_WordLength = DataBits;    //���ݸ�ʽ
	USART_InitStructure.USART_StopBits = StopBits;      //ֹͣλ
	USART_InitStructure.USART_Parity = Parity;          //У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART2, &USART_InitStructure);

	//Uart2 NVIC����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;        //����5�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
    
    USART_Cmd(USART2, ENABLE);
}
/********************************************************************************
* ������ : Uart2_SendData
* ��  �� : ���ڷ�������
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.22
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void Uart2_SendData(uint8_t *buf,uint8_t len)
{
	uint8_t i=0;
    
  	for(i=0;i<len;i++)		//ѭ����������
	{
        while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
        USART_SendData(USART2,buf[i]); //��������
	}
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
}
/********************************************************************************
* ������ : Uart2_PutChar
* ��  �� : ���ڷ���1byte����
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.22
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void Uart2_PutChar(uint8_t c)
{
    while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
    USART_SendData(USART2,c); //��������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		 
}
/********************************************************************************
* ������ : Uart2_PutString
* ��  �� : ���ڷ����ַ�������
* ˵  �� : ������Ҫ����̡��������Ϊ�ַ�����������ַ������飬ע��ĩβ'\0'��ֹ�����ܷ�
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.22
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void Uart2_PutString(uint8_t *s)
{
    while (*s != '\0')
    {
        Uart2_PutChar(*s);
        s++;
    }
}
