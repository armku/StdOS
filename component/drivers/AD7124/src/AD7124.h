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
	uint16_t CHCONFIG[16];//ͨ������

	void Init_8();		//8ͨ����ʼ��
	void Init_4();		//4ͨ����ʼ��
	uint8_t ReadID();		//��ȡID
	uint8_t ReadStatus();	//��ȡ״̬
	void Reset();		//��λ
	uint32_t ReadRlt();		//��ȡADֵ
	uint32_t ReadRlt(uint8_t& status);		//��ȡADֵ
	uint8_t Status;		//����״̬

	int Read(void); //����-1,��ʾת��δ���
	bool GetFlag(void); //��ȡADת���Ƿ�����
	void WriteReg(uint8_t reg, uint8_t bytes, uint32_t data);
	void SetReadChannel(uint8_t ch = 0, uint8_t chMax = 7);//������Ҫ��ȡ��ͨ����Ĭ��ͨ��0
private:
	uint16_t Write16(uint16_t sendData);
	uint32_t Write24(uint32_t sendData);
	uint32_t Write32(uint32_t sendData);
	uint32_t ReadRegNoCS(uint8_t reg, uint8_t bytes);
	uint32_t ReadReg(uint8_t reg, uint8_t bytes);

private:
	int flagOK; //adת������
	int readCnt; //��ȡ����
	int readCntCheck; //�ϴμ��Ķ�ȡ����

#if AD7124SPISOFT
	SpiSoft *pspi; //SPI�ӿ�
#else 
	Spi *pspi; //SPI�ӿ�
#endif 
private:
	//����
	float GetTemp1();
	float GetTemp1Res();
	float Temp1Test();
	uint32_t ad7124Buf[AD7124_BUFSIZE];
	uint8_t ad7124BufCnt = 0;
	uint8_t ad7124Int = 0;

};
#endif
