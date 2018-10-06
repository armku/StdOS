#include "Interrupt.h"

#ifdef USE_GPS
GPS *pGPS1 = 0;
GPS *pGPS2 = 0;
GPS *pGPS3 = 0;
#endif
#ifdef USE_USART1
USART *pCOM1 = 0;
#endif
#ifdef USE_USART1_DMA
USART *pCOM1 = 0;
#endif

#ifdef USE_USART2
USART *pCOM2 = 0;
#endif
#ifdef USE_USART2_DMA
USART *pCOM2 = 0;
#endif

#ifdef USE_USART3
USART *pCOM3 = 0;
#endif
#ifdef USE_USART3_DMA
USART *pCOM3 = 0;
#endif

#ifdef USE_UART4
USART *pCOM4 = 0;
#endif
#ifdef USE_UART4_DMA
USART *pCOM4 = 0;
#endif

#ifdef USE_UART5
USART *pCOM5 = 0;
#endif
#ifdef USE_UART5_DMA
USART *pCOM5 = 0;
#endif

#ifdef USE_I2C1
I2C *pI2C1;
#endif

#ifdef USE_I2C2
I2C *pI2C2;
#endif

#ifdef USE_CAPTURE_TIM1
InputCapture_TIM *pICP1;
#endif

#ifdef USE_CAPTURE_TIM2
InputCapture_TIM *pICP2;
#endif

#ifdef USE_CAPTURE_TIM3
InputCapture_TIM *pICP3;
#endif

#ifdef USE_CAPTURE_TIM4
InputCapture_TIM *pICP4;
#endif
#ifdef USE_CAPTURE_EXIT0
InputCapture_EXIT *pICPExt0;
#endif
#ifdef USE_CAPTURE_EXIT1
InputCapture_EXIT *pICPExt1;
#endif
#ifdef USE_CAPTURE_EXIT2
InputCapture_EXIT *pICPExt2;
#endif
#ifdef USE_CAPTURE_EXIT3
InputCapture_EXIT *pICPExt3;
#endif
#ifdef USE_CAPTURE_EXIT4
InputCapture_EXIT *pICPExt4;
#endif
#ifdef USE_CAPTURE_EXIT5
InputCapture_EXIT *pICPExt5;
#endif
#ifdef USE_CAPTURE_EXIT6
InputCapture_EXIT *pICPExt6;
#endif
#ifdef USE_CAPTURE_EXIT7
InputCapture_EXIT *pICPExt7;
#endif
#ifdef USE_CAPTURE_EXIT8
InputCapture_EXIT *pICPExt8;
#endif
#ifdef USE_CAPTURE_EXIT9
InputCapture_EXIT *pICPExt9;
#endif
#ifdef USE_CAPTURE_EXIT10
InputCapture_EXIT *pICPExt10;
#endif
#ifdef USE_CAPTURE_EXIT11
InputCapture_EXIT *pICPExt11;
#endif
#ifdef USE_CAPTURE_EXIT12
InputCapture_EXIT *pICPExt12;
#endif
#ifdef USE_CAPTURE_EXIT13
InputCapture_EXIT *pICPExt13;
#endif
#ifdef USE_CAPTURE_EXIT14
InputCapture_EXIT *pICPExt14;
#endif
#ifdef USE_CAPTURE_EXIT15
InputCapture_EXIT *pICPExt15;
#endif
#ifdef USE_TIMER1
Timer *pTIM1;
#endif

#ifdef USE_TIMER2
Timer *pTIM2;
#endif

#ifdef USE_TIMER3
Timer *pTIM3;
#endif

#ifdef USE_TIMER4
Timer *pTIM4;
#endif

#ifdef USE_TIMER5
Timer *pTIM5;
#endif

#ifdef USE_TIMER6
Timer *pTIM6;
#endif

#ifdef USE_TIMER7
Timer *pTIM7;
#endif

#ifdef USE_ULTRASONIC_EXTI0
Ultrasonic	*pUltExt0;
#endif
#ifdef USE_ULTRASONIC_EXTI1
Ultrasonic	*pUltExt1;
#endif
#ifdef USE_ULTRASONIC_EXTI2
Ultrasonic	*pUltExt2;
#endif
#ifdef USE_ULTRASONIC_EXTI3
Ultrasonic	*pUltExt3;
#endif
#ifdef USE_ULTRASONIC_EXTI4
Ultrasonic	*pUltExt4;
#endif
#ifdef USE_ULTRASONIC_EXTI5
Ultrasonic	*pUltExt5;
#endif
#ifdef USE_ULTRASONIC_EXTI6
Ultrasonic	*pUltExt6;
#endif
#ifdef USE_ULTRASONIC_EXTI7
Ultrasonic	*pUltExt7;
#endif
#ifdef USE_ULTRASONIC_EXTI8
Ultrasonic	*pUltExt8;
#endif
#ifdef USE_ULTRASONIC_EXTI9
Ultrasonic	*pUltExt9;
#endif
#ifdef USE_ULTRASONIC_EXTI10
Ultrasonic	*pUltExt10;
#endif
#ifdef USE_ULTRASONIC_EXTI11
Ultrasonic	*pUltExt11;
#endif
#ifdef USE_ULTRASONIC_EXTI12
Ultrasonic	*pUltExt12;
#endif
#ifdef USE_ULTRASONIC_EXTI13
Ultrasonic	*pUltExt13;
#endif
#ifdef USE_ULTRASONIC_EXTI14
Ultrasonic	*pUltExt14;
#endif
#ifdef USE_ULTRASONIC_EXTI15
Ultrasonic	*pUltExt15;
#endif

extern "C"
{
	void SysTick_Handler(void)
	{

	}
	//~~~~~~~~~~~~~~~~~~~~~ USART IQR  ~~~~~~~~~~~~~~~~~//
	void USART1_IRQHandler(void)    //----USART1 IRQ----//
	{
#ifdef USE_GPS
		if (pGPS1) {
			pGPS1->GPS_IRQHandler();
			return;
		}
#endif
#ifdef USE_USART1
		pCOM1->IRQ();
#endif
#ifdef USE_USART1_DMA
		pCOM1->IRQ();
#endif
	}
	void USART2_IRQHandler(void)    //----USART2 IRQ----//
	{
#ifdef USE_GPS
		if (pGPS2) {
			pGPS2->GPS_IRQHandler();
			return;
		}
#endif
#ifdef USE_USART2
		pCOM2->IRQ();
#endif
#ifdef USE_USART2_DMA
		pCOM2->IRQ();
#endif
	}
	void USART3_IRQHandler(void)    //----USART3 IRQ----//
	{
#ifdef USE_GPS
		if (pGPS3) {
			pGPS3->GPS_IRQHandler();
			return;
		}
#endif
#ifdef USE_USART3
		pCOM3->IRQ();
#endif
#ifdef USE_USART3_DMA
		pCOM3->IRQ();
#endif
	}
	void UART4_IRQHandler(void)    //----UART4 IRQ----//
	{
#ifdef USE_GPS
		if (pGPS4) {
			pGPS4->GPS_IRQHandler();
			return;
	}
#endif
#ifdef USE_UART4
		pCOM4->IRQ();
#endif
#ifdef USE_UART4_DMA
		pCOM4->IRQ();
#endif
}
	void UART5_IRQHandler(void)    //----UART5 IRQ----//
	{
#ifdef USE_GPS
		if (pGPS5) {
			pGPS5->GPS_IRQHandler();
			return;
	}
#endif
#ifdef USE_UART5
		pCOM5->IRQ();
#endif
#ifdef USE_UART5_DMA
		pCOM5->IRQ();
#endif
}
	//~~~~~~~~~~~~~~~~~~~~~  IIC IQR  ~~~~~~~~~~~~~~~~~~//
	void I2C1_EV_IRQHandler(void)   //----I2C1 EVENT----//
	{
#ifdef USE_I2C1
		pI2C1->EventIRQ();
#endif
	}
	void I2C1_ER_IRQHandler(void)   //----I2C1 ERROE----//
	{
#ifdef USE_I2C1
		pI2C1->ErrorIRQ();
#endif
	}
	void I2C2_EV_IRQHandler(void)   //----I2C2 EVENT----//
	{
#ifdef USE_I2C2
		pI2C2->EventIRQ();
#endif
	}
	void I2C2_ER_IRQHandler(void)   //----I2C2 ERROE----//
	{
#ifdef USE_I2C2
		pI2C2->ErrorIRQ();
#endif
	}
	//~~~~~~~~~~~~~~~~~~~~~  TIM IQR  ~~~~~~~~~~~~~~~~~~//
	void TIM1_UP_IRQHandler(void)   //----TIM1 Up-------//
	{
#ifdef USE_TIMER1
		TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);
		Timer1_IRQ();
#endif
	}

#ifdef USE_CAPTURE_TIM1
	void TIM1_CC_IRQHandler(void)
	{
		pICP1->IRQ();
	}
