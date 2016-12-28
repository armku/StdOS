/*
ϵͳ���Զ�ʶ�𳣼�оƬ���͡���Ƶ��Flash��С��Ram��С
Sys.ID ��12�ֽ�оƬΨһ��ʶ��Ҳ����ChipID��ͬһ��оƬ��ǰ�漸���ֽڲ�ͬ
���뼶˯���ڼ䣬ϵͳ���ᰲ��ִ��������ʱ�϶̵��������û�п�������ϵͳ�������͹���ģʽ���Խ�ʡ�ܺ�

 */
#include "stdio.h"
#include "TSys.h"
#include "Delay.h"
#include "Scheduling.h"

Task Scheduling; //����

TSys Sys;//ϵͳ����

TSys::TSys(uint clock, MessagePort_T messagePort)
{
    this->Clock = clock;
    this->MessagePort = messagePort;
}

void TSys::Show(bool newLine)const{

}

//��ʼ��
void TSys::Init(){

}
//����ϵͳ������ȣ��ú����ڲ�Ϊ��ѭ����*�ڴ�֮�䣬��ӵ��������������ò������ȣ�����˯�߷�����Ч��
void TSys::Start()
{
	while(true)
	{
		Scheduling.Routin();
	}	
}

//��ʾϵͳ��Ϣ
void TSys::ShowInfo()
{
    printf("STD_Embedded_Team::STD0801 Code:0801 Ver:0.0.6113 Build:2016-01-01\n");
    printf("STDOS::STM32F103C8 72MHz Flash:512k RAM:20k\n");
    printf("DevID:0x0414 RevID:0x1309\n");
    printf("CPUID:0x412fc231 ARM:ARMv7-M Cortex-M3: R1p2\n");
    printf("Heap :(0x20000720, 0x20010000) = 0xf8e0 (62k)\n");
    printf("Stack:(0x20001720, 0x20010000) = 0xe8e0 (58k)\n");
    printf("ChipType:0x42455633 3\n");
    printf("ChipID:04-53-36-35-34-31-31-31-32-30-32-34\n");
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
	delay_ms(ms);
}
//�첽������ϵͳ���ӳ�һ��������ִ�С�
void TSys::Reboot(uint msDelay){}
/*
������񣬲����ֱ��ǣ����������������״�ʱ�䡢���ʱ
�䡢���ơ�����ֵ��һ�� uint ������Ψһ��š�	
*/
uint TSys::AddTask(void(*callback)(void),void* para,uint delaycntms, uint intervalms,CString name )
{
	
	return 0;
}
//��ʱ��
uint TSys::AddTask(void(*callback)(void),void* para,uint delaycntms, uint intervalms)
{
	Scheduling.AddTask(callback,delaycntms,intervalms);
	return 0;
}
//ɾ������
void TSys::Remove(uint taskid)
{

}
