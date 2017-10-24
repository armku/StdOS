#include "Parity.h"

/******************************************************
 *��������:OddParity
 *��   ��:buf ҪУ�������; len У�����ݵĳ�
 *��   ��:У���
 *��   ��:��У��
 *******************************************************/
bool Parity::OddParity(byte *buf, uint len)
{
    uint i = 0, j = 0;
    byte d = 0;
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
 *��������:EvenParity
 *��   ��:buf ҪУ�������; len У�����ݵĳ�
 *��   ��:У���
 *��   ��:żУ��
 *******************************************************/
bool Parity::EvenParity(byte *buf, uint len)
{
    return (!OddParity(buf, len));
}
