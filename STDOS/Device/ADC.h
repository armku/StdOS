#ifndef __ADC_H__
#define __ADC_H__

#include "Device\Port.h"

/*
Analog-to-digital converter
�ù���ͨ��  ʹ��DMAʵ������Ǩ��

STM32F103�ڲ�ADC
	3�� 12λ ���αƽ���ģ������ת������ADC����   ����
	18��ͨ�����ֱ�Ϊ16���ⲿ16· �� 2���ڲ��źţ��¶Ⱥ�PVD��
	����ͨ����A/Dת�������ǵ��Ρ�������ɨ������Ǽ��ģʽִ��
	ת������������ö����ʽ  �����/�Ҷ���

	PVDģ�⿴�Ź���������Ӧ�ó����������ѹ�Ƿ�ϸ�
	Port
*/

class ADConverter
{
private:
	void*	_ADC;	// ��ǰ�ж��ߵ�����
	Pin		_Pins[16];
	AnalogInPort	_ports[16]; 
	byte	_PinCount;
public :
	byte	Line;		// �ж��� 1/2/3
	byte	Count;		// ͨ������
	int		Channel;	// ʹ����Щͨ����ÿ��ͨ��һλ
	ushort	Data[19];	// �������

	// �����Ҫ��ȡ�¶Ⱥ͵�ѹ��ͨ����Ҫ����0x30000
	ADConverter(byte line = 1, int channel = 0);

	void Add(Pin pin);
	void Remove(Pin pin);
	void Open();
	ushort Read(Pin pin);
	ushort ReadTempSensor();
	ushort ReadVrefint();
	ushort ReadVbat();
	
private:
	void OnInit();
	void OnOpen();
};
#endif
