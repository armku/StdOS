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
			uint8_t *pBuf; /* ������ */			
			int Read; /* ��������ָ�� */
			int Count; /* ���ݸ��� */
			int BufSize; /* ��������С */
		private:			
			int Write; /* ������дָ�� */			
	};

    class UART_T
    {
        public:
            USART_TypeDef *uart; /* STM32�ڲ������豸ָ�� */
			FIFO tx;
			FIFO rx;
            void(*ReciveNew)(uint8_t _byte); /* �����յ����ݵĻص�����ָ�� */
    };

    void bsp_InitUart(void);
    void comSendBuf(COM _ucPort, uint8_t *_ucaBuf, int _usLen);
    void comSendChar(COM _ucPort, uint8_t _ucByte);
    uint8_t comGetChar(COM _ucPort, uint8_t *_pByte);
#endif
