/*******************************************************************************
** �ļ���: 		common.c
** �汾��  		1.0
** ��������: 	RealView MDK-ARM 5.20
** ����: 		liupeng
** ��������: 	2016-03-20
** ����:		��ʾ���˵������˵�����ʾһϵ�в���������ض������ļ���
                ִ��Ӧ�ó����Լ���ֹд����(�������Flash��д����)��
** ����ļ�:	common.h��ymodem.h
** �޸���־��	2016-03-20   �����ĵ�
*******************************************************************************/

/* ����ͷ�ļ� *****************************************************************/
#include "common.h"
#include "ymodem.h"
#include "Rs232_Uart2_Driver.h"
#include "Rs485_Uart4_Driver.h"
/* ���� ----------------------------------------------------------------------*/
pFunction Jump_To_Application;
uint32_t JumpAddress;
uint32_t BlockNbr = 0, UserMemoryMask = 0;
__IO uint32_t FlashProtection = 0;

/********************************************************************************
* ������ : Int2Str
* ��  �� : ��������ת���ַ���
* ˵  �� : none
* ��  �� : intnum:���ݣ�str��ת��Ϊ���ַ���
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.22
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void Int2Str(uint8_t* str, int32_t intnum)
{
    uint32_t i, Div = 1000000000, j = 0, Status = 0;

    for (i = 0; i < 10; i++)
    {
        str[j++] = (intnum / Div) + 48;

        intnum = intnum % Div;
        Div /= 10;
        if ((str[j-1] == '0') & (Status == 0))
        {
            j = 0;
        }
        else
        {
            Status++;
        }
    }
}
/********************************************************************************
* ������ : Str2Int
* ��  �� : �ַ���ת������
* ˵  �� : none
* ��  �� : inputstr:��ת�����ַ�����intnum��ת�õ�����
* ��  �� : ת�����
            1����ȷ
            0������
* ��  �� : Shatang                    ʱ  �� : 2020.06.22
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum)
{
    uint32_t i = 0, res = 0;
    uint32_t val = 0;

    if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X'))
    {
        if (inputstr[2] == '\0')
        {
            return 0;
        }
        for (i = 2; i < 11; i++)
        {
            if (inputstr[i] == '\0')
            {
                *intnum = val;
                //����1
                res = 1;
                break;
            }
            if (ISVALIDHEX(inputstr[i]))
            {
                val = (val << 4) + CONVERTHEX(inputstr[i]);
            }
            else
            {
                //��Ч���뷵��0
                res = 0;
                break;
            }
        }

        if (i >= 11)
        {
            res = 0;
        }
    }
    else//���10Ϊ2����
    {
        for (i = 0; i < 11; i++)
        {
            if (inputstr[i] == '\0')
            {
                *intnum = val;
                //����1
                res = 1;
                break;
            }
            else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0))
            {
                val = val << 10;
                *intnum = val;
                res = 1;
                break;
            }
            else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0))
            {
                val = val << 20;
                *intnum = val;
                res = 1;
                break;
            }
            else if (ISVALIDDEC(inputstr[i]))
            {
                val = val * 10 + CONVERTDEC(inputstr[i]);
            }
            else
            {
                //��Ч���뷵��0
                res = 0;
                break;
            }
        }
        //����10λ��Ч������0
        if (i >= 11)
        {
            res = 0;
        }
    }

    return res;
}
/********************************************************************************
* ������ : SerialKeyPressed
* ��  �� : ���Գ����ն��Ƿ��а�������
* ˵  �� : key:����
* ��  �� : none
* ��  �� : 1����ȷ 0������
* ��  �� : Shatang                    ʱ  �� : 2020.06.18
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
uint32_t SerialKeyPressed(uint8_t fu8_Channel,uint8_t *key)
{
    if(Uart2_232 == fu8_Channel)
    {
        if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)
        {
            *key = (uint8_t)USART2->DR;
            return 1;
        }
    }
    else if(Uart4_485 == fu8_Channel)
    {
        if (USART_GetFlagStatus(UART4, USART_FLAG_RXNE) != RESET)
        {
            *key = (uint8_t)UART4->DR;
            return 1;
        }
    }
    
    return 0;
}

typedef void (*PutChar_Callback)(uint8_t);
/********************************************************************************
* ������ : Serial_PutChar
* ��  �� : ���ڷ���һ���ַ�
* ˵  �� : none
* ��  �� : fu8_Channal:����ͨ��
           C:�跢�͵��ַ�
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.22
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void Serial_PutChar(uint8_t fu8_Channal, uint8_t c)
{
    PutChar_Callback UartSend;

    if(Uart2_232 == fu8_Channal)UartSend = Uart2_PutChar;
    else if(Uart4_485 == fu8_Channal)UartSend = Uart4_PutChar;

    UartSend(c);
}
typedef void (*PutString_Callback)(uint8_t*);
/********************************************************************************
* ������ : Serial_PutString
* ��  �� : ���ڷ���һ���ַ���
* ˵  �� : none
* ��  �� : *s:�跢�͵��ַ���
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.22
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void Serial_PutString(uint8_t fu8_Channal, uint8_t *s)
{
    PutString_Callback UartSend;
    
    if(Uart2_232 == fu8_Channal)UartSend = Uart2_PutString;
    else if(Uart4_485 == fu8_Channal)UartSend = Uart4_PutString;

    UartSend(s);
}
