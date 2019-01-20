#include "std_port_gpio.h"

//���ùܽ�
void mcuGpio::SetPin(Pin pin)
{
	//this->_port = GPIOA;
	//this->_pin = GPIO_Pin_0;
	
	this->_Pin = pin;
	switch (pin>>4)
	{
	case PA0>>4:
		this->_port =GPIOA;
		break;
	case PB0>>4:
		this->_port = GPIOB;
		break;
	case PC0>>4:
		this->_port = GPIOC;
		break;
	case PD0>>4:
		this->_port = GPIOD;
		break;
	case PE0>>4:
		this->_port = GPIOE;
		break;
	case PF0>>4:
		this->_port = GPIOF;
		break;
	case PG0>>4:
		this->_port = GPIOG;
		break;
	case PH0>>4:
#ifdef STM32F10X_HD
		this->_port = GPIOH;
#endif // STM32F10X_HD
		break;
	case PI0>>4:
#ifdef STM32F10X_HD
		this->_port = GPIOI;
#endif // STM32F10X_HD
		break;
	default:
		break;
	}

	this->_pin = 1 << (pin % 16);// GPIO_Pin_0;
}
/**
  *@brief    ���캯��
  *@param    port port; pin pin
  *@retval   None
  */
mcuGpio::mcuGpio(GPIO_TypeDef *port, uint16_t pin)
{
	uint8_t temp1, temp2;
	_port = port;
	_pin = pin;
	temp1 = (((uint32_t)port - APB2PERIPH_BASE) >> 10) - 2;

	for (int i = 0; i <= 15; i++)
	{
		if (((_pin >> i) & 0xfffe) == 0)
		{
			temp2 = i;
			break;
		}
	}
	this->_Pin = (Pin)(temp1 * 16 + temp2);
}
/**
  *@brief    GPIOģʽ����
  *@param    mode:PIN_MODEö�ٱ�������
  *@retval   None
  */
void mcuGpio::mode(PIN_MODE mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//todo
	//rcc_clock_cmd((uint32_t)_port, ENABLE);

	switch ((uint8_t)mode)
	{
		/*analog input mode
		 */
	case AIN:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		break;

		/* digital input mode
		 */
	case INPUT:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		break;

	case INPUT_PD:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		break;

	case INPUT_PU:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		break;

		/*digital output mode
		 */
	case OUTPUT_OD:
	case OUTPUT_OD_PU:
	case OUTPUT_OD_PD:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		break;

	case OUTPUT:
	case OUTPUT_PP:
	case OUTPUT_PP_PU:
	case OUTPUT_PP_PD:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		break;

		/*af mode
		 */
	case AF_OD:
	case AF_OD_PU:
	case AF_OD_PD:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
		break;

	case AF_PP:
	case AF_PP_PU:
	case AF_PP_PD:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		break;
		/* if parament is other mode,set as INPUT mode
		 */
	default:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		break;
	}
	GPIO_InitStructure.GPIO_Pin = _pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(_port, &GPIO_InitStructure);   //��ʼ��GPIOC�˿�
}
/**
  *@brief    GPIOģʽ����,f1����af_configration����
  *@param    mode: PIN_MODEö�ٱ������ͣ�af_configration �ڶ�����
  *@retval   None
  */
void mcuGpio::mode(PIN_MODE mode, uint8_t af_configration)
{
	mcuGpio::mode(mode);
}
/**
 *@brief    GPIO����ߵ�ƽ
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::set()
{
	_port->BSRR = _pin;
}

/**
 *@brief    GPIO����͵�ƽ
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::reset()
{
	_port->BRR = _pin;
}


/**
 *@brief    GPIO�������
 *@param    val��1������ߵ�ƽ��0������͵�ƽ
 *@retval   NONE
*/
void mcuGpio::write(uint8_t val)
{
	// val==0,ִ��_port->BRR = _pin������_port->BSRR = _pin
	(val == 0) ? (_port->BRR = _pin) : (_port->BSRR = _pin);
}


/**
 *@brief    GPIO����ߵ�ƽ
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::read(uint8_t *val)
{
	*val = _port->IDR & _pin;
}


/**
 *@brief    ��ȡGPIO����״̬
 *@param    NONE
 *@retval   �������ŵ�ǰ�ĵ�ƽ״̬
*/
uint8_t mcuGpio::read(void)
{
	return ((_port->IDR & _pin) == _pin) ? (1) : (0);
}

/**
 *@brief    GPIO�����ƽ��ת
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::toggle()
{
	_port->ODR ^= _pin;
}

// �൱��read()
int mcuGpio::operator =(mcuGpio &)
{
	return ((_port->IDR & _pin) == _pin) ? (1) : (0);
}
// �൱��read()
mcuGpio::operator int()
{
	return ((_port->IDR & _pin) == _pin) ? (1) : (0);
}
// �൱��write()
mcuGpio mcuGpio::operator= (int value)
{
	(value == 0) ? (_port->BRR = _pin) : (_port->BSRR = _pin);
	return *this;
}

// ��ȡ�����������ţ������Ϊtrue�򷵻�true����ҪΪ��һ���ŷ���
bool mcuGpio::Read()
{
	return this->Invert ? !read() : read();
}


//
void OpenPeriphClock(Pin pin)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << (pin >> 4), ENABLE);
}
// ��ȡ������
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (uint16_t)0xF0) << 6)))
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

GPIO_TypeDef *IndexToGroup(uint8_t index)
{
	return ((GPIO_TypeDef*)(GPIOA_BASE + (index << 10)));
}

uint8_t GroupToIndex(GPIO_TypeDef *group)
{
	return (uint8_t)(((int)group - GPIOA_BASE) >> 10);
}

void Port_OnOpen(Pin pin)
{
	int pinindex;
	int portname;

	if (pin == PA15 || pin == PB3 || pin == PB4)
	{
		if (pin == P0)
			pinindex = 48;
		else
			pinindex = pin & 0xF;

		if (pin == P0)
			portname = 95;
		else
			portname = (pin >> 4) + 'A';
		//debug_printf("Close JTAG Pin P%c%d \r\n", portname, pinindex);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << (pin >> 4), ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	}
	pinindex = pinindex;
	portname = portname;
}

