#include "Modbus.h"
#include "SerialPort.h"
#include "string.h"

void ModbusSlave::Process(Buffer &bs, void *para)
{
    SerialPort *sp = (SerialPort*)para;
    debug_printf("%s �յ���[%d]", sp->Name, bs.Length());
    bs.Show(true);
    if (IsFrameOK(bs))
    {
        debug_printf("��ȷ����֡\r\n");
        byte *buf = bs.GetBuffer();

        //�㲥��ַ�򱾻���ַ����Ӧ
        if ((buf[0] == 0) || (buf[0] == this->id))
        {
            this->Entity.Function = (MBFunction)buf[1];
            //add: ��Ӵ���
        }
    }
    else
    {
        debug_printf("��������֡\r\n");
    }
}

/// <summary>
/// ����CRCУ����(0����λ��1����λ)
/// </summary>
/// <param name="byteData">���������CRCֵ</param>
/// <returns>����ֵ��byte[]��2λ��0����λ��1����λ</returns>
ushort ModbusSlave::GetCRC(byte *byteData, int len)
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
                wCrc ^= 0xA001; //������ʽ
            }
            else
            {
                wCrc >>= 1;
            }
        }
    }

    return wCrc;
}

//������һ֡����
//demo:01 03 00 00 00 28 45 D4 
bool ModbusSlave::IsFrameOK(Buffer &bs)
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
	printf("%x %x %x\r\n",crcrcv,crc,GetCRC(bs.GetBuffer(),bs.Length()-2));
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
    // ����ǹ㲥��Ϣ����������վID������������֪���ҵ���վID    
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
            // ��֧�ֵĹ�����
            //            return entity.SetError(Errors.FunctionCode);
    }

    return entity;
}
