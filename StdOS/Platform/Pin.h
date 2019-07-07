#include "ArduinoAPI/gpio.h"

#ifndef _PIN_H_
#define _PIN_H_
/* 针脚 ------------------------------------------------------------------*/

/* 串口定义 */
enum COM
{
	COM1 = 0,
	COM2 = 1,
	COM3 = 2,
	COM4 = 3,
	COM5 = 4,
	COM6 = 5,
	COM7 = 6,
	COM8 = 7,
	COM_NONE = 0xFF
};

enum SPI
{
	Spi1 = 0,
	Spi2 = 1,
	Spi3 = 2
};

enum TIMER
{
	Timer0 = 0xFF,
	Timer1 = 0,
	Timer2 = 1,
	Timer3 = 2,
	Timer4 = 3,
	Timer5 = 4,
	Timer6 = 5,
	Timer7 = 6,
	Timer8 = 7,
	Timer9 = 8,
	Timer10 = 9,
	Timer11 = 10,
	Timer12 = 11,
	Timer13 = 12,
	Timer14 = 13,
	Timer15 = 14,
	Timer16 = 15,
	Timer17 = 16,
	Timer18 = 17,
};

enum CAN
{
	Can1 = 0,
	Can2 = 1,
};

#endif
