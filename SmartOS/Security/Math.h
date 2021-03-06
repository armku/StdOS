#ifndef _MATH_H
#define _MATH_H
#include "Type.h"

#define MAX(a,b) (a)>(b)?(a):(b)
#define MIN(a,b) (a)<(b)?(a):(b)

class Math
{
public:
	static float Sum(float buf[], int len,int pos=0);//计算和
	static float Avg(float buf[], int len,int pos=0);//计算平均值
	static float Sum(int buf[], int len, int pos = 0);//计算和
	static float Avg(int buf[], int len, int pos = 0);//计算平均值
	static float Max(float buf[], int len,int pos=0);//最大值
	static float Min(float buf[], int len,int pos=0);//最小值
	static void SortUp(float buf[],int len,int pos=0);//排序，升序
	static void SortDown(float buf[],int len,int pos=0);//排序 降序
	static ushort Crc16(void * buf, ushort len);
	static byte  Crc8(void *buf, ushort len);
	
private:

};

#endif // !_MATH_H
