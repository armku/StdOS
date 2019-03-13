#include "Sys.h"
#include "Port.h"
#include "../../stm32.h"
#include "Configuration.h"
#include "Interrupt.h"

void TimeUpdate();


void BspPlatformInit()
{	
	DeviceConfigCenter::TimeTickInit();//ϵͳ�ö�ʱ����ʼ��
	DeviceConfigCenter::PTim2Update = TimeUpdate;
}

int DeviceConfigCenter::CurrentTick()
{
	return (TIM2->CNT) >> 1;
}
uint32_t DeviceConfigCenter::CurrentTicks1()
{
	return SysTick->LOAD - SysTick->VAL;
}
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
	/*ʹ��microLib�ķ���*/
	 
	int fputc(int ch, FILE *f)
	{
		USART_SendData(USART1, (uint8_t) ch);

		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}

		return ch;
	}
	int GetKey (void)  {

		while (!(USART1->SR & USART_FLAG_RXNE));

		return ((int)(USART1->DR & 0x1FF));
	}
#ifdef __cplusplus
}
#endif
