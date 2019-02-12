/*
	����1051��������PM2.5������������
	ͨ�Ų����� 2400 ���ɼ����:����10ms
	��������ֵΪ0���鿴���豸��Դ�����Բ��������ʱһֱ���Ϊ0�������ֽ���Դ��ֱ��ʱ������
	updata������Ҫ�Ľ�
	δ����
*/

#ifndef _SHARP_PM2_5_H_
#define _SHARP_PM2_5_H_

#include "stm32f10x.h"
#include "../../../../Bsp/Porting.h"

class HardwareSerial0_T;

class Sharp_PM_2_5
{
private:
	HardwareSerial0_T &mCom;
	float UD;//�۳�Ũ�� ��λ ug/m^3
	bool Check(u8 *Data, int Lenth, u8 CheckSum);

public:
	Sharp_PM_2_5(HardwareSerial0_T &com);
	bool Update();
	float GetConcentration();
};

#define  COEFFICIENT_A 800  //��ѹ��Ũ�ȵı���ϵ��

#endif
