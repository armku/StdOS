#include "Math.h"

//�����
float Math::Sum(float buf[], int len,int pos)
{
	float ret = 0;
	for (int i = pos; i < len;i++)
	{
		ret += buf[len];
	}
	return ret;
}
//����ƽ��ֵ
float Math::Avg(float buf[], int len,int pos)
{
	if (len > 0)
		return Math::Sum(buf, len) / len;
	else
		return 0;
}
//���ֵ
float Math::Max(float buf[], int len,int pos)
{
	if (len <= 0)
		return buf[pos];

	auto max = buf[pos];
	for (int i = pos; i < len; i++)
	{
		max = MAX(max, buf[pos+i]);
	}
	return max;
	
}
//��Сֵ
float Math::Min(float buf[], int len,int pos)
{
	if (len <= 0)
		return buf[pos];

	auto min = buf[pos];
	for (int i = pos; i < len; i++)
	{
		min = MIN(min, buf[pos+i]);
	}
	return min;
}
//��������
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
//���� ����
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





