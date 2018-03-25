#ifndef _MODBUS1_H
#define _MODBUS1_H

#include "Type.h"
#include "Buffer.h"

typedef enum MBFunction1
{
    /// <summary>读取线圈状态。取得一组逻辑线圈的当前状态（ON/OFF) </summary>
    ReadCoils = 1, 
    /// <summary>读取离散量输入状态。取得一组开关输入的当前状态（ON/OFF) </summary>
    ReadInputs = 2, 
    /// <summary>读取保持寄存器。在一个或多个保持寄存器中取得当前的二进制值 </summary>
    ReadHoldingRegisters = 3, 
    /// <summary>读取输入寄存器。在一个或多个输入寄存器中取得当前的二进制值</summary>
    ReadInputRegisters = 4, 
    /// <summary>强置单线圈。强置一个逻辑线圈的通断状态</summary>
    WriteSingleCoil = 5, 
    /// <summary>预置单寄存器。把具体二进值装入一个保持寄存器</summary>
    WriteSingleRegister = 6, 
    ///// <summary>读取异常状态。取得8个内部线圈的通断状态，这8个线圈的地址由控制器决定 </summary>
    //ReadError = 7,
    /// <summary>回送诊断校验。把诊断校验报文送从机，以对通信处理进行评鉴</summary>
    Diagnostics = 8, 
    /// <summary>强置多线圈。强置一串连续逻辑线圈的通断</summary>
    WriteMultipleCoils = 15, 
    /// <summary>预置多寄存器。把具体的二进制值装入一串连续的保持寄存器</summary>
    WriteMultipleRegisters = 16, 
    /// <summary>报告从机标识。可使主机判断编址从机的类型及该从机运行指示灯的状态</summary>
    ReportIdentity = 17, 
} MBFunction;
/// <summary>Modbus实体</summary>
class ModbusEntity1
{
	public:
		/// <summary>主机地址。用于485编码</summary>
        uint8_t Host;
		/// <summary>功能码</summary>
        MBFunction Function;
		/// <summary>数据</summary>
        uint8_t* Data;
		uint16_t address;//地址
		uint16_t reglength;//长度
		/// <summary>校验数据</summary>
        uint16_t Crc;
		/// <summary>分析字节数组</summary>
        /// <param name="data"></param>
        /// <param name="offset">偏移</param>
        /// <param name="count">数量</param>
        /// <returns></returns>
        ModbusEntity1 Parse(uint8_t *data, int offset = 0, int count = -1);
		/// <summary>转为字节数组</summary>
        /// <returns></returns>
        uint8_t* ToArray();
};

class ModbusSlave1
{
	public:
		uint8_t id;//本机ID
		void Process(Buffer &bs, void *para);//处理数据
		bool IsFrameOK(Buffer& bs);//完整的一帧数据
		void DealFrame(Buffer&bs,void *param);//处理数据帧
		ModbusEntity1 Entity;
	protected:
		virtual ModbusEntity1 Process(ModbusEntity1 entity);
		uint16_t GetCRC(uint8_t *byteData, int len);
};
extern uint16_t RegInputu161[];//输入寄存器
extern uint16_t RegHoilding161[];//保持寄存器
#endif
