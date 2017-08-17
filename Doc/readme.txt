GPIO�Ĵ������� ��STM32�ο��ֲ�����-p75��

1���˿����õͼĴ���(GPIOx_CRL)(x = A...E)
2���˿����ø߼Ĵ���(GPIOx_CRH)(x = A...E)

3���˿��������ݼĴ���(GPIOx_IDR)(x = A...E)
   ��ЩλΪֻ����ֻ������(16λ)����ʽ������������ֵΪ��ӦI/O�ڵ�״̬��
   
4���˿�������ݼĴ���(GPIOx_ODR)(x = A...E)
   ֻ������(16bit)����ʽ��������λֵȫ��0��д0�����0��д1�����1��
   
5���˿�λ����/����Ĵ���(GPIOx_BSRR)(x = A...E)
   ��16bitд1������0����16bitд1������λ��ͬʱд1�Ļ���16bi��Ч��
   
6���˿�λ����Ĵ���(GPIOx_BRR)(x = A...E)
   ��16λд1������λ��
   
7���˿����������Ĵ���(GPIOx_LCKR)(x = A...E)

GPIO����ģʽ
typedef enum
{ GPIO_Mode_AIN = 0x0,            /* ģ������ */   
  GPIO_Mode_IN_FLOATING = 0x04,   /* �������룬��λ���״̬ */
  GPIO_Mode_IPD = 0x28,           /* �������� */
  GPIO_Mode_IPU = 0x48,           /* �������� */
  GPIO_Mode_Out_OD = 0x14,        /* ��©��� */
  GPIO_Mode_Out_PP = 0x10,        /* ������� */
  GPIO_Mode_AF_OD = 0x1C,         /* ���ÿ�©��� */
  GPIO_Mode_AF_PP = 0x18          /* ����������� */
}GPIOMode_TypeDef;

����DLL:TARMSTM.DLL 
���Բ�����-pSTM32F103C8

�����ļ�
ld.s������С���� ��Ʒ��
md.s�������е�������Ʒ��
hd�����ڴ�������Ʒ��
�����������ָFLASH�Ĵ�С.�жϷ������£�
С������FLASH��32K
��������64K��FLASH��128K
��������256K��FLASH
hal hardware abstraction layer Ӳ���й�


#include "Sys.h"
#include "Device\Port.h"
//#include "Device\SerialPort.h"

void LedTask(void* param)
{
    auto leds	= (OutputPort*)param;
    *leds = !*leds;
}

void OnPress(InputPort& port, bool down)
{
    debug_printf("Press P%c%d down=%d\r\n", _PIN_NAME(port._Pin), down);
}

int main(void)
{
    // ��ʼ��ϵͳ
    //Sys.Clock = 72000000;
    //Sys.MessagePort = COM1;
    Sys.Init();
    //Sys.InitClock();
    Sys.ShowInfo();

    // ��ʼ��Ϊ���
	OutputPort leds[] = {PD0, PD1};
	for(int i=0; i<ArrayLength(leds); i++)
		leds[i].Invert = true;

    // ��ʼ��Ϊ�ж�����
    Pin ips[] = { PE9, PE8, PE4, PE5, PE6, PE7};
    InputPort btns[6];
	for(int i=0; i<ArrayLength(btns); i++)
	{
		btns[i].Index = i;
		btns[i].Set(ips[i]);
		btns[i].Press = OnPress;
		btns[i].UsePress();
		btns[i].Open();
	}

	Sys.AddTask(LedTask, &leds[0], 500, 500, "Led��˸");

	/*Buffer::Test();
	Array::Test();
	String::Test();
	DateTime::Test();
	IList::Test();
	IDictionary::Test();*/
	/*OutputPort power(PE2);
	power = true;
	SerialPort::Test();*/

    Sys.Start();
}
