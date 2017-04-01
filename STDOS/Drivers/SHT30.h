#ifndef _SHT30_H_
#define _SHT30_H_

#include "Device\Power.h"

class OutputPort;
class I2C;

// ��ǿ������
class SHT30 : public Power
{
public:
    I2C* IIC;		// I2Cͨ�ſ�
	byte Address;	// �豸��ַ
	byte Mode;		// ģʽ��0=CLKSTRETCH/1=POLLING/2=Periodic
	byte Freq;		// Ƶ�ʣ����������һ�Ρ�05/1/2/4/10��05��ʾ0.5s
	byte Repeat;	// �ظ��ԡ�0=��/1=��/2=�ͣ���β������࣬˵���ظ��Ը�

	OutputPort*	Pwr;	// ��Դ

    SHT30();
    virtual ~SHT30();

	void Init();
	uint ReadSerialNumber() const;
	ushort ReadStatus() const;
	bool Read(ushort& temp, ushort& humi);

	// ��Դ�ȼ�����������͹���ģʽ��ʱ����
	virtual void ChangePower(int level);

private:
	bool Write(ushort cmd);
	ushort Read2(ushort cmd) const;
	// ͬʱ��ȡ��ʪ�Ȳ�У��Crc
	uint Read4(ushort cmd) const;

	bool CheckStatus();
	void SetMode();
	ushort GetMode() const;
};

/*
������ʷ

2015-10-03
SHT30���ֲɼ����ݷ�ʽ��
1��Stretch����ģʽ�����������ɼ�����Ҫ��ʱ��ȴ�SCL���ߣ����ܷ��Ͷ�ȡͷȻ���ȡ����
2��Polling������ģʽ�����������ɼ�����Ҫ����������������Ͷ�ȡͷ���õ�ACK�Ժ���ܶ�ȡ����
3���ڲ����ڲɼ�ģʽ������ʱ����Periodic�����ȡʱ����FetchData�����ֱ�Ӷ�ȡ����
��лkazuyuki@407605899

*/

#endif
