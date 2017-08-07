#include "Port.h"
#include "stm32f0xx.h"

const byte vers[] = "yyyy-MM-dd HH:mm:ss";
OutputPort led1(PC6, true);
OutputPort led2(PC7, true);
OutputPort led3(PC8, true);
OutputPort led4(PC9, true);

void delay_ms(uint16_t nms)
{
    uint32_t temp;
    SysTick->LOAD = 6000 * nms;
    SysTick->VAL = 0X00; //清空计数器
    SysTick->CTRL = 0X01; //使能，减到零是无动作，采用外部时钟源
    do
    {
        temp = SysTick->CTRL; //读取当前倒计数值
    }
    while ((temp &0x01) && (!(temp &(1 << 16)))); //等待时间到达
    SysTick->CTRL = 0x00; //关闭计数器
    SysTick->VAL = 0X00; //清空计数器
}

void LED_Demo(void)
{
    led1=1;; //拉低PC6引脚，LED1发光二极管(发光)
    //Sys.Sleep(500);//delay_ms(500); //
    led1=0; //拉高PC6引脚，LED1发光二极管(熄灭)
    delay_ms(500);

    led4=1; //拉低PC9引脚，RGB发光二极管(发红色光)
    delay_ms(500);
    led4=0; //拉高PC9引脚，RGB发光二极管(红色熄灭)
    delay_ms(500);

    led3=1; //拉低PC8引脚，RGB发光二极管(发绿色光)
    delay_ms(500);
    led3=0; //拉低PC8引脚，RGB发光二极管(绿色熄灭)
    delay_ms(500);

    led2=1; //拉低PC7引脚，RGB发光二极管(发蓝色光)
    delay_ms(500);
    led2=0; //拉高PC7引脚，RGB发光二极管(蓝色熄灭)
    delay_ms(500);
}
void LedTask(void *param)
{
    OutputPort *leds = (OutputPort*)param;
    *leds = ! * leds;
    //    led2 = key0;
}

#define namee "StdOS"

void main()
{
    TSys &sys = (TSys &)(Sys);
    sys.Name = (char*)namee;
    sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 

//    Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");


    while (1)
    {
        LED_Demo(); //调用LED流水灯例程
    }
    Sys.Start();
}
