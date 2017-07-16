/*
ϵͳ���Զ�ʶ�𳣼�оƬ���͡���Ƶ��Flash��С��Ram��С
Sys.ID ��12�ֽ�оƬΨһ��ʶ��Ҳ����ChipID��ͬһ��оƬ��ǰ�漸���ֽڲ�ͬ
���뼶˯���ڼ䣬ϵͳ���ᰲ��ִ��������ʱ�϶̵��������û�п�������ϵͳ�������͹���ģʽ���Խ�ʡ�ܺ�

 */
#include "stdio.h"
#include "Sys.h"
#include "TTime.h"
#include "Task.h"
#include "Sys.h"
#include <string.h>
#include "Array.h"
#include "DateTime.h"
#include "TInterrupt.h"
#ifdef STM32F1
	#include "stm32f10x.h"
#endif
#ifdef STM32F4
	#include "stm32f4xx.h"
#endif

#if 0
extern "C"
{
    extern uint __heap_base;
    extern uint __heap_limit;
    extern uint __initial_sp;
}
#endif
String *CPUName;
void assert_failed2(cstring msg, cstring file, unsigned int line)
{
}
//�ⲿע�ắ��
// ����
#include "Task.h"
// ������
TaskScheduler *_Scheduler;

// �������񣬷��������š�dueTime�״ε���ʱ��ms��period���ȼ��ms��-1��ʾ������һ��
uint TSys::AddTask(Action func, void* param, int dueTime, int period, cstring name) const
{
    // �����жϣ�����������̳߳�ͻ
    SmartIRQ irq;

    if (!_Scheduler)
    {
        _Scheduler = new TaskScheduler("Sys");
    }

    return _Scheduler->Add(func, param, dueTime, period, name);
}

void TSys::RemoveTask(uint& taskid) const
{
    #if 0
        assert_ptr(_Scheduler);
    #endif 
    _Scheduler->Remove(taskid);
}
// ��������Ŀ���״̬��ͬʱ����ָ���������һ�ε��ȵ�ʱ�䣬0��ʾ���ϵ���
bool TSys::SetTask(uint taskid, bool enable, int msNextTime) const
{
//    _Scheduler->SetTask(taskid, enable, msNextTime);
	return true;
}

//����ϵͳ������ȣ��ú����ڲ�Ϊ��ѭ����*�ڴ�֮�䣬��ӵ��������������ò������ȣ�����˯�߷�����Ч��
void TSys::Start()
{
    if (!_Scheduler)
    {
        _Scheduler = new TaskScheduler("Sys");
    }

    #if 0		
        if (OnStart)
        {
            OnStart();
        }
        else
    #endif 
    {
        _Scheduler->Start();
    }
}



void TimeSleep(uint us)
{
    // �����ʱ�����棬ȥ����һ�±������
    if (_Scheduler && (us >= 1000))
    {
        // ��¼��ǰ����ִ������
        Task *task = _Scheduler->Current;

        UInt64 start = Time.Current();
        // 1msһ�㲻����������������Ӳ���ȴ�
        UInt64 end = start + us - 1000;
        // �������ʱ���㹻���������ε�����������
        int cost = 0;
        while (true)
        {
            UInt64 start2 = Time.Current();
			bool bb=false;
            _Scheduler->Execute(us,bb);

            UInt64 now = Time.Current();
            cost += (int)(now - start2);

            // us=0 ��ʾ�ͷ�һ��CPU
            if (!us)
            {
                return ;
            }

            if (now >= end)
            {
                break;
            }
        }

        if (task)
        {
            _Scheduler->Current = task;
            task->SleepTime += cost;
        }

        cost = (int)(Time.Current() - start);
        if (cost > 0)
        {
            return ;
        }

        us -= cost;
    }
    if (us)
    {
        Time.Sleep(us);
    }
}
void TSys::Sleep(int ms) const // ���뼶�ӳ�
{
    // ����ʹ���̼߳�˯��
    #if 0
        if (OnSleep)
        {
            OnSleep(ms);
        }
        else
    #endif 
    {
        if (ms > 1000)
        {
            debug_printf("Sys::Sleep ��ƴ���˯��%dms̫��������1000ms����ʹ�ö��߳�Thread��", ms);
        }
        TimeSleep(ms *1000);
    }
}
void TSys::Delay(int us) const // ΢�뼶�ӳ�
{
    // ����ӳ�΢����̫����ʹ���̼߳�˯��
    #if 0
        if (OnSleep && us >= 2000)
        {
            OnSleep((us + 500) / 1000);
        }
        else
    #endif 
    {

        if (us > 1000000)
        {
            debug_printf("Sys::Sleep ��ƴ���˯��%dus̫��������1000ms����ʹ�ö��߳�Thread��", us);
        }
        TimeSleep(us);
    }
}

TSys::TSys()
{
	this->Clock = 72000000;
    this->MessagePort = COM1;
}


UInt64	 TSys::Ms() const		// ϵͳ������ĺ�����
{
    return Time.Ms();
}
uint	TSys::Seconds() const	// ϵͳ���Ե�ǰʱ�䣬��
{
    return Time.Seconds;
}

