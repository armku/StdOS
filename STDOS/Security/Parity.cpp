#include "Parity.h"

/******************************************************
 *函数名称:OddParity
 *输   入:buf 要校验的数据; len 校验数据的长
 *输   出:校验和
 *功   能:奇校验
 *******************************************************/
bool Parity::OddParity(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0, j = 0;
    uint8_t d = 0;
    bool bParity = false;

    bParity = 1;

    for (j = 0; j < len; j++)
    {
        d = *(buf + j);

        for (i = 0; i < 8; i++)
        {

            if ((d &0x01) == 0x01)
            {
                bParity ^= 1;
            }

            d = d >> 1;
        }
    }

    return bParity;
}

/******************************************************
 *函数名称:EvenParity
 *输   入:buf 要校验的数据; len 校验数据的长
 *输   出:校验和
 *功   能:偶校验
 *******************************************************/
bool Parity::EvenParity(uint8_t *buf, uint32_t len)
{
    return (!OddParity(buf, len));
}
