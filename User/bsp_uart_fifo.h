#ifndef _BSP_USART_FIFO_H_
    #define _BSP_USART_FIFO_H_
    #include "Sys.h"
        
	class FIFO
	{
		public:
			void SetBuf(byte *buf,int len);
			void Push(byte da);
			byte Pop();
			void Clear();
			bool Full();//缓冲区满
			bool Empty();//缓冲区空			
		private:
			byte *Buf; /* 发送缓冲区 */
			ushort BufSize; /* 发送缓冲区大小 */
			__IO ushort Write; /* 发送缓冲区写指针 */
			__IO ushort Read; /* 发送缓冲区读指针 */
			__IO ushort Count; /* 等待发送的数据个数 */
			
			
	};   
    class UART_T
    {
        public:
            USART_TypeDef *uart; /* STM32内部串口设备指针 */
		
			FIFO tx;
			FIFO rx;

            void(*SendBefor)(void); /* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
            void(*SendOver)(void); /* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
            void(*ReciveNew)(byte _byte); /* 串口收到数据的回调函数指针 */
    };
    class Usart_T
    {
        public:
            void bsp_InitUart(void);                       
            byte comGetChar(COM _ucPort, byte *_pByte);

            void comClearTxFifo(COM _ucPort);
            void comClearRxFifo(COM _ucPort);
		public:
            
    };
    extern Usart_T usart1;
#endif
