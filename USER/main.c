#include "stm32f4xx.h" 
#include "download.h"
#include "flash_if.h"
#include "common.h"
typedef void (*pFunction)(void);
pFunction JumpApp;
/********************************************************************************
* ������ : IAP_LoadApp
* ��  �� : Bootloader��ת��APP
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.23
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void IAP_LoadApp(uint32_t appxaddr)
{
	if(((*(__IO uint32_t*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
	{ 
		JumpApp=(pFunction)*(__IO uint32_t*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		__set_MSP(*(__IO uint32_t*)appxaddr);				    //��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		JumpApp();								                //��ת��APP.
	}
}
/********************************************************************************
* ������ : main
* ��  �� : ������
* ˵  �� : ʵ��Bootloader����APP�ļ�������ת������ϵ����յ���λ���Ŀո���Ϣ800ms���Զ���ת��APP
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.23
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    IAP_Init();   
    FLASH_If_Init();
    
    if(Serial_CheckDownloadChar())
    {
        SerialDownload(Serial_GetDownloadChannel());
    }

    FLASH_If_Finish();
    IAP_Close();
    IAP_LoadApp(APPLICATION_START_ADDRESS); //������ת
    
    while(1){}
        
    return 0;
}

