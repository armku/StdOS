#include "DeviceConfigHelper.h"
#include "Port.h"
#include "Platform\stm32.h"

//中断
//中断线打开、关闭
void DeviceConfigHelper::SetEXIT(int pinIndex, bool enable, Trigger trigger)
{
#if defined STM32F0
	/* 配置EXTI中断线 */
	EXTI_InitTypeDef ext;
	EXTI_StructInit(&ext);
	ext.EXTI_Line = EXTI_Line0 << (pinIndex & 0X0F);
	ext.EXTI_Mode = EXTI_Mode_Interrupt;
	switch (trigger)
	{
	case InputPort::Rising:
		ext.EXTI_Trigger = EXTI_Trigger_Rising; // 上升沿触发
		break;
	case InputPort::Falling:
		ext.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
		break;
	case InputPort::Both:
	default:
		ext.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // 上升沿下降沿触发
		break;
	}
	ext.EXTI_LineCmd = enable ? ENABLE : DISABLE;
	EXTI_Init(&ext);
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
	/* 配置EXTI中断线 */
	EXTI_InitTypeDef ext;
	EXTI_StructInit(&ext);
	ext.EXTI_Line = EXTI_Line0 << (pinIndex & 0X0F);
	ext.EXTI_Mode = EXTI_Mode_Interrupt;
	switch (trigger)
	{
	case InputPort::Rising:
		ext.EXTI_Trigger = EXTI_Trigger_Rising; // 上升沿触发
		break;
	case InputPort::Falling:
		ext.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
		break;
	case InputPort::Both:
	default:
		ext.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // 上升沿下降沿触发
		break;
	}
	ext.EXTI_LineCmd = enable ? ENABLE : DISABLE;
	EXTI_Init(&ext);
#endif
}
#if defined STM32F0

#elif defined STM32F1

static const int PORT_IRQns[] =
{
#if defined STM32F0
	EXTI0_1_IRQn, EXTI0_1_IRQn,  // 基础
	EXTI2_3_IRQn, EXTI2_3_IRQn,  // 基础
	EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn,
	EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn  // EXTI15_10
#elif defined STM32F1
	EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn,  // 5个基础的
	EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,  // EXTI9_5
	EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn  // EXTI15_10
#elif defined STM32F4
	EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn,  // 5个基础的
	EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,  // EXTI9_5
	EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn  // EXTI15_10
#endif
};
#elif defined STM32F4

#endif


void DeviceConfigHelper::InputPort_OpenEXTI(Pin pin, Trigger trigger)
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

//串口
OutputPort *DeviceConfigHelper::pCOM1Rx485 = 0;
OutputPort *DeviceConfigHelper::pCOM2Rx485 = 0;
OutputPort *DeviceConfigHelper::pCOM3Rx485 = 0;
OutputPort *DeviceConfigHelper::pCOM4Rx485 = 0;
OutputPort *DeviceConfigHelper::pCOM5Rx485 = 0;

void_func_point *PRcvCOM1 = 0;

