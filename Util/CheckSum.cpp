#include "CheckSum.h"

/******************************************************
 *��������:CheckSum8
 *��   ��:buf ҪУ������� 
len У�����ݵĳ�
 *��   ��:У���
 *��   ��:У���-8
 *******************************************************/
uint8_t CheckSum::CheckSum8(uint8_t *buf, uint16_t len)
{
    uint16_t i = 0;
    uint8_t Sum = 0;

    for (i = 0; i < len; i++)
    {
        Sum +=  *buf++;
    }

    return Sum;
}

/******************************************************
 *��������:CheckSumSum16
 *��   ��:buf ҪУ�������
len У�����ݵĳ�
 *��   ��:У���
 *��   ��:У���-16
 *******************************************************/
uint16_t CheckSum::CheckSum16(uint8_t *buf, uint16_t len)
{
    uint16_t i = 0;
    uint16_t Sum = 0;

    for (i = 0; i < len; i++)
    {
        Sum +=  *buf++;
    }

    return Sum;
}

/******************************************************
 *��������:CheckSum32
 *��   ��:buf ҪУ�������
len У�����ݵĳ�
 *��   ��:У���
 *��   ��:У���-32
 *******************************************************/
uint32_t CheckSum::CheckSum32(uint8_t *buf, uint16_t len)
{
    uint16_t i = 0;
    uint32_t Sum = 0;

    for (i = 0; i < len; i++)
    {
        Sum +=  *buf++;
    }

    return Sum;
}

/******************************************************
 *��������:IntelHexCheckSum
 *��   ��:buf ҪУ�������
len У�����ݵĳ�
 *��   ��:У���
 *��   ��:У���-Intel Hex
 *******************************************************/
uint8_t CheckSum::IntelHexCheckSum(uint8_t *buf, uint16_t len)
{
    uint8_t Sum;

    Sum = CheckSum8(buf, len);
    Sum = 0x01 + (uint8_t)(~Sum);

    return Sum;
}

/******************************************************
 *��������:NetCheckSum
 *��   ��:buf ҪУ�������
len У�����ݵĳ�
 *��   ��:У���
 *��   ��:У���-TCP/IP
 *******************************************************/
uint16_t CheckSum::NetCheckSum(uint8_t *buf, uint16_t len)
{
    uint16_t Sum = 0;

    while (len--)
    {
        Sum +=  *buf++;
    }

    Sum = (Sum >> 16) + (Sum &0xffff);
    Sum += (Sum >> 16);

    return (uint16_t)(~Sum);
}

/******************************************************
 *��������:BCCVerify
 *��   ��:buf ҪУ�������
len У�����ݵĳ�
 *��   ��:У��ֵ
 *��   ��:���У��
 *******************************************************/
uint8_t CheckSum::BCCVerify(uint8_t *buf, uint16_t len)
{
    uint8_t s = 0;

    for (uint16_t i = 0; i < len; i++)
    {
        s = s ^ (*(buf + i));
    }

    return s;
}
