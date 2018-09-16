#include "DeviceConfigHelper.h"
#include "Platform\stm32.h"

extern "C"
{
	void SysTick_Handler(void)
	{

	}
	void DMA1_Channel2_IRQHandler(void)
	{
#if (defined USECOM3) && (defined COM3TXDMAFLAG)
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
	void DMA1_Channel3_IRQHandler(void)
	{
#if (defined USECOM3) && (defined COM3RXDMAFLAG)
		//USART3_RX
		if (DMA_GetITStatus(DMA1_IT_TC3))
		{
			//TODO:Add code here
			DMA_ClearITPendingBit(DMA1_IT_TC3);
		}
#endif
	}
	void DMA1_Channel4_IRQHandler(void)
	{
#if (defined USECOM1) && (defined COM1TXDMAFLAG)
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
	void DMA1_Channel5_IRQHandler()
	{
#if (defined USECOM1) && (defined COM1RXDMAFLAG)
		//USART1_RX
		if (DMA_GetITStatus(DMA1_IT_TC5))
		{
			DMA_ClearITPendingBit(DMA1_IT_TC5);
		}
#endif
	}
	void DMA1_Channel6_IRQHandler(void)
	{
#if (defined USECOM2) && (defined COM2RXDMAFLAG)
//		USART2_RX
		if (DMA_GetITStatus(DMA1_IT_TC6))
		{
			//TODO:Add code here
			DMA_ClearITPendingBit(DMA1_IT_TC6);
		}
#endif
	}
	void DMA1_Channel7_IRQHandler(void)
	{
#if (defined USECOM2) && (defined COM2TXDMAFLAG)
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
	void USART1_IRQHandler(void)
	{
#if defined USECOM1		
#if defined COM1RXDMAFLAG
#else
		volatile uint8_t ch;
		if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
		{
			ch = USART_ReceiveData(USART1);
			Rxx1.Enqueue(ch);
			DeviceConfigCenter::RcvLastTimeCOM1 = Sys.Ms();
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
			if (DeviceConfigCenter::PRcvCOM1)
			{
				(*DeviceConfigCenter::PRcvCOM1)();
			}
		}
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
	void USART2_IRQHandler(void)
	{
#if defined USECOM2
#if defined COM2RXDMAFLAG
#else
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
			//����֡�������
			USART_ReceiveData(USART2); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)   
			USART_ClearITPendingBit(USART2, USART_IT_IDLE);         //����жϱ�־
#if defined COM2RXDMAFLAG
			int curlen = 0;
			curlen = DeviceConfigCenter::BUFLEN_RX2 - DMA_GetCurrDataCounter(DMA1_Channel6);	//����ӱ�֡���ݳ���			
			Rxx2.SetLength(curlen);
			MYDMA_Enable(DMA1_Channel6);                   //�ָ�DMAָ�룬�ȴ���һ�εĽ���						
#endif
			if (DeviceConfigCenter::PRcvCOM2)
			{
				(*DeviceConfigCenter::PRcvCOM2)();
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
				if (DeviceConfigCenter::pCOM2Rx485)
				{
					Sys.Delay(200);
					*DeviceConfigCenter::pCOM2Rx485 = 0;
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
#if defined USECOM3
#if defined COM3RXDMAFLAG
#else
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
			//����֡�������
			USART_ReceiveData(USART3); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR) 
			USART_ClearITPendingBit(USART3, USART_IT_IDLE);         //����жϱ�־
#if defined COM3RXDMAFLAG
			int curlen = 0;
			curlen = DeviceConfigCenter::BUFLEN_RX3 - DMA_GetCurrDataCounter(DMA1_Channel3);	//����ӱ�֡���ݳ���			
			Rxx3.SetLength(curlen);
			MYDMA_Enable(DMA1_Channel3);                   //�ָ�DMAָ�룬�ȴ���һ�εĽ���						
#endif
			if (DeviceConfigCenter::PRcvCOM3)
			{
				(*DeviceConfigCenter::PRcvCOM3)();
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
		}
#endif
	}
	void UART4_IRQHandler(void)
	{
#if defined USECOM4
		volatile uint8_t ch;
#ifdef STM32F0
		if (USART_GetITStatus(USART4, USART_IT_RXNE) != RESET)
		{
			ch = USART_ReceiveData(USART4);
			Rxx4.Enqueue(ch);
			DeviceConfigCenter::RcvLastTimeCOM4 = Sys.Ms();
		}
		if (USART_GetITStatus(USART4, USART_IT_IDLE) == SET)
		{
			//����֡�������
			ch = USART_ReceiveData(USART4); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR) 
			if (DeviceConfigCenter::PRcvCOM4)
			{
				(*DeviceConfigCenter::PRcvCOM4)();
			}
		}
		/* �����ͻ��������ж� */
		if (USART_GetITStatus(USART4, USART_IT_TXE) != RESET)
		{
			if (Txx4.Empty())
			{
				/* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
				USART_ITConfig(USART4, USART_IT_TXE, DISABLE);
				/* ʹ�����ݷ�������ж� */
				USART_ITConfig(USART4, USART_IT_TC, ENABLE);
			}
			else
			{
				/* �ӷ���FIFOȡ1���ֽ�д�봮�ڷ������ݼĴ��� */
				USART_SendData(USART4, Txx4.Dequeue());
			}

		}
		/* ����bitλȫ��������ϵ��ж� */
		else if (USART_GetITStatus(USART4, USART_IT_TC) != RESET)
		{
			if (Txx4.Empty())
			{
				/* �������FIFO������ȫ��������ϣ���ֹ���ݷ�������ж� */
				USART_ITConfig(USART4, USART_IT_TC, DISABLE);

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
				USART_SendData(USART4, Txx4.Dequeue());
			}
		}
#elif defined STM32F1 | defined STM32F4
#if defined COM4RXDMAFLAG
#else
		if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
		{
			ch = USART_ReceiveData(UART4);
			Rxx4.Enqueue(ch);
			DeviceConfigCenter::RcvLastTimeCOM4 = Sys.Ms();
		}
#endif
		if (USART_GetITStatus(UART4, USART_IT_IDLE) == SET)
		{
			//����֡�������
			USART_ReceiveData(UART4); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR) 
			USART_ClearITPendingBit(UART4, USART_IT_IDLE);         //����жϱ�־
			if (DeviceConfigCenter::PRcvCOM4)
			{
				(*DeviceConfigCenter::PRcvCOM4)();
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
		}
#endif 
#endif
	}
	void UART5_IRQHandler(void)
	{
#if defined USECOM5
		volatile uint8_t ch;
#ifdef STM32F0
		if (USART_GetITStatus(USART5, USART_IT_RXNE) != RESET)
		{
			ch = USART_ReceiveData(USART5);
			Rxx5.Enqueue(ch);
			DeviceConfigCenter::RcvLastTimeCOM5 = Sys.Ms();
		}
		if (USART_GetITStatus(USART5, USART_IT_IDLE) == SET)
		{
			//����֡�������
			ch = USART_ReceiveData(USART5); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)  
			if (DeviceConfigCenter::PRcvCOM5)
			{
				(*DeviceConfigCenter::PRcvCOM5)();
			}
		}
		/* �����ͻ��������ж� */
		if (USART_GetITStatus(USART5, USART_IT_TXE) != RESET)
		{
			if (Txx5.Empty())
			{
				/* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
				USART_ITConfig(USART5, USART_IT_TXE, DISABLE);
				/* ʹ�����ݷ�������ж� */
				USART_ITConfig(USART5, USART_IT_TC, ENABLE);
			}
			else
			{
				/* �ӷ���FIFOȡ1���ֽ�д�봮�ڷ������ݼĴ��� */
				USART_SendData(USART5, Txx3.Dequeue());
			}

		}
		/* ����bitλȫ��������ϵ��ж� */
		else if (USART_GetITStatus(USART5, USART_IT_TC) != RESET)
		{
			if (Txx5.Empty())
			{
				/* �������FIFO������ȫ��������ϣ���ֹ���ݷ�������ж� */
				USART_ITConfig(USART5, USART_IT_TC, DISABLE);

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
				USART_SendData(USART5, Txx5.Dequeue());
			}
		}
#elif defined STM32F1 | defined STM32F4
#if defined COM5RXDMAFLAG
#else
		if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
		{
			ch = USART_ReceiveData(UART5);
			Rxx5.Enqueue(ch);
			DeviceConfigCenter::RcvLastTimeCOM5 = Sys.Ms();
		}
#endif
		if (USART_GetITStatus(UART5, USART_IT_IDLE) == SET)
		{
			//����֡�������
			USART_ReceiveData(UART5); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)  
			USART_ClearITPendingBit(UART5, USART_IT_IDLE);         //����жϱ�־
			if (DeviceConfigCenter::PRcvCOM5)
			{
				(*DeviceConfigCenter::PRcvCOM5)();
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
		}
#endif 		
#endif
	}
	void TIM2_IRQHandler(void)
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
	void TIM3_IRQHandler(void)
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
	void TIM4_IRQHandler(void)
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
	void TIM5_IRQHandler(void)
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

	void TIM6_IRQHandler(void)
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

	void TIM7_IRQHandler(void)
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
	void EXTI0_IRQHandler(void)
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
	void EXTI1_IRQHandler(void)
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

	void EXTI2_IRQHandler(void)
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
	void EXTI3_IRQHandler(void)
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
	void EXTI4_IRQHandler(void)
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

	void EXTI9_5_IRQHandler(void)
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
	void EXTI15_10_IRQHandler(void)
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
