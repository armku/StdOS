#include "type.h"

#ifndef _FONT_H
#define _FONT_H

#ifdef __cplusplus
extern "C"
{
#endif 
	/////////////////////////////////////////////////////////////////////////
	// ������ģ��                                                          //
	// ���ֿ�: ����16.dot ����ȡģ�¸�λ,��������:�����Ҵ��ϵ���         //
	/////////////////////////////////////////////////////////////////////////
	typedef struct  // ������ģ���ݽṹ
	{
		byte Index[3]; // ������������
		byte Msk[32]; // ����������
	} typFNT_GB16;
	// ���ֱ�
	// ������ʾͨ�����ñ��������ӡϵͳ�����޸Ŀ���

	typedef struct  // ������ģ���ݽṹ
	{
		byte Index[3]; // ������������
		byte Msk[24]; // ����������
	} typFNT_GB12;


	extern const byte ascii_table_8x16[95][16];
	extern const byte ascii_table_5x8[95][5];
	extern const byte ascii_table_4x8[][4];
	extern const typFNT_GB16 GB_16[];
	extern const byte ascBolt[][16];
	extern const byte ascii_table_6x12[][12];
	extern const byte nAsciiDot6X16[][16];

	/////////////////////////////////////////////////////////////////////////
	// ������ģ��                                                          //
	// ���ֿ�: ����12.dot ����ȡģ�¸�λ,��������:�����Ҵ��ϵ���         //
	/////////////////////////////////////////////////////////////////////////
	extern typFNT_GB12 const GB_12[];

#ifdef __cplusplus
}
#endif

#endif // !_FONT_H
