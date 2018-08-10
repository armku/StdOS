#include "Math.h"

//计算和
float Math::Sum(float buf[], int len)
{
	float ret = 0;
	for (int i = 0; i < len;i++)
	{
		ret += buf[len];
	}
	return ret;
}
//计算平均值
float Math::Avg(float buf[], int len)
{
	if (len > 0)
		return Math::Sum(buf, len) / len;
	else
		return 0;
}
//最大值
float Math::Max(float buf[], int len)
{
	if (len <= 0)
		return buf[0];

	float max = buf[0];
	for (int i = 0; i < len; i++)
	{
		max = MAX(max, buf[i]);
	}
	return max;
	
}
//最小值
float Math::Min(float buf[], int len)
{
	if (len <= 0)
		return buf[0];

	float min = buf[0];
	for (int i = 0; i < len; i++)
	{
		min = MIN(min, buf[i]);
	}
	return min;
}
