#include "BspPlatform.h"
#include "stm32f10x.h"
#include "Sys.h"
#include "Core\Queue.h"
#include "Port.h"
#include "Platform\stm32.h"

static char com11rx[1024], com11tx[1024];
Queue	Txx1;
Queue	Rxx1;

#ifdef STM32F0


extern "C"
{
	
}
#elif defined STM32F1
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
//调试串口初始化
void SerialPrintInit()
{
	USART_InitTypeDef USART_InitStructure;

	/* config USART1 clock */
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 256000;
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
	/* 使能串口2接收中断 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 串口接收中断配置
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 


	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);

	Txx1.SetBuf(com11tx, ArrayLength(com11tx));
	Rxx1.SetBuf(com11rx, ArrayLength(com11rx));

	SerialPort_GetPins(&Pins[0], &Pins[1], COM1);
	Ports[0] = new AlternatePort();
	Ports[1] = new InputPort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
}
#if 0
static char *itoa(int value, char *string, int radix)
{
	int i, d;
	int flag = 0;
	char *ptr = string;

	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}

	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}

	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';

		/* Make the value positive. */
		value *= -1;

	}

	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;

		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d *i);
			flag = 1;
		}
	}

	/* Null terminate the string. */
	*ptr = 0;

	return string;
}
#include <stdarg.h>
//模拟printf
void Printf(char *Data, ...)
{
	const char *s;
	int d;
	char buf[16];

	char bufSend[200];
	int bufSendPos;
	//int bufSendMax = ArrayLength(bufSend);

	bufSendPos = 0;

	va_list ap;
	va_start(ap, Data);

	while (*Data != 0)
		// 判断是否到达字符串结束符
	{
		if (*Data == 0x5c)
			//'\'
		{
			switch (*++Data)
			{
			case 'r':
				//回车符
				bufSend[bufSendPos++] = 0X0D;
				Data++;
				break;

			case 'n':
				//换行符
				bufSend[bufSendPos++] = 0X0A;
				Data++;
				break;

			default:
				Data++;
				break;
			}
		}

		else if (*Data == '%')
		{
			//
			switch (*++Data)
			{
			case 's':
				//字符串
				s = va_arg(ap, const char*);
				for (; *s; s++)
				{
					bufSend[bufSendPos++] = *s;
				}
				Data++;
				break;
			case 'd':
				//十进制
				d = va_arg(ap, int);
				itoa(d, buf, 10);
				for (s = buf; *s; s++)
				{
					bufSend[bufSendPos++] = *s;
				}
				Data++;
				break;
			default:
				Data++;
				break;
			}
		}
		else
			bufSend[bufSendPos++] = *Data++;
	}
	Buffer bs(bufSend, bufSendPos);
	//Write(bs);
}
#endif
void com3send()
{
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

void TimeTickInit()//系统用定时器初始化
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	/* 自动重装载寄存器周期的值(计数值) */
	TIM_TimeBaseStructure.TIM_Period = 2000 - 1;
	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
	/* 时钟预分频数为72 */
	TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1;

	/* 对外部时钟进行采样的时钟分频,这里没有用到 */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_DeInit(TIM6);
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_ClearFlag(TIM6, TIM_FLAG_Update); // 清除标志位  必须要有！！ 否则 开启中断立马中断给你看
	TIM_Cmd(TIM6, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, DISABLE); /*先关闭等待使用*/
	NVIC_InitTypeDef nvic;

	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannel = TIM6_IRQn;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;

	NVIC_Init(&nvic);
	NVIC_SetPriority(TIM6_IRQn, 3);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	NVIC_SetPriority(SysTick_IRQn, 0);
	NVIC_SetPriority(TIM6_IRQn, 0);
}
int CurrentTick()
{
	return (TIM6->CNT) >> 1;
}
uint32_t CurrentTicks1()
{
	return SysTick->LOAD - SysTick->VAL;
}

