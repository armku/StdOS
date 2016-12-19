#include <math.h>
#include "Util.h"

//排序
void bubbleSort(float *buf, uint32_t len)
{
    uint32_t i, j, pos;
    float tmp;
    for (i = len; i > 0; i--)
    {
        pos = 0;
        for (j = 0; j < i; j++)
        {
            if (buf[pos] < buf[j])
            {
                pos = j;
            }
        }
        tmp = buf[pos];
        buf[pos] = buf[i - 1];
        buf[i - 1] = tmp;
    }
}

union
{
    float dafloat;
    struct
    {
        uint8_t da3;
        uint8_t da2;
        uint8_t da1;
        uint8_t da0;
    } char_bit;
}

bitconvert; //数据转换用   
//设置浮点数
void SetBufFloat(uint8_t buf[], uint16_t pos, float da)
{
    bitconvert.dafloat = da;
    buf[pos] = bitconvert.char_bit.da3;
    buf[pos + 1] = bitconvert.char_bit.da2;
    buf[pos + 2] = bitconvert.char_bit.da1;
    buf[pos + 3] = bitconvert.char_bit.da0;
}

//设置浮点数
float GetBufFloat(uint8_t buf[], uint16_t pos)
{
    bitconvert.char_bit.da3 = buf[pos];
    bitconvert.char_bit.da2 = buf[pos + 1];
    bitconvert.char_bit.da1 = buf[pos + 2];
    bitconvert.char_bit.da0 = buf[pos + 3];
    return bitconvert.dafloat;
}
//从电阻获得温度
float GetTFromPT100(float om)
{
    float ret = 0;
    if (om > 100)
    {
        ret = (0.39083 - sqrt(0.39083 *0.39083 - 4 * 0.00005775 *(om - 100))) / 2 / 0.00005775;
    }
    else if (om <= 0)
    {
        ret = 0;
    }
    else
    {
        ret = (0.3905664 - sqrt(0.3905664 *0.3905664 - 4 * 0.0000667284 *(100-om))) / 2 / 0.0000667284;
        ret =  - ret;
    }

    return ret;
}

