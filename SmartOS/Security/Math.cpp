#include "Math.h"

//计算和
float Math::Sum(float buf[], int len,int pos)
{
	float ret = 0;
	for (int i = pos; i < len;i++)
	{
		ret += buf[i];
	}
	return ret;
}
//计算平均值
float Math::Avg(float buf[], int len,int pos)
{
	if (len > 0)
		return Math::Sum(buf, len) / len;
	else
		return 0;
}
//计算和
float Math::Sum(int buf[], int len, int pos)
{
	int64_t ret = 0;
	for (int i = pos; i < len; i++)
	{
		ret += buf[i];
	}
	return ret;
}
//计算平均值
float Math::Avg(int buf[], int len, int pos)
{
	if (len > 0)
		return Math::Sum(buf, len) / len;
	else
		return 0;
}
//最大值
float Math::Max(float buf[], int len,int pos)
{
	if (len <= 0)
		return buf[pos];

	float max = buf[pos];
	for (int i = pos; i < len; i++)
	{
		max = MAX(max, buf[pos+i]);
	}
	return max;
	
}
//最小值
float Math::Min(float buf[], int len,int pos)
{
	if (len <= 0)
		return buf[pos];

	float min = buf[pos];
	for (int i = pos; i < len; i++)
	{
		min = MIN(min, buf[pos+i]);
	}
	return min;
}
//排序，升序
void Math::SortUp(float buf[],int len,int pos)
{
	int minpos=pos;
	for (int i = pos; i < pos + len; i++)
	{
		minpos = i;
		for (int j = i + 1; j < pos + len; j++)
		{
			if (buf[minpos] > buf[j])
				minpos = j;
		}
		float tmp = buf[minpos];
		buf[minpos] = buf[i];
		buf[i] = tmp;
	}
}
//排序 降序
void Math::SortDown(float buf[],int len,int pos)
{
	int maxpos = pos;
	for (int i = pos; i < pos + len; i++)
	{
		maxpos = i;
		for (int j = i + 1; j < pos + len; j++)
		{
			if (buf[maxpos] < buf[j])
				maxpos = j;
		}
		float tmp = buf[maxpos];
		buf[maxpos] = buf[i];
		buf[i] = tmp;
	}
}
const unsigned char crc8_tab[256] = {
	0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
	157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
	35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
	190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
	70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
	219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
	101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
	248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
	140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
	17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
	175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
	50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
	202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
	87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
	233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
	116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53
};

byte  Math::Crc8(void *buf, uint16_t len)
{
	unsigned char i, crc8 = 0;
	for (i = 0; i<len; i++)
		crc8 = crc8_tab[crc8^((byte *)buf)[i]];
	return crc8;
}
