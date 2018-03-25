#include "CheckSum.h"

/******************************************************
 *函数名称:CheckSum8
 *输   入:buf 要校验的数据 
len 校验数据的长
 *输   出:校验和
 *功   能:校验和-8
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
 *函数名称:CheckSumSum16
 *输   入:buf 要校验的数据
len 校验数据的长
 *输   出:校验和
 *功   能:校验和-16
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
 *函数名称:CheckSum32
 *输   入:buf 要校验的数据
len 校验数据的长
 *输   出:校验和
 *功   能:校验和-32
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
 *函数名称:IntelHexCheckSum
 *输   入:buf 要校验的数据
len 校验数据的长
 *输   出:校验和
 *功   能:校验和-Intel Hex
 *******************************************************/
uint8_t CheckSum::IntelHexCheckSum(uint8_t *buf, uint16_t len)
{
    uint8_t Sum;

    Sum = CheckSum8(buf, len);
    Sum = 0x01 + (uint8_t)(~Sum);

    return Sum;
}

/******************************************************
 *函数名称:NetCheckSum
 *输   入:buf 要校验的数据
len 校验数据的长
 *输   出:校验和
 *功   能:校验和-TCP/IP
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
 *函数名称:BCCVerify
 *输   入:buf 要校验的数据
len 校验数据的长
 *输   出:校验值
 *功   能:异或校验
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
