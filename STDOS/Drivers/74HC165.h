#ifndef _74HC165_H_
#define _74HC165_H_

#include "Device\Port.h"
#include "Device\Power.h"

// ��ʪ�ȴ�����
class IC74HC165
{
private:
	OutputPort	_PL;
	OutputPort	_SCK;
	InputPort	_In;
	OutputPort _CE;
	
public:
	bool Opened;
	// pl�ɼ���cpʱ�ӣ�Q7�����ceʹ��
	IC74HC165(Pin pl, Pin sck, Pin in, Pin ce = P0);
	
	bool Open();
	bool Close();
	
	// ������ȡ  buf,���ݷ���λ�ã�count������ic��
	bool Read(byte *buf, byte count);
	// ��һ��IC ������
	byte Read();
};

#endif