#endif


	void TIM2_IRQHandler(void)      //----TIM2 IRQ------// 
	{
//		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//		{
#ifdef USE_TIMER2
		TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
		Timer2_IRQ();
#endif	

#ifdef USE_CAPTURE_TIM2
		pICP2->IRQ();
#endif
		#ifdef STM32F0
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//先清空中断标志位，以备下次使用。
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
		//}
	}
	void TIM3_IRQHandler(void)      //----TIM3 IRQ------// 
	{
//		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
//		{
#ifdef USE_TIMER3
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
		Timer3_IRQ();
#endif	

#ifdef USE_CAPTURE_TIM3
		pICP3->IRQ();
#endif
		if (DeviceConfigCenter::PTim3Update)
			{
				(*DeviceConfigCenter::PTim3Update)();
			}
		//}
	}
	void TIM4_IRQHandler(void)      //----TIM4 IRQ------// 
	{
//		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
//		{
#ifdef USE_TIMER4
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
		Timer4_IRQ();
#endif	

#ifdef USE_CAPTURE_TIM4
		pICP4->IRQ();
#endif
		if (DeviceConfigCenter::PTim4Update)
			{
				(*DeviceConfigCenter::PTim4Update)();
			}
//		}
	}
		void TIM5_IRQHandler(void)
	{
#if defined STM32F0
		//F0没有TIM5
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

	//~~~~~~~~~~~~~~~~~~~~~~EXTI IQR  ~~~~~~~~~~~~~~~~~~//
	void EXTI0_IRQHandler(void) 	  //----EXTI0 IRQ-----//
	{
		//if (EXTI_GetITStatus(EXTI_Line0) != RESET)
		//{
		EXTI_ClearITPendingBit(EXTI_Line0);
#ifdef USE_EXTI0
		EXTI0_IRQ();
#endif

#ifdef USE_ULTRASONIC_EXTI0
		pUltExt0->IRQ();
#endif

#ifdef USE_CAPTURE_EXIT0
		pICPExt0->IRQ();
#endif
		if (DeviceConfigCenter::PExit0)
			{
				(*DeviceConfigCenter::PExit0)();
			}
		//}
	}
	void EXTI1_IRQHandler(void) 	  //----EXTI1 IRQ-----//
	{
//		if (EXTI_GetITStatus(EXTI_Line1) != RESET)
//		{
		EXTI_ClearITPendingBit(EXTI_Line1);
#ifdef USE_EXTI1
		EXTI1_IRQ();
#endif

#ifdef USE_ULTRASONIC_EXTI1
		pUltExt1->IRQ();
#endif

#ifdef USE_CAPTURE_EXIT1
		pICPExt1->IRQ();
#endif
		if (DeviceConfigCenter::PExit1)
			{
				(*DeviceConfigCenter::PExit1)();
			}
		//}
	}
	void EXTI2_IRQHandler(void) 	  //----EXTI2 IRQ-----//
	{
//		if (EXTI_GetITStatus(EXTI_Line2) != RESET)
//		{
		EXTI_ClearITPendingBit(EXTI_Line2);
#ifdef USE_EXTI2
		EXTI2_IRQ();
#endif

#ifdef USE_ULTRASONIC_EXTI2
		pUltExt2->IRQ();
#endif

#ifdef USE_CAPTURE_EXIT2
		pICPExt2->IRQ();
#endif
		if (DeviceConfigCenter::PExit2)
			{
				(*DeviceConfigCenter::PExit2)();
			}
		//}
	}
	void EXTI3_IRQHandler(void) 	  //----EXTI3 IRQ-----//
	{
//		if (EXTI_GetITStatus(EXTI_Line3) != RESET)
//		{
		EXTI_ClearITPendingBit(EXTI_Line3);
#ifdef USE_EXTI3
		EXTI3_IRQ();
#endif

#ifdef USE_ULTRASONIC_EXTI3
		pUltExt3->IRQ();
#endif

#ifdef USE_CAPTURE_EXIT3
		pICPExt3->IRQ();
#endif
		if (DeviceConfigCenter::PExit3)
			{
				(*DeviceConfigCenter::PExit3)();
			}
		//}
	}
	void EXTI4_IRQHandler(void) 	  //----EXTI4 IRQ-----//
	{
//		if (EXTI_GetITStatus(EXTI_Line4) != RESET)
//		{
		EXTI_ClearITPendingBit(EXTI_Line4);
#ifdef USE_EXTI4
		EXTI4_IRQ();
#endif

#ifdef USE_ULTRASONIC_EXTI4
		pUltExt4->IRQ();
#endif

#ifdef USE_CAPTURE_EXIT4
		pICPExt4->IRQ();
#endif
		if (DeviceConfigCenter::PExit4)
			{
				(*DeviceConfigCenter::PExit4)();
			}
		//}
	}

	void EXTI9_5_IRQHandler(void) 	//---EXTI9_5 IRQ----//
	{
		if (EXTI_GetITStatus(EXTI_Line5))       //--EXTI5--//
		{
			EXTI_ClearITPendingBit(EXTI_Line5);
#ifdef USE_EXTI5
			EXTI5_IRQ();
#endif

#ifdef USE_ULTRASONIC_EXTI5
			pUltExt5->IRQ();
#endif

#ifdef USE_CAPTURE_EXIT5
			pICPExt5->IRQ();
#endif
			if (DeviceConfigCenter::PExit5)
			{
				(*DeviceConfigCenter::PExit5)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line6))       //--EXTI6--//
		{
			EXTI_ClearITPendingBit(EXTI_Line6);
#ifdef USE_EXTI6
			EXTI6_IRQ();
#endif

#ifdef USE_ULTRASONIC_EXTI6
			pUltExt6->IRQ();
#endif

#ifdef USE_CAPTURE_EXIT6
			pICPExt6->IRQ();
#endif
			if (DeviceConfigCenter::PExit6)
			{
				(*DeviceConfigCenter::PExit6)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line7))       //--EXTI7--//
		{
			EXTI_ClearITPendingBit(EXTI_Line7);
#ifdef USE_EXTI7
			EXTI7_IRQ();
#endif

#ifdef USE_ULTRASONIC_EXTI7
			pUltExt7->IRQ();
#endif

#ifdef USE_CAPTURE_EXIT7
			pICPExt7->IRQ();
#endif
			if (DeviceConfigCenter::PExit7)
			{
				(*DeviceConfigCenter::PExit7)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line8))       //--EXTI8--//
		{
			EXTI_ClearITPendingBit(EXTI_Line8);
#ifdef USE_EXTI8
			EXTI8_IRQ();
#endif

#ifdef USE_ULTRASONIC_EXTI8
			pUltExt8->IRQ();
#endif

#ifdef USE_CAPTURE_EXIT8
			pICPExt8->IRQ();
#endif
			if (DeviceConfigCenter::PExit8)
			{
				(*DeviceConfigCenter::PExit8)();
			}
		}
		if (EXTI_GetITStatus(EXTI_Line9))       //--EXTI9--//
		{
			EXTI_ClearITPendingBit(EXTI_Line9);
#ifdef USE_EXTI9
			EXTI9_IRQ();
#endif

#ifdef USE_ULTRASONIC_EXTI9
			pUltExt9->IRQ();
#endif

#ifdef USE_CAPTURE_EXIT9
			pICPExt9->IRQ();
#endif
			if (DeviceConfigCenter::PExit9)
			{
				(*DeviceConfigCenter::PExit9)();
			}
		}

	}
	void EXTI15_10_IRQHandler(void) //---EXTI15_10 IRQ--//
	{
		if (EXTI_GetITStatus(EXTI_Line10))     //--EXTI10--//
		{
			EXTI_ClearITPendingBit(EXTI_Line10);
#ifdef USE_EXTI10
			EXTI10_IRQ();
#endif

#ifdef USE_ULTRASONIC_EXTI10
			pUltExt10->IRQ();
#endif

#ifdef USE_CAPTURE_EXIT10
			pICPExt10->IRQ();
#endif
			if (DeviceConfigCenter::PExit10)
			{
				(*DeviceConfigCenter::PExit10)();
			}
		}

		if (EXTI_GetITStatus(EXTI_Line11))     //--EXTI11--//
		{
			EXTI_ClearITPendingBit(EXTI_Line11);
#ifdef USE_EXTI11
			EXTI11_IRQ();
#endif

#ifdef USE_ULTRASONIC_EXTI11
			pUltExt11->IRQ();
#endif

#ifdef USE_CAPTURE_EXIT11
			pICPExt11->IRQ();
#endif
			if (DeviceConfigCenter::PExit11)
			{
				(*DeviceConfigCenter::PExit11)();
			}
		}

		if (EXTI_GetITStatus(EXTI_Line12))     //--EXTI12--//
		{
			EXTI_ClearITPendingBit(EXTI_Line12);
#ifdef USE_EXTI12
			EXTI12_IRQ();
#endif

#ifdef USE_ULTRASONIC_EXTI12
			pUltExt12->IRQ();
#endif

#ifdef USE_CAPTURE_EXIT12
			pICPExt12->IRQ();
#endif
			if (DeviceConfigCenter::PExit12)
			{
				(*DeviceConfigCenter::PExit12)();
			}
		}

		if (EXTI_GetITStatus(EXTI_Line13))     //--EXTI13--//
		{
			EXTI_ClearITPendingBit(EXTI_Line13);
#ifdef USE_EXTI13
			EXTI13_IRQ();
#endif

#ifdef USE_ULTRASONIC_EXTI13
			pUltExt13->IRQ();
#endif

#ifdef USE_CAPTURE_EXIT13
			pICPExt13->IRQ();
#endif
			if (DeviceConfigCenter::PExit13)
			{
				(*DeviceConfigCenter::PExit13)();
			}
		}

		if (EXTI_GetITStatus(EXTI_Line14))     //--EXTI14--//
		{
			EXTI_ClearITPendingBit(EXTI_Line14);
#ifdef USE_EXTI14
			EXTI14_IRQ();
#endif

#ifdef USE_ULTRASONIC_EXTI14
			pUltExt14->IRQ();
#endif

#ifdef USE_CAPTURE_EXIT14
			pICPExt14->IRQ();
#endif
			if (DeviceConfigCenter::PExit14)
			{
				(*DeviceConfigCenter::PExit14)();
			}
		}

		if (EXTI_GetITStatus(EXTI_Line15))     //--EXTI15--//
		{
			EXTI_ClearITPendingBit(EXTI_Line15);
#ifdef USE_EXTI15
			EXTI15_IRQ();
#endif

#ifdef USE_ULTRASONIC_EXTI15
			pUltExt15->IRQ();
#endif

#ifdef USE_CAPTURE_EXIT15
			pICPExt15->IRQ();
#endif
			if (DeviceConfigCenter::PExit15)
			{
				(*DeviceConfigCenter::PExit15)();
			}
		}
	}

	//~~~~~~~~~~~~~~~~~~~~~~ DMA IQR  ~~~~~~~~~~~~~~~~~~//
	void DMA1_Channel1_IRQHandler() //----DMA1 CH1------//
	{

	}
	void DMA1_Channel2_IRQHandler() //----DMA1 CH2------//
	{
#ifdef USE_USART3_DMA
		pCOM3->DMAIRQ();
#endif	
	}
	void DMA1_Channel3_IRQHandler() //----DMA1 CH3------//
	{

	}
	void DMA1_Channel4_IRQHandler() //----DMA1 CH4------//
	{
#ifdef USE_USART1_DMA
		pCOM1->DMAIRQ();
#endif
	}
	void DMA1_Channel5_IRQHandler() //----DMA1 CH5------//
	{

	}
	void DMA1_Channel6_IRQHandler() //----DMA1 CH6------//
	{

	}
	void DMA1_Channel7_IRQHandler() //----DMA1 CH7------//
	{
#ifdef USE_USART2_DMA
		pCOM2->DMAIRQ();
#endif	
	}
	void DMA1_Channel8_IRQHandler() //----DMA1 CH8------//
	{

	}

#ifdef USE_TIMER1
	void Timer1_IRQ()
	{
	}
#endif

#ifdef USE_TIMER2
	void Timer2_IRQ()
	{
	}
#endif
#ifdef USE_TIMER3
	void Timer3_IRQ()
	{
	}
#endif
#ifdef USE_TIMER4
	void Timer4_IRQ()
	{
	}
#endif
#ifdef USE_EXTI0
	void	EXTI0_IRQ()
	{
	}
#endif
#ifdef USE_EXTI1
	void	EXTI1_IRQ()
	{
	}
#endif
#ifdef USE_EXTI2
	void	EXTI2_IRQ()
	{
	}
#endif
#ifdef USE_EXTI3
	void	EXTI3_IRQ()
	{
	}
#endif
#ifdef USE_EXTI4
	void	EXTI4_IRQ()
	{
	}
#endif
#ifdef USE_EXTI5
	void	EXTI5_IRQ()
	{
	}
#endif
#ifdef USE_EXTI6
	void	EXTI6_IRQ()
	{
	}
#endif
#ifdef USE_EXTI7
	void	EXTI7_IRQ()
	{
	}
#endif
#ifdef USE_EXTI8
	void	EXTI8_IRQ()
	{
	}
#endif
#ifdef USE_EXTI9
	void	EXTI9_IRQ()
	{
	}
#endif
#ifdef USE_EXTI10
	void	EXTI10_IRQ()
	{
	}
#endif
#ifdef USE_EXTI11
	void	EXTI11_IRQ()
	{
	}
#endif
#ifdef USE_EXTI12
	void	EXTI12_IRQ()
	{
	}
#endif
#ifdef USE_EXTI13
	void	EXTI13_IRQ()
	{
	}
#endif
#ifdef USE_EXTI14
	void	EXTI14_IRQ()
	{
	}
#endif
#ifdef USE_EXTI15
	void	EXTI15_IRQ()
	{
	}
#endif
}
///以下为需要优化内容，旧版本
#include "Platform\stm32.h"
//中断