#ifdef __cplusplus
extern "C" {
#endif
#if USECOM1
	static char com1rx[1024], com1tx[1024];
	Queue	Txx1;
	Queue	Rxx1;
#endif
#if USECOM2
	static char com2rx[1024], com2tx[1024];
	Queue	Txx2;
	Queue	Rxx2;
#endif
#if USECOM3
	static char com3rx[1024], com3tx[1024];
	Queue	Txx3;
	Queue	Rxx3;
#endif
#if USECOM4
	static char com4rx[1024], com4tx[1024];
	Queue	Txx4;
	Queue	Rxx4;
#endif
#if USECOM5
	static char com5rx[1024], com5tx[1024];
	Queue	Txx5;
	Queue	Rxx5;
#endif

#ifdef __cplusplus
}
#endif
void DeviceConfigHelper::ConfigCom(COM com, int baudRate)
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
void DeviceConfigHelper::ComChgBaudRate(COM com, int baudRate)
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
void DeviceConfigHelper::comSend(COM com, Buffer& bs)
{
	switch (com)
	{
	case COM1:
		com1send(bs);
		break;
	case COM2:
		com2send(bs);
		break;
	case COM3:
		com3send(bs);
		break;
	case COM4:
		com4send(bs);
		break;
	case COM5:
		com5send(bs);
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
void DeviceConfigHelper::com1send(Buffer& bs)
{
#if USECOM1
	Txx1.Write(bs);
	com1send();
#endif
}
void DeviceConfigHelper::com2send(Buffer& bs)
{
#if USECOM2
	Txx2.Write(bs);
	com2send();
#endif
}
void DeviceConfigHelper::com3send(Buffer& bs)
{
#if USECOM3
	Txx3.Write(bs);
	com3send();
#endif
}
void DeviceConfigHelper::com4send(Buffer& bs)
{
#if USECOM4
	Txx4.Write(bs);
	com4send();
#endif
}
void DeviceConfigHelper::com5send(Buffer& bs)
{
#if USECOM5
	Txx5.Write(bs);
	com5send();
#endif
}
void DeviceConfigHelper::com1send()
{
#if defined STM32F0

#elif defined STM32F1
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
#elif defined STM32F4

#endif
}
void DeviceConfigHelper::com2send()
{
#if defined STM32F0

#elif defined STM32F1
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	if (pCOM2Rx485)
		*pCOM2Rx485 = 1;
#elif defined STM32F4

#endif
}
void DeviceConfigHelper::com3send()
{
#if defined STM32F0

#elif defined STM32F1
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
#elif defined STM32F4

#endif
}
void DeviceConfigHelper::com4send()
{
#if defined STM32F0

#elif defined STM32F1
	USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
#elif defined STM32F4

#endif
}
void DeviceConfigHelper::com5send()
{
#if defined STM32F0

#elif defined STM32F1
	USART_ITConfig(UART5, USART_IT_TXE, ENABLE);
#elif defined STM32F4

#endif
}

void DeviceConfigHelper::configCOM1(int baudRate)
{
#if USECOM1
#if defined STM32F0

#elif defined STM32F1

	Port*		Ports[2];	// Tx/Rx
	Pin			Pins[2];	// Tx/Rx

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
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 


	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);

	Txx1.SetBuf(com1tx, ArrayLength(com1tx));
	Rxx1.SetBuf(com1rx, ArrayLength(com1rx));

	SerialPort_GetPins(&Pins[0], &Pins[1], COM1);
	Ports[0] = new AlternatePort();
	Ports[1] = new InputPort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
#elif defined STM32F4

#endif
#endif
}
void DeviceConfigHelper::configCOM2(int baudRate)
{
#if USECOM2
#if defined STM32F0

#elif defined STM32F1

	Port*		Ports[2];	// Tx/Rx
	Pin			Pins[2];	// Tx/Rx

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
	/* 使能串口2接收中断 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 串口接收中断配置
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 


	USART_Cmd(USART2, ENABLE);
	USART_ClearFlag(USART2, USART_FLAG_TC);

	Txx2.SetBuf(com2tx, ArrayLength(com2tx));
	Rxx2.SetBuf(com2rx, ArrayLength(com2rx));

	SerialPort_GetPins(&Pins[0], &Pins[1], COM2);
	Ports[0] = new AlternatePort();
	Ports[1] = new InputPort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
#elif defined STM32F4

#endif
#endif
}

void DeviceConfigHelper::configCOM3(int baudRate)
{
#if USECOM3
#if defined STM32F0

#elif defined STM32F1

	Port*		Ports[2];	// Tx/Rx
	Pin			Pins[2];	// Tx/Rx

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
	/* 使能串口3接收中断 */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // 串口接收中断配置
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 


	USART_Cmd(USART3, ENABLE);
	USART_ClearFlag(USART3, USART_FLAG_TC);

	Txx3.SetBuf(com3tx, ArrayLength(com3tx));
	Rxx3.SetBuf(com3rx, ArrayLength(com3rx));

	SerialPort_GetPins(&Pins[0], &Pins[1], COM3);
	Ports[0] = new AlternatePort();
	Ports[1] = new InputPort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
#elif defined STM32F4

#endif
#endif
}
void DeviceConfigHelper::configCOM4(int baudRate)
{
#if USECOM4
#if defined STM32F0

#elif defined STM32F1

	Port*		Ports[2];	// Tx/Rx
	Pin			Pins[2];	// Tx/Rx

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
	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 


	USART_Cmd(UART4, ENABLE);
	USART_ClearFlag(UART4, USART_FLAG_TC);

	Txx4.SetBuf(com4tx, ArrayLength(com4tx));
	Rxx4.SetBuf(com4rx, ArrayLength(com4rx));

	SerialPort_GetPins(&Pins[0], &Pins[1], COM4);
	Ports[0] = new AlternatePort();
	Ports[1] = new InputPort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
#elif defined STM32F4

#endif
#endif
}
void DeviceConfigHelper::configCOM5(int baudRate)
{
#if USECOM5
#if defined STM32F0

#elif defined STM32F1

	Port*		Ports[2];	// Tx/Rx
	Pin			Pins[2];	// Tx/Rx

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
	USART_ITConfig(UART5, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 


	USART_Cmd(UART5, ENABLE);
	USART_ClearFlag(UART5, USART_FLAG_TC);

	Txx5.SetBuf(com5tx, ArrayLength(com5tx));
	Rxx5.SetBuf(com5rx, ArrayLength(com5rx));

	SerialPort_GetPins(&Pins[0], &Pins[1], COM5);
	Ports[0] = new AlternatePort();
	Ports[1] = new InputPort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
#elif defined STM32F4

#endif
#endif
}
void DeviceConfigHelper::Com1ChgBaudRate(int baudRate)
{
#if USECOM1
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

#endif
#endif
}
void DeviceConfigHelper::Com2ChgBaudRate(int baudRate)
{
#if USECOM2
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

#endif
#endif
}
void DeviceConfigHelper::Com3ChgBaudRate(int baudRate)
{
#if USECOM3
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

#endif
#endif
}
void DeviceConfigHelper::Com4ChgBaudRate(int baudRate)
{
#if USECOM4
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

#endif
#endif
}
void DeviceConfigHelper::Com5ChgBaudRate(int baudRate)
{
#if USECOM5
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

#endif
#endif
}
void DeviceConfigHelper::SerialPort_GetPins(Pin *txPin, Pin *rxPin, COM index, bool Remap)
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
//定时器
void DeviceConfigHelper::TimeTickInit()//系统用定时器初始化
{
#if defined STM32F0

#elif defined STM32F1

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

	NVIC_SetPriority(SysTick_IRQn, 0);
	NVIC_SetPriority(TIM2_IRQn, 0);
#elif defined STM32F4

#endif
}

//定时器配置
void DeviceConfigHelper::TimerConfig(TIMER tim, int interval, int NVIC_PriorityGroup,int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
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
void DeviceConfigHelper::Timer0Config(int interval)
{
#if defined STM32F0

#elif defined STM32F1

#elif defined STM32F4

#endif
}
//定时器配置
void DeviceConfigHelper::Timer1Config(int interval)
{
#if defined STM32F0

#elif defined STM32F1

#elif defined STM32F4

#endif
}
//定时器配置
void DeviceConfigHelper::Timer2Config(int interval)
{
#if defined STM32F0

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

#endif
}
//定时器配置
void DeviceConfigHelper::Timer3Config(int interval)
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

#endif
}
//定时器配置
void DeviceConfigHelper::Timer4Config(int interval)
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

#endif
}
//定时器配置
void DeviceConfigHelper::Timer5Config(int interval)
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

#endif
}
//定时器配置
void DeviceConfigHelper::Timer6Config(int interval)
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

