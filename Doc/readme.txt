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
    auto leds	= (OutputPort*)param;
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
	OutputPort leds[] = {PD0, PD1};
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
	/*OutputPort power(PE2);
	power = true;
	SerialPort::Test();*/

    Sys.Start();
}
