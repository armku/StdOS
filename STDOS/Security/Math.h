#ifndef _MATH_H
#define _MATH_H
#include "Type.h"

#define MAX(a,b) (a)>(b)?(a):(b)
#define MIN(a,b) (a)<(b)?(a):(b)

class Math
{
public:
	static float Sum(float buf[], int len);//�����
	static float Avg(float buf[], int len);//����ƽ��ֵ
	static float Max(float buf[], int len);//���ֵ
	static float Min(float buf[], int len);//��Сֵ
	
private:

};

#endif // !_MATH_H
