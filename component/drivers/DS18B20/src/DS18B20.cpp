#include "DS18B20.h"
#include "../../../../StdOS/Kernel/Sys.h"

void DS18B20::SetPin(Pin pin)
{
	this->_dio.Set(pin);
	
	this->_dio.OpenDrain=true;
	
	this->_dio.Invert=0;
	
	this->_dio.Open();
	this->_dio = 1;
}

/*
 *�������ӻ����͸�λ����
 */
void DS18B20::Rest()
{
    this->_dio=0;
    /* �������ٲ���480us�ĵ͵�ƽ��λ�ź� */
    delay_us(480);
    /* �����ڲ�����λ�źź��轫�������� */
	this->_dio=1;
}

/*
 * ���ӻ����������صĴ�������
 * 0���ɹ�
 * 1��ʧ��
 */
bool DS18B20::Presence()
{
    uint8_t pulse_time = 0;
    /* ��������Ϊ�������� */
    /* �ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�ź� 
     * �����������û����������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
     */
	this->_dio=1;
    while (this->_dio && pulse_time < 100)
    {
        pulse_time++;
        delay_us(1);
    }
    /* ����100us�󣬴������嶼��û�е���*/
    if (pulse_time >= 100)
        return false;
    else
        pulse_time = 0;

    /* �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us */
    while (!this->_dio && pulse_time < 240)
    {
        pulse_time++;
        delay_us(1);
    }
    if (pulse_time >= 240)
        return false;
    else
        return true;
}

/*
 * ��DS18B20��ȡһ��bit
 */
uint8_t DS18B20::ReadBit()
{
    uint8_t dat;

    /* ��0�Ͷ�1��ʱ������Ҫ����60us */
    /* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
    this->_dio=0;
	delay_us(10);

    /* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
    //delay_us(2);
	this->_dio=1;

    if (this->_dio)
        dat = 1;
    else
        dat = 0;
    /* �����ʱ������ο�ʱ��ͼ */
    delay_us(45);

    return dat;
}

/*
 * ��DS18B20��һ���ֽڣ���λ����
 */
uint8_t DS18B20::ReadByte()
{
    uint8_t i, j, dat = 0;

    for (i = 0; i < 8; i++)
    {
        j = this->ReadBit();
        dat = (dat) | (j << i);
    }

    return dat;
}

/*
 * дһ���ֽڵ�DS18B20����λ����
 */
void DS18B20::WriteByte(uint8_t dat)
{
    uint8_t i, testb;

    for (i = 0; i < 8; i++)
    {
        testb = dat &0x01;
        dat = dat >> 1;
        /* д0��д1��ʱ������Ҫ����60us */
        if (testb)
        {
            this->_dio=0;
            /* 1us < �����ʱ < 15us */
            delay_us(8);

            this->_dio=1;
            delay_us(58);
        }
        else
        {
            this->_dio=0;
            /* 60us < Tx 0 < 120us */
            delay_us(70);

            this->_dio=1;
            /* 1us < Trec(�ָ�ʱ��) < �����*/
            delay_us(2);
        }
    }
}

void DS18B20::Start()
{
    this->Rest();
    this->Presence();
    this->WriteByte(0XCC); /* ���� ROM */
    this->WriteByte(0X44); /* ��ʼת�� */
}

uint8_t DS18B20::Init()
{
    this->Rest();
    return 0;
}
/**
 * @brief  ��ƥ�� ROM ����»�ȡ DS18B20 �¶�ֵ 
 * @param  ds18b20_id�����ڴ�� DS18B20 ���кŵ�������׵�ַ
 * @retval ��
 */
void DS18B20::ReadId()
{  
  this->WriteByte(0x33); //��ȡ���к�

  for (int uc = 0; uc < 8; uc++)
    this->id[uc] = this->ReadByte();

}
void DS18B20::SkipRom()
{
  this->Rest();

  this->Presence();

  this->WriteByte(0XCC); /* ���� ROM */

}
/*
 * �洢���¶���16 λ�Ĵ�������չ�Ķ����Ʋ�����ʽ
 * ��������12λ�ֱ���ʱ������5������λ��7������λ��4��С��λ
 *
 *         |---------����----------|-----С�� �ֱ��� 1/(2^4)=0.0625----|
 * ���ֽ�  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----����λ��0->��  1->��-------|-----------����-----------|
 * ���ֽ�  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * �¶� = ����λ + ���� + С��*0.0625
 */
float DS18B20::GetTemp()
{
    uint8_t tpmsb, tplsb;
    short s_tem;
    float f_tem;

	this->SkipRom();
    this->WriteByte(0X44); /* ��ʼת�� */

    this->SkipRom();
#if 1  
    this->WriteByte(0XBE); /* ���¶�ֵ */

    tplsb = this->ReadByte();
    tpmsb = this->ReadByte();
#endif
    s_tem = tpmsb << 8;
    s_tem = s_tem | tplsb;

    if (s_tem < 0)
    /* ���¶� */
        f_tem = (~s_tem + 1) *0.0625;
    else
        f_tem = s_tem * 0.0625;

    return f_tem;
}
