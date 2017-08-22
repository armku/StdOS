/*
ϵͳ���Զ�ʶ�𳣼�оƬ���͡���Ƶ��Flash��С��Ram��С
Sys.ID ��12�ֽ�оƬΨһ��ʶ��Ҳ����ChipID��ͬһ��оƬ��ǰ�漸���ֽڲ�ͬ
���뼶˯���ڼ䣬ϵͳ���ᰲ��ִ��������ʱ�϶̵��������û�п�������ϵͳ�������͹���ģʽ���Խ�ʡ�ܺ�

 */
#include "TTime.h"
#include "Task.h"
#include <string.h>
#include "Array.h"
#include "TInterrupt.h"
#include "SerialPort.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif
TSys Sys; //ϵͳ����
extern "C"
{
    extern uint __heap_base;
    extern uint __heap_limit;
    extern uint __initial_sp;
}

String *CPUName;





//�ⲿע�ắ��
// ����
// ������
TaskScheduler *_Scheduler;
void TimeSleep(uint us)
{
    // �����ʱ�����棬ȥ����һ�±������
    if (_Scheduler && (us >= 1000))
    {
        // ��¼��ǰ����ִ������
        Task *task = _Scheduler->Current;

        UInt64 start = Time.Current()*1000;
        // 1msһ�㲻����������������Ӳ���ȴ�
        UInt64 end = start + us - 1000;
        // �������ʱ���㹻���������ε�����������
        int cost = 0;
        while (true)
        {
            UInt64 start2 = Time.Current()*1000;
            bool bb = false;
            _Scheduler->Execute(us, bb);

            UInt64 now = Time.Current()*1000;
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

        cost = (int)(Time.Current()*1000 - start);
        if (cost > 0)
        {
            return ;
        }

        us -= cost;
    }
    if (us)
    {
        Time.Delay(us);
    }
}

uint Get_JTAG_ID(void)
{
  uint result;

//  if ( vE00FFFE8 & 8 )
//    result = (vE00FFFD0 << 8) & 0xFFF | ((uint)vE00FFFE4 >> 3) | (32 * (vE00FFFE8 & 7) + 1);
//  else
    result = 0;
  return result;
}
int SmartOS_Log(const String *a1)
{
  const Buffer *v1; 
  int result; 
  SerialPort* v3; 

  v1 = (const Buffer *)a1;
  if (Sys.Clock  && Sys.MessagePort !=COM_NONE)
  {
    v3 = SerialPort::GetMessagePort();
//    if ( v3 && v3->Remap)
//      result = v3->Write(v1);
//    else
      result = 0;
  }
  else
  {
    result = 0;
  }
  return result;
}
void ExitCritical(void)
{
//  enable_irq();
}
void EnterCritical(void)
{
//  disable_irq();
}
//uint _REV(uint a1)
//{
//  return rev(a1);
//}

int REV16(ushort a1)
{
  return (ushort)REV16(a1);
}

// ���캯��
TSys::TSys()
{
	#ifdef STM32F0
		this->CystalClock=8;
		this->Clock = 48;
	#elif defined STM32F1
		this->CystalClock=8;
		this->Clock = 72;
	#elif defined STM32F4
		this->CystalClock=8;
		this->Clock = 168;
	#endif
    this->MessagePort = COM1;
	
	Interrupt.Init();
}
// ��ʼ��ϵͳʱ��
void TSys::InitClock()
{
//	TSys *v1; // r4@1
//  int result; // r0@2
//  int v3; // r0@3

//  v1 = this;
//  if ( *((_DWORD *)this + 1) != RCC_GetSysClock(this) || (result = *((_DWORD *)v1 + 2), result != HSE_VALUE) )
//  {
//    v3 = SetSysClock(*(_QWORD *)((char *)v1 + 4), *(_QWORD *)((char *)v1 + 4) >> 32);
//    *((_DWORD *)v1 + 1) = RCC_GetSysClock(v3);
//    result = *((_DWORD *)v1 + 1);
//    SystemCoreClock = *((_DWORD *)v1 + 1);
//  }
//  return result;
}	
// ��ʼ��ϵͳ
void TSys::Init()
{       
    #ifdef STM32F0
        void *p = (void*)0x1FFFF7AC;
    #elif defined STM32F1 
        void *p = (void*)0x1FFFF7E8;
    #elif defined STM32F4 
        void *p = (void*)0x1FFFF7E8;
    #endif 
    memcpy(ID, p, ArrayLength(ID));

    this->CPUID = SCB->CPUID;
    uint MCUID = DBGMCU->IDCODE; // MCU���롣�����豸�汾�������Ӱ汾
    this->RevID = MCUID >> 16;
    this->DevID = MCUID &0x0FFF;

    #ifdef STM32F0
        FlashSize = *(__IO ushort*)(0x1FFFF7CC); // ����
    #elif defined STM32F1 
        FlashSize = *(__IO ushort*)(0x1FFFF7E0); // ����
    #elif defined STM32F4 
        FlashSize = *(__IO ushort*)(0x1FFFF7E0); // ����
    #endif 
    switch (this->DevID)
    {
		#ifdef STM32F0
			case 0X0448:
				CPUName = new String("STM32F072VB");
                this->RAMSize = 64;
                break;
        #elif defined STM32F1
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
         #elif defined STM32F4
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
	this->InitClock();
	Time.Init();
}		
void TSys::ShowInfo() const
{
	this->OnShowInfo();
}	
uint TSys::HeapBase() const	// ����ʼ��ַ��ǰ���Ǿ�̬�����ڴ�
{
//	return &_heap_base;
}	
uint TSys::StackTop() const	// ջ���������ǳ�ʼ������������
{
	//return (*((_WORD *)this + 31) << 10) + 536870656;
}	
void TSys::SetStackTop(uint addr)
{
//	return _set_MSP(addr);
}	

// ϵͳ������ĺ�����
UInt64 TSys::Ms()const  
{
    return Time.Current();
}	

// ϵͳ���Ե�ǰʱ�䣬��
uint TSys::Seconds()const  
{
    return Time.Seconds;
}

// ���뼶�ӳ�
void TSys::Sleep(int ms)const  
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
// ΢�뼶�ӳ�
void TSys::Delay(int us)const  
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
bool TSys::CheckMemory() const
{
	return true;
}	

// �ӳ��첽����
void TSys::Reboot(int msDelay)const
{
    this->Reset();
}
// ϵͳ����
void TSys::InitTrace(void* port) const
{
	
}	
void TSys::Trace(int times) const
{
	
}	


// ����ϵͳ
void TSys::Reset() const
{
	NVIC_SystemReset();
//	vE000ED0C = (vE000ED0C & 0x700 | 0x5FA0000) + 4;
//  __dsb();
//  while ( 1 )
//    ;
}
void TSys::OnInit()
{
//	TSys *v1; // r4@1
//  int v2; // r6@1
//  int v3; // r3@1
//  int v4; // r5@1
//  signed int i; // r0@7
//  int v6; // r8@13
//  void *v7; // r0@13
//  int v8; // r11@13
//  int v9; // r3@13
//  int result; // r0@13

//  v1 = this;
//  *((_DWORD *)this + 1) = 72000000;
//  *((_DWORD *)this + 2) = HSE_VALUE;
//  *(_BYTE *)this = 0;
//  v2 = Get_JTAG_ID() == 1955;
//  Buffer::Copy((TSys *)((char *)v1 + 40), (void *)0x1FFFF7E8, &loc_C, v3);
//  *((_DWORD *)v1 + 14) = vE000ED00;
//  v4 = vE0042000;
//  if ( !vE0042000 && v2 )
//    v4 = vE0042000;
//  *((_WORD *)v1 + 27) = HIWORD(v4);
//  *((_WORD *)v1 + 26) = v4 & 0xFFF;
//  if ( v2 )
//    *((_DWORD *)v1 + 1) = 120000000;
//  Index = 0;
//  *((_WORD *)v1 + 30) = v1FFFF7E0;
//  if ( *((_WORD *)v1 + 30) != 0xFFFF )
//  {
//    *((_WORD *)v1 + 31) = (signed int)*((_WORD *)v1 + 30) >> 3;
//    for ( i = 0; i < 11; ++i )
//    {
//      if ( MemSizes[i] == *((_WORD *)&Sys + 30) )
//      {
//        Index = i;
//        break;
//      }
//    }
//    *((_WORD *)v1 + 31) = RamSizes[2 * Index];
//  }
//  v6 = TSys::StackTop(v1);
//  v7 = (void *)_get_MSP();
//  v8 = v6 - (&_initial_sp - (_UNKNOWN *)v7);
//  Buffer::Copy((Buffer *)(v6 - (&_initial_sp - (_UNKNOWN *)v7)), v7, (const void *)(&_initial_sp - (_UNKNOWN *)v7), v9);
//  _set_MSP(v8);
//  v40021018 |= 1u;
//  result = v40010004 | 0x2000000;
//  v40010004 |= 0x2000000u;
//  return result;
}	
void TSys::OnShowInfo()const
{
    uint Rx = 0;
    uint Px = 0;

    uint HeapSize = 0;
    uint StackSize = 0;


    HeapSize = ((uint) &__heap_limit - (uint) &__heap_base);
    StackSize = ((uint) &__initial_sp - (uint) &__heap_limit);

    switch (this->CPUID &SCB_CPUID_VARIANT_Msk)
    {
        case 0:
            if ((this->CPUID &SCB_CPUID_REVISION_Msk) == 1)
            {
                Rx = 1;
                Px = 0;
            }
            else
            {
                Rx = 0;
                Px = 0;
            }
            break;
        case 0X00100000:
            Rx = 1;
            if ((this->CPUID &SCB_CPUID_REVISION_Msk) == 1)
            {
                Px = 1;
            }
            break;
        case 0X00200000:
            Rx = 2;
            if ((this->CPUID &SCB_CPUID_REVISION_Msk) == 0)
            {
                Px = 0;
            }
            break;
        default:
            Px = 9;
            Rx = 9;
            break;
    }


    printf("STD_Embedded_Team::STD0801 Code:Demo Ver:0.0.6113 Build:%s\n", __DATE__);
    printf("STDOS::%s %dMHz Flash:%dk RAM:%dk\n", CPUName->GetBuffer(), this->Clock, this->FlashSize, this->RAMSize);
    printf("DevID:0X%04X RevID:0X%04X\n", this->DevID, this->RevID);
    printf("CPUID:0X%X ARM:ARMv7-M Cortex-M3: R%dp%d\n", this->CPUID, Rx, Px);
    printf("Heap :(0X%X, 0X%X) = 0X%X (%dk)\n", (uint) &__heap_base, (uint) &__heap_limit, HeapSize, HeapSize / 1024);
    printf("Stack:(0X%X, 0X%X) = 0X%X (%dk)\n", (uint) &__heap_limit, (uint) &__initial_sp, StackSize, StackSize / 1024);
    printf("ChipType:0x42455633 3\n");
    printf("ChipID:%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\n", ID[0], ID[1], ID[2], ID[3], ID[4], ID[5], ID[6], ID[7], ID[8], ID[9], ID[10], ID[11]);
    printf("Time : ");
    DateTime dt;
    dt.Show();

    printf("Support: http://www.armku.com\n");
	
	
	
	
	/////////////////////
//	TSys *v1; // r4@1
//  unsigned __int8 *v2; // r5@4
//  int v3; // r1@18
//  int v4; // r1@30
//  int v5; // r0@30
//  int v6; // r3@30
//  int v7; // r2@30
//  int v8; // r2@30
//  int v9; // r1@30
//  int v10; // r1@30
//  int v11; // r1@37
//  int v12; // r0@38
//  int v13; // r1@38
//  int v14; // r7@38

//  v1 = this;
//  SmartOS_printf("SmartOS::");
//  if ( Get_JTAG_ID() == 1955 )
//    SmartOS_printf("GD32");
//  else
//    SmartOS_printf("STM32");
//  v2 = (unsigned __int8 *)v1 + 56;
//  if ( (signed int)*((_WORD *)v1 + 26) <= 0 )
//  {
//    if ( *((_DWORD *)v1 + 14) )
//    {
//      if ( *((_DWORD *)v1 + 1) == 48000000 )
//        SmartOS_printf("F130/F150");
//      else
//        SmartOS_printf("F103");
//    }
//  }
//  else if ( *((_WORD *)v1 + 26) )
//  {
//    if ( *((_WORD *)v1 + 26) != 1040
//      && *((_WORD *)v1 + 26) != 1042
//      && *((_WORD *)v1 + 26) != 1044
//      && *((_WORD *)v1 + 26) != 1072 )
//    {
//      if ( *((_WORD *)v1 + 26) == 1048 )
//      {
//        SmartOS_printf("F107");
//      }
//      else if ( *((_WORD *)v1 + 26) == 1042 )
//      {
//        SmartOS_printf("F130");
//      }
//      else if ( *((_WORD *)v1 + 26) != 1088 && *((_WORD *)v1 + 26) != 1092 )
//      {
//        v3 = *((_WORD *)v1 + 26);
//        SmartOS_printf("F%03x");
//      }
//      else
//      {
//        SmartOS_printf("F030/F051");
//      }
//    }
//    else
//    {
//      SmartOS_printf("F103");
//    }
//  }
//  if ( Index >= 2 )
//  {
//    if ( Index >= 4 )
//    {
//      if ( Index >= 6 )
//        SmartOS_printf(&dword_430);
//      else
//        SmartOS_printf(&dword_42C);
//    }
//    else
//    {
//      SmartOS_printf(&dword_428);
//    }
//  }
//  else
//  {
//    SmartOS_printf(&dword_424);
//  }
//  v4 = (unsigned __int8)MemNames[Index];
//  v5 = SmartOS_printf(&dword_438);
//  RCC_GetSysClock(v5);
//  v6 = *((_WORD *)v1 + 31);
//  v7 = *((_WORD *)v1 + 30);
//  SmartOS_printf(" %dMHz Flash:%dk RAM:%dk\r\n");
//  v8 = *((_WORD *)v1 + 27);
//  v9 = *((_WORD *)v1 + 26);
//  SmartOS_printf("DevID:0x%04X RevID:0x%04X \r\n");
//  v10 = *((_DWORD *)v1 + 14);
//  SmartOS_printf("CPUID:%p");
//  if ( *((_BYTE *)v1 + 59) == 65 )
//    SmartOS_printf(" ARM:");
//  if ( (*((_BYTE *)v1 + 58) & 0xF) == 12 )
//  {
//    SmartOS_printf(" ARMv6-M");
//  }
//  else if ( (*((_BYTE *)v1 + 58) & 0xF) == 15 )
//  {
//    SmartOS_printf(" ARMv7-M");
//  }
//  if ( (((unsigned int)*(_WORD *)v2 >> 4) & 0xFF0) == 3104 )
//  {
//    v11 = *v2 >> 4;
//    SmartOS_printf(" Cortex-M%d:");
//  }
//  v12 = *((_BYTE *)v1 + 58);
//  v13 = *v2 & 0xF;
//  SmartOS_printf(" R%dp%d");
//  SmartOS_printf("\r\n");
//  TSys::HeapBase(v1);
//  v14 = (*((_WORD *)v1 + 31) << 10) + 0x20000000;
//  SmartOS_printf("Heap :(%p, %p) = 0x%x (%dk)\r\n");
//  return SmartOS_printf("Stack:(%p, %p) = 0x%x (%dk)\r\n");
}
	
void TSys::OnStart()
{
	
}	

// �������񣬷��������š�dueTime�״ε���ʱ��ms��period���ȼ��ms��-1��ʾ������һ��
uint TSys::AddTask(Action func, void *param, int dueTime, int period, cstring name)const
{
    // �����жϣ�����������̳߳�ͻ
    SmartIRQ irq;

    if (!_Scheduler)
    {
        _Scheduler = new TaskScheduler("Sys");
    }

    return _Scheduler->Add(func, param, dueTime, period, name);
}
void TSys::RemoveTask(uint &taskid)const
{
    #if 0
        assert_ptr(_Scheduler);
    #endif 
    _Scheduler->Remove(taskid);
}
// ��������Ŀ���״̬��ͬʱ����ָ���������һ�ε��ȵ�ʱ�䣬0��ʾ���ϵ���
bool TSys::SetTask(uint taskid, bool enable, int msNextTime)const
{
    //    _Scheduler->SetTask(taskid, enable, msNextTime);
    return true;
}

// �ı���������
bool TSys::SetTaskPeriod(uint taskid, int period) const
{
	return false;
}
// ��ʼϵͳ��ѭ��
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



























/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
uint _REV(uint value)
{
	return value;
}
ushort _REV16(ushort value)
{
	return value;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
SmartIRQ::SmartIRQ(bool enable)
{
    TInterrupt::GlobalDisable();
}

SmartIRQ::~SmartIRQ()
{
    TInterrupt::GlobalEnable();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#if DEBUG
    // ����ջ��
    // ���뺯��ʱѹջ���������뿪ʱ�����������쳣ʱ��ȡ���̵߳����б�
    TraceStack::TraceStack(cstring name){}
    TraceStack::~TraceStack(){}

    void TraceStack::Show(){}

#endif
