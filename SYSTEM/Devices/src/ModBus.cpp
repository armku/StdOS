#include "Modbus.h"
#include "SerialPort.h"
#include "string.h"

void ModbusSlave::Process(Buffer &bs, void *para)
{
    SerialPort *sp = (SerialPort*)para;
    debug_printf("%s 收到：[%d]", sp->Name, bs.Length());
    bs.Show(true);
    if (IsFrameOK(bs))
    {
        debug_printf("正确数据帧\r\n");
		byte *buf=bs.GetBuffer();
		
		//广播地址或本机地址，响应
		if((buf[0]==0)||(buf[0]==this->id))
		{			
			this->Entity.Function=(MBFunction)buf[1];
			//add: 添加处理
		}
    }
    else
    {
        debug_printf("错误数据帧\r\n");
    }
}

ushort ModbusSlave::CalcCRC(byte *u8Data, byte u8Size)
{
    ushort ReturnData;
    byte num = 0xff;
    byte num2 = 0xff;

    byte num3 = 1;
    byte num4 = 160;
    byte buffer[5];
    memcpy(buffer, u8Data, u8Size);

    for (int i = 0; i < u8Size; i++)
    {
        num = (byte)(num ^ buffer[i]);

        for (int j = 0; j <= 7; j++)
        {
            byte num5 = num2;
            byte num6 = num;

            num2 = (byte)(num2 >> 1);
            num = (byte)(num >> 1);

            if ((num5 &1) == 1)
            {
                num = (byte)(num | 0x80);
            }
            if ((num6 &1) == 1)
            {
                num2 = (byte)(num2 ^ num4);
                num = (byte)(num ^ num3);
            }
        }
    }
    ReturnData = num;
    ReturnData = ReturnData << 8;
    ReturnData |= num2;
    return ReturnData;

}

//完整的一帧数据
//demo:01 03 00 00 00 28 45 D4 
bool ModbusSlave::IsFrameOK(Buffer &bs)
{
    ushort crc = 0;
    ushort crcrcv = 0;
    if (bs.Length() < 6)
    {
        return false;
    }
    crc = CalcCRC(bs.GetBuffer(), bs.Length() - 2);
    crcrcv = bs.GetBuffer()[bs.Length() - 2];
    crcrcv <<= 8;
    crcrcv |= bs.GetBuffer()[bs.Length() - 1];
    if (crcrcv == crc)
    {
        return true;		
    }
    else
    {
        return false;
    }
}


ModbusEntity ModbusSlave::Process(ModbusEntity entity)
{
    // 如果是广播消息，则设置主站ID，便于其他人知道我的主站ID    
    switch (entity.Function)
    {
        case ReadCoils:
        case ReadInputs:
            //            entity = ReadCoils(entity);
            break;
        case ReadHoldingRegisters:
        case ReadInputRegisters:
            //            entity = ReadRegisters(entity);
            break;
        case WriteSingleCoil:
            //            entity = WriteSingleCoil(entity);
            break;
        case WriteSingleRegister:
            //            entity = WriteSingleRegister(entity);
            break;
        case WriteMultipleCoils:
            //            entity = WriteMultipleCoils(entity);
            break;
        case WriteMultipleRegisters:
            //            entity = WriteMultipleRegisters(entity);
            break;
        case Diagnostics:
            //            entity = Diagnostics(entity);
            break;
        case ReportIdentity:
            //            entity = ReportIdentity(entity);
            break;
        default:
            // 不支持的功能码
            //            return entity.SetError(Errors.FunctionCode);
    }

    return entity;
}
