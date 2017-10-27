#include "DS18B20.h"
#include "stm32f10x.h"
#include "Sys.h"

#define HIGH  1
#define LOW   0

#define DS18B20_CLK     RCC_APB2Periph_GPIOB
#define DS18B20_PIN     GPIO_Pin_10                  
#define DS18B20_PORT		GPIOB 

void DS18B20::SetPin(Pin pin)
{
	this->_dio.Set(pin);
	
	this->_dio.OpenDrain=true;
	
	this->_dio.Invert=0;
	
	this->_dio.Open();
}

typedef struct
{
    byte humi_int; //ʪ�ȵ���������
    byte humi_deci; //ʪ�ȵ�С������
    byte temp_int; //�¶ȵ���������
    byte temp_deci; //�¶ȵ�С������
    byte check_sum; //У���

} DS18B20_Data_TypeDef;

/*
 * ��������DS18B20_Mode_IPU
 * ����  ��ʹDS18B20-DATA���ű�Ϊ����ģʽ
 * ����  ����
 * ���  ����
 */
void DS18B20::DS18B20_Mode_IPU()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /*ѡ��Ҫ���Ƶ�DS18B20_PORT����*/
    GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
    /*��������ģʽΪ��������ģʽ*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    /*���ÿ⺯������ʼ��DS18B20_PORT*/
    GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}


/*
 * ��������DS18B20_Mode_Out_PP
 * ����  ��ʹDS18B20-DATA���ű�Ϊ���ģʽ
 * ����  ����
 * ���  ����
 */
void DS18B20::DS18B20_Mode_Out_PP()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /*ѡ��Ҫ���Ƶ�DS18B20_PORT����*/
    GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
    /*��������ģʽΪͨ���������*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    /*������������Ϊ50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*���ÿ⺯������ʼ��DS18B20_PORT*/
    GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

/*
 *�������ӻ����͸�λ����
 */
void DS18B20::Rest()
{
    /* ��������Ϊ������� */
    DS18B20_Mode_Out_PP();
	this->_dio=0;
    //DS18B20_DATA_OUT(LOW);
    /* �������ٲ���480us�ĵ͵�ƽ��λ�ź� */
    Sys.Delay(750);
    /* �����ڲ�����λ�źź��轫�������� */
	this->_dio=1;
    //DS18B20_DATA_OUT(HIGH);
    /*�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������*/
    Sys.Delay(15);
}

/*
 * ���ӻ����������صĴ�������
 * 0���ɹ�
 * 1��ʧ��
 */
byte DS18B20::Presence()
{
    byte pulse_time = 0;
    /* ��������Ϊ�������� */
    DS18B20_Mode_IPU();
    /* �ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�ź� 
     * �����������û����������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
     */
    while (this->_dio && pulse_time < 100)
    {
        pulse_time++;
        Sys.Delay(1);
    }
    /* ����100us�󣬴������嶼��û�е���*/
    if (pulse_time >= 100)
        return 1;
    else
        pulse_time = 0;

    /* �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us */
    while (!this->_dio && pulse_time < 240)
    {
        pulse_time++;
        Sys.Delay(1);
    }
    if (pulse_time >= 240)
        return 1;
    else
        return 0;
}

/*
 * ��DS18B20��ȡһ��bit
 */
byte DS18B20::ReadBit()
{
    byte dat;

    /* ��0�Ͷ�1��ʱ������Ҫ����60us */
    DS18B20_Mode_Out_PP();
    /* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
    this->_dio=0;
	Sys.Delay(10);

    /* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
    DS18B20_Mode_IPU();
    //Sys.Delay(2);

    if (this->_dio == SET)
        dat = 1;
    else
        dat = 0;
    /* �����ʱ������ο�ʱ��ͼ */
    Sys.Delay(45);

    return dat;
}

/*
 * ��DS18B20��һ���ֽڣ���λ����
 */
byte DS18B20::ReadByte()
{
    byte i, j, dat = 0;

    for (i = 0; i < 8; i++)
    {
        j = ReadBit();
        dat = (dat) | (j << i);
    }

    return dat;
}

/*
 * дһ���ֽڵ�DS18B20����λ����
 */
void DS18B20::WriteByte(byte dat)
{
    byte i, testb;
    DS18B20_Mode_Out_PP();

    for (i = 0; i < 8; i++)
    {
        testb = dat &0x01;
        dat = dat >> 1;
        /* д0��д1��ʱ������Ҫ����60us */
        if (testb)
        {
            this->_dio=0;
            /* 1us < �����ʱ < 15us */
            Sys.Delay(8);

            this->_dio=1;
            Sys.Delay(58);
        }
        else
        {
            this->_dio=0;
            /* 60us < Tx 0 < 120us */
            Sys.Delay(70);

            this->_dio=1;
            /* 1us < Trec(�ָ�ʱ��) < �����*/
            Sys.Delay(2);
        }
    }
}

void DS18B20::Start()
{
    Rest();
    Presence();
    this->WriteByte(0XCC); /* ���� ROM */
    this->WriteByte(0X44); /* ��ʼת�� */
}

byte DS18B20::Init()
{
    Rest();

    return Presence();
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
    byte tpmsb, tplsb;
    short s_tem;
    float f_tem;

    Rest();
    Presence();
    this->WriteByte(0XCC); /* ���� ROM */
    this->WriteByte(0X44); /* ��ʼת�� */

    Rest();
    Presence();
    this->WriteByte(0XCC); /* ���� ROM */
    this->WriteByte(0XBE); /* ���¶�ֵ */

    tplsb = ReadByte();
    tpmsb = ReadByte();

    s_tem = tpmsb << 8;
    s_tem = s_tem | tplsb;

    if (s_tem < 0)
    /* ���¶� */
        f_tem = (~s_tem + 1) *0.0625;
    else
        f_tem = s_tem * 0.0625;

    return f_tem;
}
