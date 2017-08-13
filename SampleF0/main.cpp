#include "Timer.h"
#include "SerialPort.h"
#include "Drivers\AT24CXX.h"
#include "Drivers\W25QXXX.h"
#include "stm32f0xx.h"

const byte vers[] = "yyyy-MM-dd HH:mm:ss";
OutputPort led1(PC6, true);
OutputPort led2(PC7, true);
OutputPort led3(PC8, true);
OutputPort led4(PC9, true);

OutputPort ledss[] = 
{
    led1, led2, led3, led4
};

void LedTask(void *param)
{
    static byte flag = 1;
    OutputPort *leds = (OutputPort*)param;

    flag <<= 1;
    flag &= 0x0f;
    if (flag == 0)
    {
        flag = 1;
    }

    leds[0] = flag &0x01;
    leds[1] = flag &0x02;
    leds[2] = flag &0x04;
    leds[3] = flag &0x08;

    //    led2 = key0;
}

#define namee "StdOS"

uint OnUsart1Read(ITransport *transport, Buffer &bs, void *para, void *para2)
{
    bs.Show(true);
    return 0;
}

uint time6cnt;
void TimerTask(void *param)
{
    static int i = 0;
    printf("\r\n%d: cnt:%d", i++, time6cnt);
}
void TimeTest();
void tim67test();
int main()
{
    Sys.Name = (char*)namee;
    Sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 
    
	TimeTest();
	tim67test();
    Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
    Sys.AddTask(TimerTask, &led1, 0, 1000, "TimerTask");

    Sys.Start();
}

Delegate < Timer & > abc;
void tim2refesh(void *param)
{
    time6cnt++;
}

Timer *timer2;
void TimeTest()
{
    // 初始化为输出
//    timer2 = new Timer(Timer7);
//    abc.Bind(tim2refesh);
//    timer2->Register(abc);
//    timer2->Open();
    //        timer2->SetFrequency(1000);
    //        timer2->Config();
}

void RCC_Configuration(void) //时钟初始化函数
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); //使能端口B时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE); //使能端口C时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //使能TIM14时钟
}

void TIM14_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //定义一个TIM_InitTypeDef类型的结构体

    TIM_ClearITPendingBit(TIM7, TIM_IT_Update); //清中断标志，以备下次中断到来使用

    TIM_TimeBaseStructure.TIM_Period = 2000; //1秒钟机2000个脉冲
    TIM_TimeBaseStructure.TIM_Prescaler = 35999; //36000分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //TIM_CKD_DIV1
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //初始化定时器
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE); //使能溢出中断

    TIM_Cmd(TIM7, ENABLE); //定时器使能
}

static void NVIC_Configuration(void) //中断优先级初始化函
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void tim67test()
{
    RCC_Configuration(); //调用时钟初始化函数声明
    NVIC_Configuration(); //调用中断优先级初始化函数声明
    TIM14_Configuration(); //调用TIM3初始化函数声明
	
	timer2 = new Timer(Timer7);
    abc.Bind(tim2refesh);
    timer2->Register(abc);
    timer2->Open();
}
