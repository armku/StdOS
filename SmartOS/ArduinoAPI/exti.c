#include "exti.h"
#include "gpio.h"

static EXTI_CallbackFunction_t EXTI_Function[16] = {0};
											
/**
  * @brief  �ⲿ�жϳ�ʼ��
  * @param  Pin: ���ű��
  * @param  function: �ص�����
  * @param  Trigger_Mode: ������ʽ
  * @param  PreemptionPriority: ��ռ���ȼ�
  * @param  SubPriority: �����ȼ�
  * @retval ��
  */
void EXTIx_Init(uint8_t Pin, EXTI_CallbackFunction_t function, EXTITrigger_TypeDef EXTI_Trigger_x, uint8_t PreemptionPriority, uint8_t SubPriority)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    IRQn_Type EXTIx_IRQn;
    uint8_t Pinx = Get_Pinx(Pin);

    if(Pinx > 15)return;
    EXTI_Function[Pinx] = function;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    //GPIO�ж����Լ��жϳ�ʼ������
    GPIO_EXTILineConfig(Get_GPIOx(Pin), Pinx);		//ѡ��GPIO��Ϊ�ⲿ�ж���·

    EXTI_InitStructure.EXTI_Line = Get_EXTI_Line_x(Pinx);						//�����ж���
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;				//���ô���ģʽ���жϴ������¼�������
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_x;			//���ô�����ʽ
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    if(Pinx <= 4)
    {
        switch(Pinx)
        {
        case 0:
            EXTIx_IRQn = EXTI0_IRQn;
            break;
        case 1:
            EXTIx_IRQn = EXTI1_IRQn;
            break;
        case 2:
            EXTIx_IRQn = EXTI2_IRQn;
            break;
        case 3:
            EXTIx_IRQn = EXTI3_IRQn;
            break;
        case 4:
            EXTIx_IRQn = EXTI4_IRQn;
            break;
        }
    }
    else if(Pinx >= 5 && Pinx <= 9) 
        EXTIx_IRQn = EXTI9_5_IRQn;
    else if(Pinx >= 10 && Pinx <= 15) 
        EXTIx_IRQn = EXTI15_10_IRQn;

    NVIC_InitStructure.NVIC_IRQChannel = EXTIx_IRQn;					//ʹ�����ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;		//��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;				//�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  �ⲿ�жϳ�ʼ��
  * @param  Pin: ���ű��
  * @param  function: �ص�����
  * @param  Trigger_Mode: ������ʽ
  * @retval ��
  */
void attachInterrupt(uint8_t Pin, EXTI_CallbackFunction_t function, EXTITrigger_TypeDef EXTI_Trigger_x)
{
    EXTIx_Init(
        Pin, 
        function, 
        EXTI_Trigger_x, 
        EXTI_PreemptionPriority_Default, 
        EXTI_SubPriority_Default
    );
}

/**
  * @brief  �رո������ж�
  * @param  Pin: ���ű��
  * @retval ��
  */
void detachInterrupt1(uint8_t Pin)
{
    IRQn_Type EXTIx_IRQn;
    uint8_t Pinx = Get_Pinx(Pin);
    if(Pinx > 15)return;

    if(Pinx <= 4)
    {
        switch(Pinx)
        {
        case 0:
            EXTIx_IRQn = EXTI0_IRQn;
            break;
        case 1:
            EXTIx_IRQn = EXTI1_IRQn;
            break;
        case 2:
            EXTIx_IRQn = EXTI2_IRQn;
            break;
        case 3:
            EXTIx_IRQn = EXTI3_IRQn;
            break;
        case 4:
            EXTIx_IRQn = EXTI4_IRQn;
            break;
        }
    }
    else if(Pinx >= 5 && Pinx <= 9)
        EXTIx_IRQn = EXTI9_5_IRQn;
    else if(Pinx >= 10 && Pinx <= 15) 
        EXTIx_IRQn = EXTI15_10_IRQn;
    NVIC_DisableIRQ(EXTIx_IRQn);
}

/**
  * @brief  �ⲿ�ж���ڣ�ͨ��0
  * @param  ��
  * @retval ��
  */
void EXTI0_IRQHandler1(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        if(EXTI_Function[0]) EXTI_Function[0]();
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

/**
  * @brief  �ⲿ�ж���ڣ�ͨ��1
  * @param  ��
  * @retval ��
  */
void EXTI1_IRQHandler1(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        if(EXTI_Function[1]) EXTI_Function[1]();
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

/**
  * @brief  �ⲿ�ж���ڣ�ͨ��2
  * @param  ��
  * @retval ��
  */
void EXTI2_IRQHandler1(void)
{
    if(EXTI_GetITStatus(EXTI_Line2) != RESET)
    {
        if(EXTI_Function[2]) EXTI_Function[2]();
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}

/**
  * @brief  �ⲿ�ж���ڣ�ͨ��3
  * @param  ��
  * @retval ��
  */
void EXTI3_IRQHandler1(void)
{
    if(EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        if(EXTI_Function[3]) EXTI_Function[3]();
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}

/**
  * @brief  �ⲿ�ж���ڣ�ͨ��4
  * @param  ��
  * @retval ��
  */
void EXTI4_IRQHandler1(void)
{
    if(EXTI_GetITStatus(EXTI_Line4) != RESET)
    {
        if(EXTI_Function[4]) EXTI_Function[4]();
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}

/**
  * @brief  �ⲿ�ж���ڣ�ͨ��9~5
  * @param  ��
  * @retval ��
  */
void EXTI9_5_IRQHandler1(void)
{
    if(EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
        if(EXTI_Function[5]) EXTI_Function[5]();
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    if(EXTI_GetITStatus(EXTI_Line6) != RESET)
    {
        if(EXTI_Function[6]) EXTI_Function[6]();
        EXTI_ClearITPendingBit(EXTI_Line6);
    }
    if(EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
        if(EXTI_Function[7]) EXTI_Function[7]();
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
    if(EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
        if(EXTI_Function[8]) EXTI_Function[8]();
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    if(EXTI_GetITStatus(EXTI_Line9) != RESET)
    {
        if(EXTI_Function[9]) EXTI_Function[9]();
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}

/**
  * @brief  �ⲿ�ж���ڣ�ͨ��15~10
  * @param  ��
  * @retval ��
  */
void EXTI15_10_IRQHandler1(void)
{
    if(EXTI_GetITStatus(EXTI_Line10) != RESET)
    {
        if(EXTI_Function[10]) EXTI_Function[10]();
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
    if(EXTI_GetITStatus(EXTI_Line11) != RESET)
    {
        if(EXTI_Function[11]) EXTI_Function[11]();
        EXTI_ClearITPendingBit(EXTI_Line11);
    }
    if(EXTI_GetITStatus(EXTI_Line12) != RESET)
    {
        if(EXTI_Function[12]) EXTI_Function[12]();
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
    if(EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
        if(EXTI_Function[13]) EXTI_Function[13]();
        EXTI_ClearITPendingBit(EXTI_Line13);
    }
    if(EXTI_GetITStatus(EXTI_Line14) != RESET)
    {
        if(EXTI_Function[14]) EXTI_Function[14]();
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
    if(EXTI_GetITStatus(EXTI_Line15) != RESET)
    {
        if(EXTI_Function[15]) EXTI_Function[15]();
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}
