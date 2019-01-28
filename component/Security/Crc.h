#ifndef __CRC_H__
#define __CRC_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	// CRC 校验算法
	uint8_t CRC8(uint8_t *buf, uint32_t len);
	uint16_t CRC16RTU(uint8_t *pszBuf, uint32_t unLength);
	uint16_t CRC16CCITT(uint8_t *pszBuf, uint32_t unLength);
	uint16_t CRC16Default(uint8_t *pszBuf, uint32_t unLength);
	uint64_t CRC32Default(uint8_t *pszBuf, uint64_t ulLength);
	uint64_t Reflect(uint64_t ref, char ch);

#ifdef __cplusplus
}
#endif

#endif