Func DeviceConfigCenter::PExit0 = 0;
Func DeviceConfigCenter::PExit1 = 0;
Func DeviceConfigCenter::PExit2 = 0;
Func DeviceConfigCenter::PExit3 = 0;
Func DeviceConfigCenter::PExit4 = 0;
Func DeviceConfigCenter::PExit5 = 0;
Func DeviceConfigCenter::PExit6 = 0;
Func DeviceConfigCenter::PExit7 = 0;
Func DeviceConfigCenter::PExit8 = 0;
Func DeviceConfigCenter::PExit9 = 0;
Func DeviceConfigCenter::PExit10 = 0;
Func DeviceConfigCenter::PExit11 = 0;
Func DeviceConfigCenter::PExit12 = 0;
Func DeviceConfigCenter::PExit13 = 0;
Func DeviceConfigCenter::PExit14 = 0;
Func DeviceConfigCenter::PExit15 = 0;

//串口
OutputPort *DeviceConfigCenter::pCOM1Rx485 = 0;
OutputPort *DeviceConfigCenter::pCOM2Rx485 = 0;
OutputPort *DeviceConfigCenter::pCOM3Rx485 = 0;
OutputPort *DeviceConfigCenter::pCOM4Rx485 = 0;
OutputPort *DeviceConfigCenter::pCOM5Rx485 = 0;

Func DeviceConfigCenter::PRcvCOM1 = 0;
Func DeviceConfigCenter::PRcvCOM2 = 0;
Func DeviceConfigCenter::PRcvCOM3 = 0;
Func DeviceConfigCenter::PRcvCOM4 = 0;
Func DeviceConfigCenter::PRcvCOM5 = 0;

int DeviceConfigCenter::RcvLastTimeCOM1 = 0;//串口1最后接收数据时间
int DeviceConfigCenter::RcvLastTimeCOM2 = 0;//串口2最后接收数据时间
int DeviceConfigCenter::RcvLastTimeCOM3 = 0;//串口3最后接收数据时间
int DeviceConfigCenter::RcvLastTimeCOM4 = 0;//串口4最后接收数据时间
int DeviceConfigCenter::RcvLastTimeCOM5 = 0;//串口5最后接收数据时间

int DeviceConfigCenter::RcvCom1PackInterval = 1;//串口1接收数据帧时间间隔
int DeviceConfigCenter::RcvCom2PackInterval = 1;//串口2接收数据帧时间间隔
int DeviceConfigCenter::RcvCom3PackInterval = 1;//串口3接收数据帧时间间隔
int DeviceConfigCenter::RcvCom4PackInterval = 1;//串口4接收数据帧时间间隔
int DeviceConfigCenter::RcvCom5PackInterval = 1;//串口5接收数据帧时间间隔

int DeviceConfigCenter::BUFLEN_TX1 = 0;	//串口1发送缓冲区长度
int DeviceConfigCenter::BUFLEN_RX1 = 0;	//串口1接收缓冲区长度
int DeviceConfigCenter::BUFLEN_TX2 = 0;	//串口2发送缓冲区长度
int DeviceConfigCenter::BUFLEN_RX2 = 0;	//串口2接收缓冲区长度
int DeviceConfigCenter::BUFLEN_TX3 = 0;	//串口3发送缓冲区长度
int DeviceConfigCenter::BUFLEN_RX3 = 0;	//串口3接收缓冲区长度
int DeviceConfigCenter::BUFLEN_TX4 = 0;	//串口4发送缓冲区长度
int DeviceConfigCenter::BUFLEN_RX4 = 0;	//串口4接收缓冲区长度
int DeviceConfigCenter::BUFLEN_TX5 = 0;	//串口5发送缓冲区长度
int DeviceConfigCenter::BUFLEN_RX5 = 0;	//串口5接收缓冲区长度

int DeviceConfigCenter::FLAG_TX1EN = 1;	//串口1可以发送
int DeviceConfigCenter::FLAG_TX2EN = 1;	//串口2可以发送
int DeviceConfigCenter::FLAG_TX3EN = 1;	//串口3可以发送
int DeviceConfigCenter::FLAG_TX4EN = 1;	//串口4可以发送
int DeviceConfigCenter::FLAG_TX5EN = 1;	//串口5可以发送

//定时器
Func DeviceConfigCenter::PTim2Update = 0;
Func DeviceConfigCenter::PTim3Update = 0;
Func DeviceConfigCenter::PTim4Update = 0;
Func DeviceConfigCenter::PTim5Update = 0;
Func DeviceConfigCenter::PTim6Update = 0;
Func DeviceConfigCenter::PTim7Update = 0;
Func DeviceConfigCenter::PTim8Update = 0;


//中断线打开、关闭
void DeviceConfigCenter::SetEXIT(int pinIndex, bool enable, Trigger trigger)
{
#if defined STM32F0

#elif defined STM32F1
	/* 配置EXTI中断线 */
	EXTI_InitTypeDef ext;
	EXTI_StructInit(&ext);
	ext.EXTI_Line = EXTI_Line0 << (pinIndex & 0X0F);
	ext.EXTI_Mode = EXTI_Mode_Interrupt;
	switch (trigger)
	{
	case Rising: ext.EXTI_Trigger = EXTI_Trigger_Rising; // 上升沿触发
		break;
	case Falling: ext.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
		break;
	case Both:
	default:
		ext.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // 上升沿下降沿触发
		break;
	}
	ext.EXTI_LineCmd = enable ? ENABLE : DISABLE;
	EXTI_Init(&ext);
#elif defined STM32F4

#endif
}
#if defined STM32F0

