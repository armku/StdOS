#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H



#include "stm32f10x.h"



/************************** SPI Flash 连接引脚定义********************************/
#define      macSPIx                                     SPI1
#define      macSPI_APBxClock_FUN                        RCC_APB2PeriphClockCmd
#define      macSPI_CLK                                  RCC_APB2Periph_SPI1

#define      macSPI_CS_APBxClock_FUN                     RCC_APB2PeriphClockCmd
#define      macSPI_CS_CLK                               RCC_APB2Periph_GPIOA    
#define      macSPI_CS_PORT                              GPIOA
#define      macSPI_CS_PIN                               GPIO_Pin_4

#define      macSPI_SCK_APBxClock_FUN                    RCC_APB2PeriphClockCmd
#define      macSPI_SCK_CLK                              RCC_APB2Periph_GPIOA   
#define      macSPI_SCK_PORT                             GPIOA   
#define      macSPI_SCK_PIN                              GPIO_Pin_5

#define      macSPI_MISO_APBxClock_FUN                   RCC_APB2PeriphClockCmd
#define      macSPI_MISO_CLK                             RCC_APB2Periph_GPIOA    
#define      macSPI_MISO_PORT                            GPIOA 
#define      macSPI_MISO_PIN                             GPIO_Pin_6

#define      macSPI_MOSI_APBxClock_FUN                   RCC_APB2PeriphClockCmd
#define      macSPI_MOSI_CLK                             RCC_APB2Periph_GPIOA    
#define      macSPI_MOSI_PORT                            GPIOA 
#define      macSPI_MOSI_PIN                             GPIO_Pin_7



/************************** SPI Flash 函数宏定义********************************/
#define      macSPI_FLASH_CS_ENABLE()                       GPIO_ResetBits( macSPI_CS_PORT, macSPI_CS_PIN )
#define      macSPI_FLASH_CS_DISABLE()                      GPIO_SetBits( macSPI_CS_PORT, macSPI_CS_PIN )



void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 SPI_FLASH_ReadID(void);
u32 SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);


u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);


#endif /* __SPI_FLASH_H */

