#include "Tone.h"

static TIM_TypeDef *ToneTimer_Last = 0, *ToneTimer = ToneTimer_Default;
static uint8_t tone_State = Off;
static uint8_t tone_Pin;
static uint32_t tone_StopTimePoint;

/**
  * @brief  tone�ж���ڣ�����ʱ�жϵ���
  * @param  ��
  * @retval ��
  */
static void toneTimer_Handler()
{
    togglePin(tone_Pin);
    if(millis() > tone_StopTimePoint && !tone_State)
        noTone(tone_Pin);
}

/**
  * @brief  �ı�tone��ʹ�õĶ�ʱ��
  * @param  TIMx: ��ʱ����ַ
  * @retval ��
  */
void toneSetTimer(TIM_TypeDef* TIMx)
{
    ToneTimer = TIMx;
}

/**
  * @brief  ��Pin������ָ��Ƶ�� (50%ռ�ձȵķ���)
  * @param  pin: ���������� Pin
  * @param  freq: Ƶ��(Hz)
  * @param  Time_ms: �����ĳ���ʱ�� (�Ժ���Ϊ��λ)
  * @retval ��
  */
void tone(uint8_t Pin, uint32_t freq, uint32_t Time_ms)
{
    if(!IS_PIN(Pin) || freq == 0 || freq > 500000 || Time_ms == 0)
    {
        noTone(Pin);
        return;
    }
    tone(Pin, freq);
    tone_StopTimePoint = millis() + Time_ms;
    tone_State = Off;
}

/**
  * @brief  ��Pin������ָ��Ƶ�� (50%ռ�ձȵķ���)
  * @param  pin: �������������ű��
  * @param  freq: Ƶ��(Hz)
  * @retval ��
  */
void tone(uint8_t Pin, uint32_t freq)
{
    if(!IS_PIN(Pin) || freq == 0 || freq > 500000)
    {
        noTone(Pin);
        return;
    }
    tone_Pin = Pin;
    tone_State = On;

    if(ToneTimer != ToneTimer_Last)
    {
        Timer_Init(ToneTimer, (500000.0 / freq), toneTimer_Handler, 0, 0);
        TIM_Cmd(ToneTimer, ENABLE);
        ToneTimer_Last = ToneTimer;
    }
    else
    {
        TimerSet_InterruptTimeUpdate(ToneTimer, 500000.0 / freq);
        TIM_Cmd(ToneTimer, ENABLE);
    }
}

/**
  * @brief  �ر�����
  * @param  Pin: �������������ű��
  * @retval ��
  */
void noTone(uint8_t Pin)
{
    if(!IS_PIN(Pin))
        return;
    
    TIM_Cmd(ToneTimer, DISABLE);
    digitalWrite_LOW(Pin);
    tone_State = Off;
}

/**
* @brief  ��Pin������ָ��Ƶ�� (50%ռ�ձȵķ���������ʽ��ռ�ö�ʱ��)
  * @param  pin: �������������ű��
  * @param  freq: Ƶ��(Hz)
  * @param  Time_ms: �����ĳ���ʱ�� (�Ժ���Ϊ��λ)
  * @retval ��
  */
void toneBlock(uint8_t Pin, uint32_t freq, uint32_t Time_ms)
{
    if(!IS_PIN(Pin) || freq == 0 || Time_ms == 0)
        return;
    
    uint32_t TimePoint = millis() + Time_ms;
    uint32_t dlyus = 500000 / freq; 
    do
    {
        digitalWrite_HIGH(Pin);
        delayMicroseconds(dlyus);
        digitalWrite_LOW(Pin);
        delayMicroseconds(dlyus);
    } while(millis() < TimePoint);
    digitalWrite_LOW(Pin);
}

/**
  * @brief  ��Pin������ָ��Ƶ�ʺ����� (�ɵ�ռ�ձȵķ���������ʽ��ռ�ö�ʱ��)
  * @param  pin: �������������ű��
  * @param  freq: Ƶ��(Hz)
  * @param  Time_ms: �����ĳ���ʱ�� (�Ժ���Ϊ��λ)
  * @param  vol: ����(0.0~1.0��Ӧռ�ձ�0%~100%)
  * @retval ��
  */
void toneBlock_Volumn(uint8_t Pin, uint32_t freq, uint32_t Time_ms, float vol)
{
    if(!IS_PIN(Pin) || freq == 0 || Time_ms == 0 || vol < 0.0)
        return;
    
    uint32_t TimePoint = millis() + Time_ms;
    uint32_t dlyus = 500000 / freq;
    uint32_t dlHigh = dlyus * vol;
    uint32_t dlLow = 2 * dlyus - dlHigh;
    if(freq == 0)return;
    do
    {
        digitalWrite_HIGH(Pin);
        delayMicroseconds(dlHigh);
        digitalWrite_LOW(Pin);
        delayMicroseconds(dlLow);
    } while(millis() < TimePoint);
    digitalWrite_LOW(Pin);
}

/**
  * @brief  ��Pin������ָ��Ƶ�ʺ����� (�ɵ�ռ�ձȵķ���������ʽ��ռ�ö�ʱ��)
  * @param  pin: �������������ű��
  * @param  freq: Ƶ��(Hz)
  * @param  Time_us: �����ĳ���ʱ�� (��΢��Ϊ��λ)
  * @param  vol: ����(0.0~1.0��Ӧռ�ձ�0%~100%)
  * @retval ��
  */
void toneBlock_Volumn_us(uint8_t Pin, uint32_t freq, uint32_t Time_us, float vol)
{
    if(!IS_PIN(Pin) || freq == 0 || Time_us == 0 || vol < 0.0)
        return;
    
    uint32_t TimePoint = micros() + Time_us;
    uint32_t dlyus = 500000 / freq;
    uint32_t dlHigh = dlyus * vol;
    uint32_t dlLow = 2 * dlyus - dlHigh;
    if(freq == 0)return;
    do
    {
        digitalWrite_HIGH(Pin);
        delayMicroseconds(dlHigh);
        digitalWrite_LOW(Pin);
        delayMicroseconds(dlLow);
    } while(micros() < TimePoint);
    digitalWrite_LOW(Pin);
}
