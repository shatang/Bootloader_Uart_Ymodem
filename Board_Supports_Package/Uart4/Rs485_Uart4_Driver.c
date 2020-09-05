#include "Rs485_Uart4_Driver.h"


/********************************************************************************
* ������ : Uart4_Init
* ��  �� : ����4��ʼ��
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.23
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void Uart4_Init(uint32_t BaudRate, uint16_t DataBits, uint16_t Parity, uint16_t StopBits)
{
   //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);

	//����4��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4); 

	//Rs485_Tx/Rx �˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);               

    //Rs485_EN
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
    //UART4 ��ʼ������
	USART_InitStructure.USART_BaudRate = BaudRate;      //������
	USART_InitStructure.USART_WordLength = DataBits;    //���ݸ�ʽ
	USART_InitStructure.USART_StopBits = StopBits;      //ֹͣλ
	USART_InitStructure.USART_Parity = Parity;          //У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(UART4, &USART_InitStructure);

    USART_Cmd(UART4, ENABLE);

	//Uart4 NVIC����
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;        //����4�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
    
    RS485_RX;//��Ϊ����ģʽ
}

/********************************************************************************
* ������ : Uart4_SendData
* ��  �� : ���ڷ�������
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.22
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void Uart4_SendData(uint8_t *buf,uint8_t len)
{
	uint8_t i=0;
    
    RS485_TX;			    //����Ϊ����ģʽ
    
  	for(i=0;i<len;i++)		//ѭ����������
	{
        while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET){} //�ȴ����ͽ���		
        USART_SendData(UART4,buf[i]); //��������
	}
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET){}     //�ȴ����ͽ���
        
    RS485_RX;		    //����Ϊ����ģʽ		
}
/********************************************************************************
* ������ : Uart4_PutChar
* ��  �� : ���ڷ���1byte����
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.22
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void Uart4_PutChar(uint8_t c)
{
    RS485_TX;
    while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET){} //�ȴ����ͽ���
    USART_SendData(UART4,c); //��������
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET){} //�ȴ����ͽ���
    RS485_RX;
}
/********************************************************************************
* ������ : Uart4_PutString
* ��  �� : ���ڷ����ַ�������
* ˵  �� : ������Ҫ����̡��������Ϊ�ַ�����������ַ������飬ע��ĩβ'\0'��ֹ�����ܷ�
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.22
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void Uart4_PutString(uint8_t *s)
{
    while (*s != '\0')
    {
        Uart4_PutChar(*s);
        s++;
    }
}

