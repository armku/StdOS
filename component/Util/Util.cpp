#include <math.h>
#include "Util.h"

static float DOTMASK[] = { 1,10,100,1000 };
static float boxcartmp[100];//滤波辅助变量
#ifdef __cplusplus
extern "C" {
#endif
//矩形波过滤器,目前限定100个数据量
void BoxCar(int* buf, int len)
{
	int start = 0;
	int end = 0;

	if (len < 2)
		return;
	for (int i = 0; i < len; i++)
	{
		start = i - 1;
		end = i + 1;
		if (start < 0)
			start = 0;
		if (end > (len - 1))
			end = len - 1;

		boxcartmp[i] = (buf[start] + buf[i] + buf[i] + buf[end]) / 4;
	}
	for (int i = 0; i < len; i++)
	{
		buf[i] = boxcartmp[i];
	}
}
//根据小数点获取值
float  GetValueDot(float value, int dot)
{
	if (dot > 3)
		return 0;
	return value * 1.0 / DOTMASK[dot];
}
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
	uint16_t daushort[2];
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
void SetBufFloat(uint8_t buf[], float da, uint16_t pos, uint8_t type)
{
	bitconvert.dafloat = da;
	switch (type)
	{
	case 0:
		buf[pos + 0] = bitconvert.char_bit.da3;
		buf[pos + 1] = bitconvert.char_bit.da2;
		buf[pos + 2] = bitconvert.char_bit.da1;
		buf[pos + 3] = bitconvert.char_bit.da0;
		break;
	case 1:
		buf[pos + 2] = bitconvert.char_bit.da3;
		buf[pos + 3] = bitconvert.char_bit.da2;
		buf[pos + 0] = bitconvert.char_bit.da1;
		buf[pos + 1] = bitconvert.char_bit.da0;
		break;
	default:
		break;
	}
}

void SetBufUshort(uint8_t buf[], uint16_t pos, uint16_t da, uint8_t type)
{
	bitconvert.daushort[0] = da;
	switch (type)
	{
	case 0:
		buf[pos] = bitconvert.char_bit.da3;
		buf[pos + 1] = bitconvert.char_bit.da2;
		break;
	case 1:
		buf[pos + 1] = bitconvert.char_bit.da3;
		buf[pos + 0] = bitconvert.char_bit.da2;
		break;
	default:
		break;
	}
}

//设置浮点数 0:小端 1：大端
float GetBufFloat(uint8_t buf[], uint16_t pos, uint8_t type)
{
	switch (type)
	{
	case 0:
		bitconvert.char_bit.da3 = buf[pos + 0];
		bitconvert.char_bit.da2 = buf[pos + 1];
		bitconvert.char_bit.da1 = buf[pos + 2];
		bitconvert.char_bit.da0 = buf[pos + 3];
		break;
	case 1:
		bitconvert.char_bit.da3 = buf[pos + 2];
		bitconvert.char_bit.da2 = buf[pos + 3];
		bitconvert.char_bit.da1 = buf[pos + 0];
		bitconvert.char_bit.da0 = buf[pos + 1];
		break;
	case 2:
		bitconvert.char_bit.da3 = buf[pos + 1];
		bitconvert.char_bit.da2 = buf[pos + 0];
		bitconvert.char_bit.da1 = buf[pos + 3];
		bitconvert.char_bit.da0 = buf[pos + 2];
		break;
	case 3:
		bitconvert.char_bit.da3 = buf[pos + 3];
		bitconvert.char_bit.da2 = buf[pos + 2];
		bitconvert.char_bit.da1 = buf[pos + 1];
		bitconvert.char_bit.da0 = buf[pos + 0];
	default:
		break;
	}
	return bitconvert.dafloat;
}

//设置浮点数
uint16_t GetBufUshort(uint8_t buf[], uint16_t pos, uint8_t type)
{
	switch (type)
	{
	case 0:
		bitconvert.char_bit.da3 = buf[pos + 0];
		bitconvert.char_bit.da2 = buf[pos + 1];
		bitconvert.char_bit.da1 = buf[pos + 2];
		bitconvert.char_bit.da0 = buf[pos + 3];
		break;
	case 1:
		bitconvert.char_bit.da3 = buf[pos + 1];
		bitconvert.char_bit.da2 = buf[pos + 0];
		bitconvert.char_bit.da1 = buf[pos + 2];
		bitconvert.char_bit.da0 = buf[pos + 3];
		break;
	default:
		break;
	}
	return bitconvert.daushort[0];
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
		ret = (0.3905664 - sqrt(0.3905664 *0.3905664 - 4 * 0.0000667284 *(100 - om))) / 2 / 0.0000667284;
		ret = -ret;
	}

	return ret;
}
#ifdef __cplusplus
}
#endif
template <typename T1, typename T2> T1 sum(T1 x, T2 y)
{
	return x + y;
}

