#ifndef __ADC_H
#define	__ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mcu_type.h"

#define ADC_AverageTimes 8 //ADC��ƽ������

// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��

// ת��ͨ������
#define ADC_CHANNEL_NUM 8
extern uint16_t ADC_ConvertedValue[ADC_CHANNEL_NUM];//ADC DMA Buffer

// ADC1 ��Ӧ DMA1ͨ��1��ADC3��ӦDMA2ͨ��5��ADC2û��DMA����


/**************************��������********************************/
void ADCx_DMA_Init(void);
void ADCx_Init(ADC_TypeDef* ADCx);

uint16_t Get_DMA_ADC(uint8_t Channel);
uint16_t Get_ADC(ADC_TypeDef* ADCx, uint16_t ADC_Channel);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
