#ifndef _MATH_H
#define _MATH_H
#include "Type.h"

#define MAX(a,b) (a)>(b)?(a):(b)
#define MIN(a,b) (a)<(b)?(a):(b)

class Math
{
public:
	static float Sum(float buf[], int len,int pos=0);//�����
	static float Avg(float buf[], int len,int pos=0);//����ƽ��ֵ
	static float Sum(int buf[], int len, int pos = 0);//�����
	static float Avg(int buf[], int len, int pos = 0);//����ƽ��ֵ
	static float Max(float buf[], int len,int pos=0);//���ֵ
	static float Min(float buf[], int len,int pos=0);//��Сֵ
	static void SortUp(float buf[],int len,int pos=0);//��������
	static void SortDown(float buf[],int len,int pos=0);//���� ����
	static uint16_t Crc16(uint8_t * buf, uint16_t len);
	static uint8_t  Crc8(uint8_t *buf, uint16_t len);
	
private:

};

#endif // !_MATH_H
