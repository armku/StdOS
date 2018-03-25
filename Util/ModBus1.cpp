#include "Modbus1.h"
#include "SerialPort.h"
#include "string.h"
#include "Util.h"

uint8_t reginbuf[200]; //����Ĵ���
uint16_t RegInputu16[40];//����Ĵ���
uint16_t RegHoilding16[(22*16+8*12+4+16+5)*2];

void ModbusSlave1::Process(Buffer &bs, void *param)
{
    SerialPort *sp = (SerialPort*)param;
    debug_printf("%s �յ���[%d]", sp->Name, bs.Length());
    bs.Show(true);
    if (IsFrameOK(bs))
    {
        uint8_t *buf = bs.GetBuffer();
        debug_printf("��ȷ����֡ %d %d\r\n", buf[0], buf[1]);

        //�㲥��ַ�򱾻���ַ����Ӧ
        if ((buf[0] == 0) || (buf[0] == this->id))
        {
            this->Entity.Function = (MBFunction)buf[1];
            this->Entity.address = (buf[2] << 8) + buf[3];
            this->Entity.reglength = (buf[4] << 8) + buf[5];
            //add: ��Ӵ���
            this->DealFrame(bs, param);
        }
    }
    else
    {
        debug_printf("��������֡\r\n");
    }
}

//��������֡
void ModbusSlave1::DealFrame(Buffer &bs, void *param)
{
    debug_printf("address:%d length:%d\r\n", this->Entity.address, this->Entity.reglength);
    uint16_t crc;
    switch (this->Entity.Function)
    {
        case ReadInputRegisters:
            //��ȡ����Ĵ���
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
			//��ȡ���ּĴ���
			break;
		default:
            break;
    }
}

/// <summary>
/// ����CRCУ����(0����λ��1����λ)
/// </summary>
/// <param name="byteData">���������CRCֵ</param>
/// <returns>����ֵ��uint8_t[]��2λ��0����λ��1����λ</returns>
uint16_t ModbusSlave1::GetCRC(uint8_t *byteData, int len)
{
    uint16_t wCrc = 0xFFFF;
    for (int i = 0; i < len; i++)
    {
        wCrc ^= (uint16_t)(byteData[i]);
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
//11 04 00 08 00 02 F2 99
bool ModbusSlave1::IsFrameOK(Buffer &bs)
{
    uint16_t crc = 0;
    uint16_t crcrcv = 0;
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
            break;
    }

    return entity;
}
