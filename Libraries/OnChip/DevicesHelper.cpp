#include "Configuration.h"
#include "Platform\stm32.h"

extern "C"
{
	void SysTick_Handler(void)
	{

	}
	void DMA11_Channel2_IRQHandler(void)
	{
#if defined COM3TXDMAFLAG
		//USART3_TX
		if (DMA_GetITStatus(DMA1_IT_HT2))
		{
			//���ж�
			DMA_ClearITPendingBit(DMA1_IT_HT2);
		}
		if (DMA_GetITStatus(DMA1_IT_TC2))
		{
			//TODO:Add code here
			DMA_ClearITPendingBit(DMA1_IT_TC2);
			DeviceConfigCenter::FLAG_TX3EN = 1;	//����3���Է���
		}
#endif
	}
	void DMA11_Channel3_IRQHandler(void)
	{
#if defined COM3RXDMAFLAG
		//USART3_RX
		if (DMA_GetITStatus(DMA1_IT_TC3))
		{
			//TODO:Add code here
			DMA_ClearITPendingBit(DMA1_IT_TC3);
#if (!defined COM3RCVIDLEINTFLAG)
			DeviceConfigCenter::RcvLastTimeCOM3 = Sys.Ms();
#endif
		}
#endif
	}
	void DMA11_Channel4_IRQHandler(void)
	{
#if defined COM1TXDMAFLAG
		//USART1_TX
		if (DMA_GetITStatus(DMA1_IT_HT4))
		{
			//���ж�
			DMA_ClearITPendingBit(DMA1_IT_HT4);
		}
		if (DMA_GetITStatus(DMA1_IT_TC4))
		{
			//TODO:Add code here
			DMA_ClearITPendingBit(DMA1_IT_TC4);
			DeviceConfigCenter::FLAG_TX1EN = 1;	//����1���Է���
		}
#endif
	}
	void DMA11_Channel5_IRQHandler()
	{
#if defined COM1RXDMAFLAG
		//USART1_RX
		if (DMA_GetITStatus(DMA1_IT_TC5))
		{
			DMA_ClearITPendingBit(DMA1_IT_TC5);
#if (!defined COM1RCVIDLEINTFLAG)
			DeviceConfigCenter::RcvLastTimeCOM1 = Sys.Ms();
#endif
		}
#endif
	}
	void DMA11_Channel6_IRQHandler(void)
	{
#if defined COM2RXDMAFLAG
		//		USART2_RX
		if (DMA_GetITStatus(DMA1_IT_TC6))
		{
			//TODO:Add code here
			DMA_ClearITPendingBit(DMA1_IT_TC6);
#if (!defined COM2RCVIDLEINTFLAG)
			DeviceConfigCenter::RcvLastTimeCOM2 = Sys.Ms();
#endif
		}
#endif
	}
	void DMA11_Channel7_IRQHandler(void)
	{
#if defined COM2TXDMAFLAG
		//USART2_TX
		if (DMA_GetITStatus(DMA1_IT_HT7))
		{
			//���ж�
			DMA_ClearITPendingBit(DMA1_IT_HT7);
		}
		if (DMA_GetITStatus(DMA1_IT_TC7))
		{
			//TODO:Add code here
			DMA_ClearITPendingBit(DMA1_IT_TC7);
			DeviceConfigCenter::FLAG_TX2EN = 1;	//����2���Է���
		}
#endif
	}
	//���»ָ�DMAָ��
	void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
	{
		DMA_Cmd(DMA_CHx, DISABLE);  //�ر�USART1 TX DMA1 ��ָʾ��ͨ��      
		DMA_SetCurrDataCounter(DMA_CHx, 256);//DMAͨ����DMA����Ĵ�С
		DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
	}
	void USART11_IRQHandler(void)
	{
#if defined USECOM1		
		//����
#if (defined USECOM1RXD)&&(!defined COM1RXDMAFLAG)
		volatile uint8_t ch;
		if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
		{
			ch = USART_ReceiveData(USART1);
			Rxx1.Enqueue(ch);
#if (!defined COM1RCVIDLEINTFLAG)
			DeviceConfigCenter::RcvLastTimeCOM1 = Sys.Ms();
#endif
		}
#endif
		if (USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
		{
			USART_ReceiveData(USART1);//��ȡ���� ע�⣺������Ҫ�������ܹ�����жϱ�־λ����Ҳ��֪��Ϊɶ��
			USART_ClearITPendingBit(USART1, USART_IT_IDLE);         //����жϱ�־
#if defined COM1RXDMAFLAG
			int curlen = 0;
			curlen = DeviceConfigCenter::BUFLEN_RX1 - DMA_GetCurrDataCounter(DMA1_Channel5);	//����ӱ�֡���ݳ���			
			Rxx1.SetLength(curlen);
			MYDMA_Enable(DMA1_Channel5);                   //�ָ�DMAָ�룬�ȴ���һ�εĽ���						
#endif
#if defined COM1RCVIDLEINTFLAG
			if (DeviceConfigCenter::PRcvCOM1)
			{
				(*DeviceConfigCenter::PRcvCOM1)();
			}
#endif
		}
		//����
				/* �����ͻ��������ж� */
		if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
		{
#if defined COM1SENDINTFLAG
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
#endif
		}
		/* ����bitλȫ��������ϵ��ж� */
		else if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)
		{
#if defined COM1SENDINTFLAG
			if (Txx1.Empty())
			{
				/* �������FIFO������ȫ��������ϣ���ֹ���ݷ�������ж� */
				USART_ITConfig(USART1, USART_IT_TC, DISABLE);

				/* �ص�����, һ����������RS485ͨ�ţ���RS485оƬ����Ϊ����ģʽ��������ռ���� */
				Txx1.Clear();
				if (DeviceConfigCenter::pCOM1Rx485)
				{
					*DeviceConfigCenter::pCOM1Rx485 = 0;
				}
			}
			else
			{
				/* ��������£��������˷�֧ */
				/* �������FIFO�����ݻ�δ��ϣ���ӷ���FIFOȡ1������д�뷢�����ݼĴ��� */
				USART_SendData(USART1, Txx1.Dequeue());
			}
#endif
		}
#endif
	}
	void USART21_IRQHandler(void)
	{
#if defined USECOM2		
		//����
#if (defined USECOM2RXD)&&(!defined COM2RXDMAFLAG)
		volatile uint8_t ch;
		if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
		{
			ch = USART_ReceiveData(USART2);
			Rxx2.Enqueue(ch);
			DeviceConfigCenter::RcvLastTimeCOM2 = Sys.Ms();
		}
#endif
		if (USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
		{
			USART_ReceiveData(USART2);//��ȡ���� ע�⣺������Ҫ�������ܹ�����жϱ�־λ����Ҳ��֪��Ϊɶ��
			USART_ClearITPendingBit(USART2, USART_IT_IDLE);         //����жϱ�־
#if defined COM2RXDMAFLAG
			int curlen = 0;
			curlen = DeviceConfigCenter::BUFLEN_RX2 - DMA_GetCurrDataCounter(DMA1_Channel6);	//����ӱ�֡���ݳ���			
			Rxx2.SetLength(curlen);
			MYDMA_Enable(DMA1_Channel6);                   //�ָ�DMAָ�룬�ȴ���һ�εĽ���						
#endif
#if defined COM2RCVIDLEINTFLAG
			if (DeviceConfigCenter::PRcvCOM2)
			{
				(*DeviceConfigCenter::PRcvCOM2)();
			}
#endif
		}
		//����
		/* �����ͻ��������ж� */
		if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
		{
#if defined COM2SENDINTFLAG
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
#endif
		}
		/* ����bitλȫ��������ϵ��ж� */
		else if (USART_GetITStatus(USART2, USART_IT_TC) != RESET)
		{
#if defined COM2SENDINTFLAG
			if (Txx2.Empty())
			{
				/* �������FIFO������ȫ��������ϣ���ֹ���ݷ�������ж� */
				USART_ITConfig(USART2, USART_IT_TC, DISABLE);

				/* �ص�����, һ����������RS485ͨ�ţ���RS485оƬ����Ϊ����ģʽ��������ռ���� */
				Txx2.Clear();
				if (DeviceConfigCenter::pCOM2Rx485)
				{
					*DeviceConfigCenter::pCOM2Rx485 = 0;
				}
			}
			else
			{
				/* ��������£��������˷�֧ */
				/* �������FIFO�����ݻ�δ��ϣ���ӷ���FIFOȡ1������д�뷢�����ݼĴ��� */
				USART_SendData(USART2, Txx2.Dequeue());
			}
#endif
		}
#endif
	}
	void USART31_IRQHandler(void)
	{
#if defined USECOM3		
		//����
#if (defined USECOM3RXD)&&(!defined COM3RXDMAFLAG)
		volatile uint8_t ch;
		if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
		{
			ch = USART_ReceiveData(USART3);
			Rxx3.Enqueue(ch);
			DeviceConfigCenter::RcvLastTimeCOM3 = Sys.Ms();
		}
#endif
		if (USART_GetITStatus(USART3, USART_IT_IDLE) == SET)
		{
			USART_ReceiveData(USART3);//��ȡ���� ע�⣺������Ҫ�������ܹ�����жϱ�־λ����Ҳ��֪��Ϊɶ��
			USART_ClearITPendingBit(USART3, USART_IT_IDLE);         //����жϱ�־
#if defined COM3RXDMAFLAG
			int curlen = 0;
			curlen = DeviceConfigCenter::BUFLEN_RX3 - DMA_GetCurrDataCounter(DMA1_Channel3);	//����ӱ�֡���ݳ���			
			Rxx3.SetLength(curlen);
			MYDMA_Enable(DMA1_Channel3);                   //�ָ�DMAָ�룬�ȴ���һ�εĽ���						
#endif
#if defined COM3RCVIDLEINTFLAG
			if (DeviceConfigCenter::PRcvCOM3)
			{
				(*DeviceConfigCenter::PRcvCOM3)();
			}
#endif
		}
		//����
		/* �����ͻ��������ж� */
		if (USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
		{
#if defined COM3SENDINTFLAG
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
#endif
		}
		/* ����bitλȫ��������ϵ��ж� */
		else if (USART_GetITStatus(USART3, USART_IT_TC) != RESET)
		{
#if defined COM3SENDINTFLAG
			if (Txx3.Empty())
			{
				/* �������FIFO������ȫ��������ϣ���ֹ���ݷ�������ж� */
				USART_ITConfig(USART3, USART_IT_TC, DISABLE);

				/* �ص�����, һ����������RS485ͨ�ţ���RS485оƬ����Ϊ����ģʽ��������ռ���� */
				Txx3.Clear();
				if (DeviceConfigCenter::pCOM3Rx485)
				{
					*DeviceConfigCenter::pCOM3Rx485 = 0;
				}
			}
			else
			{
				/* ��������£��������˷�֧ */
				/* �������FIFO�����ݻ�δ��ϣ���ӷ���FIFOȡ1������д�뷢�����ݼĴ��� */
				USART_SendData(USART3, Txx3.Dequeue());
			}
#endif
		}
#endif
	}
	void UART41_IRQHandler(void)
	{
#if defined USECOM4		
		//����
#if (defined USECOM4RXD)&&(!defined COM4RXDMAFLAG)
		volatile uint8_t ch;
		if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
		{
			ch = USART_ReceiveData(UART4);
			Rxx4.Enqueue(ch);
			DeviceConfigCenter::RcvLastTimeCOM4 = Sys.Ms();
		}
#endif
		if (USART_GetITStatus(UART4, USART_IT_IDLE) == SET)
		{
			USART_ReceiveData(UART4);//��ȡ���� ע�⣺������Ҫ�������ܹ�����жϱ�־λ����Ҳ��֪��Ϊɶ��
			USART_ClearITPendingBit(UART4, USART_IT_IDLE);         //����жϱ�־
#if defined COM4RXDMAFLAG
			int curlen = 0;
			curlen = DeviceConfigCenter::BUFLEN_RX4 - DMA_GetCurrDataCounter(DMA2_Channel5);	//����ӱ�֡���ݳ���			
			Rxx4.SetLength(curlen);
			MYDMA_Enable(DMA2_Channel5);                   //�ָ�DMAָ�룬�ȴ���һ�εĽ���						
#endif
#if defined COM4RCVIDLEINTFLAG
			if (DeviceConfigCenter::PRcvCOM4)
			{
				(*DeviceConfigCenter::PRcvCOM4)();
			}
#endif
		}
		//����
		/* �����ͻ��������ж� */
		if (USART_GetITStatus(UART4, USART_IT_TXE) != RESET)
		{
#if defined COM4SENDINTFLAG
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
#endif
		}
		/* ����bitλȫ��������ϵ��ж� */
		else if (USART_GetITStatus(UART4, USART_IT_TC) != RESET)
		{
#if defined COM4SENDINTFLAG
			if (Txx4.Empty())
			{
				/* �������FIFO������ȫ��������ϣ���ֹ���ݷ�������ж� */
				USART_ITConfig(UART4, USART_IT_TC, DISABLE);

				/* �ص�����, һ����������RS485ͨ�ţ���RS485оƬ����Ϊ����ģʽ��������ռ���� */
				Txx4.Clear();
				if (DeviceConfigCenter::pCOM4Rx485)
				{
					*DeviceConfigCenter::pCOM4Rx485 = 0;
				}
			}
			else
			{
				/* ��������£��������˷�֧ */
				/* �������FIFO�����ݻ�δ��ϣ���ӷ���FIFOȡ1������д�뷢�����ݼĴ��� */
				USART_SendData(UART4, Txx4.Dequeue());
			}
#endif
		}
#endif
	}
	void UART51_IRQHandler(void)
	{
#if defined USECOM5		
		//����
#if (defined USECOM5RXD)&&(!defined COM5RXDMAFLAG)
		volatile uint8_t ch;
		if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
		{
			ch = USART_ReceiveData(UART5);
			Rxx5.Enqueue(ch);
			DeviceConfigCenter::RcvLastTimeCOM5 = Sys.Ms();
		}
#endif
		if (USART_GetITStatus(UART5, USART_IT_IDLE) == SET)
		{
			USART_ReceiveData(UART5);//��ȡ���� ע�⣺������Ҫ�������ܹ�����жϱ�־λ����Ҳ��֪��Ϊɶ��
			USART_ClearITPendingBit(UART5, USART_IT_IDLE);         //����жϱ�־
#if defined COM5RXDMAFLAG
			int curlen = 0;
			curlen = DeviceConfigCenter::BUFLEN_RX5 - DMA_GetCurrDataCounter(DMA2_Channel5);	//����ӱ�֡���ݳ���			
			Rxx5.SetLength(curlen);
			MYDMA_Enable(DMA2_Channel5);                   //�ָ�DMAָ�룬�ȴ���һ�εĽ���						
#endif
#if defined COM5RCVIDLEINTFLAG
			if (DeviceConfigCenter::PRcvCOM5)
			{
				(*DeviceConfigCenter::PRcvCOM5)();
			}
#endif
		}
		//����
		/* �����ͻ��������ж� */
		if (USART_GetITStatus(UART5, USART_IT_TXE) != RESET)
		{
#if defined COM5SENDINTFLAG
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
				USART_SendData(UART5, Txx5.Dequeue());
			}
#endif
		}
		/* ����bitλȫ��������ϵ��ж� */
		else if (USART_GetITStatus(UART5, USART_IT_TC) != RESET)
		{
#if defined COM5SENDINTFLAG
			if (Txx5.Empty())
			{
				/* �������FIFO������ȫ��������ϣ���ֹ���ݷ�������ж� */
				USART_ITConfig(UART5, USART_IT_TC, DISABLE);

				/* �ص�����, һ����������RS485ͨ�ţ���RS485оƬ����Ϊ����ģʽ��������ռ���� */
				Txx5.Clear();
				if (DeviceConfigCenter::pCOM5Rx485)
				{
					*DeviceConfigCenter::pCOM5Rx485 = 0;
				}
			}
			else
			{
				/* ��������£��������˷�֧ */
				/* �������FIFO�����ݻ�δ��ϣ���ӷ���FIFOȡ1������д�뷢�����ݼĴ��� */
				USART_SendData(UART5, Txx5.Dequeue());
			}
#endif
		}
#endif
	}
	void TIM21_IRQHandler(void)
	{
#ifdef STM32F0
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//������жϱ�־λ���Ա��´�ʹ�á�
		if (DeviceConfigCenter::PTim2Update)
		{
			(*DeviceConfigCenter::PTim2Update)();
		}
#elif defined STM32F1 | defined STM32F4
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
			if (DeviceConfigCenter::PTim2Update)
			{
				(*DeviceConfigCenter::PTim2Update)();
			}
		}
#endif
	}
	
	void TIM31_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
			if (DeviceConfigCenter::PTim3Update)
			{
				(*DeviceConfigCenter::PTim3Update)();
			}
		}
	}
	void TIM41_IRQHandler(void)
	{
#if defined STM32F0
		//F0û��TIM4
#else
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
			if (DeviceConfigCenter::PTim4Update)
			{
				(*DeviceConfigCenter::PTim4Update)();
			}
		}
