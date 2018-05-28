#include "BspPlatform.h"
#if defined STM32F0
#elif defined STM32F1
#include "stm32f10x.h"
#elif defined STM32F4
#endif
#include "Sys.h"
#include "Core\Queue.h"
#include "Port.h"
#include "Platform\stm32.h"
#include "Device\DeviceConfigHelper.h"

void SerialPort_GetPins(Pin *txPin, Pin *rxPin, COM index, bool Remap = false)
{
	*rxPin = *txPin = P0;

	const Pin g_Uart_Pins[] = UART_PINS;
	const Pin g_Uart_Pins_Map[] = UART_PINS_FULLREMAP;
	const Pin *p = g_Uart_Pins;
	if (Remap)
	{
		p = g_Uart_Pins_Map;
	}
	int n = index << 2;
	*txPin = p[n];
	*rxPin = p[n + 1];
}
Port*		Ports[2];	// Tx/Rx
Pin			Pins[2];	// Tx/Rx

#ifdef STM32F0


extern "C"
{

}
#elif defined STM32F1

//�ж��ߴ򿪡��ر�
void SetEXIT(int pinIndex, bool enable, Trigger trigger = Both)
{
#if defined STM32F0
	/* ����EXTI�ж��� */
	EXTI_InitTypeDef ext;
	EXTI_StructInit(&ext);
	ext.EXTI_Line = EXTI_Line0 << (pinIndex & 0X0F);
	ext.EXTI_Mode = EXTI_Mode_Interrupt;
	switch (trigger)
	{
	case InputPort::Rising:
		ext.EXTI_Trigger = EXTI_Trigger_Rising; // �����ش���
		break;
	case InputPort::Falling:
		ext.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش���
		break;
	case InputPort::Both:
	default:
		ext.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // �������½��ش���
		break;
	}
	ext.EXTI_LineCmd = enable ? ENABLE : DISABLE;
	EXTI_Init(&ext);
#elif defined STM32F1
	/* ����EXTI�ж��� */
	EXTI_InitTypeDef ext;
	EXTI_StructInit(&ext);
	ext.EXTI_Line = EXTI_Line0 << (pinIndex & 0X0F);
	ext.EXTI_Mode = EXTI_Mode_Interrupt;
	switch (trigger)
	{
	case Rising: ext.EXTI_Trigger = EXTI_Trigger_Rising; // �����ش���
		break;
	case Falling: ext.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش���
		break;
	case Both:
	default:
		ext.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // �������½��ش���
		break;
	}
	ext.EXTI_LineCmd = enable ? ENABLE : DISABLE;
	EXTI_Init(&ext);
#elif defined STM32F4
	/* ����EXTI�ж��� */
	EXTI_InitTypeDef ext;
	EXTI_StructInit(&ext);
	ext.EXTI_Line = EXTI_Line0 << (pinIndex & 0X0F);
	ext.EXTI_Mode = EXTI_Mode_Interrupt;
	switch (trigger)
	{
	case InputPort::Rising:
		ext.EXTI_Trigger = EXTI_Trigger_Rising; // �����ش���
		break;
	case InputPort::Falling:
		ext.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش���
		break;
	case InputPort::Both:
	default:
		ext.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // �������½��ش���
		break;
	}
	ext.EXTI_LineCmd = enable ? ENABLE : DISABLE;
	EXTI_Init(&ext);
#endif
}
static const int PORT_IRQns[] =
{
	#if defined STM32F0
	EXTI0_1_IRQn, EXTI0_1_IRQn,  // ����
	EXTI2_3_IRQn, EXTI2_3_IRQn,  // ����
	EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn,
	EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn  // EXTI15_10
	#elif defined STM32F1
	EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn,  // 5��������
	EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,  // EXTI9_5
	EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn  // EXTI15_10
	#elif defined STM32F4
	EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn,  // 5��������
	EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,  // EXTI9_5
	EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn  // EXTI15_10
	#endif
};
void InputPort_OpenEXTI(Pin pin, Trigger trigger = Both)
{
#if defined STM32F0
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA + pin >> 4, pin & 0x0f);

	SetEXIT(pin, true, trigger);
	Interrupt.SetPriority(PORT_IRQns[pin & 0x0f], 1u);
	//Interrupt.Activate(PORT_IRQns[v3],(void (__cdecl *)(unsigned __int16, void *))EXTI_IRQHandler,v1);
