#include "CheckSum.h"

/******************************************************
 *函数名称:CheckSum8
 *输   入:buf 要校验的数据 
len 校验数据的长
 *输   出:校验和
 *功   能:校验和-8
 *******************************************************/
 byte CheckSum::CheckSum8(byte *buf, uint32_t len)
{
    uint32_t i = 0;
    byte Sum = 0;

    for (i = 0; i < len; i++)
    {
        Sum +=  *buf++;
    }

    return Sum;
}

/******************************************************
 *函数名称:CheckSumSum16
 *输   入:buf 要校验的数据
len 校验数据的长
 *输   出:校验和
 *功   能:校验和-16
 *******************************************************/
ushort CheckSum::CheckSum16(byte *buf, uint32_t len)
{
    uint32_t i = 0;
    ushort Sum = 0;

    for (i = 0; i < len; i++)
    {
        Sum +=  *buf++;
    }

    return Sum;
}

/******************************************************
 *函数名称:CheckSum32
 *输   入:buf 要校验的数据
len 校验数据的长
 *输   出:校验和
 *功   能:校验和-32
 *******************************************************/
uint32_t CheckSum::CheckSum32(byte *buf, uint32_t len)
{
    uint32_t i = 0;
    uint32_t Sum = 0;

    for (i = 0; i < len; i++)
    {
        Sum +=  *buf++;
    }

    return Sum;
}

/******************************************************
 *函数名称:IntelHexCheckSum
 *输   入:buf 要校验的数据
len 校验数据的长
 *输   出:校验和
 *功   能:校验和-Intel Hex
 *******************************************************/
byte CheckSum::IntelHexCheckSum(byte *buf, uint32_t len)
{
    byte Sum;

    Sum = CheckSum8(buf, len);
    Sum = 0x01 + (byte)(~Sum);

    return Sum;
}

/******************************************************
 *函数名称:NetCheckSum
 *输   入:buf 要校验的数据
len 校验数据的长
 *输   出:校验和
 *功   能:校验和-TCP/IP
 *******************************************************/
ushort CheckSum::NetCheckSum(byte *buf, uint32_t len)
{
    uint32_t Sum = 0;

    while (len--)
    {
        Sum +=  *buf++;
    }

    Sum = (Sum >> 16) + (Sum &0xffff);
    Sum += (Sum >> 16);

    return (ushort)(~Sum);
}

/******************************************************
 *函数名称:BCCVerify
 *输   入:buf 要校验的数据
len 校验数据的长
 *输   出:校验值
 *功   能:异或校验
 *******************************************************/
byte CheckSum::BCCVerify(byte *buf, uint32_t len)
{
    byte s = 0;

    for (uint32_t i = 0; i < len; i++)
    {
        s = s ^ (*(buf + i));
    }

    return s;
}