#elif defined STM32F1

static const int PORT_IRQns[] =
{
	EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn,  // 5个基础的
	EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,  // EXTI9_5
	EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn  // EXTI15_10
};
#elif defined STM32F4

#endif


void DeviceConfigCenter::InputPort_OpenEXTI(Pin pin, Trigger trigger)
{
#if defined STM32F0

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

#endif
}

#ifdef __cplusplus
extern "C" {
#endif	
#if defined USECOM1TXD
	static char com1tx[256];
	Queue	Txx1;
#endif
#if defined USECOM1RXD
	static char com1rx[256];
	Queue	Rxx1;
#endif
#if defined USECOM2TXD
	static char com2tx[256];
	Queue	Txx2;
#endif
#if defined USECOM2RXD
	static char com2rx[256];
	Queue	Rxx2;
#endif
#if defined USECOM3TXD
	static char com3tx[256];
	Queue	Txx3;
#endif
#if defined USECOM3RXD
	static char com3rx[256];
	Queue	Rxx3;
#endif
#if defined USECOM4TXD
	static char com4tx[256];
	Queue	Txx4;
#endif
#if defined USECOM4RXD
	static char com4rx[256];
	Queue	Rxx4;
#endif
#if defined USECOM5TXD
	static char com5tx[256];
	Queue	Txx5;
#endif
#if defined USECOM5RXD
	static char com5rx[256];
	Queue	Rxx5;
#endif

#ifdef __cplusplus
}
#endif
void DeviceConfigCenter::ConfigCom(COM com, int baudRate)
{
	switch (com)
	{
	case COM1:
		configCOM1(baudRate);
		break;
	case COM2:
		configCOM2(baudRate);
		break;
	case COM3:
		configCOM3(baudRate);
		break;
	case COM4:
		configCOM4(baudRate);
		break;
	case COM5:
		configCOM5(baudRate);
		break;
	case COM6:
		break;
	case COM7:
		break;
	case COM8:
		break;
	case COM_NONE:
		break;
	default:
		break;
	}
}
//改变波特率
void DeviceConfigCenter::ComChgBaudRate(COM com, int baudRate)
{
	switch (com)
	{
	case COM1:
		Com1ChgBaudRate(baudRate);
		break;
	case COM2:
		Com2ChgBaudRate(baudRate);
		break;
	case COM3:
		Com3ChgBaudRate(baudRate);
		break;
	case COM4:
		Com4ChgBaudRate(baudRate);
		break;
	case COM5:
		Com5ChgBaudRate(baudRate);
		break;
	case COM6:
		break;
	case COM7:
		break;
	case COM8:
		break;
	case COM_NONE:
		break;
	default:
		break;
	}
}
//发送数据
void DeviceConfigCenter::comSend(COM com, void* buf, int len)
{
	switch (com)
	{
	case COM1:
		com1send(buf, len);
		break;
	case COM2:
		com2send(buf, len);
		break;
	case COM3:
		com3send(buf, len);
		break;
	case COM4:
		com4send(buf, len);
		break;
	case COM5:
		com5send(buf, len);
		break;
	case COM6:
		break;
	case COM7:
		break;
	case COM8:
		break;
	case COM_NONE:
		break;
	default:
		break;
	}
}
#if defined COM1TXDMAFLAG
void USART1_SendDMA(uint8_t* buf, int len)
{
	DMA_InitTypeDef DMA_InitStruct;
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);

	DMA_Cmd(DMA1_Channel4, DISABLE);

	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)buf;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_BufferSize = len;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStruct);

	DMA_Cmd(DMA1_Channel4, ENABLE);
	DeviceConfigCenter::FLAG_TX1EN = 0;	//串口1不可以发送
}
#endif
#if defined COM2TXDMAFLAG
void USART2_SendDMA(uint8_t* buf, int len)
{
	DMA_InitTypeDef DMA_InitStruct;
	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);

	DMA_Cmd(DMA1_Channel7, DISABLE);

	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)buf;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_BufferSize = len;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel7, &DMA_InitStruct);

	DMA_Cmd(DMA1_Channel7, ENABLE);
	DeviceConfigCenter::FLAG_TX2EN = 0;	//串口2不可以发送
}
#endif
#if defined COM3TXDMAFLAG
void USART3_SendDMA(uint8_t* buf, int len)
{
	DMA_InitTypeDef DMA_InitStruct;
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

	DMA_Cmd(DMA1_Channel2, DISABLE);

	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)(&USART3->DR);
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)buf;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_BufferSize = len;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel2, &DMA_InitStruct);

	DMA_Cmd(DMA1_Channel2, ENABLE);
	DeviceConfigCenter::FLAG_TX3EN = 0;	//串口3不可以发送
}
#endif
#if defined COM4TXDMAFLAG
void USART4_SendDMA(uint8_t* buf, int len)
{
	DMA_InitTypeDef DMA_InitStruct;
	//DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

	//DMA_Cmd(DMA1_Channel2, DISABLE);

	//DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)(&UART4->DR);
	//DMA_InitStruct.DMA_MemoryBaseAddr = (u32)buf;
	//DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	//DMA_InitStruct.DMA_BufferSize = len;
	//DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	//DMA_InitStruct.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	//DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	//DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
	//DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	//DMA_Init(DMA1_Channel2, &DMA_InitStruct);

	//DMA_Cmd(DMA1_Channel2, ENABLE);
	DeviceConfigCenter::FLAG_TX4EN = 0;	//串口4不可以发送
}
#endif
#if defined COM5TXDMAFLAG
void USART5_SendDMA(uint8_t* buf, int len)
{
	DMA_InitTypeDef DMA_InitStruct;
	/*DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

	DMA_Cmd(DMA1_Channel2, DISABLE);

	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)(&UART5->DR);
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)buf;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_BufferSize = len;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel2, &DMA_InitStruct);

	DMA_Cmd(DMA1_Channel2, ENABLE);*/
	DeviceConfigCenter::FLAG_TX5EN = 0;	//串口5不可以发送
}
#endif
#if defined COM1TXDMAFLAG
uint8_t com1bufff[300];
#endif
#if defined COM2TXDMAFLAG
uint8_t com2bufff[300];
#endif
#if defined COM3TXDMAFLAG
uint8_t com3bufff[300];
#endif
#if defined COM4TXDMAFLAG
uint8_t com4bufff[300];
#endif
#if defined COM5TXDMAFLAG
uint8_t com5bufff[300];
#endif
void OS_ComSendChk(void *param);
void DeviceConfigCenter::com1send(void* buf, int len)
{
#if defined USECOM1TXD
	if (pCOM1Rx485)
	{
		*pCOM1Rx485 = 1;
	}

#if defined COM1TXDMAFLAG			
	Txx1.Write(bs);
#elif defined COM1SENDINTFLAG
	while (len > Txx1.RemainLength());//等待发送缓冲区可容纳足够内容
	//中断发送
	Sys.GlobalDisable();
	Txx1.Write(buf,len);
	Sys.GlobalEnable();
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
#else	
	for (int i = 0; i < len; i++)
	{
		/* 发送一个字节数据到USART */
		USART_SendData(USART1, ((uint8_t*)buf)[i]);

		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
	if (pCOM1Rx485)
	{
		Sys.Delay(100);
		*pCOM1Rx485 = 0;
	}
#endif
#endif
}
void DeviceConfigCenter::com2send(void* buf, int len)
{
#if defined USECOM2TXD
	if (pCOM2Rx485)
	{
		*pCOM2Rx485 = 1;
	}

#if defined COM2TXDMAFLAG			
	Txx2.Write(bs);
#elif defined COM2SENDINTFLAG
	while (len > Txx2.RemainLength());//等待发送缓冲区可容纳足够内容
											  //中断发送
	Sys.GlobalDisable();
	Txx2.Write(buf,len);
	Sys.GlobalEnable();
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
#else	
	for (int i = 0; i < bs.Length(); i++)
	{
		/* 发送一个字节数据到USART */
		USART_SendData(USART2, bs[i]);

		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}
	if (pCOM2Rx485)
	{
		Sys.Delay(100);
		*pCOM2Rx485 = 0;
	}
#endif
#endif
}
void DeviceConfigCenter::com3send(void* buf, int len)
{
#if defined USECOM3TXD
	if (pCOM3Rx485)
	{
		*pCOM3Rx485 = 1;
	}

#if defined COM3TXDMAFLAG			
	Txx3.Write(bs.GetBuffer(),bs.Length());
#elif defined COM3SENDINTFLAG
	while (bs.Length() > Txx3.RemainLength());//等待发送缓冲区可容纳足够内容
											  //中断发送
	Sys.GlobalDisable();
	Txx3.Write(bs.GetBuffer(),bs.Length());
	Sys.GlobalEnable();
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
#else	
	for (int i = 0; i < len; i++)
	{
		/* 发送一个字节数据到USART */
		USART_SendData(USART3, ((char*)buf)[i]);

		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	}
	if (pCOM3Rx485)
	{
		Sys.Delay(100);
		*pCOM3Rx485 = 0;
	}
#endif
#endif
}
void DeviceConfigCenter::com4send(void* buf, int len)
{
#if defined USECOM4TXD
	if (pCOM4Rx485)
	{
		*pCOM4Rx485 = 1;
	}

#if defined COM4TXDMAFLAG			
	Txx4.Write(bs);
#elif defined COM4SENDINTFLAG
	while (len > Txx4.RemainLength());//等待发送缓冲区可容纳足够内容
											  //中断发送
	Sys.GlobalDisable();
	Txx4.Write(buf,len);
	Sys.GlobalEnable();
	USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
#else	
	for (int i = 0; i < bs.Length(); i++)
	{
		/* 发送一个字节数据到USART */
		USART_SendData(UART4, bs[i]);

		/* 等待发送完毕 */
		while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
	}
	if (pCOM4Rx485)
	{
		Sys.Delay(100);
		*pCOM4Rx485 = 0;
	}
#endif
#endif
}
void DeviceConfigCenter::com5send(void* buf, int len)
{
#if defined USECOM5TXD
	if (pCOM5Rx485)
	{
		*pCOM5Rx485 = 1;
	}

#if defined COM5TXDMAFLAG			
	Txx5.Write(bs.GetBuffer(),bs.Length());
#elif defined COM5SENDINTFLAG
	while (len > Txx5.RemainLength());//等待发送缓冲区可容纳足够内容
											  //中断发送
	Sys.GlobalDisable();
	Txx5.Write(buf,len);
	Sys.GlobalEnable();
	USART_ITConfig(UART5, USART_IT_TXE, ENABLE);
#else	
	for (int i = 0; i < bs.Length(); i++)
	{
		/* 发送一个字节数据到USART */
		USART_SendData(UART5, （char* buf)[i]);

		/* 等待发送完毕 */
		while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
	}
	if (pCOM5Rx485)
	{
		Sys.Delay(100);
		*pCOM5Rx485 = 0;
	}
#endif
#endif
}

