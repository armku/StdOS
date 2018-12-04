#include "DataFrameModbus.h"
#include "stm32f10x.h"
#include "FIFOBuffer.h"
#include "USART.h"

#ifndef _MODBUSLINK_H
#define _MODBUSLINK_H

typedef enum MBFunction
{
	/// <summary>��ȡ��Ȧ״̬��ȡ��һ���߼���Ȧ�ĵ�ǰ״̬��ON/OFF) </summary>
	ReadCoils = 1,
	/// <summary>��ȡ��ɢ������״̬��ȡ��һ�鿪������ĵ�ǰ״̬��ON/OFF) </summary>
	ReadInputs = 2,
	/// <summary>��ȡ���ּĴ�������һ���������ּĴ�����ȡ�õ�ǰ�Ķ�����ֵ </summary>
	ReadHoldingRegisters = 3,
	/// <summary>��ȡ����Ĵ�������һ����������Ĵ�����ȡ�õ�ǰ�Ķ�����ֵ</summary>
	ReadInputRegisters = 4,
	/// <summary>ǿ�õ���Ȧ��ǿ��һ���߼���Ȧ��ͨ��״̬</summary>
	WriteSingleCoil = 5,
	/// <summary>Ԥ�õ��Ĵ������Ѿ������ֵװ��һ�����ּĴ���</summary>
	WriteSingleRegister = 6,
	///// <summary>��ȡ�쳣״̬��ȡ��8���ڲ���Ȧ��ͨ��״̬����8����Ȧ�ĵ�ַ�ɿ��������� </summary>
	//ReadError = 7,
	/// <summary>�������У�顣�����У�鱨���ʹӻ����Զ�ͨ�Ŵ����������</summary>
	Diagnostics = 8,
	/// <summary>ǿ�ö���Ȧ��ǿ��һ�������߼���Ȧ��ͨ��</summary>
	WriteMultipleCoils = 15,
	/// <summary>Ԥ�ö�Ĵ������Ѿ���Ķ�����ֵװ��һ�������ı��ּĴ���</summary>
	WriteMultipleRegisters = 16,
	/// <summary>����ӻ���ʶ����ʹ�����жϱ�ַ�ӻ������ͼ��ôӻ�����ָʾ�Ƶ�״̬</summary>
	ReportIdentity = 17,
} MBFunction;

typedef struct
{
	int Addr0;//��ʼ��ַ
	int Lenth;//�Ĵ�������
	uint16_t *Reg;//�Ĵ���ָ��
}ModbusReg_T;//�Ĵ����ṹ��

class ModbusSlaveLink
{
public:
	DataFrameModbus txFrame;
	DataFrameModbus rxFrame;
	USART &com;
	const int static RegInputLen = 3;//����Ĵ������С
	const int static RegHoilding = 3;//���ּĴ������С
	ModbusReg_T RegInputs[RegInputLen];//����Ĵ�����
	ModbusReg_T RegHoildings[RegInputLen];//���ּĴ�����
public:
	ModbusSlaveLink(USART &uart);
	bool CheckFrame();
	bool Send();
	void SetRegInput(int addr0, int reglen, uint16_t* reg, int reggroup = 0);//��������Ĵ���	
private:
	int dealRegInputRead(uint16_t addr,uint16_t len);//�����ȡ����Ĵ��� 0 ��ȷ 1 �Ƿ���ַ 2�Ƿ�����
	int searchRegInGroup(uint16_t addr, uint16_t len);//���ҼĴ����飬û�в��ҵ����ظ�ֵ
public:
	uint8_t id; //����ID
	void DealFrame(); //��������֡
	void(*DUpdateReg)(uint16_t regaddr,uint16_t reglen); //���²���
	//�����豸������pc
	void (*pupdatedevparafrompc)();
	//����ͨ������ ͨ���� 0-15
	void(*pcommupdatereghoildchannel)(uint8_t ch);
	//����ͨ������
	void(*pupdatechannelparafrompc)();
	void(*pupdatewarparafrompc)(uint16_t ch);
};

extern uint16_t RegHoilding16[]; //���ּĴ���

#endif // !_MODBUSLINK_H