#elif defined STM32F1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA + pin >> 4, pin & 0x0f);
	SetEXIT(pin, true, trigger);
	NVIC_InitTypeDef nvic;

	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannel = PORT_IRQns[pin & 0x0f];

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;

	NVIC_Init(&nvic);
	NVIC_SetPriority((IRQn_Type)PORT_IRQns[pin & 0x0f], 1);
#elif defined STM32F4
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA + pin >> 4, pin & 0x0f);

	SetEXIT(pin, true, trigger);
	Interrupt.SetPriority(PORT_IRQns[pin & 0x0f], 1u);
	//Interrupt.Activate(PORT_IRQns[v3],(void (__cdecl *)(unsigned __int16, void *))EXTI_IRQHandler,v1);
#endif
}
/// TIMx,x[6,7]�ж����ȼ�����
void TIMx_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	// �����ж���Ϊ0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	// �����ж���Դ
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;

	// ���������ȼ�Ϊ 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

	// ������ռ���ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIMx_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// ����TIMx_CLK,x[6,7],���ڲ�ʱ��CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// �Զ���װ�ؼĴ����ܵ�ֵ(����ֵ)
	TIM_TimeBaseStructure.TIM_Period = 1000;

	// �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж�
	// ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (71+1)=1M
	TIM_TimeBaseStructure.TIM_Prescaler = 71;

	// ��ʼ����ʱ��TIMx, x[6,7]
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	// ����������жϱ�־λ
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);

	// �����������ж�
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	// ʹ�ܼ�����
	TIM_Cmd(TIM2, ENABLE);

	// ��ʱ�ر�TIMx,x[6,7]��ʱ�ӣ��ȴ�ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
}
void BspPlatformInit()
{
	/* ������ʱ�� TIMx,x[6,7] ��ʱ���� */
	TIMx_Configuration();

	/* ���û�����ʱ�� TIMx,x[6,7]���ж����ȼ� */
	TIMx_NVIC_Configuration();

	/* ������ʱ�� TIMx,x[6,7] ���¿�ʱ�ӣ���ʼ��ʱ */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	DeviceConfigHelper::TimeTickInit();
}
void TimeUpdate();
extern "C"
{
	void SysTick_Handler(void)
	{

	}

	void USART1_IRQHandler(void)
	{
#if USECOM1
		volatile uint8_t ch;
		if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
		{
			ch = USART_ReceiveData(USART1);
			Rxx1.Enqueue(ch);
		}
		if (USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
			//����֡�������
		{
			ch = USART_ReceiveData(USART1); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)
		}
		/* �����ͻ��������ж� */
		if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
		{
			if (Txx1.Empty())
			{
				/* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
				USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
				/* ʹ�����ݷ�������ж� */
				USART_ITConfig(USART1, USART_IT_TC, ENABLE);
			}
			else
			{
				/* �ӷ���FIFOȡ1���ֽ�д�봮�ڷ������ݼĴ��� */
				USART_SendData(USART1, Txx1.Dequeue());
			}

		}
		/* ����bitλȫ��������ϵ��ж� */
		else if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)
		{
			if (Txx1.Empty())
			{
				/* �������FIFO������ȫ��������ϣ���ֹ���ݷ�������ж� */
				USART_ITConfig(USART1, USART_IT_TC, DISABLE);

				/* �ص�����, һ����������RS485ͨ�ţ���RS485оƬ����Ϊ����ģʽ��������ռ���� */
				Txx1.Clear();
				if (DeviceConfigHelper::pCOM1Rx485)
				{
					*DeviceConfigHelper::pCOM1Rx485 = 0;
				}
			}
			else
			{
				/* ��������£��������˷�֧ */
				/* �������FIFO�����ݻ�δ��ϣ���ӷ���FIFOȡ1������д�뷢�����ݼĴ��� */
				USART_SendData(USART1, Txx1.Dequeue());
			}
		}
#endif
	}
	void USART2_IRQHandler(void)
	{
#if USECOM2
		volatile uint8_t ch;
		if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
		{
			ch = USART_ReceiveData(USART2);
			Rxx2.Enqueue(ch);
		}
		if (USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
			//����֡�������
		{
			ch = USART_ReceiveData(USART2); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)    
			//com2test();
		}
		/* �����ͻ��������ж� */
		if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
		{
			if (Txx2.Empty())
			{
				/* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
				USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
				/* ʹ�����ݷ�������ж� */
				USART_ITConfig(USART2, USART_IT_TC, ENABLE);
			}
			else
			{
				/* �ӷ���FIFOȡ1���ֽ�д�봮�ڷ������ݼĴ��� */
				USART_SendData(USART2, Txx2.Dequeue());
			}

		}
		/* ����bitλȫ��������ϵ��ж� */
		else if (USART_GetITStatus(USART2, USART_IT_TC) != RESET)
		{
			if (Txx2.Empty())
			{
				/* �������FIFO������ȫ��������ϣ���ֹ���ݷ�������ж� */
				USART_ITConfig(USART2, USART_IT_TC, DISABLE);

				/* �ص�����, һ����������RS485ͨ�ţ���RS485оƬ����Ϊ����ģʽ��������ռ���� */
				Txx2.Clear();
				if (DeviceConfigHelper::pCOM2Rx485)
				{
					*DeviceConfigHelper::pCOM2Rx485 = 0;
				}
			}
			else
			{
				/* ��������£��������˷�֧ */
				/* �������FIFO�����ݻ�δ��ϣ���ӷ���FIFOȡ1������д�뷢�����ݼĴ��� */
				USART_SendData(USART2, Txx2.Dequeue());
			}
		}
#endif
	}
	void USART3_IRQHandler(void)
	{
#if USECOM3
		volatile uint8_t ch;
		if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
		{
			ch = USART_ReceiveData(USART3);
			Rxx3.Enqueue(ch);
		}
		if (USART_GetITStatus(USART3, USART_IT_IDLE) == SET)
			//����֡�������
		{
			ch = USART_ReceiveData(USART3); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR) 
		}
		/* �����ͻ��������ж� */
		if (USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
		{
			if (Txx3.Empty())
			{
				/* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
				USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
				/* ʹ�����ݷ�������ж� */
				USART_ITConfig(USART3, USART_IT_TC, ENABLE);
			}
			else
			{
				/* �ӷ���FIFOȡ1���ֽ�д�봮�ڷ������ݼĴ��� */
				USART_SendData(USART3, Txx3.Dequeue());
			}

		}
		/* ����bitλȫ��������ϵ��ж� */
		else if (USART_GetITStatus(USART3, USART_IT_TC) != RESET)
		{
			if (Txx3.Empty())
			{
				/* �������FIFO������ȫ��������ϣ���ֹ���ݷ�������ж� */
				USART_ITConfig(USART3, USART_IT_TC, DISABLE);

				/* �ص�����, һ����������RS485ͨ�ţ���RS485оƬ����Ϊ����ģʽ��������ռ���� */
				Txx3.Clear();
				if (DeviceConfigHelper::pCOM3Rx485)
				{
					*DeviceConfigHelper::pCOM3Rx485 = 0;
				}
			}
			else
			{
				/* ��������£��������˷�֧ */
				/* �������FIFO�����ݻ�δ��ϣ���ӷ���FIFOȡ1������д�뷢�����ݼĴ��� */
				USART_SendData(USART3, Txx3.Dequeue());
			}
		}
#endif
	}
	void UART4_IRQHandler(void)
	{
#if USECOM4
		volatile uint8_t ch;
		if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
		{
			ch = USART_ReceiveData(UART4);
			Rxx3.Enqueue(ch);
		}
		if (USART_GetITStatus(UART4, USART_IT_IDLE) == SET)
			//����֡�������
		{
			ch = USART_ReceiveData(UART4); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR) 
		}
		/* �����ͻ��������ж� */
		if (USART_GetITStatus(UART4, USART_IT_TXE) != RESET)
		{
			if (Txx4.Empty())
			{
				/* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
				USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
				/* ʹ�����ݷ�������ж� */
				USART_ITConfig(UART4, USART_IT_TC, ENABLE);
			}
			else
			{
				/* �ӷ���FIFOȡ1���ֽ�д�봮�ڷ������ݼĴ��� */
				USART_SendData(UART4, Txx4.Dequeue());
			}

		}
		/* ����bitλȫ��������ϵ��ж� */
		else if (USART_GetITStatus(UART4, USART_IT_TC) != RESET)
		{
			if (Txx4.Empty())
			{
				/* �������FIFO������ȫ��������ϣ���ֹ���ݷ�������ж� */
				USART_ITConfig(UART4, USART_IT_TC, DISABLE);

				/* �ص�����, һ����������RS485ͨ�ţ���RS485оƬ����Ϊ����ģʽ��������ռ���� */
				Txx4.Clear();
				if (DeviceConfigHelper::pCOM4Rx485)
				{
					*DeviceConfigHelper::pCOM4Rx485 = 0;
				}
			}
			else
			{
				/* ��������£��������˷�֧ */
				/* �������FIFO�����ݻ�δ��ϣ���ӷ���FIFOȡ1������д�뷢�����ݼĴ��� */
				USART_SendData(UART4, Txx4.Dequeue());
			}
		}
