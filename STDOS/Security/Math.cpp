#include "Math.h"

//�����
float Math::Sum(float buf[], int len)
{
	float ret = 0;
	for (int i = 0; i < len;i++)
	{
		ret += buf[len];
	}
	return ret;
}
//����ƽ��ֵ
float Math::Avg(float buf[], int len)
{
	if (len > 0)
		return Math::Sum(buf, len) / len;
	else
		return 0;
}
//���ֵ
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
//��Сֵ
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
