/*
  Copyright (c) 2014 MakerLab.me & Andy Sze(andy.sze.mail@gmail.com)  All right reserved.
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

//#include "Arduino.h"
#include "wiring_digital.h"
#include "stm32f10x.h"
#include "../Bsp/Pin.h"
#include "../Porting/mcuGpio.h"

#ifdef __cplusplus
 extern "C" {
#endif
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (uint16_t)0xF0) << 6)))
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

extern void pinMode( uint32_t ulPin, uint32_t ulMode )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_TypeDef *_port = _GROUP(ulPin);
	int pin = 1 << ((ulPin % 16));// GPIO_Pin_0;
	//todo
	//rcc_clock_cmd((uint32_t)_port, ENABLE);

	switch ((uint8_t)ulMode)
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
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(_port, &GPIO_InitStructure);   //³õÊ¼»¯GPIOC¶Ë¿Ú

//	if ( ulPin > PINS_COUNT )
//    {
//        return ;
//    }

//  GPIO_TypeDef *gpio_port = g_APinDescription[ulPin].pPort;
//  uint16_t gpio_pin = g_APinDescription[ulPin].ulPin;

//  GPIO_InitTypeDef GPIO_InitStructure;

//#if defined (STM32F10X_HD) || (STM32F10X_MD)
//  RCC_APB2PeriphClockCmd(g_APinDescription[ulPin].ulPeripheral,ENABLE);
//#elif defined (STM32F40_41xxx)
//  RCC_AHB1PeriphClockCmd(g_APinDescription[ulPin].ulPeripheral,ENABLE);
//#endif

//  GPIO_InitStructure.GPIO_Pin = gpio_pin;

//	switch ( ulMode )
//    {
//        case INPUT:
//#if defined (STM32F10X_HD) || (STM32F10X_MD)
//          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//#elif defined (STM32F40_41xxx)
//          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//          GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//#endif
//        break ;

//        case INPUT_PULLUP:
//#if defined (STM32F10X_HD) || (STM32F10X_MD)
//          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//#elif defined (STM32F40_41xxx)
//          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//          GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//#endif
//        break ;

//        case INPUT_PULLDOWN:
//#if defined (STM32F10X_HD) || (STM32F10X_MD)
//          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
//#elif defined (STM32F40_41xxx)
//          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//          GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
//#endif
//        break;

//        case OUTPUT:
//#if defined (STM32F10X_HD) || (STM32F10X_MD)
//          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//#elif defined (STM32F40_41xxx)
//          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//          GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//          GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//#endif
//        break ;

//        case AF_OUTPUT_PUSHPULL:	//Used internally for Alternate Function Output PushPull(TIM, UART, SPI etc)
//#if defined (STM32F10X_HD) || (STM32F10X_MD)
//          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//#elif defined (STM32F40_41xxx)
//          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//          GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//          GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//#endif
//        break;

//        case AF_OUTPUT_DRAIN:		//Used internally for Alternate Function Output Drain(I2C etc)
//#if defined (STM32F10X_HD) || (STM32F10X_MD)
//          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
//          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//#elif defined (STM32F40_41xxx)
//          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//          GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//#endif
//        break;

//        case AN_INPUT:				//Used internally for ADC Input
//#if defined (STM32F10X_HD) || (STM32F10X_MD)
//          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//#elif defined (STM32F40_41xxx)
//          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//          GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
//#endif
//        break;

//        default:
//        break ;
//    }

//  GPIO_Init(gpio_port, &GPIO_InitStructure);
}

extern void digitalWrite( uint32_t ulPin, uint32_t ulVal )
{
#define FAST
		
#ifdef FAST
	GPIO_TypeDef *_port = _GROUP(ulPin);
	int pin= 1 << ((ulPin % 16));// GPIO_Pin_0;
#endif // FAST

	if (ulVal)
	{
#ifdef FAST
		_port->BSRR = pin;
#else
		GPIO_SetBits(_GROUP(ulPin), _PORT(ulPin));
#endif
		
	}
	else
	{
#ifdef  FAST
		_port->BRR = pin;
#else
		GPIO_ResetBits(_GROUP(ulPin), _PORT(ulPin));
#endif //  FAST
	}
//  /* Handle */
//	if ( ulPin > PINS_COUNT )
//  {
//    return ;
//  }

//  if ( ulVal == HIGH )
//  {
//#if defined (STM32F10X_HD) || (STM32F10X_MD)
//    g_APinDescription[ulPin].pPort->BSRR = g_APinDescription[ulPin].ulPin;
//#elif defined (STM32F40_41xxx)
//    g_APinDescription[ulPin].pPort->BSRRL = g_APinDescription[ulPin].ulPin;
//#endif
//  }
//  else
//  {
//#if defined (STM32F10X_HD) || (STM32F10X_MD)
//    g_APinDescription[ulPin].pPort->BRR = g_APinDescription[ulPin].ulPin;
//#elif defined (STM32F40_41xxx)
//    g_APinDescription[ulPin].pPort->BSRRH = g_APinDescription[ulPin].ulPin;
//#endif
//  }
}

extern int digitalRead( uint32_t ulPin )
{
//	if ( ulPin > PINS_COUNT )
//    {
//        return LOW ;
//    }

//	return GPIO_ReadInputDataBit(g_APinDescription[ulPin].pPort,g_APinDescription[ulPin].ulPin) ;
}

#ifdef __cplusplus
}
#endif
