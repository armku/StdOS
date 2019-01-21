GPIO寄存器描述 《STM32参考手册中文-p75》

1、端口配置低寄存器(GPIOx_CRL)(x = A...E)
2、端口配置高寄存器(GPIOx_CRH)(x = A...E)

3、端口输入数据寄存器(GPIOx_IDR)(x = A...E)
   这些位为只读并只能以字(16位)的形式读出。读出的值为对应I/O口的状态。
   
4、端口输出数据寄存器(GPIOx_ODR)(x = A...E)
   只能以字(16bit)的形式操作，复位值全是0。写0即输出0，写1即输出1。
   
5、端口位设置/清除寄存器(GPIOx_BSRR)(x = A...E)
   高16bit写1用于清0，低16bit写1用于置位，同时写1的话低16bi有效。
   
6、端口位清除寄存器(GPIOx_BRR)(x = A...E)
   低16位写1用于置位。
   
7、端口配置锁定寄存器(GPIOx_LCKR)(x = A...E)

GPIO八种模式
typedef enum
{ GPIO_Mode_AIN = 0x0,            /* 模拟输入 */   
  GPIO_Mode_IN_FLOATING = 0x04,   /* 浮空输入，复位后的状态 */
  GPIO_Mode_IPD = 0x28,           /* 下拉输入 */
  GPIO_Mode_IPU = 0x48,           /* 上拉输入 */
  GPIO_Mode_Out_OD = 0x14,        /* 开漏输出 */
  GPIO_Mode_Out_PP = 0x10,        /* 推挽输出 */
  GPIO_Mode_AF_OD = 0x1C,         /* 复用开漏输出 */
  GPIO_Mode_AF_PP = 0x18          /* 复用推挽输出 */
}GPIOMode_TypeDef;

调试DLL:TARMSTM.DLL 
调试参数：-pSTM32F103C8

启动文件
ld.s适用于小容量 产品；
md.s适用于中等容量产品；
hd适用于大容量产品；
这里的容量是指FLASH的大小.判断方法如下：
小容量：FLASH≤32K
中容量：64K≤FLASH≤128K
大容量：256K≤FLASH
hal hardware abstraction layer 硬件有关


#include "Sys.h"
#include "Device\Port.h"
//#include "Device\SerialPort.h"

void LedTask(void* param)
{
    auto leds	= (mcuGpio*)param;
    *leds = !*leds;
}

void OnPress(InputPort& port, bool down)
{
    debug_printf("Press P%c%d down=%d\r\n", _PIN_NAME(port._Pin), down);
}

int main(void)
{
    // 初始化系统
    //Sys.Clock = 72000000;
    //Sys.MessagePort = COM1;
    Sys.Init();
    //Sys.InitClock();
    Sys.ShowInfo();

    // 初始化为输出
	mcuGpio leds[] = {PD0, PD1};
	for(int i=0; i<ArrayLength(leds); i++)
		leds[i].Invert = true;

    // 初始化为中断输入
    Pin ips[] = { PE9, PE8, PE4, PE5, PE6, PE7};
    InputPort btns[6];
	for(int i=0; i<ArrayLength(btns); i++)
	{
		btns[i].Index = i;
		btns[i].Set(ips[i]);
		btns[i].Press = OnPress;
		btns[i].UsePress();
		btns[i].Open();
	}

	Sys.AddTask(LedTask, &leds[0], 500, 500, "Led闪烁");

	/*Buffer::Test();
	Array::Test();
	String::Test();
	DateTime::Test();
	IList::Test();
	IDictionary::Test();*/
	/*mcuGpio power(PE2);
	power = true;
	SerialPort::Test();*/

    Sys.Start();
}
STM32F103R8t6 64脚 PA-PC
端口配置：
PA00:NO	TIM2-CH1
PA01:NO	TIM2-CH2
PA02:NO	USART2-TX 	
PA03:NO	USART2-RX 	
PA04:NO					
PA05:NO	SPI1-SCK    
PA06:NO	SPI1-MISO	TIM3-CH1
PA07:NO	SPI1-MISI	TIM3-CH2
PA08:FT	
PA09:FT	USART1-TX
PA10:FT	USART1-RX
PA11:FT	
PA12:FT	
PA13:FT	SWDIO	SWD下载口
PA14:FT	SWDCLK	SWD下载口
PA15:FT	

PB00:NO	TIM3-CH3 
PB01:NO	TIM3-CH4
PB02:FT	
PB03:FT	
PB04:FT	
PB05:NO	
PB06:FT	I2C1-SCL	TIM4-CH1
PB07:FT	I2C1-SDA	TIM4-CH2
PB08:FT				TIM4-CH3
PB09:FT				TIM4-CH4
PB10:FT	USART3-TX
PB11:FT	USART3-RX
PB12:FT			
PB13:FT	SPI2-SCK	
PB14:FT	SPI2-MISO	
PB15:FT	SPI2-MISI	

PC00:NO SPI1-CS		
PC01:NO
PC02:NO	SPI2-CS
PC03:NO
PC04:NO		
PC05:NO		
PC06:FT 	
PC07:FT 	
PC08:FT 	
PC09:FT 	
PC10:FT	USART4_TX	
PC11:FT	USART4_RX	
PC12:FT	USART5_TX
PC13:NO
PC14:NO
PC15:NO

PD00:FT
PD01:FT
PD02:FT	USART5-RX
PD03:FT
PD04:FT
PD05:FT
PD06:FT 
PD07:FT 
PD08:FT 
PD09:FT 
PD10:FT
PD11:FT
PD12:FT
PD13:FT
PD14:FT
PD15:FT

PE00:FT
PE01:FT
PE02:FT
PE03:FT
PE04:FT
PE05:FT
PE06:FT 
PE07:FT 
PE08:FT 
PE09:FT 
PE10:FT
PE11:FT
PE12:FT
PE13:FT
PE14:FT
PE15:FT
