/*
  HardwareSerial.h - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
*/

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>

#include "Stream.h"

// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer (I think), in which head is the index of the location
// to which to write the next incoming character and tail is the index of the
// location from which to read.
// NOTE: a "power of 2" buffer size is reccomended to dramatically
//       optimize all the modulo operations for ring buffers.
// WARNING: When buffer sizes are increased to > 256, the buffer index
// variables are automatically increased in size, but the extra
// atomicity guards needed for that are not implemented. This will
// often work, but occasionally a race condition can occur that makes
// Serial behave erratically. See https://github.com/arduino/Arduino/issues/2405
#if !defined(SERIAL_TX_BUFFER_SIZE)
#if ((RAMEND - RAMSTART) < 1023)
#define SERIAL_TX_BUFFER_SIZE 16
#else
#define SERIAL_TX_BUFFER_SIZE 64
#endif
#endif
#if !defined(SERIAL_RX_BUFFER_SIZE)
#if ((RAMEND - RAMSTART) < 1023)
#define SERIAL_RX_BUFFER_SIZE 16
#else
#define SERIAL_RX_BUFFER_SIZE 64
#endif
#endif
#if (SERIAL_TX_BUFFER_SIZE>256)
typedef uint16_t tx_buffer_index_t;
#else
typedef uint8_t tx_buffer_index_t;
#endif
#if  (SERIAL_RX_BUFFER_SIZE>256)
typedef uint16_t rx_buffer_index_t;
#else
typedef uint8_t rx_buffer_index_t;
#endif

// Define config for Serial.begin(baud, config);
#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06
#define SERIAL_5N2 0x08
#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
#define SERIAL_5E1 0x20
#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_5E2 0x28
#define SERIAL_6E2 0x2A
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_5O1 0x30
#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_5O2 0x38
#define SERIAL_6O2 0x3A
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E

class HardwareSerial : public Stream
{
  protected:    
    // Has any byte been written to the UART since begin()
    bool _written;

    volatile rx_buffer_index_t _rx_buffer_head;
    volatile rx_buffer_index_t _rx_buffer_tail;
    volatile tx_buffer_index_t _tx_buffer_head;
    volatile tx_buffer_index_t _tx_buffer_tail;

    // Don't put any members after these buffers, since only the first
    // 32 bytes of this struct can be accessed quickly using the ldd
    // instruction.
    unsigned char _rx_buffer[SERIAL_RX_BUFFER_SIZE];
    unsigned char _tx_buffer[SERIAL_TX_BUFFER_SIZE];

  public:
    inline HardwareSerial();
    void begin(unsigned long baud) { begin(baud, SERIAL_8N1); }
    void begin(unsigned long, uint8_t);
    void end();
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual int availableForWrite(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool() { return true; }

    // Interrupt handlers - Not intended to be called externally
    inline void _rx_complete_irq(void);
    void _tx_udr_empty_irq(void);
};

class HardwareSerial0 : public HardwareSerial
{
public:
	HardwareSerial0();
	void begin(unsigned long baud) { begin(baud, SERIAL_8N1); }
	void begin(unsigned long, uint8_t);
	virtual size_t write(uint8_t);
};

extern HardwareSerial0 Serial; 

#if defined(UBRR1H)
  extern HardwareSerial Serial1;
  #define HAVE_HWSERIAL1
#endif
#if defined(UBRR2H)
  extern HardwareSerial Serial2;
  #define HAVE_HWSERIAL2
#endif
#if defined(UBRR3H)
  extern HardwareSerial Serial3;
  #define HAVE_HWSERIAL3
#endif

extern void serialEventRun(void) __attribute__((weak));

///////////////////////////////////////////////////////////////////////////////////
#include "stm32f10x.h"
#include "../Bsp/ST/STM32F1/Porting/FIFOBuffer.h"
#include "../Bsp/ST/STM32F1/Porting/Configuration.h"
#include "Port.h"
//串口数据处理
typedef struct
{
	int TimeCnt;//接收到数据的时间
	int UsartReadCnt;//接收到的数据长度
	int UsartIdleCnt;//串口空闲时间
	int UsartFlag;//接收到一帧数据
	int UsartReadCntCopy;
}UsartKernel_T;

extern UsartKernel_T UsartKernel[];//串口数据处理
/******************************************************************************************************/
/****configuration，使用前请自行配置****/
#define USART_TX_BUFFER_SIZE     200              //USART BUFFER FIFO SIZE
#define USART_RX_BUFFER_SIZE     200              //USART BUFFER FIFO SIZE
#define USART_DMA_TX_BUFFER_SIZE 200             //USART DMA BUFFER SIZE

/*******************************************************************************************************/

class USART
{
private:
	bool isBusySend;

