/*
ϵͳ���Զ�ʶ�𳣼�оƬ���͡���Ƶ��Flash��С��Ram��С
Sys.ID ��12�ֽ�оƬΨһ��ʶ��Ҳ����ChipID��ͬһ��оƬ��ǰ�漸���ֽڲ�ͬ
���뼶˯���ڼ䣬ϵͳ���ᰲ��ִ��������ʱ�϶̵��������û�п�������ϵͳ�������͹���ģʽ���Խ�ʡ�ܺ�

 */
#include "stdio.h"
#include "TSys.h"
#include "Task.h"
#include "stm32f10x.h"

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




Task Scheduling; //����
TSys Sys; //ϵͳ����

TSys::TSys(uint clock, MessagePort_T messagePort)
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
	
	this->FlashSize = *(uint16_t *)(0X1FFFF7E0);  // ����
	
	for(int i=0;i<12;i++)
	{
		this->ID[i]=*(byte*)(0X1FFFF7E8+i);
	}
	this->FlashSize=*(ushort*)(0X1FFFF7E0);
}

//����ϵͳ������ȣ��ú����ڲ�Ϊ��ѭ����*�ڴ�֮�䣬��ӵ��������������ò������ȣ�����˯�߷�����Ч��
void TSys::Start()
{
    while (true)
    {
        Scheduling.Routin();
    }
}

//��ʾϵͳ��Ϣ
void TSys::ShowInfo()
{
    printf("STD_Embedded_Team::STD0801 Code:0801 Ver:0.0.6113 Build:2016-01-01\n");
    printf("STDOS::STM32F103C8 72MHz Flash:%dk RAM:20k\n",this->FlashSize);
    printf("DevID:0x0414 RevID:0x1309\n");
    printf("CPUID:0x412fc231 ARM:ARMv7-M Cortex-M3: R1p2\n");
    printf("Heap :(0x20000720, 0x20010000) = 0xf8e0 (62k)\n");
    printf("Stack:(0x20001720, 0x20010000) = 0xe8e0 (58k)\n");
    printf("ChipType:0x42455633 3\n");
    printf("ChipID:");
	for(int i=0;i<11;i++)
	{
		printf("%02X-",this->ID[i]);
	}
	printf("%2X\n",this->ID[11]);
    printf("Time : 2016-12-28 10:56:32\n");
    printf("Support: http://www.armku.com\n");
}

//ϵͳ���������ĺ��������޷��ų�����8�ֽ�
uint64_t TSys::Ms()
{
    return this->ms;
}

//ϵͳ����UTCʱ�䣬����4�ֽڣ�Unix��ʽ��1970����������������
uint TSys::Seconds()
{
    return this->seconds;
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
/*
������񣬲����ֱ��ǣ����������������״�ʱ�䡢���ʱ
�䡢���ơ�����ֵ��һ�� uint ������Ψһ��š�	
 */
uint TSys::AddTask(void(*callback)(void), void *para, uint delaycntms, uint intervalms, String name)
{

    return 0;
}

//��ʱ��
uint TSys::AddTask(void(*callback)(void), void *para, uint delaycntms, uint intervalms)
{
    Scheduling.AddTask(callback, delaycntms, intervalms);
    return 0;
}

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
        Scheduling.TimeTick();
		Sys.ms++;
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
    #ifdef __cplusplus
    }
#endif
