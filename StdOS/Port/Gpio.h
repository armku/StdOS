#ifndef _STD_GPIO_H
#define _STD_GPIO_H

#include <stdint.h>
#include "../../Bsp/Pin.h"

typedef enum
{
	OUTPUT = 0x00,

	OUTPUT_PP,
	OUTPUT_PP_PU,
	OUTPUT_PP_PD,

	OUTPUT_OD,
	OUTPUT_OD_PU,
	OUTPUT_OD_PD,

	AIN,

	INPUT,
	INPUT_PD,
	INPUT_PU,

	AF_OD,
	AF_OD_PU,
	AF_OD_PD,

	AF_PP,
	AF_PP_PU,
	AF_PP_PD,
} PIN_MODE;

class Gpio
{
public:
	Gpio();
	virtual void SetPin(Pin pin)=0;//���ùܽ�
	virtual void mode(PIN_MODE mode) = 0;
	virtual void read(uint8_t *val) = 0;
	virtual uint8_t read(void) = 0;
	/*virtual void toggle() = 0;*/

public:
	bool Empty() const;
	Gpio& Set(Pin pin);	// ��������
public:
	Pin _Pin;
	bool	Opened;		// �Ƿ��Ѿ���
	uint8_t	Invert = 0;	// �Ƿ����������
	bool OpenDrain = false;	// �Ƿ�©���
};

#endif // !_STD_GPIO_H
