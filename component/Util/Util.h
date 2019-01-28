#ifndef _UTIL_H
#define _UTIL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	enum MyEnum_20190128
	{
		BIG_ENDIAN = 3,					// 大端模式
		LITTLE_ENDIAN = 0,				// 小端模式
		BIG_ENDIAN_BYTE_SWAP = 1,		// 大端模式-字节交换
		LITTLE_ENDIAN_BYTE_SWAT = 2,		// 小端模式-字节交换
	};

	void bubbleSort(float *buf, uint32_t len); //设置浮点数
	void SetBufFloat(uint8_t buf[], float da, uint16_t pos = 0, uint8_t type = 0);//设置浮点数 0:小端 1：大端
	float GetBufFloat(uint8_t buf[], uint16_t pos = 0, uint8_t type = 0);//设置浮点数 0:小端 1：大端
	uint16_t GetBufUshort(uint8_t buf[], uint16_t pos, uint8_t type = 0);
	void SetBufUshort(uint8_t buf[], uint16_t pos, uint16_t da, uint8_t type = 0); //从电阻获得温度
	float GetTFromPT100(float om);
	float GetValueDot(float value, int dot);//根据小数点获取值	
	void BoxCar(int* buf, int len);//矩形波过滤器,目前限定100个数据量

#ifdef __cplusplus
}
#endif
#endif