//中断线打开、关闭
void SetEXIT(int pinIndex, bool enable, Trigger trigger = Both)
{
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
}
static const int PORT_IRQns[] =
{
	EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn,  // 5个基础的
	EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,  // EXTI9_5
	EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn  // EXTI15_10
};
void InputPort_OpenEXTI(Pin pin, Trigger trigger = Both)
{
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
}
/// TIMx,x[6,7]中断优先级配置
void TIMx_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	// 设置中断组为0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;

	// 设置主优先级为 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

	// 设置抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIMx_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// 开启TIMx_CLK,x[6,7],即内部时钟CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	// 自动重装载寄存器周的值(计数值)
	TIM_TimeBaseStructure.TIM_Period = 1000;

	// 累计 TIM_Period个频率后产生一个更新或者中断
	// 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M
	TIM_TimeBaseStructure.TIM_Prescaler = 71;

	// 初始化定时器TIMx, x[6,7]
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

	// 清除计数器中断标志位
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);

	// 开启计数器中断
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

	// 使能计数器
	TIM_Cmd(TIM7, ENABLE);

	// 暂时关闭TIMx,x[6,7]的时钟，等待使用
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, DISABLE);
}
void BspPlatformInit()
{
	/* 基本定时器 TIMx,x[6,7] 定时配置 */
	TIMx_Configuration();

	/* 配置基本定时器 TIMx,x[6,7]的中断优先级 */
	TIMx_NVIC_Configuration();

	/* 基本定时器 TIMx,x[6,7] 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	TimeTickInit();//系统用定时器初始化
}
void TimeUpdate();
extern "C"
{
	void SysTick_Handler(void)
	{

	}

	void USART1_IRQHandler(void)
	{
volatile uint8_t ch;
		if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
		{
			ch = USART_ReceiveData(USART1);
			Rxx1.Enqueue(ch);
		}
		if (USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
			//数据帧接收完毕
		{
			ch = USART_ReceiveData(USART1); //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)    
			//sp->OnRxHandler();			
		}
		/* 处理发送缓冲区空中断 */
		if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
		{
			if (Txx1.Empty())
			{
				/* 发送缓冲区的数据已取完时， 禁止发送缓冲区空中断 （注意：此时最后1个数据还未真正发送完毕）*/
				USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
				/* 使能数据发送完毕中断 */
				USART_ITConfig(USART1, USART_IT_TC, ENABLE);
			}
			else
			{
				/* 从发送FIFO取1个字节写入串口发送数据寄存器 */
				USART_SendData(USART1, Txx1.Dequeue());
			}

		}
		/* 数据bit位全部发送完毕的中断 */
		else if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)
		{
			if (Txx1.Empty())
			{
				/* 如果发送FIFO的数据全部发送完毕，禁止数据发送完毕中断 */
				USART_ITConfig(USART1, USART_IT_TC, DISABLE);

				/* 回调函数, 一般用来处理RS485通信，将RS485芯片设置为接收模式，避免抢占总线 */
				Txx1.Clear();
				//				if(sp->RS485)
				//				{
				//					*sp->RS485=0;
				//				}
			}
			else
			{
				/* 正常情况下，不会进入此分支 */
				/* 如果发送FIFO的数据还未完毕，则从发送FIFO取1个数据写入发送数据寄存器 */
				USART_SendData(USART1, Txx1.Dequeue());
			}
		}
	}
	void USART2_IRQHandler(void)
	{

	}
	void USART3_IRQHandler(void)
	{
		
	}
	void UART4_IRQHandler(void)
	{
#ifndef STM32F10X_MD

#endif
	}
	void UART5_IRQHandler(void)
	{
#ifndef STM32F10X_MD

#endif
	}

	void TIM2_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		{

			TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
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
#ifndef STM32F10X_MD

#endif
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
		}
	}

	void TIM6_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
		{
#ifndef STM32F10X_MD

#endif
			TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
			TimeUpdate();
		}
	}

	void TIM7_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
		{
#ifndef STM32F10X_MD

#endif
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
