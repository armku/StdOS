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
    // ��ʼ��Ϊ���
//    timer2 = new Timer(Timer7);
//    abc.Bind(tim2refesh);
//    timer2->Register(abc);
//    timer2->Open();
    //        timer2->SetFrequency(1000);
    //        timer2->Config();
}

void RCC_Configuration(void) //ʱ�ӳ�ʼ������
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); //ʹ�ܶ˿�Bʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE); //ʹ�ܶ˿�Cʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //ʹ��TIM14ʱ��
}

void TIM14_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //����һ��TIM_InitTypeDef���͵Ľṹ��

    TIM_ClearITPendingBit(TIM7, TIM_IT_Update); //���жϱ�־���Ա��´��жϵ���ʹ��

    TIM_TimeBaseStructure.TIM_Period = 2000; //1���ӻ�2000������
    TIM_TimeBaseStructure.TIM_Prescaler = 35999; //36000��Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //TIM_CKD_DIV1
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //��ʼ����ʱ��
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE); //ʹ������ж�

    TIM_Cmd(TIM7, ENABLE); //��ʱ��ʹ��
}

static void NVIC_Configuration(void) //�ж����ȼ���ʼ����
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void tim67test()
{
    RCC_Configuration(); //����ʱ�ӳ�ʼ����������
    NVIC_Configuration(); //�����ж����ȼ���ʼ����������
    TIM14_Configuration(); //����TIM3��ʼ����������
	
	timer2 = new Timer(Timer7);
    abc.Bind(tim2refesh);
    timer2->Register(abc);
    timer2->Open();
}
