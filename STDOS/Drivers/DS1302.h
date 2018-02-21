#ifndef _DS1302_H
#define _DS1302_H

#include "Device\Port.h"
#include "DateTime.h"

class DS1302
{
public:
	DS1302();
	~DS1302(); 	
	void SetPin(Pin pinsck,Pin pinio,Pin pinrst);
	void Init();
	void SetTime(uint seconds);//����ʱ��
	void SetTime(DateTime & dt);//����ʱ��-����ʱ���ʽ
	DateTime& GetTime(DateTime & dt);//��ȡʱ��	
private:
	OutputPort psck;
	OutputPort pio;
	OutputPort prst;

	void dsReadPage(byte Addr, byte *da, byte len);
	void dsWritePage(byte Addr, byte *da, byte len);
	byte dsReadByte(byte Addr);
	void dsWriteByte(byte Addr, byte ucDa);
	void DS_Get1302(byte *ucCurtime);
	void DS_Set1302(byte *pSecDa);
	byte DS_R1302(byte ucAddr);
	void DS_W1302(byte ucAddr, byte ucDa);
	byte Byteout();
	void Bytein(byte ucDa);
	void reset1302();
	void set1302();
};
#endif // !_DS1302_H
