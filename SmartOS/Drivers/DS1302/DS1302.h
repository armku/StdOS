#ifndef _DS1302_H
#define _DS1302_H

#include "OnChip\Port.h"
#include "DateTime.h"

class DS1302
{
public:
	DS1302();
	~DS1302(); 	
	void SetPin(Pin pinsck,Pin pinio,Pin pinrst);
	void Init();
	void SetTime(uint32_t seconds);//设置时间
	void SetTime(DateTime & dt);//设置时间-北京时间格式
	DateTime& GetTime(DateTime & dt);//读取时间	
private:
	Port psck;
	Port pio;
	Port prst;

	void dsReadPage(uint8_t Addr, uint8_t *da, uint8_t len);
	void dsWritePage(uint8_t Addr, uint8_t *da, uint8_t len);
	uint8_t dsReadByte(uint8_t Addr);
	void dsWriteByte(uint8_t Addr, uint8_t ucDa);
	void DS_Get1302(uint8_t *ucCurtime);
	void DS_Set1302(uint8_t *pSecDa);
	uint8_t DS_R1302(uint8_t ucAddr);
	void DS_W1302(uint8_t ucAddr, uint8_t ucDa);
	uint8_t Byteout();
	void Bytein(uint8_t ucDa);
	void reset1302();
	void set1302();
};
#endif // !_DS1302_H
