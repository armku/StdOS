#pragma once

#include "Type.h"

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
	extern const byte nAsciiDot6X16[][16] ;
	
	extern unsigned char const g_Ascii12[];
	extern unsigned char const g_Ascii16[];
	
	extern unsigned char const g_RA8875_Ascii16_width[];
	extern unsigned char const g_RA8875_Ascii24_width[];
	extern unsigned char const g_RA8875_Ascii32_width[];


    /////////////////////////////////////////////////////////////////////////
    // ������ģ��                                                          //
    // ���ֿ�: ����12.dot ����ȡģ�¸�λ,��������:�����Ҵ��ϵ���         //
    /////////////////////////////////////////////////////////////////////////
    extern typFNT_GB12 const GB_12[];
    
    extern const char FONT_UNIT[][10]; //��λ
    extern const char FONT_CHANNEL[][10]; //����ͨ��
    extern const char FONT_INPUT_TYPE[][10]; //��������
    extern const char FONT_ALARM_TYPE[][10]; //��������;
    extern const char FONT_OUTPUT_TYPE[][10];          
    extern const char FONT_CONTROL_MANNER[][10];
    extern const char FONT_CONTROL_SE[][10];
    extern const uint FONT_BAUD_RATE[];	
    #ifdef __cplusplus
    }
#endif