void DeviceConfigCenter::configCOM1(int baudRate)
{
#if defined USECOM1
	Port*		Ports[2];	// Tx/Rx
	Pin			Pins[2];	// Tx/Rx

#if defined USECOM1RXD
	DeviceConfigCenter::BUFLEN_RX1 = ArrayLength(com1rx);
	Rxx1.SetBuf(com1rx, ArrayLength(com1rx));
#endif
#if defined USECOM1TXD
	DeviceConfigCenter::BUFLEN_TX1 = ArrayLength(com1tx);
	Txx1.SetBuf(com1tx, ArrayLength(com1tx));
#endif
	SerialPort_GetPins(&Pins[0], &Pins[1], COM1);
	Ports[0] = new AlternatePort();
	Ports[1] = new InputPort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
#if defined STM32F0
#elif defined STM32F1	

	USART_InitTypeDef USART_InitStructure;

	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);

	/*	配置中断优先级 */
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* 使能串口1接收中断 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 串口接收中断配置

#if defined COM1TXDMAFLAG
	//DMA发送
	/*开启DMA时钟*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);  //配置DMA发送完成后产生中断

	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

	NVIC_InitTypeDef NVIC_InitStruct;

	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	//ENABLE DMA TX ISR
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
#endif // COM1TXDMAFLAG
#if defined COM1RXDMAFLAG
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//开启空闲中断
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);   //使能串口1 DMA接收

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	DMA_InitTypeDef DMA_InitStructureRcv;
	DMA_DeInit(DMA1_Channel5);   //将DMA的通道5寄存器重设为缺省值  串口1对应的是DMA通道5
	DMA_InitStructureRcv.DMA_PeripheralBaseAddr = (u32)&USART1->DR;  //DMA外设ADC基地址
	DMA_InitStructureRcv.DMA_MemoryBaseAddr = (u32)com1rx;  //DMA内存基地址
	DMA_InitStructureRcv.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
	DMA_InitStructureRcv.DMA_BufferSize = DeviceConfigCenter::BUFLEN_RX1;  //DMA通道的DMA缓存的大小
	DMA_InitStructureRcv.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructureRcv.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructureRcv.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructureRcv.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructureRcv.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructureRcv.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructureRcv.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel5, &DMA_InitStructureRcv);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器

	DMA_Cmd(DMA1_Channel5, ENABLE);  //正式驱动DMA传输
#endif

#if defined COM1RCVIDLEINTFLAG
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 
#endif

	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);
#elif defined STM32F4
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART1时钟
															  //串口1对应引脚复用映射
	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = baudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1

	USART_Cmd(USART1, ENABLE);  //使能串口1 

	//USART_ClearFlag(USART1, USART_FLAG_TC);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
												  //Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
#if defined COM1RCVIDLEINTFLAG
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 
#endif

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); //GPIO复用为USART
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //GPIO复用为USART

	SerialPort_GetPins(&Pins[0], &Pins[1], COM1);
	Ports[0] = new AlternatePort();
	Ports[1] = new AlternatePort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
#endif	
#endif
}
void DeviceConfigCenter::configCOM2(int baudRate)
{
#if defined USECOM2
	Port*		Ports[2];	// Tx/Rx
	Pin			Pins[2];	// Tx/Rx
#if defined USECOM2RXD
	DeviceConfigCenter::BUFLEN_RX2 = ArrayLength(com2rx);
	Rxx2.SetBuf(com2rx, ArrayLength(com2rx));
#endif
#if defined USECOM2TXD
	DeviceConfigCenter::BUFLEN_TX2 = ArrayLength(com2tx);
	Txx2.SetBuf(com2tx, ArrayLength(com2tx));
#endif
	SerialPort_GetPins(&Pins[0], &Pins[1], COM2);
	Ports[0] = new AlternatePort();
	Ports[1] = new InputPort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
#if defined STM32F0
#elif defined STM32F1

	USART_InitTypeDef USART_InitStructure;

	/* config USART2 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure);

	/*	配置中断优先级 */
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

#if defined COM2TXDMAFLAG
	//DMA发送
	/*开启DMA时钟*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);  //配置DMA发送完成后产生中断

	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

	NVIC_InitTypeDef NVIC_InitStruct;

	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	//ENABLE DMA TX ISR
	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
#endif // COM1TXDMAFLAG

	/* 使能串口2接收中断 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 串口接收中断配置
#if defined COM2RXDMAFLAG
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//开启空闲中断
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);   //使能串口 DMA接收

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	DMA_InitTypeDef DMA_InitStructureRcv;
	DMA_DeInit(DMA1_Channel6);   //将DMA的通道5寄存器重设为缺省值  串口1对应的是DMA通道5
	DMA_InitStructureRcv.DMA_PeripheralBaseAddr = (u32)&USART2->DR;  //DMA外设ADC基地址
	DMA_InitStructureRcv.DMA_MemoryBaseAddr = (u32)com2rx;  //DMA内存基地址
	DMA_InitStructureRcv.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
	DMA_InitStructureRcv.DMA_BufferSize = DeviceConfigCenter::BUFLEN_RX2;  //DMA通道的DMA缓存的大小
	DMA_InitStructureRcv.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructureRcv.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructureRcv.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructureRcv.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructureRcv.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructureRcv.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructureRcv.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel6, &DMA_InitStructureRcv);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器

	DMA_Cmd(DMA1_Channel6, ENABLE);  //正式驱动DMA传输
#endif
#if defined COM2RCVIDLEINTFLAG
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 
#endif
	USART_Cmd(USART2, ENABLE);
	USART_ClearFlag(USART2, USART_FLAG_TC);
#elif defined STM32F4
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART1时钟
														  //串口1对应引脚复用映射
														  //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = baudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口1

	USART_Cmd(USART2, ENABLE);  //使能串口1 

	//USART_ClearFlag(USART2, USART_FLAG_TC);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断
												  //Usart NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
#if defined COM2RCVIDLEINTFLAG
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 
#endif

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //GPIO复用为USART
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //GPIO复用为USART

	SerialPort_GetPins(&Pins[0], &Pins[1], COM1);
	Ports[0] = new AlternatePort();
	Ports[1] = new AlternatePort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
#endif	
#endif
}

void DeviceConfigCenter::configCOM3(int baudRate)
{
#if defined USECOM3
	Port*		Ports[2];	// Tx/Rx
	Pin			Pins[2];	// Tx/Rx
#if defined USECOM3RXD
	DeviceConfigCenter::BUFLEN_RX3 = ArrayLength(com3rx);
	Rxx3.SetBuf(com3rx, ArrayLength(com3rx));
#endif
#if defined USECOM3TXD
	DeviceConfigCenter::BUFLEN_TX3 = ArrayLength(com3tx);
	Txx3.SetBuf(com3tx, ArrayLength(com3tx));
#endif
	SerialPort_GetPins(&Pins[0], &Pins[1], COM3);
	Ports[0] = new AlternatePort();
	Ports[1] = new InputPort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
#if defined STM32F0

#elif defined STM32F1

	USART_InitTypeDef USART_InitStructure;

	/* config USART3 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* USART3 mode config */
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure);

	/*	配置中断优先级 */
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#if defined COM3TXDMAFLAG
	//DMA发送
	/*开启DMA时钟*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);  //配置DMA发送完成后产生中断

	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);

	NVIC_InitTypeDef NVIC_InitStruct;

	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	//ENABLE DMA TX ISR
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
#endif // COM1TXDMAFLAG
	/* 使能串口3接收中断 */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // 串口接收中断配置