	uint32_t            mBaudrate; //baudrate of usart
	uint32_t			mRemapvalue;// remap value	
	uint16_t            mParity;   //parity of usart
	uint16_t            mWordLen;  //world length of usart
	uint16_t            mStopBits; //stop bits of usart
	uint16_t            mTxPin;    //Tx gpio pin
	uint16_t            mRxPin;    //Rx gpio pin
	uint8_t             mIRQn;     //USART IRQn
	GPIO_TypeDef*  mPort;     //GPIO port
	USART_TypeDef* mUSARTx;   //USARTx
	uint32_t            mGPIORcc;  //GPIO Clock
	uint32_t            mUSARTRcc; //USART Clock
	bool           mRemap;    //gpio remap flag
	uint8_t             mPrePri;   //preemption priority
	uint8_t             mSubPri;   //sub priority
	uint8_t             mPriGroup; //priority group
	FIFOBuffer<uint8_t, USART_TX_BUFFER_SIZE>  mTxBuf;  //USART Tx Buffer
	FIFOBuffer<uint8_t, USART_RX_BUFFER_SIZE>  mRxBuf;  //USART Rx Buffer
	uint8_t mPrecision;   //when show precision after dot "."  when use "<<" to show float value

	uint16_t mTxOverflow; //Tx overflow byte count
	uint16_t mRxOverflow; //Rx overflow byte count

private:
	void InitGPIO();
	void InitUSART();
	void InitNVIC();
public:
	USART(USART_TypeDef* USARTx, uint32_t baud, uint8_t priGroup = 3, uint8_t prePri = 7, uint8_t subPri = 1, bool remap = false, uint32_t remapvalue = 1, uint16_t parity = USART_Parity_No, uint16_t wordLen = USART_WordLength_8b, uint16_t stopBits = USART_StopBits_1);
	void Initialize();
	uint64_t LastRcvTime;
	int RxCnt;
	int TxCnt;
	bool FlagIdleOK;//发生空闲中断 不建议使用，容易误判
	//////////////////////////
	///@bief 设置波特率
	///@param baudRate 波特率大小
	//////////////////////////
	void SetBaudRate(uint32_t baudRate);

	virtual bool SendBytes(uint8_t txData[], uint16_t size);
	bool SendBytes(char txData[], uint16_t size) { return this->SendBytes((uint8_t*)txData, size); }
	virtual bool SendByte(uint8_t data);
	virtual bool GetBytes(uint8_t data[], uint16_t num);
	virtual bool GetByte(uint8_t &data);

	virtual uint16_t TxSize();
	virtual uint16_t RxSize();

	virtual uint16_t TxOverflowSize();
	virtual uint16_t RxOverflowSize();

	virtual void ClearRxBuf();
	virtual void ClearTxBuf();

	void IRQ();
	void(*OnReceive)(void* param);

	USART& operator<<(int val);
	USART& operator<<(double val);
	USART& operator<<(const char* pStr);
	int SendTimeMs(int buflen)
	{
		if (this->mBaudrate < 100)
			return 1;
		return buflen * 8 * 1000 / this->mBaudrate + 1;
	}
	mcuGpio* RS485;
#ifdef USE_USART_DMA
private:
	uint8_t                   mDMAIRQn;
	DMA_Channel_TypeDef* mDMATxCh;
	uint32_t                  mDMATCFlag;
	uint32_t                  mDMAGLFlag;
	uint8_t                   mDMATxBuf[USART_DMA_TX_BUFFER_SIZE];
	void InitDMA();
public:
	void DMAIRQ();
#endif
};

/**
===========================================================================
|			remap value				|
usartName	|	0x00	|	0x01	|	0x11	|
usart1Tx	|	PA9		|	PB6		|			|
usart1Rx	|	PA10	|	PB7		|			|
usart2Tx	|	PA2		|	PD5		|			|
usart2Rx	|	PA3		|	PD6		|			|
usart3Tx	|	PB10	|	PC10	|	PD8		|
usart3Rx	|	PB11	|	PC11	|	PD9		|
===========================================================================
*/

//485通信 1：发送模式 0：接收模式
#endif
