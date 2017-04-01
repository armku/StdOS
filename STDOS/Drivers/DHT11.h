#ifndef _DHT11_H_
#define _DHT11_H_

#include "Device\Port.h"
#include "Device\Power.h"

// ��ʪ�ȴ�����
class DHT11
{
public:
	OutputPort DA;	// ��������

    DHT11(Pin da);
    ~DHT11();

	void Init();
	bool Read(ushort& temp, ushort& humi);

	// ��Դ�ȼ�����������͹���ģʽ��ʱ����
	virtual void ChangePower(int level);

private:
	byte ReadByte();
};

#endif
