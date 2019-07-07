#ifndef _FONT_H
#define _FONT_H

#include "Type.h"

#ifdef __cplusplus
extern "C"
{
#endif 
	/////////////////////////////////////////////////////////////////////////
	// 汉字字模表                                                          //
	// 汉字库: 宋体16.dot 纵向取模下高位,数据排列:从左到右从上到下         //
	/////////////////////////////////////////////////////////////////////////
	typedef struct  // 汉字字模数据结构
	{
		uint8_t Index[3]; // 汉字内码索引
		uint8_t Msk[32]; // 点阵码数据
	} typFNT_GB16;
	// 汉字表：
	// 测量显示通道设置报警输出打印系统密码修改开锁

	typedef struct  // 汉字字模数据结构
	{
		uint8_t Index[3]; // 汉字内码索引
		uint8_t Msk[24]; // 点阵码数据
	} typFNT_GB12;


	extern const uint8_t ascii_table_8x16[95][16];
	extern const uint8_t ascii_table_5x8[95][5];
	extern const uint8_t ascii_table_4x8[][4];
	extern const typFNT_GB16 GB_16[];
	extern const uint8_t ascBolt[][16];
	extern const uint8_t ascii_table_6x12[][12];
	extern const uint8_t nAsciiDot6X16[][16];

	/////////////////////////////////////////////////////////////////////////
	// 汉字字模表                                                          //
	// 汉字库: 宋体12.dot 纵向取模下高位,数据排列:从左到右从上到下         //
	/////////////////////////////////////////////////////////////////////////
	extern typFNT_GB12 const GB_12[];

#ifdef __cplusplus
}
#endif

#endif // !_FONT_H
