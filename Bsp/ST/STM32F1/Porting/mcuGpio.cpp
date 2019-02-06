#include "mcuGpio.h"
#include "wiring_digital.h"

//设置管脚
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
  *@brief    构造函数
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
  *@brief    GPIO模式设置
  *@param    mode:PIN_MODE枚举变量类型
  *@retval   None
  */
void mcuGpio::mode(PIN_MODE mode)
{
	pinMode(this->_Pin, mode);
}

/**
 *@brief    GPIO输出高电平
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::read(uint8_t *val)
{
	*val = _port->IDR & _pin;
}


/**
 *@brief    读取GPIO引脚状态
 *@param    NONE
 *@retval   返回引脚当前的电平状态
*/
uint8_t mcuGpio::read(void)
{
	return digitalRead(this->_Pin);
}

/**
 *@brief    GPIO输出电平翻转
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::toggle()
{
	digitalToggle(this->_Pin);
}

// 相当与read()
int mcuGpio::operator =(mcuGpio &)
{
	return ((_port->IDR & _pin) == _pin) ? (1) : (0);
}
// 相当与read()
mcuGpio::operator int()
{
	return this->read();
}

// 读取本组所有引脚，任意脚为true则返回true，主要为单一引脚服务
bool mcuGpio::Read()
{
	return this->Invert ? !read() : read();
}

void mcuGpio::Write(Pin pin, bool value)
{
	digitalWrite(pin, value);
}
void mcuGpio::Write(bool value)
{
	if (this->_Pin == P0)
		return;
	this->Write(this->_Pin, this->Invert ? !value : value);
}
GPIO_TypeDef *IndexToGroup(uint8_t index);
bool mcuGpio::Open()
{
	if (this->Opened == false)
	{
		if (_Pin != P0)
		{
			// 打开时钟
			int gi = _Pin >> 4;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << gi, ENABLE);
			// PA15/PB3/PB4 需要关闭JTAG
			switch (_Pin)
			{
			case PA15:
			case PB3:
			case PB4:
			{
				//debug_printf("Close JTAG for P%c%d\r\n", _PIN_NAME(_Pin));
				// PA15是jtag接口中的一员 想要使用 必须开启remap
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
				GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
				break;
			}
			}

			GPIO_InitTypeDef gpio;
			// 特别要慎重，有些结构体成员可能因为没有初始化而酿成大错
			GPIO_StructInit(&gpio);
			gpio.GPIO_Pin = 1 << (this->_Pin & 0x0F);
			gpio.GPIO_Speed = GPIO_Speed_50MHz;

			GPIO_Init(IndexToGroup(this->_Pin >> 4), &gpio);
		}

		this->Opened = true;
	}
	return true;
}
////////////////////////////////////////////////////////////////////
uint8_t GroupToIndex(GPIO_TypeDef *group);
void Port_OnOpen(Pin pin);

mcuGpio::mcuGpio()
{
	this->OpenDrain = 0;
}

mcuGpio::mcuGpio(Pin pin, uint8_t invert, bool openDrain)
{
	this->Opened = false;

	this->Invert = 2;
	this->OpenDrain = false;
	this->OpenDrain = openDrain;
	this->Invert = invert;
	if (pin != P0)
	{
		this->Set(pin);
		this->Open();
	}
}

////////////////////////////////////////////////////////////////////////

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

