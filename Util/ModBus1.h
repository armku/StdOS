#ifndef _MODBUS1_H
#define _MODBUS1_H

#include "Type.h"
#include "Buffer.h"

typedef enum MBFunction1
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
/// <summary>Modbusʵ��</summary>
class ModbusEntity1
{
	public:
		/// <summary>������ַ������485����</summary>
        byte Host;
		/// <summary>������</summary>
        MBFunction Function;
		/// <summary>����</summary>
        byte* Data;
		uint16_t address;//��ַ
		uint16_t reglength;//����
		/// <summary>У������</summary>
        uint16_t Crc;
		/// <summary>�����ֽ�����</summary>
        /// <param name="data"></param>
        /// <param name="offset">ƫ��</param>
        /// <param name="count">����</param>
        /// <returns></returns>
        ModbusEntity1 Parse(byte *data, int offset = 0, int count = -1);
		/// <summary>תΪ�ֽ�����</summary>
        /// <returns></returns>
        byte* ToArray();
};

class ModbusSlave1
{
	public:
		byte id;//����ID
		void Process(Buffer &bs, void *para);//��������
		bool IsFrameOK(Buffer& bs);//������һ֡����
		void DealFrame(Buffer&bs,void *param);//��������֡
		ModbusEntity1 Entity;
	protected:
		virtual ModbusEntity1 Process(ModbusEntity1 entity);
		uint16_t GetCRC(byte *byteData, int len);
};
extern uint16_t RegInputu161[];//����Ĵ���
extern uint16_t RegHoilding161[];//���ּĴ���
#endif