#endif
	}
	void TIM51_IRQHandler(void)
	{
#if defined STM32F0
		//F0û��TIM5
#else
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
			if (DeviceConfigCenter::PTim5Update)
			{
				(*DeviceConfigCenter::PTim5Update)();
			}
		}
#endif
	}

	void TIM61_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
			if (DeviceConfigCenter::PTim6Update)
			{
				(*DeviceConfigCenter::PTim6Update)();
			}
		}
	}

	void TIM71_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);
			if (DeviceConfigCenter::PTim7Update)
			{
				(*DeviceConfigCenter::PTim7Update)();
			}
		}
	}
	void EXTI01_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line0) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line0);
			if (DeviceConfigCenter::PExit0)
			{
				(*DeviceConfigCenter::PExit0)();
			}
		}
	}
	void EXTI11_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line1) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line1);
			if (DeviceConfigCenter::PExit1)
			{
				(*DeviceConfigCenter::PExit1)();
			}
		}
	}

	void EXTI21_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line2) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line2);
			if (DeviceConfigCenter::PExit2)
			{
				(*DeviceConfigCenter::PExit2)();
			}
		}
	}
	void EXTI31_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line3) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line3);
			if (DeviceConfigCenter::PExit3)
			{
				(*DeviceConfigCenter::PExit3)();
			}
		}
	}
	void EXTI41_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line4) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line4);
			if (DeviceConfigCenter::PExit4)
			{
				(*DeviceConfigCenter::PExit4)();
			}
		}
	}

	void EXTI9_51_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line5) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line5);
			if (DeviceConfigCenter::PExit5)
			{
				(*DeviceConfigCenter::PExit5)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line6) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line6);
			if (DeviceConfigCenter::PExit6)
			{
				(*DeviceConfigCenter::PExit6)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line7) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line7);
			if (DeviceConfigCenter::PExit7)
			{
				(*DeviceConfigCenter::PExit7)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line8) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line8);
			if (DeviceConfigCenter::PExit8)
			{
				(*DeviceConfigCenter::PExit8)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line9) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line9);
			if (DeviceConfigCenter::PExit9)
			{
				(*DeviceConfigCenter::PExit9)();
			}
		}
	}
	void EXTI15_101_IRQHandler(void)
	{
		if (EXTI_GetITStatus(EXTI_Line10) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line10);
			if (DeviceConfigCenter::PExit10)
			{
				(*DeviceConfigCenter::PExit10)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line11) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line11);
			if (DeviceConfigCenter::PExit11)
			{
				(*DeviceConfigCenter::PExit11)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line12) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line12);
			if (DeviceConfigCenter::PExit12)
			{
				(*DeviceConfigCenter::PExit12)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line13) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line13);
			if (DeviceConfigCenter::PExit13)
			{
				(*DeviceConfigCenter::PExit13)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line14) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line14);
			if (DeviceConfigCenter::PExit14)
			{
				(*DeviceConfigCenter::PExit14)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line15) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line15);
			if (DeviceConfigCenter::PExit15)
			{
				(*DeviceConfigCenter::PExit15)();
			}
		}
	}
}
