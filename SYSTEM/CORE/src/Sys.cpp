/*
ϵͳ���Զ�ʶ�𳣼�оƬ���͡���Ƶ��Flash��С��Ram��С
Sys.ID ��12�ֽ�оƬΨһ��ʶ��Ҳ����ChipID��ͬһ��оƬ��ǰ�漸���ֽڲ�ͬ
���뼶˯���ڼ䣬ϵͳ���ᰲ��ִ��������ʱ�϶̵��������û�п�������ϵͳ�������͹���ģʽ���Խ�ʡ�ܺ�

 */
#include "stdio.h"
#include "Sys.h"
#include "TTime.h"
#include "stm32f10x.h"
#include "TaskScheduler.h"
#include "SmartIrq.h"

#define delay_ostickspersec 1000			//ʱ��Ƶ��
static byte fac_us = 0; //us��ʱ������
//static uint16_t fac_ms = 0;							//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��

#ifdef __cplusplus
    extern "C"
    {
    #endif 
    void delay_us(uint nus);
    #ifdef __cplusplus
    }
#endif 

TSys Sys; //ϵͳ����
TTime Time; //ϵͳʱ�䣬�������û�ֱ��ʹ��
TaskScheduler *_Scheduler; //�������


TSys::TSys(uint clock, COM_Def messagePort)
{
    this->Clock = clock;
    this->MessagePort = messagePort;
}

void TSys::Show(bool newLine)const{

}

//��ʼ��
//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void TSys::Init()
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //ѡ���ⲿʱ��  HCLK/8
    fac_us = SystemCoreClock / 8000000 * 8; //Ϊϵͳʱ�ӵ�1/8
    //fac_ms = (uint16_t)fac_us * 1000;					//��OS��,����ÿ��ms��Ҫ��systickʱ����
    SysTick_Config(SystemCoreClock / delay_ostickspersec); //tick is 1ms	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //�ر�jtag������swd	
    this->FlashSize = *(uint16_t*)(0X1FFFF7E0); // ����

    for (int i = 0; i < 12; i++)
    {
        this->ID[i] = *(byte*)(0X1FFFF7E8 + i);
    }
    this->FlashSize = *(ushort*)(0X1FFFF7E0);
}

uint TSys::AddTask(Action func, void *param, uint dueTime, int period, const char *name)
{
    // �����жϣ�����������̳߳�ͻ
    SmartIRQ irq;
    if (!_Scheduler)
    {
        _Scheduler = new TaskScheduler("ϵͳ");
    }

    return _Scheduler->Add(func, param, dueTime, period);
}

//���1ms����һ��
void TSys::TimeTick()
{
    Time.mCurrent++;
}

//����ϵͳ������ȣ��ú����ڲ�Ϊ��ѭ����*�ڴ�֮�䣬��ӵ��������������ò������ȣ�����˯�߷�����Ч��
void TSys::Start()
{
    _Scheduler->Start();
}

//����  
void TSys::Routin(){

}

//�����������
void TSys::SetTask(uint taskid, bool onoff, int delayms){

}

//��ʾϵͳ��Ϣ
void TSys::ShowInfo()
{
    printf("STD_Embedded_Team::STD0801 Code:0801 Ver:0.0.6113 Build:2016-01-01\n");
    printf("STDOS::STM32F103C8 72MHz Flash:%dk RAM:20k\n", this->FlashSize);
    printf("DevID:0x0414 RevID:0x1309\n");
    printf("CPUID:0x412fc231 ARM:ARMv7-M Cortex-M3: R1p2\n");
    printf("Heap :(0x20000720, 0x20010000) = 0xf8e0 (62k)\n");
    printf("Stack:(0x20001720, 0x20010000) = 0xe8e0 (58k)\n");
    printf("ChipType:0x42455633 3\n");
    printf("ChipID:%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\n", ID[0], ID[1], ID[2], ID[3], ID[4], ID[5], ID[6], ID[7], ID[8], ID[9], ID[10], ID[11]);
    printf("Time : 2016-12-28 10:56:32\n");
    printf("Support: http://www.armku.com\n");
}

//ϵͳ���������ĺ��������޷��ų�����8�ֽ�
UInt64 TSys::Ms()
{
    return Time.Current();
}

//ϵͳ����UTCʱ�䣬����4�ֽڣ�Unix��ʽ��1970����������������
uint TSys::Seconds()
{
    return Time.Seconds();
}

//΢��ӳ٣������ڸ߾��������źſ���
void TSys::Delay(uint us)
{
    delay_us(us);
}

//���뼶˯�ߣ�������ҵ��������ȴ�һ��ʱ��
void TSys::Sleep(uint ms)
{
    while (ms--)
    {
        this->Delay(1000);
    }
}

//�첽������ϵͳ���ӳ�һ��������ִ�С�
void TSys::Reboot(uint msDelay){}
//ɾ������
void TSys::Remove(uint taskid){

}

#ifdef __cplusplus
    extern "C"
    {
    #endif 

    //systick�жϷ�����,ʹ��ucosʱ�õ�
    void SysTick_Handler(void)
    {
        Sys.TimeTick();
    }

    //��ʱnus
    //nusΪҪ��ʱ��us��.
    void delay_us(uint nus)
    {
        uint ticks;
        uint told, tnow, tcnt = 0;
        uint reload = SysTick->LOAD; //LOAD��ֵ
        ticks = nus * fac_us; //��Ҫ�Ľ�����
        tcnt = 0;
        told = SysTick->VAL; //�ս���ʱ�ļ�����ֵ
        while (1)
        {
            tnow = SysTick->VAL;
            if (tnow != told)
            {
                if (tnow < told)
                    tcnt += told - tnow;
                //����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
                else
                    tcnt += reload - tnow + told;
                told = tnow;
                if (tcnt >= ticks)
                    break;
                //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
            }
        };
    }
    //����Ϊ��ຯ��
    void WFI_SET(void); //ִ��WFIָ��
    void INTX_DISABLE(void); //�ر������ж�
    void INTX_ENABLE(void); //���������ж�
    void MSR_MSP(uint addr); //���ö�ջ��ַ
    //THUMBָ�֧�ֻ������
    //�������·���ʵ��ִ�л��ָ��WFI  
    void WFI_SET(void)
    {
        __ASM volatile("wfi");
    }

    //�ر������ж�
    void INTX_DISABLE(void)
    {
        __ASM volatile("cpsid i");
    }

    //���������ж�
    void INTX_ENABLE(void)
    {
        __ASM volatile("cpsie i");
    }

    //����ջ����ַ
    //addr:ջ����ַ
    __asm void MSR_MSP(uint addr)
    {
        MSR MSP, r0  //set Main Stack value
        BX r14
    }
    #ifdef __cplusplus
    }
#endif
