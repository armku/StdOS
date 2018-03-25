#include "CheckSum.h"

/******************************************************
 *��������:CheckSum8
 *��   ��:buf ҪУ������� 
len У�����ݵĳ�
 *��   ��:У���
 *��   ��:У���-8
 *******************************************************/
byte CheckSum::CheckSum8(byte *buf, ushort len)
{
    ushort i = 0;
    byte Sum = 0;

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
ushort CheckSum::CheckSum16(byte *buf, ushort len)
{
    ushort i = 0;
    ushort Sum = 0;

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
uint32_t CheckSum::CheckSum32(byte *buf, ushort len)
{
    ushort i = 0;
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
byte CheckSum::IntelHexCheckSum(byte *buf, ushort len)
{
    byte Sum;

    Sum = CheckSum8(buf, len);
    Sum = 0x01 + (byte)(~Sum);

    return Sum;
}

/******************************************************
 *��������:NetCheckSum
 *��   ��:buf ҪУ�������
len У�����ݵĳ�
 *��   ��:У���
 *��   ��:У���-TCP/IP
 *******************************************************/
ushort CheckSum::NetCheckSum(byte *buf, ushort len)
{
    ushort Sum = 0;

    while (len--)
    {
        Sum +=  *buf++;
    }

    Sum = (Sum >> 16) + (Sum &0xffff);
    Sum += (Sum >> 16);

    return (ushort)(~Sum);
}

/******************************************************
 *��������:BCCVerify
 *��   ��:buf ҪУ�������
len У�����ݵĳ�
 *��   ��:У��ֵ
 *��   ��:���У��
 *******************************************************/
byte CheckSum::BCCVerify(byte *buf, ushort len)
{
    byte s = 0;

    for (ushort i = 0; i < len; i++)
    {
        s = s ^ (*(buf + i));
    }

    return s;
}