#if defined COM3RXDMAFLAG
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//开启空闲中断
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);   //使能串口1 DMA接收

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	DMA_InitTypeDef DMA_InitStructureRcv;
	DMA_DeInit(DMA1_Channel3);   //将DMA的通道5寄存器重设为缺省值  串口对应的是DMA通道
	DMA_InitStructureRcv.DMA_PeripheralBaseAddr = (u32)&USART3->DR;  //DMA外设ADC基地址
	DMA_InitStructureRcv.DMA_MemoryBaseAddr = (u32)com3rx;  //DMA内存基地址
	DMA_InitStructureRcv.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
	DMA_InitStructureRcv.DMA_BufferSize = DeviceConfigCenter::BUFLEN_RX3;  //DMA通道的DMA缓存的大小
	DMA_InitStructureRcv.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructureRcv.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructureRcv.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructureRcv.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructureRcv.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructureRcv.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructureRcv.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel3, &DMA_InitStructureRcv);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器

	DMA_Cmd(DMA1_Channel3, ENABLE);  //正式驱动DMA传输
#endif
#if defined COM3RCVIDLEINTFLAG
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 
#endif
	USART_Cmd(USART3, ENABLE);
	USART_ClearFlag(USART3, USART_FLAG_TC);
#elif defined STM32F4
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//使能USART时钟
														  //串口1对应引脚复用映射
														  //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = baudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口1

	USART_Cmd(USART3, ENABLE);  //使能串口 

	//USART_ClearFlag(USART1, USART_FLAG_TC);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断
												  //Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
#if defined COM3RCVIDLEINTFLAG
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 
#endif

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3); //GPIO复用为USART
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3); //GPIO复用为USART

	SerialPort_GetPins(&Pins[0], &Pins[1], COM1);
	Ports[0] = new AlternatePort();
	Ports[1] = new AlternatePort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
#endif
#endif
}
void DeviceConfigCenter::configCOM4(int baudRate)
{
#if defined USECOM4
	Port*		Ports[2];	// Tx/Rx
	Pin			Pins[2];	// Tx/Rx
#if defined USECOM4RXD
	DeviceConfigCenter::BUFLEN_RX4 = ArrayLength(com4rx);
	Rxx4.SetBuf(com4rx, ArrayLength(com4rx));
#endif
#if defined USECOM4TXD
	DeviceConfigCenter::BUFLEN_TX4 = ArrayLength(com4tx);
	Txx4.SetBuf(com4tx, ArrayLength(com4tx));
#endif
#if defined STM32F0

#elif defined STM32F1 && defined STM32F10X_HD

	USART_InitTypeDef USART_InitStructure;

	/* config USART4 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	/* USART4 mode config */
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(UART4, &USART_InitStructure);

	/*	配置中断优先级 */
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* 使能串口4接收中断 */
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); // 串口接收中断配置
#if COM4RCVIDLEINTFLAG
	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 
#endif

	USART_Cmd(UART4, ENABLE);
	USART_ClearFlag(UART4, USART_FLAG_TC);

	SerialPort_GetPins(&Pins[0], &Pins[1], COM4);
	Ports[0] = new AlternatePort();
	Ports[1] = new InputPort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
#elif defined STM32F4
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);//使能USART时钟
														  //USART 初始化设置
	USART_InitStructure.USART_BaudRate = baudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART4, &USART_InitStructure); //初始化串口

	USART_Cmd(UART4, ENABLE);  //使能串口4 

	//USART_ClearFlag(USART4, USART_FLAG_TC);

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启相关中断
												  //Usart4 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
#if COM1RCVIDLEINTFLAG
	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 
#endif

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4); //GPIO复用为USART
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4); //GPIO复用为USART

	SerialPort_GetPins(&Pins[0], &Pins[1], COM1);
	Ports[0] = new AlternatePort();
	Ports[1] = new AlternatePort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
#endif

#endif
}
void DeviceConfigCenter::configCOM5(int baudRate)
{
#if defined USECOM5
	Port*		Ports[2];	// Tx/Rx
	Pin			Pins[2];	// Tx/Rx
#if defined USECOM5RXD
	DeviceConfigCenter::BUFLEN_RX5 = ArrayLength(com5rx);
	Rxx5.SetBuf(com5rx, ArrayLength(com5rx));
#endif
#if defined USECOM5TXD
	DeviceConfigCenter::BUFLEN_TX5 = ArrayLength(com5tx);
	Txx5.SetBuf(com5tx, ArrayLength(com5tx));
#endif
#if defined STM32F0

#elif defined STM32F1 && defined STM32F10X_HD

	USART_InitTypeDef USART_InitStructure;

	/* config USART5 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

	/* USART5 mode config */
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(UART5, &USART_InitStructure);

	/*	配置中断优先级 */
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* 使能串口5接收中断 */
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE); // 串口接收中断配置
#if COM5RCVIDLEINTFLAG
	USART_ITConfig(UART5, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 
#endif

	USART_Cmd(UART5, ENABLE);
	USART_ClearFlag(UART5, USART_FLAG_TC);

	SerialPort_GetPins(&Pins[0], &Pins[1], COM5);
	Ports[0] = new AlternatePort();
	Ports[1] = new InputPort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();

#elif defined STM32F4
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE); //使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);//使能USART时钟
														  //USART5 初始化设置
	USART_InitStructure.USART_BaudRate = baudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART5, &USART_InitStructure); //初始化串口

	USART_Cmd(UART5, ENABLE);  //使能串口 

	//USART_ClearFlag(USART5, USART_FLAG_TC);

	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启相关中断
												  //Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
#if COM1RCVIDLEINTFLAG
	USART_ITConfig(UART5, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 
#endif

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5); //GPIO复用为USART
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5); //GPIO复用为USART

	SerialPort_GetPins(&Pins[0], &Pins[1], COM1);
	Ports[0] = new AlternatePort();
	Ports[1] = new AlternatePort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
#endif
#endif
}
void DeviceConfigCenter::Com1ChgBaudRate(int baudRate)
{
#if defined USECOM1
#if defined STM32F0

#elif defined STM32F1
	USART_InitTypeDef USART_InitStructure;

	/* USART mode config */
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
#elif defined STM32F4
	USART_InitTypeDef USART_InitStructure;

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = baudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口
#endif
#endif
}
void DeviceConfigCenter::Com2ChgBaudRate(int baudRate)
{
#if defined USECOM2
#if defined STM32F0

#elif defined STM32F1
	USART_InitTypeDef USART_InitStructure;

	/* USART mode config */
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure);
#elif defined STM32F4
	USART_InitTypeDef USART_InitStructure;

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = baudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口
#endif
#endif
}
void DeviceConfigCenter::Com3ChgBaudRate(int baudRate)
{
#if defined USECOM3
#if defined STM32F0

#elif defined STM32F1
	USART_InitTypeDef USART_InitStructure;

	/* USART mode config */
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure);
#elif defined STM32F4
	USART_InitTypeDef USART_InitStructure;

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = baudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口
#endif
#endif
}
void DeviceConfigCenter::Com4ChgBaudRate(int baudRate)
{
#if defined USECOM4
#if defined STM32F0

#elif defined STM32F1
	USART_InitTypeDef USART_InitStructure;

	/* USART mode config */
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(UART4, &USART_InitStructure);
#elif defined STM32F4
	USART_InitTypeDef USART_InitStructure;

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = baudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART4, &USART_InitStructure); //初始化串口
#endif
#endif
}
void DeviceConfigCenter::Com5ChgBaudRate(int baudRate)
{
#if defined USECOM5
#if defined STM32F0

#elif defined STM32F1
	USART_InitTypeDef USART_InitStructure;

	/* USART mode config */
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(UART5, &USART_InitStructure);
#elif defined STM32F4
	USART_InitTypeDef USART_InitStructure;

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = baudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART5, &USART_InitStructure); //初始化串口
#endif
#endif
}
void DeviceConfigCenter::SerialPort_GetPins(Pin *txPin, Pin *rxPin, COM index, bool Remap)
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

void DeviceConfigCenter::TimeTickInit()//系统用定时器初始化
{
#if defined STM32F0
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能TIM2时钟

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//定义一个TIM_InitTypeDef类型的结构体
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清中断标志，以备下次中断到来使用

	TIM_TimeBaseStructure.TIM_Period = 1000;//1秒钟机2000个脉冲
	TIM_TimeBaseStructure.TIM_Prescaler = 35999; //36000分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //TIM_CKD_DIV1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//初始化定时器
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//使能溢出中断

	TIM_Cmd(TIM2, ENABLE);//定时器使能

	NVIC_InitTypeDef   NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#elif defined STM32F1
#if 1
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	/* 自动重装载寄存器周期的值(计数值) */
	TIM_TimeBaseStructure.TIM_Period = 2000 - 1;
	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
	/* 时钟预分频数为72 */
	TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1;

	/* 对外部时钟进行采样的时钟分频,这里没有用到 */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_DeInit(TIM2);
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update); // 清除标志位  必须要有！！ 否则 开启中断立马中断给你看
	TIM_Cmd(TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE); /*先关闭等待使用*/
	NVIC_InitTypeDef nvic;

	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannel = TIM2_IRQn;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;

	NVIC_Init(&nvic);
	NVIC_SetPriority(TIM2_IRQn, 3);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	NVIC_SetPriority(TIM2_IRQn, 0);
#else
	Timer2Config(1000);
#endif
	NVIC_SetPriority(SysTick_IRQn, 0);
#elif defined STM32F4
	Timer2Config(10000);
#endif
}

