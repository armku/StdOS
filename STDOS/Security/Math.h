#ifndef _MATH_H
#define _MATH_H
#include "Type.h"

#define MAX(a,b) (a)>(b)?(a):(b)
#define MIN(a,b) (a)<(b)?(a):(b)

class Math
{
public:
	static float Sum(float buf[], int len);//计算和
	static float Avg(float buf[], int len);//计算平均值
	static float Max(float buf[], int len);//最大值
	static float Min(float buf[], int len);//最小值
	
private:

};

#endif // !_MATH_H
