#ifndef _BSP_USART_FIFO_H_
    #define _BSP_USART_FIFO_H_
    #include "Sys.h"
        
	class FIFO
	{
		public:
			void SetBuf(uint8_t *buf, int len);
			int Push(uint8_t da);
			int Pop(uint8_t *da);
			bool BufIsEmpty();
			bool BufIsFull();
			void Clear();
		public:
			uint8_t *pBuf; /* 缓冲区 */			
			int Read; /* 缓冲区读指针 */
			int Count; /* 数据个数 */
			int BufSize; /* 缓冲区大小 */
		private:			
			int Write; /* 缓冲区写指针 */			
	};

    class UART_T
    {
        public:
            USART_TypeDef *uart; /* STM32内部串口设备指针 */
			FIFO tx;
			FIFO rx;
            void(*ReciveNew)(uint8_t _byte); /* 串口收到数据的回调函数指针 */
    };

    void bsp_InitUart(void);
    void comSendBuf(COM _ucPort, uint8_t *_ucaBuf, int _usLen);
    void comSendChar(COM _ucPort, uint8_t _ucByte);
    uint8_t comGetChar(COM _ucPort, uint8_t *_pByte);
#endif
