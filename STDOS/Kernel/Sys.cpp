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


int SmartOS_printf(const char *format, ...);

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
	
	//this->Config=g_config;
	this->OnInit();
	this->MessagePort=COM1;
	this->Name="stdos";
	this->Company="armku";
	this->Code=0x0201;
	this->Ver=0x00;
	this->ID[0]=0;
	this->DevID=0x00;
	this->RevID=0x00;
	this->CPUID=0x00;	
	
	Interrupt.Init();
	this->FlashSize=0x01;
	this->RAMSize=0x01;
	this->OnSleep=NULL;
}

// ��ʼ��ϵͳ
void TSys::Init()
{    		
	this->InitClock();
	Time.Init();
}		
void TSys::ShowInfo() const
{
	printf("%s::%s Code:%04X %s \r\n","stdos","std",12,"222");
    printf("Build:%s %s\r\n","armku","yyyy-MM-dd HH:mm:ss");
    
	this->OnShowInfo();
	printf("ChipID:%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\r\n", ID[0], ID[1], ID[2], ID[3], ID[4], ID[5], ID[6], ID[7], ID[8], ID[9], ID[10], ID[11]);
    printf("Time : ");
    DateTime dt(Time.Seconds+Time.BaseSeconds);
    dt.Show(true);

    printf("Support: http://www.armku.com\r\n");
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
	
	 //return Time + dword_6A0;
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


// �ӳ��첽����
void TSys::Reboot(int msDelay)const
{
	if(msDelay<=0)
		this->Reset();
    //Sys.AddTask((void *)TSys::Reset,(void *)this,msDelay,-1,"");
}
// ϵͳ����
void TSys::InitTrace(void* port) const
{
	//*(_DWORD *)trace = a2;
}	
void TSys::Trace(int times) const
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
	this->OnStart();
	if (!_Scheduler)
	_Scheduler=new TaskScheduler("Sys"); 
	_Scheduler->Start();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
SmartIRQ::SmartIRQ(bool enable)
{
    _state = __get_PRIMASK();
    if (enable)
        __enable_irq();
    else
        __disable_irq();
}

SmartIRQ::~SmartIRQ()
{
    __set_PRIMASK(_state);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
extern SerialPort *printf_sp;
extern "C"
{
    /* ����fputc�������û�����ʹ��printf���� */
    int fputc(int ch, FILE *f)
    {
		static bool isInFPutc; //���ڴ������
        //        if (!Sys.Inited)
        //            return ch;

        if (Sys.MessagePort == COM_NONE)
            return ch;

        if (isInFPutc)
            return ch;
        isInFPutc = true;

        // ��鲢�򿪴���
        //if ((port->CR1 &USART_CR1_UE) != USART_CR1_UE && printf_sp == NULL)
        if (printf_sp == NULL)
        {
            printf_sp = new SerialPort(COM(Sys.MessagePort));
            printf_sp->Open();
        }
        printf_sp->SendData((byte)ch);
        isInFPutc = false;
        return ch;
    }
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if DEBUG
    // ����ջ��
    // ���뺯��ʱѹջ���������뿪ʱ�����������쳣ʱ��ȡ���̵߳����б�
    TraceStack::TraceStack(cstring name){}
    TraceStack::~TraceStack(){}

    void TraceStack::Show(){}

#endif
