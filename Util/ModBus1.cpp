#include "Modbus1.h"
#include "SerialPort.h"
#include "string.h"
#include "Util.h"

byte reginbuf[200]; //输入寄存器
ushort RegInputu16[40];//输入寄存器
ushort RegHoilding16[(22*16+8*12+4+16+5)*2];

void ModbusSlave1::Process(Buffer &bs, void *param)
{
    SerialPort *sp = (SerialPort*)param;
    debug_printf("%s 收到：[%d]", sp->Name, bs.Length());
    bs.Show(true);
    if (IsFrameOK(bs))
    {
        byte *buf = bs.GetBuffer();
        debug_printf("正确数据帧 %d %d\r\n", buf[0], buf[1]);

        //广播地址或本机地址，响应
        if ((buf[0] == 0) || (buf[0] == this->id))
        {
            this->Entity.Function = (MBFunction)buf[1];
            this->Entity.address = (buf[2] << 8) + buf[3];
            this->Entity.reglength = (buf[4] << 8) + buf[5];
            //add: 添加处理
            this->DealFrame(bs, param);
        }
    }
    else
    {
        debug_printf("错误数据帧\r\n");
    }
}

//处理数据帧
void ModbusSlave1::DealFrame(Buffer &bs, void *param)
{
    debug_printf("address:%d length:%d\r\n", this->Entity.address, this->Entity.reglength);
    ushort crc;
    switch (this->Entity.Function)
    {
        case ReadInputRegisters:
            //读取输入寄存器
            reginbuf[0] = this->id;
            reginbuf[1] = ReadInputRegisters;
            reginbuf[2] = this->Entity.reglength *2;            
			for(int i=0;i<this->Entity.reglength;i++)
			{
				reginbuf[3+i*2+1]=RegInputu16[i]&0xff;
				reginbuf[3+i*2]=(RegInputu16[i]>>8)&0xff;
			}
            crc = this->GetCRC(reginbuf, this->Entity.reglength *2+3);
            reginbuf[this->Entity.reglength *2+4] = (crc >> 8) &0x00ff;
            reginbuf[this->Entity.reglength *2+3] = crc &0x00ff;
            //sp->SendBuffer(reginbuf, this->Entity.reglength *2+5);
            break;
        case ReadHoldingRegisters:
			//读取保持寄存器
			break;
		default:
            break;
    }
}

/// <summary>
/// 计算CRC校验码(0：地位，1：高位)
/// </summary>
/// <param name="byteData">输入参数：CRC值</param>
/// <returns>返回值：byte[]，2位，0：地位，1：高位</returns>
ushort ModbusSlave1::GetCRC(byte *byteData, int len)
{
    ushort wCrc = 0xFFFF;
    for (int i = 0; i < len; i++)
    {
        wCrc ^= (ushort)(byteData[i]);
        for (int j = 0; j < 8; j++)
        {
            if ((wCrc &0x0001) == 1)
            {
                wCrc >>= 1;
                wCrc ^= 0xA001; //异或多项式
            }
            else
            {
                wCrc >>= 1;
            }
        }
    }

    return wCrc;
}

//完整的一帧数据
//demo:01 03 00 00 00 28 45 D4 
//11 04 00 08 00 02 F2 99
bool ModbusSlave1::IsFrameOK(Buffer &bs)
{
    ushort crc = 0;
    ushort crcrcv = 0;
    if (bs.Length() < 6)
    {
        return false;
    }
    crc = this->GetCRC(bs.GetBuffer(), bs.Length() - 2);
    crcrcv = bs.GetBuffer()[bs.Length() - 1];
    crcrcv <<= 8;
    crcrcv |= bs.GetBuffer()[bs.Length() - 2];
    if (crcrcv == crc)
    {
        return true;
    }
    else
    {
        return false;
    }
}


ModbusEntity1 ModbusSlave1::Process(ModbusEntity1 entity)
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
            break;
    }

    return entity;
}