#endif
	}
	void UART5_IRQHandler(void)
	{
#if USECOM5
		volatile uint8_t ch;
		if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
		{
			ch = USART_ReceiveData(UART5);
			Rxx5.Enqueue(ch);
		}
		if (USART_GetITStatus(UART5, USART_IT_IDLE) == SET)
			//����֡�������
		{
			ch = USART_ReceiveData(UART5); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)  
		}
		/* �����ͻ��������ж� */
		if (USART_GetITStatus(UART5, USART_IT_TXE) != RESET)
		{
			if (Txx5.Empty())
			{
				/* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
				USART_ITConfig(UART5, USART_IT_TXE, DISABLE);
				/* ʹ�����ݷ�������ж� */
				USART_ITConfig(UART5, USART_IT_TC, ENABLE);
			}
			else
			{
				/* �ӷ���FIFOȡ1���ֽ�д�봮�ڷ������ݼĴ��� */
				USART_SendData(UART5, Txx3.Dequeue());
			}

		}
		/* ����bitλȫ��������ϵ��ж� */
		else if (USART_GetITStatus(UART5, USART_IT_TC) != RESET)
		{
			if (Txx5.Empty())
			{
				/* �������FIFO������ȫ��������ϣ���ֹ���ݷ�������ж� */
				USART_ITConfig(UART5, USART_IT_TC, DISABLE);

				/* �ص�����, һ����������RS485ͨ�ţ���RS485оƬ����Ϊ����ģʽ��������ռ���� */
				Txx5.Clear();
				if (DeviceConfigHelper::pCOM5Rx485)
				{
					*DeviceConfigHelper::pCOM5Rx485 = 0;
				}
			}
			else
			{
				/* ��������£��������˷�֧ */
				/* �������FIFO�����ݻ�δ��ϣ���ӷ���FIFOȡ1������д�뷢�����ݼĴ��� */
				USART_SendData(UART5, Txx5.Dequeue());
			}
		}
