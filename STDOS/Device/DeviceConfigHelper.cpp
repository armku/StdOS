#include "DeviceConfigHelper.h"
#include "Port.h"
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
int DeviceConfigCenter::RcvLastTimeCOM2 = 0;//串口1最后接收数据时间
int DeviceConfigCenter::RcvLastTimeCOM3 = 0;//串口1最后接收数据时间
int DeviceConfigCenter::RcvLastTimeCOM4 = 0;//串口1最后接收数据时间
int DeviceConfigCenter::RcvLastTimeCOM5 = 0;//串口1最后接收数据时间

#ifdef __cplusplus
extern "C" {
#endif
#if USECOM1
	static char com1rx[256], com1tx[256];
	Queue	Txx1;
	Queue	Rxx1;
#if COM1TXDMAFLAG
#define            SENDBUFF_SIZE                             5000
	uint8_t SendBuff[5000];
#endif
#endif
#if USECOM2
	static char com2rx[256], com2tx[256];
	Queue	Txx2;
	Queue	Rxx2;
#endif
#if USECOM3
	static char com3rx[256], com3tx[256];
	Queue	Txx3;
	Queue	Rxx3;
#endif
#if USECOM4
	static char com4rx[256], com4tx[256];
	Queue	Txx4;
	Queue	Rxx4;
#endif
#if USECOM5
	static char com5rx[256], com5tx[256];
	Queue	Txx5;
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
void DeviceConfigCenter::comSend(COM com, Buffer bs)
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
void DeviceConfigCenter::com1send(Buffer& bs)
{
#if USECOM1
#if COM1TXDMAFLAG
	for (int i = 0; i < bs.Length(); i++)
	{
		SendBuff[i] = bs[i];
	}
	//DMA发送
	DMA_InitTypeDef DMA_InitStructure;
		
	/*设置DMA源：串口数据寄存器地址*/
	//		DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;	  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(USART1->DR));

	/*内存地址(要传输的变量的指针)*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff;

	/*方向：从内存到外设*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;

	/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/
	DMA_InitStructure.DMA_BufferSize = bs.Length();

	/*外设地址不增*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	/*内存地址自增*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

	/*外设数据单位*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

	/*内存数据单位 8bit*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

	/*DMA模式：不断循环*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

	/*优先级：中*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;

	/*禁止内存到内存的传输	*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	/*关闭DMA，否则参数配置无效*/
	DMA_Cmd(DMA1_Channel4, DISABLE);
	/*配置DMA1的4通道*/
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	/*使能DMA*/
	DMA_Cmd(DMA1_Channel4, ENABLE);

	//DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  //配置DMA发送完成后产生中断
	
	/* USART1 向 DMA发出TX请求 */
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
#elif COM1SENDINTFLAG
	while (bs.Length() > Txx1.RemainLength());//等待发送缓冲区可容纳足够内容
	//中断发送
	Sys.GlobalDisable();
	Txx1.Write(bs);
	Sys.GlobalEnable();
	com1send();
#else
	if (pCOM1Rx485)
	{
		*pCOM1Rx485 = 1;
	}
	for (int i = 0; i < bs.Length(); i++)
	{
		/* 发送一个字节数据到USART */
		USART_SendData(USART1, bs[i]);

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
void DeviceConfigCenter::com2send(Buffer& bs)
{
#if USECOM2
#if COM2SENDINTFLAG
	while (bs.Length() > Txx2.RemainLength());//等待发送缓冲区可容纳足够内容
	//中断发送
	Sys.GlobalDisable();
	Txx2.Write(bs);
	Sys.GlobalEnable();
	com2send();
#else
	if (pCOM2Rx485)
	{
		*pCOM2Rx485 = 1;
	}
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
void DeviceConfigCenter::com3send(Buffer& bs)
{
#if USECOM3
#if COM3SENDINTFLAG
	while (bs.Length() > Txx3.RemainLength());//等待发送缓冲区可容纳足够内容
	//中断发送
	Sys.GlobalDisable();
	Txx3.Write(bs);
	Sys.GlobalEnable();
	com3send();
#else
	if (pCOM3Rx485)
	{
		*pCOM3Rx485 = 1;
	}
	for (int i = 0; i < bs.Length(); i++)
	{
		/* 发送一个字节数据到USART */
		USART_SendData(USART3, bs[i]);

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
void DeviceConfigCenter::com4send(Buffer& bs)
{
#if USECOM4
#if COM4SENDINTFLAG
	while (bs.Length() > Txx4.RemainLength());//等待发送缓冲区可容纳足够内容
	//中断发送
	Sys.GlobalDisable();
	Txx4.Write(bs);
	Sys.GlobalEnable();
	com4send();
#else
	if (pCOM4Rx485)
	{
		*pCOM4Rx485 = 1;
	}
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
void DeviceConfigCenter::com5send(Buffer& bs)
{
#if USECOM5
#if COM5SENDINTFLAG
	while (bs.Length() > Txx5.RemainLength());//等待发送缓冲区可容纳足够内容
	//中断发送
	Sys.GlobalDisable();
	Txx5.Write(bs);
	Sys.GlobalEnable();
	com5send();
#else
	if (pCOM5Rx485)
	{
		*pCOM5Rx485 = 1;
	}
	for (int i = 0; i < bs.Length(); i++)
	{
		/* 发送一个字节数据到USART */
		USART_SendData(UART5, bs[i]);

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
void DeviceConfigCenter::com1send()
{
#if defined STM32F0

#elif defined STM32F1
#if COM1TXDMAFLAG
	/* USART1 向 DMA发出TX请求 */
	//USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
#elif COM1SENDINTFLAG
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
#endif
#elif defined STM32F4

#endif
}
void DeviceConfigCenter::com2send()
{
#if defined STM32F0

#elif defined STM32F1
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	if (pCOM2Rx485)
		*pCOM2Rx485 = 1;
#elif defined STM32F4

#endif
}
void DeviceConfigCenter::com3send()
{
#if defined STM32F0

#elif defined STM32F1
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
#elif defined STM32F4

#endif
}
void DeviceConfigCenter::com4send()
{
#if defined STM32F0

#elif defined STM32F1
	USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
#elif defined STM32F4

#endif
}
void DeviceConfigCenter::com5send()
{
#if defined STM32F0

#elif defined STM32F1
	USART_ITConfig(UART5, USART_IT_TXE, ENABLE);
#elif defined STM32F4

#endif
}

void DeviceConfigCenter::configCOM1(int baudRate)
{	
#if USECOM1
	Port*		Ports[2];	// Tx/Rx
	Pin			Pins[2];	// Tx/Rx
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

#if COM1TXDMAFLAG
	//DMA发送
	DMA_InitTypeDef DMA_InitStructure;

	/*开启DMA时钟*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/*设置DMA源：串口数据寄存器地址*/
	//		DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;	  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(USART1->DR));

	/*内存地址(要传输的变量的指针)*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff;

	/*方向：从内存到外设*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;

	/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/
	DMA_InitStructure.DMA_BufferSize = 20;

	/*外设地址不增*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	/*内存地址自增*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

	/*外设数据单位*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

	/*内存数据单位 8bit*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

	/*DMA模式：不断循环*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

	/*优先级：中*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;

	/*禁止内存到内存的传输	*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	/*配置DMA1的4通道*/
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	/*使能DMA*/
	DMA_Cmd(DMA1_Channel4, ENABLE);
	//DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  //配置DMA发送完成后产生中断
#endif // COM1TXDMAFLAG

#if COM1RCVIDLEINTFLAG
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 
#endif

	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);

	SerialPort_GetPins(&Pins[0], &Pins[1], COM1);
	Ports[0] = new AlternatePort();
	Ports[1] = new InputPort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
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
#if COM1RCVIDLEINTFLAG
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

	Txx1.SetBuf(com1tx, ArrayLength(com1tx));
	Rxx1.SetBuf(com1rx, ArrayLength(com1rx));

#if  COM1RCVIDLEINTFLAG
#else
	Sys.AddTask(Com1RcvRoutin, 0, 100, 1, "RcvCom1");
#endif //  COM1RCVIDLEINTFLAG
#endif
}
void DeviceConfigCenter::configCOM2(int baudRate)
{
#if USECOM2
	Port*		Ports[2];	// Tx/Rx
	Pin			Pins[2];	// Tx/Rx
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
	/* 使能串口2接收中断 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 串口接收中断配置
#if COM2RCVIDLEINTFLAG
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 
#endif

	USART_Cmd(USART2, ENABLE);
	USART_ClearFlag(USART2, USART_FLAG_TC);

	SerialPort_GetPins(&Pins[0], &Pins[1], COM2);
	Ports[0] = new AlternatePort();
	Ports[1] = new InputPort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
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
#if COM1RCVIDLEINTFLAG
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

	Txx2.SetBuf(com2tx, ArrayLength(com2tx));
	Rxx2.SetBuf(com2rx, ArrayLength(com2rx));
		
#if  COM2RCVIDLEINTFLAG
#else
	Sys.AddTask(Com2RcvRoutin, 0, 100, 1, "RcvCom2");
#endif //  COM2RCVIDLEINTFLAG
#endif
}

void DeviceConfigCenter::configCOM3(int baudRate)
{
#if USECOM3
	Port*		Ports[2];	// Tx/Rx
	Pin			Pins[2];	// Tx/Rx
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
	/* 使能串口3接收中断 */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // 串口接收中断配置
#if COM3RCVIDLEINTFLAG
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 
#endif
	
	USART_Cmd(USART3, ENABLE);
	USART_ClearFlag(USART3, USART_FLAG_TC);

	SerialPort_GetPins(&Pins[0], &Pins[1], COM3);
	Ports[0] = new AlternatePort();
	Ports[1] = new InputPort();
	Ports[0]->Set(Pins[0]);
	Ports[1]->Set(Pins[1]);
	Ports[0]->Open();
	Ports[1]->Open();
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
#if COM1RCVIDLEINTFLAG
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

	Txx3.SetBuf(com3tx, ArrayLength(com3tx));
	Rxx3.SetBuf(com3rx, ArrayLength(com3rx));

	

#if  COM3RCVIDLEINTFLAG
#else
	Sys.AddTask(Com3RcvRoutin, 0, 100, 1, "RcvCom3");
#endif //  COM3RCVIDLEINTFLAG
#endif
}
void DeviceConfigCenter::configCOM4(int baudRate)
{
#if USECOM4
	Port*		Ports[2];	// Tx/Rx
	Pin			Pins[2];	// Tx/Rx
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

	Txx4.SetBuf(com4tx, ArrayLength(com4tx));
	Rxx4.SetBuf(com4rx, ArrayLength(com4rx));
		
#if  COM4RCVIDLEINTFLAG
#else
	Sys.AddTask(Com4RcvRoutin, 0, 100, 1, "RcvCom4");
#endif //  COM4RCVIDLEINTFLAG
#endif
}
void DeviceConfigCenter::configCOM5(int baudRate)
{
#if USECOM5
	Port*		Ports[2];	// Tx/Rx
	Pin			Pins[2];	// Tx/Rx
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

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC| RCC_AHB1Periph_GPIOD, ENABLE); //使能GPIO时钟
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

	Txx5.SetBuf(com5tx, ArrayLength(com5tx));
	Rxx5.SetBuf(com5rx, ArrayLength(com5rx));
		
#if  COM5RCVIDLEINTFLAG
#else
	Sys.AddTask(Com5RcvRoutin, 0, 100, 1, "RcvCom5");
#endif //  COM5RCVIDLEINTFLAG
#endif
}
void DeviceConfigCenter::Com1ChgBaudRate(int baudRate)
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
//串口1接收判断
void DeviceConfigCenter::Com1RcvRoutin(void *param)
{
#if USECOM1
	int ms = Sys.Ms();
	if ((ms - RcvLastTimeCOM1 > 1) && (Rxx1.Length() > 0))
	{
		if (DeviceConfigCenter::PRcvCOM1)
		{
			(*DeviceConfigCenter::PRcvCOM1)();
		}
	}
#endif
}
//串口2接收判断
void DeviceConfigCenter::Com2RcvRoutin(void *param)
{
#if USECOM2
	int ms = Sys.Ms();
	if ((ms - RcvLastTimeCOM2 > 1) && (Rxx2.Length() > 0))
	{
		if (DeviceConfigCenter::PRcvCOM2)
		{
			(*DeviceConfigCenter::PRcvCOM2)();
		}
	}
#endif
}
//串口3接收判断
void DeviceConfigCenter::Com3RcvRoutin(void *param)
{
#if USECOM3
	int ms = Sys.Ms();
	if ((ms - RcvLastTimeCOM3 > 1) && (Rxx3.Length() > 0))
	{
		if (DeviceConfigCenter::PRcvCOM3)
		{
			(*DeviceConfigCenter::PRcvCOM3)();
		}
	}
#endif
}
//串口4接收判断
void DeviceConfigCenter::Com4RcvRoutin(void *param)
{
#if USECOM4
	int ms = Sys.Ms();
	if ((ms - RcvLastTimeCOM4 > 1) && (Rxx4.Length() > 0))
	{
		if (DeviceConfigCenter::PRcvCOM4)
		{
			(*DeviceConfigCenter::PRcvCOM4)();
		}
	}
#endif
}
//串口5接收判断
void DeviceConfigCenter::Com5RcvRoutin(void *param)
{
#if USECOM5
	int ms = Sys.Ms();
	if ((ms - RcvLastTimeCOM5 > 1) && (Rxx5.Length() > 0))
	{
		if (DeviceConfigCenter::PRcvCOM5)
		{
			(*DeviceConfigCenter::PRcvCOM5)();
		}
	}
#endif
}
//定时器
Func DeviceConfigCenter::PTim2Update = 0;
Func DeviceConfigCenter::PTim3Update = 0;
Func DeviceConfigCenter::PTim4Update = 0;
Func DeviceConfigCenter::PTim5Update = 0;
Func DeviceConfigCenter::PTim6Update = 0;
Func DeviceConfigCenter::PTim7Update = 0;
Func DeviceConfigCenter::PTim8Update = 0;


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