//定时器配置
void DeviceConfigCenter::TimerConfig(TIMER tim, int interval, int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
{
	switch (tim)
	{
	case Timer0:
		break;
	case Timer1:
		Timer1Config(interval);
		break;
	case Timer2:
		Timer2Config(interval);
		break;
	case Timer3:
		Timer3Config(interval);
		break;
	case Timer4:
		Timer4Config(interval);
		break;
	case Timer5:
		Timer5Config(interval);
		break;
	case Timer6:
		Timer6Config(interval);
		break;
	case Timer7:
		Timer7Config(interval);
		break;
	case Timer8:
		Timer8Config(interval);
		break;
	case Timer9:
		break;
	case Timer10:
		break;
	case Timer11:
		break;
	case Timer12:
		break;
	case Timer13:
		break;
	case Timer14:
		break;
	case Timer15:
		break;
	case Timer16:
		break;
	case Timer17:
		break;
	case Timer18:
		break;
	default:
		break;
	}
}
//定时器配置
void DeviceConfigCenter::Timer0Config(int interval)
{
#if defined STM32F0

#elif defined STM32F1

#elif defined STM32F4

#endif
}
//定时器配置
void DeviceConfigCenter::Timer1Config(int interval)
{
#if defined STM32F0

#elif defined STM32F1

#elif defined STM32F4

#endif
}
//定时器配置
void DeviceConfigCenter::Timer2Config(int interval)
{
#if defined STM32F0
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//定义一个TIM_InitTypeDef类型的结构体
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清中断标志，以备下次中断到来使用

	TIM_TimeBaseStructure.TIM_Period = interval;//1秒钟机2000个脉冲
	TIM_TimeBaseStructure.TIM_Prescaler = 35999; //36000分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //TIM_CKD_DIV1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//初始化定时器
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//使能溢出中断

	TIM_Cmd(TIM2, ENABLE);//定时器使能
#elif defined STM32F1
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// 开启TIMx_CLK,x,即内部时钟CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// 自动重装载寄存器周的值(计数值)
	TIM_TimeBaseStructure.TIM_Period = 1000;

	// 累计 TIM_Period个频率后产生一个更新或者中断
	// 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M
	TIM_TimeBaseStructure.TIM_Prescaler = 71;

	// 初始化定时器TIMx, x
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	// 清除计数器中断标志位
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);

	// 开启计数器中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	// 使能计数器
	TIM_Cmd(TIM2, ENABLE);

	// 暂时关闭TIMx,x的时钟，等待使用
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);

	/* 基本定时器 TIMx,x 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
#elif defined STM32F4
	//arr：自动重装值。
	//psc：时钟预分频数
	//定时器溢出时间计算方法 : Tout = ((arr + 1)*(psc + 1)) / Ft us.
	//Ft=定时器工作频率,单位:Mhz
	u16 arr = interval - 1;
	u16 psc = 8400 - 1;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  ///使能TIM时钟

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);//初始化TIM

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //允许定时器更新中断
	TIM_Cmd(TIM2, ENABLE); //使能定时器

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //定时器中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}
//定时器配置
void DeviceConfigCenter::Timer3Config(int interval)
{
#if defined STM32F0

#elif defined STM32F1
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// 开启TIMx_CLK,x,即内部时钟CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	// 自动重装载寄存器周的值(计数值)
	TIM_TimeBaseStructure.TIM_Period = 1000;

	// 累计 TIM_Period个频率后产生一个更新或者中断
	// 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M
	TIM_TimeBaseStructure.TIM_Prescaler = 71;

	// 初始化定时器TIMx, x
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	// 清除计数器中断标志位
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);

	// 开启计数器中断
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	// 使能计数器
	TIM_Cmd(TIM3, ENABLE);

	// 暂时关闭TIMx,x的时钟，等待使用
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);

	/* 基本定时器 TIMx,x 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
#elif defined STM32F4
	//arr：自动重装值。
	//psc：时钟预分频数
	//定时器溢出时间计算方法 : Tout = ((arr + 1)*(psc + 1)) / Ft us.
	//Ft=定时器工作频率,单位:Mhz
	u16 arr = interval - 1;
	u16 psc = 8400 - 1;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  ///使能TIM时钟

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);//初始化TIM

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //允许定时器更新中断
	TIM_Cmd(TIM3, ENABLE); //使能定时器

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //定时器中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}
//定时器配置
void DeviceConfigCenter::Timer4Config(int interval)
{
#if defined STM32F0

#elif defined STM32F1
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// 开启TIMx_CLK,x,即内部时钟CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	// 自动重装载寄存器周的值(计数值)
	TIM_TimeBaseStructure.TIM_Period = 1000;

	// 累计 TIM_Period个频率后产生一个更新或者中断
	// 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M
	TIM_TimeBaseStructure.TIM_Prescaler = 71;

	// 初始化定时器TIMx, x
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	// 清除计数器中断标志位
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);

	// 开启计数器中断
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	// 使能计数器
	TIM_Cmd(TIM4, ENABLE);

	// 暂时关闭TIMx,x的时钟，等待使用
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE);

	/* 基本定时器 TIMx,x 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
#elif defined STM32F4
	//arr：自动重装值。
	//psc：时钟预分频数
	//定时器溢出时间计算方法 : Tout = ((arr + 1)*(psc + 1)) / Ft us.
	//Ft=定时器工作频率,单位:Mhz
	u16 arr = interval - 1;
	u16 psc = 8400 - 1;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  ///使能TIM时钟

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);//初始化TIM

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //允许定时器更新中断
	TIM_Cmd(TIM4, ENABLE); //使能定时器

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //定时器中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}
//定时器配置
void DeviceConfigCenter::Timer5Config(int interval)
{
#if defined STM32F0

#elif defined STM32F1
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// 开启TIMx_CLK,x,即内部时钟CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	// 自动重装载寄存器周的值(计数值)
	TIM_TimeBaseStructure.TIM_Period = 1000;

	// 累计 TIM_Period个频率后产生一个更新或者中断
	// 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M
	TIM_TimeBaseStructure.TIM_Prescaler = 71;

	// 初始化定时器TIMx, x
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	// 清除计数器中断标志位
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);

	// 开启计数器中断
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

	// 使能计数器
	TIM_Cmd(TIM5, ENABLE);

	// 暂时关闭TIMx,x的时钟，等待使用
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, DISABLE);

	/* 基本定时器 TIMx,x 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
#elif defined STM32F4
	//arr：自动重装值。
	//psc：时钟预分频数
	//定时器溢出时间计算方法 : Tout = ((arr + 1)*(psc + 1)) / Ft us.
	//Ft=定时器工作频率,单位:Mhz
	u16 arr = interval - 1;
	u16 psc = 8400 - 1;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  ///使能TIM时钟

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);//初始化TIM

	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); //允许定时器更新中断
	TIM_Cmd(TIM5, ENABLE); //使能定时器

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; //定时器中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}
//定时器配置
void DeviceConfigCenter::Timer6Config(int interval)
{
#if defined STM32F0

#elif defined STM32F1
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// 开启TIMx_CLK,x,即内部时钟CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	// 自动重装载寄存器周的值(计数值)
	TIM_TimeBaseStructure.TIM_Period = 1000;

	// 累计 TIM_Period个频率后产生一个更新或者中断
	// 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M
	TIM_TimeBaseStructure.TIM_Prescaler = 71;

	// 初始化定时器TIMx, x
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	// 清除计数器中断标志位
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);

	// 开启计数器中断
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	// 使能计数器
	TIM_Cmd(TIM6, ENABLE);

	// 暂时关闭TIMx,x的时钟，等待使用
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, DISABLE);

	/* 基本定时器 TIMx,x 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
#elif defined STM32F4
	//arr：自动重装值。
	//psc：时钟预分频数
	//定时器溢出时间计算方法 : Tout = ((arr + 1)*(psc + 1)) / Ft us.
	//Ft=定时器工作频率,单位:Mhz
	u16 arr = interval - 1;
	u16 psc = 8400 - 1;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);  ///使能TIM时钟

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);//初始化TIM

	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE); //允许定时器更新中断
	TIM_Cmd(TIM6, ENABLE); //使能定时器

	//NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn; //定时器中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}
//定时器配置
void DeviceConfigCenter::Timer7Config(int interval)
{
#if defined STM32F0

#elif defined STM32F1
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// 开启TIMx_CLK,x,即内部时钟CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	// 自动重装载寄存器周的值(计数值)
	TIM_TimeBaseStructure.TIM_Period = 1000;

	// 累计 TIM_Period个频率后产生一个更新或者中断
	// 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M
	TIM_TimeBaseStructure.TIM_Prescaler = 71;

	// 初始化定时器TIMx, x
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

	// 清除计数器中断标志位
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);

	// 开启计数器中断
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

	// 使能计数器
	TIM_Cmd(TIM7, ENABLE);

	// 暂时关闭TIMx,x的时钟，等待使用
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, DISABLE);

	/* 基本定时器 TIMx,x 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
#elif defined STM32F4
	//arr：自动重装值。
	//psc：时钟预分频数
	//定时器溢出时间计算方法 : Tout = ((arr + 1)*(psc + 1)) / Ft us.
	//Ft=定时器工作频率,单位:Mhz
	u16 arr = interval - 1;
	u16 psc = 8400 - 1;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);  ///使能TIM时钟

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStructure);//初始化TIM

	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE); //允许定时器更新中断
	TIM_Cmd(TIM7, ENABLE); //使能定时器

	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn; //定时器中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}
//定时器配置
void DeviceConfigCenter::Timer8Config(int interval)
{
#if defined STM32F0

#elif defined STM32F1

#elif defined STM32F4
	//arr：自动重装值。
	//psc：时钟预分频数
	//定时器溢出时间计算方法 : Tout = ((arr + 1)*(psc + 1)) / Ft us.
	//Ft=定时器工作频率,单位:Mhz
	u16 arr = interval - 1;
	u16 psc = 8400 - 1;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM8, ENABLE);  ///使能TIM时钟

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStructure);//初始化TIM

	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE); //允许定时器更新中断
	TIM_Cmd(TIM8, ENABLE); //使能定时器

	//NVIC_InitStructure.NVIC_IRQChannel = TIM8_IRQn; //定时器中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}
void DeviceConfigCenter::TimerConfigNvic(TIMER tim, int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
{
	switch (tim)
	{
	case Timer0:
		Timer0ConfigNvic(NVIC_PriorityGroup, NVIC_IRQChannelPreemptionPriority, NVIC_IRQChannelSubPriorit);
		break;
	case Timer1:
		Timer1ConfigNvic(NVIC_PriorityGroup, NVIC_IRQChannelPreemptionPriority, NVIC_IRQChannelSubPriorit);
		break;
	case Timer2:
		Timer2ConfigNvic(NVIC_PriorityGroup, NVIC_IRQChannelPreemptionPriority, NVIC_IRQChannelSubPriorit);
		break;
	case Timer3:
		Timer3ConfigNvic(NVIC_PriorityGroup, NVIC_IRQChannelPreemptionPriority, NVIC_IRQChannelSubPriorit);
		break;
	case Timer4:
		Timer4ConfigNvic(NVIC_PriorityGroup, NVIC_IRQChannelPreemptionPriority, NVIC_IRQChannelSubPriorit);
		break;
	case Timer5:
		Timer5ConfigNvic(NVIC_PriorityGroup, NVIC_IRQChannelPreemptionPriority, NVIC_IRQChannelSubPriorit);
		break;
	case Timer6:
		Timer6ConfigNvic(NVIC_PriorityGroup, NVIC_IRQChannelPreemptionPriority, NVIC_IRQChannelSubPriorit);
		break;
	case Timer7:
		Timer7ConfigNvic(NVIC_PriorityGroup, NVIC_IRQChannelPreemptionPriority, NVIC_IRQChannelSubPriorit);
		break;
	case Timer8:
		Timer8ConfigNvic(NVIC_PriorityGroup, NVIC_IRQChannelPreemptionPriority, NVIC_IRQChannelSubPriorit);
		break;
	case Timer9:
		break;
	case Timer10:
		break;
	case Timer11:
		break;
	case Timer12:
		break;
	case Timer13:
		break;
	case Timer14:
		break;
	case Timer15:
		break;
	case Timer16:
		break;
	case Timer17:
		break;
	case Timer18:
		break;
	default:
		break;
	}
}
void DeviceConfigCenter::Timer0ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
{
#if defined STM32F0

#elif defined STM32F1

#elif defined STM32F4

#endif
}
void DeviceConfigCenter::Timer1ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
{
#if defined STM32F0

#elif defined STM32F1

#elif defined STM32F4

#endif
}
void DeviceConfigCenter::Timer2ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
{
#if defined STM32F0
	NVIC_InitTypeDef   NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#elif defined STM32F1
	NVIC_InitTypeDef NVIC_InitStructure;
	// 设置中断组为0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;

	// 设置主优先级为 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;

	// 设置抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriorit;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#elif defined STM32F4

#endif
}
void DeviceConfigCenter::Timer3ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
{
#if defined STM32F0

#elif defined STM32F1
	NVIC_InitTypeDef NVIC_InitStructure;
	// 设置中断组为0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;

	// 设置主优先级为 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;

	// 设置抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriorit;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#elif defined STM32F4

#endif
}
void DeviceConfigCenter::Timer4ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
{
#if defined STM32F0

#elif defined STM32F1
	NVIC_InitTypeDef NVIC_InitStructure;
	// 设置中断组为0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;

	// 设置主优先级为 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;

	// 设置抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriorit;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#elif defined STM32F4

#endif
}
void DeviceConfigCenter::Timer5ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
{
#if defined STM32F0

#elif defined STM32F1
#ifdef STM32F10X_HD
	NVIC_InitTypeDef NVIC_InitStructure;
	// 设置中断组为0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;

	// 设置主优先级为 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;

	// 设置抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriorit;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
#elif defined STM32F4

#endif
}
void DeviceConfigCenter::Timer6ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
{
#if defined STM32F0

#elif defined STM32F1
#ifdef STM32F10X_HD
	NVIC_InitTypeDef NVIC_InitStructure;
	// 设置中断组为0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;

	// 设置主优先级为 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;

	// 设置抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriorit;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
#elif defined STM32F4

#endif
}
void DeviceConfigCenter::Timer7ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
{
#if defined STM32F0

#elif defined STM32F1
#ifdef STM32F10X_HD
	NVIC_InitTypeDef NVIC_InitStructure;
	// 设置中断组为0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;

	// 设置主优先级为 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;

	// 设置抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriorit;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
#elif defined STM32F4

#endif
}
void DeviceConfigCenter::Timer8ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
{
#if defined STM32F0

#elif defined STM32F1

#elif defined STM32F4

#endif
}

//系统
void OS_ComSendChk(void *param)
{
	//检查串口是否可以发送
#if defined COM1TXDMAFLAG
	if (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != RESET)
	{
		int len = Txx1.Length();
		if (len == 0)
			return;
		for (int i = 0; i < len; i++)
		{
			com1bufff[i] = Txx1.Dequeue();
		}
		Txx1.Clear();
		USART1_SendDMA((uint8_t*)com1bufff, len);
	}
#endif

#if defined COM2TXDMAFLAG
	if (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != RESET)
	{
		int len = Txx2.Length();
		if (len == 0)
			return;
		for (int i = 0; i < len; i++)
		{
			com2bufff[i] = Txx2.Dequeue();
		}
		Txx2.Clear();
		USART2_SendDMA((uint8_t*)com2bufff, len);
	}
#endif

#if defined COM3TXDMAFLAG
	if (USART_GetFlagStatus(USART3, USART_FLAG_TXE) != RESET)
	{
		int len = Txx3.Length();
		if (len == 0)
			return;
		for (int i = 0; i < len; i++)
		{
			com3bufff[i] = Txx3.Dequeue();
		}
		Txx3.Clear();
		USART3_SendDMA((uint8_t*)com3bufff, len);
	}
#endif

#if defined COM4TXDMAFLAG
	if (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != RESET)
	{
		int len = Txx4.Length();
		if (len == 0)
			return;
		for (int i = 0; i < len; i++)
		{
			com4bufff[i] = Txx4.Dequeue();
		}
		Txx4.Clear();
		//UART4_SendDMA((uint8_t*)com4bufff, len);
	}
#endif

#if defined COM5TXDMAFLAG
	if (USART_GetFlagStatus(UART5, USART_FLAG_TXE) != RESET)
	{
		int len = Txx5.Length();
		if (len == 0)
			return;
		for (int i = 0; i < len; i++)
		{
			com5bufff[i] = Txx5.Dequeue();
		}
		Txx5.Clear();
		//UART5_SendDMA((uint8_t*)com5bufff, len);
	}
#endif
}
//串口接收判断
void OS_ComRcvChk(void *param)
{
	int ms = Sys.Ms();
#if (defined USECOM1RXD)&&(! defined COM1RCVIDLEINTFLAG)
	if ((ms - DeviceConfigCenter::RcvLastTimeCOM1 > DeviceConfigCenter::RcvCom1PackInterval) && (Rxx1.Length() > 0))
	{
		if (DeviceConfigCenter::PRcvCOM1)
		{
			(*DeviceConfigCenter::PRcvCOM1)();
		}
	}
#endif
#if (defined USECOM2RXD)&&(! defined COM2RCVIDLEINTFLAG)
	if ((ms - DeviceConfigCenter::RcvLastTimeCOM2 > DeviceConfigCenter::RcvCom2PackInterval) && (Rxx2.Length() > 0))
	{
		if (DeviceConfigCenter::PRcvCOM2)
		{
			(*DeviceConfigCenter::PRcvCOM2)();
		}
	}
#endif
#if (defined USECOM3RXD)&&(! defined COM3RCVIDLEINTFLAG)
	if ((ms - DeviceConfigCenter::RcvLastTimeCOM3 > DeviceConfigCenter::RcvCom3PackInterval) && (Rxx3.Length() > 0))
	{
		if (DeviceConfigCenter::PRcvCOM3)
		{
			(*DeviceConfigCenter::PRcvCOM3)();
		}
	}
#endif
#if (defined USECOM4RXD)&&(! defined COM4RCVIDLEINTFLAG)
	if ((ms - DeviceConfigCenter::RcvLastTimeCOM4 > DeviceConfigCenter::RcvCom4PackInterval) && (Rxx4.Length() > 0))
	{
		if (DeviceConfigCenter::PRcvCOM4)
		{
			(*DeviceConfigCenter::PRcvCOM4)();
		}
	}
#endif
#if (defined USECOM5RXD)&&(! defined COM5RCVIDLEINTFLAG)
	if ((ms - DeviceConfigCenter::RcvLastTimeCOM5 > DeviceConfigCenter::RcvCom5PackInterval) && (Rxx5.Length() > 0))
	{
		if (DeviceConfigCenter::PRcvCOM5)
		{
			(*DeviceConfigCenter::PRcvCOM5)();
		}
	}
#endif
}

