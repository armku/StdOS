#include "stm32f10x.h"
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
#if 0
	void USART1_IRQHandler(void)    //----USART1 IRQ----//
	{
//#ifdef USE_GPS
//		if (pGPS1) 
//		{
//			pGPS1->GPS_IRQHandler();
//		}
//#endif
#ifdef USE_USART1
		pCOM1->IRQ();
#endif
	}
#endif
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
