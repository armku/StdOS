/*
�������Ź�IWDG�����ж���ʱ�ӣ��ڲ�����ʱ��LSI---40KHz�������Բ���ϵͳӲ��Ӱ���ϵͳ����̽��������Ҫ���ڼ���Ӳ������ 
���ڿ��Ź�WWDG����ʱ����ϵͳ��ͬ�����ϵͳʱ�Ӳ����ˣ������Ҳ��ʧȥ�����ˣ���Ҫ���ڼ���������� 

�������Ź�
���Ź���ʱʱ��=IWDG_SetReload()��ֵ / ���Ź�ʱ��Ƶ��
���Ź�ʱ��Ƶ��=LSI���ڲ�����ʱ�ӣ���Ƶ�ʣ�40KHz��/ ��Ƶ��

�������Ź�(IWDG)��ר�õ�40kHz�ĵ���ʱ��Ϊ��������ˣ���ʹ��ʱ�ӷ���������Ҳ��Ȼ��Ч��
���ڿ��Ź��ɴ�APB1ʱ�ӷ�Ƶ��õ���ʱ��������ͨ�������õ�ʱ�䴰�������Ӧ�ó���������Ĺ��ٻ�������Ϊ��
IWDG_SetPrescaler()        ��Ƶ��4~256
IWDG_SetReload()        ����ֵ12λ 0~0x0FFF
IWDG_ReloadCounter()    ι���������ڼ���ֵ����1֮ǰι��

IWDG_PR��IWDG_RLR�Ĵ�������д�������ܡ�Ҫ�޸��������Ĵ�����ֵ����������IWDG_KR�Ĵ�����д��0x5555��
�Բ�ͬ��ֵд������Ĵ���������Ҳ���˳�򣬼Ĵ��������±���������װ�ز���(��д��0xAAAA)Ҳ������д�������ܡ�

#define IWDG_WriteAccess_Enable     ((uint16_t)0x5555)
#define IWDG_WriteAccess_Disable    ((uint16_t)0x0000)

void IWDG_WriteAccessCmd(uint16_t IWDG_WriteAccess)
{
assert_param(IS_IWDG_WRITE_ACCESS(IWDG_WriteAccess));
IWDG->KR = IWDG_WriteAccess;
}

void IWDG_ReloadCounter(void)
{
IWDG->KR = KR_KEY_Reload;
}
void IWDG_Enable(void)
{
IWDG->KR = KR_KEY_Enable;
}

void IWDG_SetPrescaler(uint8_t IWDG_Prescaler)
{
assert_param(IS_IWDG_PRESCALER(IWDG_Prescaler));
IWDG->PR = IWDG_Prescaler;
}
void IWDG_SetReload(uint16_t Reload)
{
assert_param(IS_IWDG_RELOAD(Reload));
IWDG->RLR = Reload;
}

 */
 #include <stdio.h>
#include "stm32f10x.h"
#include "WatchDog.h"
#include "Sys.h"

WatchDog::WatchDog(uint ms)
{
    Timeout = ms;
    Config(ms);
}

WatchDog::~WatchDog()
{
    ConfigMax();
}


bool WatchDog::Config(uint ms)
{
    if (ms == 0)
    {
        debug_printf("WatchDog msTimeout %dms must larger than 0ms\r\n", ms);
        return false;
    }
    RCC_LSICmd(ENABLE);
    /* ���ϵͳ�Ƿ��IWDG���ûָ� */
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
    {
        /* ������ñ�ʶ */
        RCC_ClearFlag();
    }
    /* ��IWDG_PR��IWDG_RLR�Ĵ�����д���� */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    byte pre = IWDG_Prescaler_4;
    int mul = 4;
    // ������12λ 0~0x0FFF����reload = ms/1000 / (1/(LSI/mul)) = ms * LSI / (mul*1000) = ms * 40 / mul
    // ���ǵ�reload����Ŀ��ܣ�ÿ�ַ�Ƶ���ms = reload * mul / 40 ~= 102 * mul
    int i = 0;
    /*
    #define IWDG_Prescaler_4            ((uint8_t)0x00)
    #define IWDG_Prescaler_8            ((uint8_t)0x01)
    #define IWDG_Prescaler_16           ((uint8_t)0x02)
    #define IWDG_Prescaler_32           ((uint8_t)0x03)
    #define IWDG_Prescaler_64           ((uint8_t)0x04)
    #define IWDG_Prescaler_128          ((uint8_t)0x05)
    #define IWDG_Prescaler_256          ((uint8_t)0x06)
     */
    for (i = IWDG_Prescaler_4; i <= IWDG_Prescaler_256; i++)
    {
        pre = i;
        mul = 1 << (i + 2);

        // �ж��Ƿ��ڷ�Χ֮��
        if (ms *40 / mul < 0x0FFF)
            break;
    }
    if (i > IWDG_Prescaler_256)
    {
        debug_printf("WatchDog msTimeout must smaller than %dms\r\n", 0x0FFF *256 / 40);
        return false;
    }
    IWDG_SetPrescaler(pre);
    /*if(ms < (0x0FFF * 32 / 40))
    {
    // IWDG������ʱ��: LSI/32=40KHz/32=1250Hz��ÿ����0.8ms
    IWDG_SetPrescaler(IWDG_Prescaler_32);
    }
    else
    {
    // IWDG������ʱ��: LSI/64=40KHz/64=625Hz��ÿ����0.4ms
    IWDG_SetPrescaler(IWDG_Prescaler_64);

    // ֱ�ӳ���2�����治���ظ�����
    ms >>= 2;
    }*/

    /* ���ü���������ֵΪ��ʱʱ��
    Counter Reload Value = ms / 1000 / IWDG������ʱ������
    = ms / 1000 / (1/(LSI/mul))
    = ms * LSI / (mul * 1000)
    = ms * 40k / (mul * 1000)
    = ms * 40 / mul
     */
    IWDG_SetReload(ms *40 / mul);

    /* ���� IWDG ������ */
    IWDG_ReloadCounter();

    /* �� IWDG (LSI����Ӳ����) */
    IWDG_Enable();

    Timeout = ms;

    return true;
}

void WatchDog::ConfigMax()
{
    /* ��IWDG_PR��IWDG_RLR�Ĵ�����д���� */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    // �������Ź��޷��ر�
    IWDG_SetPrescaler(IWDG_Prescaler_256);
    IWDG_SetReload(0x0FFF);
    IWDG_ReloadCounter();
}

void WatchDog::Feed()
{
    IWDG_ReloadCounter();
}
