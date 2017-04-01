#ifndef _74HC165MOR_H_
#define _74HC165MOR_H_

#include "Device\Port.h"
#include "Device\Power.h"

/* ��ע
���ù��캯���� ��ҪOpen()һ��
�����Open()  ��Read ����Ҳ��Open  ����һ�ζ�ȡ����
*/
class IC74HC165MOR
{
private:
	InputPort	_PL;
	InputPort	_SCK;
	InputPort	_In;
	
	ByteArray _Bs;		// �ڲ�������
	byte 	_irqCount;	// ��¼pl�źź�ڼ�λ����
	bool	_sckHighSpeed;
	
	void OnTrigger(InputPort& port, bool down);
	void OnReaBit(InputPort& port, bool down);
public:
	bool Opened;
	// pl�ɼ���cpʱ�ӣ�Q7����� bufsize �Ǽ���������highSpeed �Ƿ��Ǹ���clk ms���������
	IC74HC165MOR(Pin pl, Pin sck, Pin in, byte bufSize = 0,bool highSpeed = true);
	
	bool Open();
	bool Close();
	
	// �ڲ��ж�ί�к���
	void Trigger();
	void ReaBit();
	
	// ������ȡ  buf,���ݷ���λ�ã�count������ic��
	byte Read(byte *buf, byte count);
	// ��һ��IC ������
	byte Read();
};

#endif
