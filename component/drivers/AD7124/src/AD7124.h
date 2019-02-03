#ifndef __AD7124_H
#define __AD7124_H


#include "../../../../Bsp/Porting.h"

#define AD7124SPISOFT 1	
#define AD7124_BUFSIZE 6

class AD7124
{
public:
#if AD7124SPISOFT
	AD7124(SpiSoft *spi);
#else 
	AD7124(Spi *spi);
#endif 
	uint16_t CHCONFIG[16];//通道配置

	void Init_8();		//8通道初始化
	void Init_4();		//4通道初始化
	uint8_t ReadID();		//读取ID
	uint8_t ReadStatus();	//读取状态
	void Reset();		//复位
	uint32_t ReadRlt();		//读取AD值
	uint32_t ReadRlt(uint8_t& status);		//读取AD值
	uint8_t Status;		//器件状态

	int Read(void); //返回-1,表示转换未完成
	bool GetFlag(void); //读取AD转换是否正常
	void WriteReg(uint8_t reg, uint8_t bytes, uint32_t data);
	void SetReadChannel(uint8_t ch = 0, uint8_t chMax = 7);//设置需要读取的通道，默认通道0
private:
	uint16_t Write16(uint16_t sendData);
	uint32_t Write24(uint32_t sendData);
	uint32_t Write32(uint32_t sendData);
	uint32_t ReadRegNoCS(uint8_t reg, uint8_t bytes);
	uint32_t ReadReg(uint8_t reg, uint8_t bytes);

private:
	int flagOK; //ad转换正常
	int readCnt; //读取次数
	int readCntCheck; //上次检查的读取周期

#if AD7124SPISOFT
	SpiSoft *pspi; //SPI接口
#else 
	Spi *pspi; //SPI接口
#endif 
private:
	//测试
	float GetTemp1();
	float GetTemp1Res();
	float Temp1Test();
	uint32_t ad7124Buf[AD7124_BUFSIZE];
	uint8_t ad7124BufCnt = 0;
	uint8_t ad7124Int = 0;

};
#endif
