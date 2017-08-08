#include "Port.h"
#include "stm32f0xx.h"

const byte vers[] = "yyyy-MM-dd HH:mm:ss";
OutputPort led1(PC6, true);
OutputPort led2(PC7, true);
OutputPort led3(PC8, true);
OutputPort led4(PC9, true);

OutputPort ledss[]={led1,led2,led3,led4};

void LedTask(void *param)
{
	static byte flag=1;
	OutputPort *leds = (OutputPort*)param;
	
	flag<<=1;
	flag&=0x0f;
	if(flag==0)
	{
		flag=1;
	}
	
	leds[0]=flag&0x01;
	leds[1]=flag&0x02;
	leds[2]=flag&0x04;
	leds[3]=flag&0x08;
	
    //    led2 = key0;
}

#define namee "StdOS"


void RCC_Configuration(void)//ʱ�ӳ�ʼ������
{  
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); //����A�˿�ʱ��ʹ��
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
	RS232_GPIO();
	RS232_Configuration();
}
void Company_Information(void)//printf��ӡ�ַ����
{
	printf("******************************����ȭ-������STM32F072VB�����塿********************************\r\n"); 
    printf("����̳��ַ: http://www.zxkjmcu.com\r\n"); 
    printf("���Ա�����: http://zxkjmcu.taobao.com\r\n"); 
    printf("����ע΢�Ź���ƽ̨΢�źš�zxkj-ly��\r\n");     
    printf("��STM32F0�����弼������QQȺ��373645551��\r\n"); 
    printf("��������׿���Ƽ��������޹�˾\r\n"); 
    printf("*************************************************************************************************************\r\n"); 
}
int main()
{
	Sys.Name = (char*)namee;
    Sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 
	BSP_Configuration(); //����Ӳ����ʼ������
	NVIC_Configuration(); //�����ж����ȼ���ʼ������
	Company_Information();
	Sys.AddTask(LedTask, &ledss, 0, 500, "LedTask");

    Sys.Start();
}
