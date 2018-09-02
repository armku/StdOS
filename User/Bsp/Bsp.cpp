#include "BspPlatform\BspPlatform.h"
#include "Bsp.h"
#include "Device\Port.h"
#include "Device\DeviceConfigHelper.h"

#ifdef STM32F0
OutputPort led1(PC6, true);
OutputPort led2(PC7, true);
OutputPort led3(PC8, true);
OutputPort led4(PC9, true);

OutputPort ledss[] =
{
	led1, led2, led3, led4
};
#elif defined STM32F1
#if 1
OutputPort led1(PB0, false);
OutputPort led2(PF7, false);
OutputPort led3(PF8, false);
#else 
OutputPort led1(PD0, false);
OutputPort led2(PD1, false);
OutputPort led3(PD2, false);
OutputPort ledLCD(PD12, false);
#endif 
#elif defined STM32F4
#if 1
//正点原子开发板F4
OutputPort led1(PF9, false);
OutputPort led2(PF10, false);
#else 
//安福莱开发板F4-V5
OutputPort led1(PI10, false);
OutputPort led2(PF7, false);
OutputPort led3(PC2, false);
OutputPort led4(PC2, false);
#endif 
#endif

void LedTask(void *param)
{
#ifdef STM32F0
	static uint8_t flag = 1;
	auto leds = (OutputPort*)param;

	flag <<= 1;
	flag &= 0x0f;
	if (flag == 0)
	{
		flag = 1;
	}

	led1 = !led1;
	led2 = !led2;
	led3 = !led3;
	led4 = !led4;
#elif defined STM32F1
	auto leds = (OutputPort*)param;
	*leds = !* leds;
	//    led2 = key0;
#elif defined STM32F4
	OutputPort *leds = (OutputPort*)param;
	*leds = !* leds;
	//led2 = !led2;
	//    led3 = !led3;
	//    led4 = !led4;
	//led2 = keyup;
#endif 
}

uint8_t chbuf[1000];

void com1rcv()
{
	Buffer bs1(chbuf, ArrayLength(chbuf));

	Rxx1.Read(bs1);

	debug_printf("COM1RCV:\n");
	bs1.ShowHex(true);
}
void Esp8266TestInit();
void W5500Test();
void dmacom1test();
void BspInit()
{
	led1 = 1;
	led2 = 1;
	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
	DeviceConfigCenter::PRcvCOM1 = com1rcv;
			
	//W5500Test();
	dmacom1test();
}
#include "stm32f10x.h"
#define            SENDBUFF_SIZE                             5000
uint8_t SendBuff[SENDBUFF_SIZE];
/**
* @brief  USARTx TX DMA 配置，内存到外设(USART1->DR)
* @param  无
* @retval 无
*/
void USARTx_DMA_Config()
{
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
	DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;

	/*外设地址不增*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	/*内存地址自增*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

	/*外设数据单位*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

	/*内存数据单位 8bit*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

	/*DMA模式：不断循环*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
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
}

void dmacom1test()
{
	USARTx_DMA_Config();
	debug_printf("\r\n usart1 DMA TX 测试 \r\n");

	{
		uint16_t i;

		/*填充将要发送的数据*/
		for (i = 0; i<SENDBUFF_SIZE; i++)
		{
			SendBuff[i] = 'A';
		}
	}

	/* USART1 向 DMA发出TX请求 */
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}

