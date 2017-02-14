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
#include <string.h>
#include "Array.h"
#include "DateTime.h"

//�ⲿע�ắ��
// ����
#include "Task.h"
// ������
TaskScheduler *_Scheduler;

// �������񣬷��������š�priority���ȼ���dueTime�״ε���ʱ��us��period���ȼ��us��-1��ʾ������һ��
uint TSys::AddTask(Action func, void *param, ulong dueTime, long period, const char *name)
{
    // �����жϣ�����������̳߳�ͻ
    SmartIRQ irq;

    if (!_Scheduler)
    {
        _Scheduler = new TaskScheduler("Sys");
    }

    return _Scheduler->Add(func, param, dueTime, period, name);
}

void TSys::RemoveTask(uint taskid)
{
    #if 0
        assert_ptr(_Scheduler);
    #endif 
    _Scheduler->Remove(taskid);
}

void TSys::SetTask(uint taskid, bool enable)
{
    Task *task = (*_Scheduler)[taskid];
    if (task)
    {
        task->Enable = enable;
    }
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

void TSys::StartInternal()
{
    _Scheduler->Start();
}

void TSys::Stop()
{
    _Scheduler->Stop();
}


void TimeSleep(uint us)
{
    // �����ʱ�����棬ȥ����һ�±������
    if (_Scheduler && (us >= 1000))
    {
        // ��¼��ǰ����ִ������
        Task *task = _Scheduler->Current;

        ulong start = Time.Current();
        // 1msһ�㲻����������������Ӳ���ȴ�
        ulong end = start + us - 1000;
        // �������ʱ���㹻���������ε�����������
        int cost = 0;
        while (true)
        {
            ulong start2 = Time.Current();

            _Scheduler->Execute(us);

            ulong now = Time.Current();
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

//���뼶˯�ߣ�������ҵ��������ȴ�һ��ʱ��
void TSys::Sleep(uint ms)
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

//΢��ӳ٣������ڸ߾��������źſ���
void TSys::Delay(uint us)
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

TSys::TSys(uint clock, COM_Def messagePort)
{
    this->Clock = clock;
    this->MessagePort = messagePort;
}

void TSys::Show(bool newLine)const{

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

//�첽������ϵͳ���ӳ�һ��������ִ�С�
void TSys::Reboot(uint msDelay){}
//��ʾϵͳ��Ϣ
void TSys::ShowInfo()
{
    printf("STD_Embedded_Team::STD0801 Code:Demo Ver:0.0.6113 Build:%s\n",__DATE__);
    printf("STDOS::%s 72MHz Flash:%dk RAM:%dk\n", this->CPUName->GetBuffer(), this->FlashSize, this->RamSize);
    printf("DevID:0X%04X RevID:0X%04X\n", this->DevID, this->RevID);
    printf("CPUID:0X%X ARM:ARMv7-M Cortex-M3: R1p2\n", this->CPUID);
    printf("Heap :(0x20000720, 0x20010000) = 0xf8e0 (62k)\n");
    printf("Stack:(0x20001720, 0x20010000) = 0xe8e0 (58k)\n");
    printf("ChipType:0x42455633 3\n");
    printf("ChipID:%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\n", ID[0], ID[1], ID[2], ID[3], ID[4], ID[5], ID[6], ID[7], ID[8], ID[9], ID[10], ID[11]);
    printf("Time : ");
    DateTime dt;
    dt.Show();

    printf("Support: http://www.armku.com\n");
}

#define delay_ostickspersec 1000			//ʱ��Ƶ��
static byte fac_us = 0; //us��ʱ������
//static uint16_t fac_ms = 0;							//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��

TSys Sys; //ϵͳ����
TTime Time; //ϵͳʱ�䣬�������û�ֱ��ʹ��

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

    this->_Index = 0;

    #ifdef STM32F0XX
        FlashSize = *(__IO ushort*)(0x1FFFF7CC); // ����
    #else 
        FlashSize = *(__IO ushort*)(0x1FFFF7E0); // ����
    #endif 
    this->Initjs();
    this->Inited = 1;
}

//����ram���ͺŵ�
void TSys::Initjs()
{    
    switch (this->DevID)
    {
        case 0x0410:
            this->CPUName=new String("STM32F103C8");
			this->RamSize=20;
            break;
		case 0X0414:
            this->CPUName=new String("STM32F103ZE");
			this->RamSize=64;
            break;
		default:
			this->CPUName=new String("δ֪");
			break;
    }
}

#ifdef __cplusplus
    extern "C"
    {
    #endif 

    //systick�жϷ�����,ʹ��ucosʱ�õ�
    void SysTick_Handler(void)
    {
        Time.mCurrent++;
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
#if 0

    extern uint __heap_base;
    extern uint __heap_limit;

    void *operator new(uint size)
    {
        debug_printf(" new size: %d ", size);
        void *p = malloc(size);
        if (!p)
            debug_printf("malloc failed! size=%d ", size);
        else
        {
            debug_printf("0x%08x ", p);
            // �����ֻʣ��64�ֽڣ��򱨸�ʧ�ܣ�Ҫ���û�����ѿռ����ⲻ��
            uint end = (uint) &__heap_limit;
            if ((uint)p + size + 0x40 >= end)
                debug_printf(" + %d near HeapEnd=0x%08x", size, end);
        }
        assert_param(p);
        return p;
    }

    void *operator new[](uint size)
    {
        debug_printf(" new size[]: %d ", size);
        void *p = malloc(size);
        if (!p)
            debug_printf("malloc failed! size=%d ", size);
        else
        {
            debug_printf("0x%08x ", p);
            // �����ֻʣ��64�ֽڣ��򱨸�ʧ�ܣ�Ҫ���û�����ѿռ����ⲻ��
            uint end = (uint) &__heap_limit;
            if ((uint)p + size + 0x40 >= end)
                debug_printf(" + %d near HeapEnd=0x%08x", size, end);
        }
        assert_param(p);
        return p;
    }

    void operator delete (void *p)
    {
        debug_printf(" delete 0x%08x ", p);
        if (p)
            free(p);
    }

    void operator delete [](void *p)
    {
        debug_printf(" delete[] 0x%08x ", p);
        if (p)
            free(p);
    }
#endif
