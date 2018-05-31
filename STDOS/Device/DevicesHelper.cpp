#include "DeviceConfigHelper.h"
#include "Platform\stm32.h"

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
		{
			//����֡�������
			ch = USART_ReceiveData(USART1); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)
			if (DeviceConfigHelper::PRcvCOM1)
			{
				(*DeviceConfigHelper::PRcvCOM1)();
			}
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
		{
			//����֡�������
			ch = USART_ReceiveData(USART2); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)    
			if (DeviceConfigHelper::PRcvCOM2)
			{
				(*DeviceConfigHelper::PRcvCOM2)();
			}
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
		{
			//����֡�������
			ch = USART_ReceiveData(USART3); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR) 
			if (DeviceConfigHelper::PRcvCOM3)
			{
				(*DeviceConfigHelper::PRcvCOM3)();
			}
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
		{
			//����֡�������
			ch = USART_ReceiveData(UART4); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR) 
			if (DeviceConfigHelper::PRcvCOM4)
			{
				(*DeviceConfigHelper::PRcvCOM4)();
			}
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
		{
			//����֡�������
			ch = USART_ReceiveData(UART5); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)  
			if (DeviceConfigHelper::PRcvCOM5)
			{
				(*DeviceConfigHelper::PRcvCOM5)();
			}
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
			if (DeviceConfigHelper::PTim2Update)
			{
				(*DeviceConfigHelper::PTim2Update)();
			}
		}
	}
	void TIM3_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
			if (DeviceConfigHelper::PTim3Update)
			{
				(*DeviceConfigHelper::PTim3Update)();
			}
		}
	}
	void TIM4_IRQHandler(void)
	{
#if defined STM32F0
		//F0û��TIM4
#else
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
			if (DeviceConfigHelper::PTim4Update)
			{
				(*DeviceConfigHelper::PTim4Update)();
			}
		}
#endif
	}
	void TIM5_IRQHandler(void)
	{
#if defined STM32F0
		//F0û��TIM5
#else
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
			if (DeviceConfigHelper::PTim5Update)
			{
				(*DeviceConfigHelper::PTim5Update)();
			}
		}
#endif
	}

	void TIM6_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
			if (DeviceConfigHelper::PTim6Update)
			{
				(*DeviceConfigHelper::PTim6Update)();
			}
		}
	}

	void TIM7_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);
			if (DeviceConfigHelper::PTim7Update)
			{
				(*DeviceConfigHelper::PTim7Update)();
			}
		}
	}
	void EXTI0_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line0) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line0);
			if (DeviceConfigHelper::PExit0)
			{
				(*DeviceConfigHelper::PExit0)();
			}
		}
	}
	void EXTI1_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line1) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line1);
			if (DeviceConfigHelper::PExit1)
			{
				(*DeviceConfigHelper::PExit1)();
			}
		}
	}

	void EXTI2_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line2) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line2);
			if (DeviceConfigHelper::PExit2)
			{
				(*DeviceConfigHelper::PExit2)();
			}
		}
	}
	void EXTI3_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line3) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line3);
			if (DeviceConfigHelper::PExit3)
			{
				(*DeviceConfigHelper::PExit3)();
			}
		}
	}
	void EXTI4_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line4) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line4);
			if (DeviceConfigHelper::PExit4)
			{
				(*DeviceConfigHelper::PExit4)();
			}
		}
	}

	void EXTI9_5_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line5) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line5);
			if (DeviceConfigHelper::PExit5)
			{
				(*DeviceConfigHelper::PExit5)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line6) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line6);
			if (DeviceConfigHelper::PExit6)
			{
				(*DeviceConfigHelper::PExit6)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line7) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line7);
			if (DeviceConfigHelper::PExit7)
			{
				(*DeviceConfigHelper::PExit7)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line8) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line8);
			if (DeviceConfigHelper::PExit8)
			{
				(*DeviceConfigHelper::PExit8)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line9) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line9);
			if (DeviceConfigHelper::PExit9)
			{
				(*DeviceConfigHelper::PExit9)();
			}
		}
	}
	void EXTI15_10_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line10) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line10);
			if (DeviceConfigHelper::PExit10)
			{
				(*DeviceConfigHelper::PExit10)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line11) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line11);
			if (DeviceConfigHelper::PExit11)
			{
				(*DeviceConfigHelper::PExit11)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line12) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line12);
			if (DeviceConfigHelper::PExit12)
			{
				(*DeviceConfigHelper::PExit12)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line13) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line13);
			if (DeviceConfigHelper::PExit13)
			{
				(*DeviceConfigHelper::PExit13)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line14) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line14);
			if (DeviceConfigHelper::PExit14)
			{
				(*DeviceConfigHelper::PExit14)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line15) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line15);
			if (DeviceConfigHelper::PExit15)
			{
				(*DeviceConfigHelper::PExit15)();
			}
		}
	}
}
