#ifndef _LCDOCM_H
#define _LCDOCM_H
#include "Drivers\OCM240128\OCM240128.h"
class LCDOCM :public OCM240128
{
public:
	void DispStrAsc8x16(uint8_t x, uint8_t y, uint8_t *text, uint8_t num, uint8_t mode);
	void DispStrHz16x16(uint8_t x, uint8_t y, uint8_t *text, uint8_t num, uint8_t mode);
	void DispAscNum16x16(uint8_t x, uint8_t y, uint8_t text);
	void DisplayHz16x16(uint8_t x, uint8_t y, uint8_t *text, uint8_t mode);
	void DisplayAsc8x16(uint8_t x, uint8_t y, uint8_t *text, uint8_t mode);
	void Clrchar(uint8_t x, uint8_t y, uint16_t count);
};

#endif // _LCDOCM_H
