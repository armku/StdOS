#include "Arduino.h"
#include "HardwareSerial.h"
#include "HardwareSerial_private.h"
#include "stm32f10x.h"

//#include "../Bsp/ST/STM32F1/Porting/USART.h"
//#include "../Bsp/ST/STM32F1/Porting/Interrupt.h"
//#include "../Bsp/ST/STM32F1/Porting/Configuration.h"
//#include "Sys.h"

#if defined(HAVE_HWSERIAL0)

HardwareSerial0 Serial;

HardwareSerial0::HardwareSerial0()
{

}
void HardwareSerial0::begin(unsigned long baud, byte config)
{
	
}
#include "Sys.h"
size_t HardwareSerial0::write(uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(USART1, ch);

	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return 1;
}
#endif // HAVE_HWSERIAL0

//////////////////////////////////////////////////////////////////////////////////
