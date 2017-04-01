#ifndef __DAC_H__
#define __DAC_H__

/*
STM32F1��
����DACת������һ�����ͨ����Ӧһ��ת����
8λ/12λ�������
12λģʽ ����� �ֶ���
ͬ�����¹���
DACͬ������  �ֱ����
ÿ��ͨ������DMA����
�ⲿ�����ο�
�� Vref+ Ϊ�ο�
IO ����Ϊģ�����루AIN��
PA4  PA5
*/

class DAConverter
{
public:
	byte	Align;

	AnalogInPort* Port = nullptr;

	byte Channel;
	DAConverter(Pin pin);

	bool Open();
	bool Close();
	bool Write(ushort value);

private:
	Pin		_Pin;

	void OnInit();
	bool OnOpen();
	bool OnClose();
	bool OnWrite(ushort value);
};

#endif
