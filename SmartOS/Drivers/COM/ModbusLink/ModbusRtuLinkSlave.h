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
	void SetRegInput(int addr0, int reglen, ushort* reg, int reggroup = 0);//��������Ĵ���	
	void SetRegHoid(int addr0, int reglen, ushort* reg, int reggroup = 0);//���ñ��ּĴ���	
private:
	int dealRegInputRead(ushort addr, ushort len);//�����ȡ����Ĵ��� 0 ��ȷ 1 �Ƿ���ַ 2�Ƿ�����
	int dealRegHoildRead(ushort addr, ushort len);//�����ȡ���ּĴ��� 0 ��ȷ 1 �Ƿ���ַ 2�Ƿ�����
	int dealRegHoildWrite(ushort addr, ushort len);//����д�뱣�ּĴ��� 0 ��ȷ 1 �Ƿ���ַ 2�Ƿ�����
	int dealRegHoildWriteOne(ushort addr, ushort val);//����д�뵥�����ּĴ��� 0 ��ȷ 1 �Ƿ���ַ 2�Ƿ�����
	int searchRegInGroup(ushort addr, ushort len);//��������Ĵ����飬û�в��ҵ����ظ�ֵ
	int searchRegHoildGroup(ushort addr, ushort len);//���ұ��ּĴ����飬û�в��ҵ����ظ�ֵ
public:
	byte id; //����ID
	void DealFrame(); //��������֡
	void(*OnUpdateRegHoid)(ushort regaddr, ushort reglen); //���²���	
};

#endif // !_MODBUSLINKSLAVE_H
