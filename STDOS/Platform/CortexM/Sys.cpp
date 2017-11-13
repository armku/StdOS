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

String *CPUName;
int SmartOS_Log(const String *str)
{
    int ret;

    if (Sys.Clock && Sys.MessagePort != COM_NONE)
    {
        auto sp = SerialPort::GetMessagePort();
        if (sp && sp->ByteTime)
            ret = sp->Write(*str);
        else
            ret = 0;
    }
    else
    {
        ret = 0;
    }
    return ret;
}

void TSys::SetStackTop(uint addr)
{
	//return __set_MSP(addr);
}	

void TSys::OnStart()
{
	
}	
// ��ʼ��ϵͳʱ��
void TSys::InitClock()
{	    
    //    this->Inited = 1;
}	

bool TSys::CheckMemory() const
{
	return true;
}		

uint TSys::HeapBase() const	// ����ʼ��ַ��ǰ���Ǿ�̬�����ڴ�
{
//	return &_heap_base;
	return 0;
}	
uint TSys::StackTop() const	// ջ���������ǳ�ʼ������������
{
	//return (*((_WORD *)this + 31) << 10) + 536870656;
	return 0;
}	
