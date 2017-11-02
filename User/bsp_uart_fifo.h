#ifndef _BSP_USART_FIFO_H_
    #define _BSP_USART_FIFO_H_
    #include "Sys.h"
    /* 定义端口号 */
//    typedef enum
//    {
//        COM1 = 0,  /* USART1  PA9, PA10 */
//        COM2 = 1,  /* USART2, PA2, PA3 */
//        COM3 = 2,  /* USART3, PB10, PB11 */
//        COM4 = 3,  /* UART4, PC10, PC11 */
//        COM5 = 4,  /* UART5, PC12, PD2 */
//    } COM_PORT_E;
        
	class FIFO
	{
		public:
			void SetBuf(uint8_t *buf,int len);
			void Push(uint8_t da);
			uint8_t Pop();
			void Clear();
			bool Full();//缓冲区满
			bool Empty();//缓冲区空			
		private:
			uint8_t *Buf; /* 发送缓冲区 */
			uint16_t BufSize; /* 发送缓冲区大小 */
			__IO uint16_t Write; /* 发送缓冲区写指针 */
			__IO uint16_t Read; /* 发送缓冲区读指针 */
			__IO uint16_t Count; /* 等待发送的数据个数 */
			
			
	};   
    class UART_T
    {
        public:
            USART_TypeDef *uart; /* STM32内部串口设备指针 */
		
			FIFO tx;
			FIFO rx;

            void(*SendBefor)(void); /* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
            void(*SendOver)(void); /* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
            void(*ReciveNew)(uint8_t _byte); /* 串口收到数据的回调函数指针 */
    };
    class Usart_T
    {
        public:
            void bsp_InitUart(void);                       
            uint8_t comGetChar(COM _ucPort, uint8_t *_pByte);

            void comClearTxFifo(COM _ucPort);
            void comClearRxFifo(COM _ucPort);
		public:
            
    };
    extern Usart_T usart1;
#endif