// �ӳ��첽����
void TSys::Reboot(int msDelay ) const
{
	NVIC_SystemReset();
}
void TSys::ShowInfo() const
{
	uint Rx=0;
	uint Px=0;
#if 0	
    uint HeapSize = 0;
    uint StackSize = 0;
	

    HeapSize = ((uint) &__heap_limit - (uint) &__heap_base);
    StackSize = ((uint) &__initial_sp - (uint) &__heap_limit);
#endif
	switch(this->CPUID&SCB_CPUID_VARIANT_Msk)
	{
		case 0:
			if((this->CPUID&SCB_CPUID_REVISION_Msk)==1)
			{
				Rx=1;
				Px=0;
			}
			else
			{
				Rx=0;
				Px=0;
			}
			break;
		case 0X00100000:
			Rx=1;
			if((this->CPUID&SCB_CPUID_REVISION_Msk)==1)
			{
				Px=1;
			}
			break;
		case 0X00200000:
			Rx=2;
			if((this->CPUID&SCB_CPUID_REVISION_Msk)==0)
			{
				Px=0;
			}
			break;
		default:
			Px=9;
			Rx=9;
			break;
	}
	
	
    printf("STD_Embedded_Team::STD0801 Code:Demo Ver:0.0.6113 Build:%s\n", __DATE__);
    printf("STDOS::%s %dMHz Flash:%dk RAM:%dk\n", CPUName->GetBuffer(),this->Clock/1000000, this->FlashSize, this->RAMSize);
    printf("DevID:0X%04X RevID:0X%04X\n", this->DevID, this->RevID);
    printf("CPUID:0X%X ARM:ARMv7-M Cortex-M3: R%dp%d\n", this->CPUID,Rx,Px);
	#if 0
    printf("Heap :(0X%X, 0X%X) = 0X%X (%dk)\n", (uint) &__heap_base, (uint) &__heap_limit, HeapSize, HeapSize / 1024);
    printf("Stack:(0X%X, 0X%X) = 0X%X (%dk)\n", (uint) &__heap_limit, (uint) &__initial_sp, StackSize, StackSize / 1024);
	#endif
    printf("ChipType:0x42455633 3\n");
    printf("ChipID:%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\n", ID[0], ID[1], ID[2], ID[3], ID[4], ID[5], ID[6], ID[7], ID[8], ID[9], ID[10], ID[11]);
    printf("Time : ");
    DateTime dt;
    dt.Show();

    printf("Support: http://www.armku.com\n");
}

#define delay_ostickspersec 1000			//ʱ��Ƶ��
byte fac_us = 0; //ÿ��us��Ҫ��systickʱ���� 

TSys Sys; //ϵͳ����
UInt64 TicksPerms = 0;

//��ʼ��
//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void TSys::Init()
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //ѡ���ⲿʱ��  HCLK/8
    fac_us = SystemCoreClock / 8000000 * 8; //Ϊϵͳʱ�ӵ�1/8 //��OS��,����ÿ��us��Ҫ��systickʱ����   
    TicksPerms = SystemCoreClock / delay_ostickspersec;
    SysTick_Config(TicksPerms); //tick is 1ms	
    #ifdef STM32F1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //�ر�jtag������swd	
	#endif

    NVIC_SetPriority(SysTick_IRQn, 0);

    #ifdef STM32F0XX
        void *p = (void*)0x1FFFF7AC;
    #else 
        void *p = (void*)0x1FFFF7E8;
    #endif 
    memcpy(ID, p, ArrayLength(ID));


    this->CPUID = SCB->CPUID;
    uint MCUID = DBGMCU->IDCODE; // MCU���롣�����豸�汾�������Ӱ汾
    this->RevID = MCUID >> 16;
    this->DevID = MCUID &0x0FFF;

    #ifdef STM32F0XX
        FlashSize = *(__IO ushort*)(0x1FFFF7CC); // ����
    #else 
        FlashSize = *(__IO ushort*)(0x1FFFF7E0); // ����
    #endif 
   switch (this->DevID)
    {
        case 0X0307:
			CPUName = new String("STM32F103RD");
            this->RAMSize = 64;
			break;
        case 0x0410:
            CPUName = new String("STM32F103C8");
            this->RAMSize = 20;
            break;
        case 0X0414:
            CPUName = new String("STM32F103ZE");
            this->RAMSize = 64;
            break;
        case 0X0418:
            CPUName = new String("STM32F105VC");
            this->RAMSize = 64;
            break;
        case 0X0430:
			CPUName = new String("STM32F103VG");
            this->RAMSize = 768;
			break;
		#ifdef STM32F4
		case 0X0413:
			CPUName = new String("STM32F407ZG");
            this->RAMSize = 192;
			break;
		#endif
		default:
            CPUName = new String("δ֪");
            this->RAMSize = 0;
			break;
    }
//    this->Inited = 1;
}

SmartIRQ::SmartIRQ(bool enable)
{    
	TInterrupt::GlobalDisable();
}

SmartIRQ::~SmartIRQ()
{
    TInterrupt::GlobalEnable();
}