#endif
}
//定时器配置
void DeviceConfigHelper::Timer7Config(int interval)
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

#endif
}
//定时器配置
void DeviceConfigHelper::Timer8Config(int interval)
{
#if defined STM32F0

#elif defined STM32F1

#elif defined STM32F4

#endif
}
void DeviceConfigHelper::TimerConfigNvic(TIMER tim, int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority , int NVIC_IRQChannelSubPriorit )
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
void DeviceConfigHelper::Timer0ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
{
#if defined STM32F0

#elif defined STM32F1

#elif defined STM32F4

#endif
}
void DeviceConfigHelper::Timer1ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
{
#if defined STM32F0

#elif defined STM32F1
	
#elif defined STM32F4

#endif
}
void DeviceConfigHelper::Timer2ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
{
#if defined STM32F0

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
void DeviceConfigHelper::Timer3ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
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
void DeviceConfigHelper::Timer4ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
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
void DeviceConfigHelper::Timer5ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
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
void DeviceConfigHelper::Timer6ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
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
void DeviceConfigHelper::Timer7ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
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
void DeviceConfigHelper::Timer8ConfigNvic(int NVIC_PriorityGroup, int NVIC_IRQChannelPreemptionPriority, int NVIC_IRQChannelSubPriorit)
{
#if defined STM32F0

#elif defined STM32F1

#elif defined STM32F4

#endif
}

//系统
int DeviceConfigHelper::CurrentTick()
{
#if defined STM32F0

#elif defined STM32F1
	return (TIM2->CNT) >> 1;
#elif defined STM32F4

#endif
}
uint32_t DeviceConfigHelper::CurrentTicks1()
{
#if defined STM32F0

#elif defined STM32F1
	return SysTick->LOAD - SysTick->VAL;
#elif defined STM32F4

#endif
}

//其他
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

