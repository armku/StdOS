#include "adc.h"
#include "Arduino.h"

uint16_t ADC_ConvertedValue[ADC_CHANNEL_NUM];//ADC DMA Buffer

/**
  * @brief  ��PA0~PA7����Ϊģ������ģʽ������DMA
  * @param  ��
  * @retval ��
  */
static void ADCx_GPIO_Config(void)
{
    pinMode(PA0, INPUT_ANALOG);
    pinMode(PA1, INPUT_ANALOG);
    pinMode(PA2, INPUT_ANALOG);
    pinMode(PA3, INPUT_ANALOG);
    pinMode(PA4, INPUT_ANALOG);
    pinMode(PA5, INPUT_ANALOG);
    pinMode(PA6, INPUT_ANALOG);
    pinMode(PA7, INPUT_ANALOG);
}

/**
  * @brief  ADC DMA ����
  * @param  ��
  * @retval ��
  */
static void ADCx_DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    // ��DMAʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // ��ADCʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // ��λDMA������
    DMA_DeInit(DMA1_Channel1);

    // ���� DMA ��ʼ���ṹ��
    // �����ַΪ��ADC ���ݼĴ�����ַ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&( ADC1->DR));

    // �洢����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_ConvertedValue;

    // ����Դ��������
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;

    // ��������С��Ӧ�õ�������Ŀ�ĵصĴ�С
    DMA_InitStructure.DMA_BufferSize = ADC_CHANNEL_NUM;

    // ����Ĵ���ֻ��һ������ַ���õ���
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

    // �洢����ַ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

    // �������ݴ�СΪ���֣��������ֽ�
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;

    // �ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

    // ѭ������ģʽ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

    // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;

    // ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    // ��ʼ��DMA
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    // ʹ�� DMA ͨ��
    DMA_Cmd(DMA1_Channel1, ENABLE);

    // ADC ģʽ����
    // ֻʹ��һ��ADC�����ڵ�ģʽ
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;

    // ɨ��ģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE ;

    // ����ת��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

    // �����ⲿ����ת���������������
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

    // ת������Ҷ���
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

    // ת��ͨ������
    ADC_InitStructure.ADC_NbrOfChannel = ADC_CHANNEL_NUM;

    // ��ʼ��ADC
    ADC_Init(ADC1, &ADC_InitStructure);

    // ����ADCʱ��CLK2��8��Ƶ����9MHz
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    // ����ADC ͨ����ת��˳��Ͳ���ʱ��
    ADC_RegularChannelConfig(ADC1, PIN_MAP[PA0].ADC_Channel, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, PIN_MAP[PA1].ADC_Channel, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, PIN_MAP[PA2].ADC_Channel, 3, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, PIN_MAP[PA3].ADC_Channel, 4, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, PIN_MAP[PA4].ADC_Channel, 5, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, PIN_MAP[PA5].ADC_Channel, 6, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, PIN_MAP[PA6].ADC_Channel, 7, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, PIN_MAP[PA7].ADC_Channel, 8, ADC_SampleTime_55Cycles5);

    // ʹ��ADC DMA ����
    ADC_DMACmd(ADC1, ENABLE);

    // ����ADC ������ʼת��
    ADC_Cmd(ADC1, ENABLE);

    // ��ʼ��ADC У׼�Ĵ���
    ADC_ResetCalibration(ADC1);
    // �ȴ�У׼�Ĵ�����ʼ�����
    while(ADC_GetResetCalibrationStatus(ADC1));

    // ADC��ʼУ׼
    ADC_StartCalibration(ADC1);
    // �ȴ�У׼���
    while(ADC_GetCalibrationStatus(ADC1));

    // ����û�в����ⲿ����������ʹ���������ADCת��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/**
  * @brief  ADC DMA ��ʼ��
  * @param  ��
  * @retval ��
  */
void ADCx_DMA_Init(void)
{
    ADCx_GPIO_Config();
    ADCx_DMA_Config();
}


/**
  * @brief  ��ȡ ADC DMA ֵ
  * @param  Channel: ADCͨ����(����)
  * @retval ��
  */
uint16_t Get_DMA_ADC(uint8_t Channel)
{
    return ADC_ConvertedValue[Channel];
}

/**
  * @brief  ADC ����
  * @param  ADCx: ADC��ַ
  * @retval ��
  */
void ADCx_Init(ADC_TypeDef* ADCx)
{
    ADC_InitTypeDef ADC_InitStructure;
    if(ADCx == ADC1)RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
    else if(ADCx == ADC2)RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE );
    else if(ADCx == ADC3)RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE );
    else return;
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    ADC_DeInit(ADCx);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADCx, &ADC_InitStructure);

    ADC_Cmd(ADCx, ENABLE);
    ADC_ResetCalibration(ADCx);
    while(ADC_GetResetCalibrationStatus(ADCx));
    ADC_StartCalibration(ADCx);
    while(ADC_GetCalibrationStatus(ADCx));
}

/**
  * @brief  ��ȡ ADC ֵ
  * @param  ADCx: ADC��ַ
  * @param  ADC_Channel: ADCͨ��
  * @retval ��
  */
uint16_t Get_ADC(ADC_TypeDef* ADCx, uint16_t ADC_Channel)
{
    uint32_t result = 0;
    uint16_t i;

    ADC_RegularChannelConfig(ADCx, ADC_Channel, 1, ADC_SampleTime_1Cycles5 );
    for(i = 0; i < ADC_AverageTimes; i++)
    {
        ADC_SoftwareStartConvCmd(ADCx, ENABLE);
        while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC));
        result += ADC_GetConversionValue(ADCx);
    }
    result = result / ADC_AverageTimes;
    return result;
}
