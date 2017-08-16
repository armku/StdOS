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
extern uint com1cnt;
void TimerTask(void *param)
{
    static int i = 0;
    printf("\r\n%d: cnt:%d %d", i++, time6cnt,com1cnt);
}
void TimeTest();
void tim67test();
void BSP_Configuration(void); //Ӳ����ʼ����������
void RCC_Configuration(void); //ʱ�ӳ�ʼ����������
void NVIC_Configuration(void); //�жϳ�ʼ����������
int main()
{
    Sys.Name = (char*)namee;
    Sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 
    
	TimeTest();
	BSP_Configuration(); //����Ӳ����ʼ������
	NVIC_Configuration(); //�����ж����ȼ���ʼ������
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
    timer2 = new Timer(Timer7);
    abc.Bind(tim2refesh);
    timer2->Register(abc);
    timer2->Open();
    //        timer2->SetFrequency(1000);
    //        timer2->Config();
}
void RS232_GPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;//����һ��GPIO_InitTypeDef���͵Ľṹ��
	
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); //����PA9���ù���
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);  //����PA10���ù��� 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;//ѡ��Ҫ���Ƶ�GPIOA����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
}

void RS232_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}
void BSP_Configuration(void)//Ӳ����ʼ������
{	
	RCC_Configuration();
//	LED_GPIO();
	RS232_GPIO();
	RS232_Configuration();
}

void RCC_Configuration(void)//ʱ�ӳ�ʼ������
{  
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); //����A�˿�ʱ��ʹ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE); //����C�˿�ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //���ô���1ʱ��ʹ��
}

static void NVIC_Configuration(void)//�ж����ȼ���ʼ������
{
	NVIC_InitTypeDef   NVIC_InitStructure;//����һ��NVIC_InitTypeDef���͵Ľṹ��

	// rs232
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
