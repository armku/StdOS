/*
F1:
Ұ��ISO-V2 ISO-V3
K0:PA0
K1:PC13


F4
����ԭ��̽����
K0:PA0
K1:PE2
K2:PE3
K3:PE4

 */
#include "Port.h"
#include "Platform\Pin.h"
void EXTIX_Init(void);
#ifdef DEBUG

    //���Դ���
    extern OutputPort led2;
    InputPort key(PC13);
    InputPort key0(PA0);

    void OnPress(InputPort &port, bool down)
    {
        debug_printf("Press P%c%d down=%d\r\n", _PIN_NAME(port._Pin), down);
    }

    void led2flash(void *param)
    {
        led2 = !led2;
    }
    void InterruptTest()
    {
        key0.Press = OnPress;
        key0.UsePress();
        key0.Open();

        key.Press = OnPress;
        key.UsePress();
        key.Open();
		EXTIX_Init();

        Sys.AddTask(led2flash, &led2, 0, 200, "led2");
    }
#endif 
#include "stm32f4xx.h" 
//�ⲿ�ж�0�������
//void EXTI0_IRQHandler(void)
//{
//    delay_ms(10); //����
//    if (WK_UP == 1)
//    {
//        BEEP = !BEEP; //��������ת 
//    }
//    EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ 
//}

////�ⲿ�ж�2�������
//void EXTI2_IRQHandler(void)
//{
//    delay_ms(10); //����
//    if (KEY2 == 0)
//    {
//        LED0 = !LED0;
//    }
//    EXTI_ClearITPendingBit(EXTI_Line2); //���LINE2�ϵ��жϱ�־λ 
//}

////�ⲿ�ж�3�������
//void EXTI3_IRQHandler(void)
//{
//    delay_ms(10); //����
//    if (KEY1 == 0)
//    {
//        LED1 = !LED1;
//    }
//    EXTI_ClearITPendingBit(EXTI_Line3); //���LINE3�ϵ��жϱ�־λ  
//}

////�ⲿ�ж�4�������
//void EXTI4_IRQHandler(void)
//{
//    delay_ms(10); //����
//    if (KEY0 == 0)
//    {
//        LED0 = !LED0;
//        LED1 = !LED1;
//    }
//    EXTI_ClearITPendingBit(EXTI_Line4); //���LINE4�ϵ��жϱ�־λ  
//}
void KEY_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE); //ʹ��GPIOA,GPIOEʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4; //KEY0 KEY1 KEY2��Ӧ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //��ͨ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
    GPIO_Init(GPIOE, &GPIO_InitStructure); //��ʼ��GPIOE2,3,4


//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //WK_UP��Ӧ����PA0
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
//    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��GPIOA0

}
//�ⲿ�жϳ�ʼ������
//��ʼ��PE2~4,PA0Ϊ�ж�����.
void EXTIX_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    KEY_Init(); //������Ӧ��IO�ڳ�ʼ��

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //ʹ��SYSCFGʱ��


    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2); //PE2 ���ӵ��ж���2
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3); //PE3 ���ӵ��ж���3
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4); //PE4 ���ӵ��ж���4
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0); //PA0 ���ӵ��ж���0

    /* ����EXTI_Line0 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line0; //LINE0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�ж��¼�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE; //ʹ��LINE0
    EXTI_Init(&EXTI_InitStructure); //����

    /* ����EXTI_Line2,3,4 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3 | EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�ж��¼�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE; //�ж���ʹ��
    EXTI_Init(&EXTI_InitStructure); //����

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //�ⲿ�ж�0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure); //����

    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn; //�ⲿ�ж�2
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03; //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure); //����


    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn; //�ⲿ�ж�3
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure); //����


    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; //�ⲿ�ж�4
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure); //����

}
