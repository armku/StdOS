#ifndef _BH1750_H_
#define _BH1750_H_

#include "Device\Power.h"

class I2C;

// ��ǿ������
class BH1750 : public Power
{
public:
    I2C*	IIC;		// I2Cͨ�ſ�
	byte	Address;	// �豸��ַ

    BH1750();
    virtual ~BH1750();

	void Init();
	ushort Read();

	// ��Դ�ȼ�����������͹���ģʽ��ʱ����
	virtual void ChangePower(int level);

private:
	void Write(byte cmd);
};

#endif
