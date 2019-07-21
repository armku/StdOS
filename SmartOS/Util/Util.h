#ifndef _UTIL_H
#define _UTIL_H

#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

	enum MyEnum_20190128
	{
		BIG_ENDIAN = 3,					// ���ģʽ
		LITTLE_ENDIAN = 0,				// С��ģʽ
		BIG_ENDIAN_BYTE_SWAP = 1,		// ���ģʽ-�ֽڽ���
		LITTLE_ENDIAN_BYTE_SWAT = 2,		// С��ģʽ-�ֽڽ���
	};

	void bubbleSort(float *buf, uint len); //���ø�����
	void SetBufFloat(byte buf[], float da, uint16_t pos = 0, byte type = 0);//���ø����� 0:С�� 1�����
	float GetBufFloat(byte buf[], uint16_t pos = 0, byte type = 0);//���ø����� 0:С�� 1�����
	uint16_t GetBufUshort(byte buf[], uint16_t pos, byte type = 0);
	void SetBufUshort(byte buf[], uint16_t pos, uint16_t da, byte type = 0); //�ӵ������¶�
	float GetTFromPT100(float om);
	float GetValueDot(float value, int dot);//����С�����ȡֵ	
	void BoxCar(int* buf, int len);//���β�������,Ŀǰ�޶�100��������

#ifdef __cplusplus
}
#endif
#endif
