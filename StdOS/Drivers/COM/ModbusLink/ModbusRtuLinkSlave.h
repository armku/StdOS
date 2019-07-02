#include "ModbusLink.h"

#ifndef _MODBUSRTULINKSLAVE_H
#define _MODBUSRTULINKSLAVE_H

class ModbusRtuLinkSlave :public ModbusLink
{
public:
	/*USARTHAL&com;*/
	const int static RegInputLen = 3;//����Ĵ������С
	const int static RegHoildingLen = 17;//���ּĴ������С
	ModbusReg_T RegInputs[RegInputLen];//����Ĵ�����
	ModbusReg_T RegHoildings[RegHoildingLen];//���ּĴ�����
public:
	ModbusRtuLinkSlave();
	bool CheckFrame();
	bool Send();
	void SetRegInput(int addr0, int reglen, uint16_t* reg, int reggroup = 0);//��������Ĵ���	
	void SetRegHoid(int addr0, int reglen, uint16_t* reg, int reggroup = 0);//���ñ��ּĴ���	
private:
	int dealRegInputRead(uint16_t addr, uint16_t len);//�����ȡ����Ĵ��� 0 ��ȷ 1 �Ƿ���ַ 2�Ƿ�����
	int dealRegHoildRead(uint16_t addr, uint16_t len);//�����ȡ���ּĴ��� 0 ��ȷ 1 �Ƿ���ַ 2�Ƿ�����
	int dealRegHoildWrite(uint16_t addr, uint16_t len);//����д�뱣�ּĴ��� 0 ��ȷ 1 �Ƿ���ַ 2�Ƿ�����
	int dealRegHoildWriteOne(uint16_t addr, uint16_t val);//����д�뵥�����ּĴ��� 0 ��ȷ 1 �Ƿ���ַ 2�Ƿ�����
	int searchRegInGroup(uint16_t addr, uint16_t len);//��������Ĵ����飬û�в��ҵ����ظ�ֵ
	int searchRegHoildGroup(uint16_t addr, uint16_t len);//���ұ��ּĴ����飬û�в��ҵ����ظ�ֵ
public:
	uint8_t id; //����ID
	void DealFrame(); //��������֡
	void(*OnUpdateRegHoid)(uint16_t regaddr, uint16_t reglen); //���²���	
};

#endif // !_MODBUSLINKSLAVE_H
