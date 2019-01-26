#ifndef _PARITY_H
#define _PARITY_H
#include <stdint.h>
class Parity
{
public:
	static bool OddParity(uint8_t *buf, uint32_t len);
	static bool EvenParity(uint8_t *buf, uint32_t len);

}; // ∆Ê≈º–£—È	
#endif