#endif
	}

	void TIM2_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		{

			TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
			TimeUpdate();
		}
	}
	void TIM3_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
		{

			TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
		}
	}
	void TIM4_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
		{

			TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
		}
	}
	void TIM5_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
		}
	}

	void TIM6_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
		}
	}

	void TIM7_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);
		}
	}
	void EXTI0_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line0) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line0);
		}
	}
	void EXTI1_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line1) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line1);
		}
	}

	void EXTI2_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line2) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line2);
		}
	}
	void EXTI3_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line3) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line3);
		}
	}
	void EXTI4_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line4) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line4);
		}
	}

	void EXTI9_5_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line5) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line5);
		}
		if (EXTI_GetITStatus(EXTI_Line6) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line6);
		}
		if (EXTI_GetITStatus(EXTI_Line7) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line7);
		}
		if (EXTI_GetITStatus(EXTI_Line8) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line8);
		}
		if (EXTI_GetITStatus(EXTI_Line9) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line9);
		}
	}
	void EXTI15_10_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line10) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line10);
		}
		if (EXTI_GetITStatus(EXTI_Line11) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line11);
		}
		if (EXTI_GetITStatus(EXTI_Line12) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line12);
		}
		if (EXTI_GetITStatus(EXTI_Line13) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line13);
		}
		if (EXTI_GetITStatus(EXTI_Line14) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line14);
		}
		if (EXTI_GetITStatus(EXTI_Line15) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line15);
		}
	}
}

#elif defined STM32F4


extern "C"
{

}
#endif
