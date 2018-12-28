#include "RX8025T.h"

/* RX8025T I2C��д��ַ */
#define RX8025T_RD_ADDR		0x65
#define RX8025T_WR_ADDR		0x64
/* RX8025T �Ĵ�����ַ */
#define SEC_ADDR			0x00
#define MIN_ADDR			0x01
#define HOUR_ADDR			0x02
#define WEEK_ADDR			0x03
#define DAY_ADDR			0x04
#define MONTH_ADDR			0x05
#define YEAR_ADDR			0x06
#define RAM_ADDR			0x07
#define MIN_ALM_ADDR	0x08
#define HOUR_ALM_ADDR	0x09
#define WEEK_ALM_ADDR	0x0A
#define DAY_ALM_ADDR	0x0A
#define TMR_CNT0_ADDR	0x0B
#define TMR_CNT1_ADDR	0x0C
#define EXT_REG_ADDR	0x0D
#define FLG_REG_ADDR	0x0E
#define CTL_REG_ADDR	0x0F

RX8025T::RX8025T(){}
RX8025T::RX8025T(Pin pinsck, Pin pinsda, uint8_t devaddr, uint32_t wnus)
{
    this->IIC.SetPin(pinsck, pinsda);
}
uint8_t BCD2Dec(uint8_t val)
{
    return (val >> 4) *10+(val &0x0f);
}

uint8_t Dec2BCD(uint8_t val)
{
    return ((val / 10) << 4)+(val % 10);
}

void RX8025T::Init()
{
    this->devaddr = RX8025T_WR_ADDR;
    
    buf[RAM_ADDR] = 0x00;
    RegWrite(RAM_ADDR, 1); //��ʹ�þ��ȵ���

    buf[FLG_REG_ADDR] = 0x24; //0010 0100,bit5:1 24Сʱ�ƣ�bit2:0 INTA���1HZ��ƽ���������壬�½������ʱͬ��
    RegRead(FLG_REG_ADDR, 1); //����24Сʱ��

//    RX8025_Read(0x0F, 1); //�ȶ��Ĵ���
//    buf[0] &= 0xFB; //CTFG����Ϊ0
//    RX8025_Write(0x0F, 1); //ֻ��������ƽģʽ����ʹ��

}
//
uint8_t RX8025T::CheckDevice(void)
{
	uint8_t re;

    this->IIC.Start(); /* ���������ź� */
    this->IIC.WriteByte(this->devaddr|1);
    re = this->IIC.WaitAck();
    this->IIC.Stop(); /* ����ֹͣ�ź� */
    return re;
}
uint8_t RX8025T::Output1s_Start(void)
{	
	//������
	this->buf[EXT_REG_ADDR]=0x00;
	this->buf[FLG_REG_ADDR]=0x00;
	this->buf[CTL_REG_ADDR]=0x60;
	
	return this->RegWrite(EXT_REG_ADDR,3);	
}
uint8_t RX8025T::Output1s_Stop(void)
{
	//������
	this->buf[EXT_REG_ADDR]=0x00;
	this->buf[FLG_REG_ADDR]=0x00;
	this->buf[CTL_REG_ADDR]=0x40;
	
	return this->RegWrite(EXT_REG_ADDR,3);	
}

//��RX8025�Ĵ���
int RX8025T::RegRead(uint8_t sadd, uint8_t len)
{
    this->IIC.Start();
    this->IIC.WriteByte(this->devaddr);
    if (this->IIC.WaitAck() == 1)
    {
        return 1;
    }
    this->IIC.WriteByte(SEC_ADDR);
    if (this->IIC.WaitAck() == 1)
    {
        return 1;
    }
    this->IIC.Start();
    this->IIC.WriteByte(this->devaddr|1);
    if (this->IIC.WaitAck() == 1)
    {
        return 1;
    }
	for(int i=0;i<len;i++)
	{
		this->buf[sadd+i] = this->IIC.ReadByte(i!=len-1?true:false);
	}
    
    this->IIC.Stop();
	return 0;
}

//дRX8025�Ĵ���
int RX8025T::RegWrite(uint8_t sadd, uint8_t len)
{
    this->IIC.Start();
    this->IIC.WriteByte(this->devaddr);
    if (this->IIC.WaitAck() == 1)
    {
        return 1;
    }
	this->IIC.WriteByte(sadd);
    if (this->IIC.WaitAck() == 1)
    {
        return 1;
    }
    for(int i=0;i<len;i++)
	{
		this->IIC.WriteByte(this->buf[sadd+i]);
		if (this->IIC.WaitAck() == 1)
		{
			return 1;
		}
	}
    
    this->IIC.Stop();
    return 0;
}

int RX8025T::LoadTime(DateTime &dt)
{
    if (this->RegRead(SEC_ADDR, 17) == 0)
    {
        dt.Second = BCD2Dec(this->buf[SEC_ADDR]);
        dt.Minute = BCD2Dec(this->buf[MIN_ADDR]);
        dt.Hour = BCD2Dec(this->buf[HOUR_ADDR]);
        dt.Day = BCD2Dec(this->buf[DAY_ADDR]);
        dt.Month = BCD2Dec(this->buf[MONTH_ADDR]);
        dt.Year = 2000+BCD2Dec(this->buf[YEAR_ADDR]);
        return 0;
    }

    return 1;
}

int RX8025T::SaveTime(DateTime &dt)
{
	this->buf[SEC_ADDR]=Dec2BCD(dt.Second);
    this->buf[MIN_ADDR]=Dec2BCD(dt.Minute);
    this->buf[HOUR_ADDR]=Dec2BCD(dt.Hour);
    this->buf[DAY_ADDR]=Dec2BCD(dt.Day);
    this->buf[MONTH_ADDR]=Dec2BCD(dt.Month);
    this->buf[YEAR_ADDR]=Dec2BCD(dt.Year - 2000);
	this->buf[WEEK_ADDR]=0;
	return this->RegWrite(SEC_ADDR,7);	
}
