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
			bool Full();//��������
			bool Empty();//��������			
		private:
			byte *Buf; /* ���ͻ����� */
			ushort BufSize; /* ���ͻ�������С */
			__IO ushort Write; /* ���ͻ�����дָ�� */
			__IO ushort Read; /* ���ͻ�������ָ�� */
			__IO ushort Count; /* �ȴ����͵����ݸ��� */
			
			
	};   
    class UART_T
    {
        public:
            USART_TypeDef *uart; /* STM32�ڲ������豸ָ�� */
		
			FIFO tx;
			FIFO rx;

            void(*SendBefor)(void); /* ��ʼ����֮ǰ�Ļص�����ָ�루��Ҫ����RS485�л�������ģʽ�� */
            void(*SendOver)(void); /* ������ϵĻص�����ָ�루��Ҫ����RS485������ģʽ�л�Ϊ����ģʽ�� */
            void(*ReciveNew)(byte _byte); /* �����յ����ݵĻص�����ָ�� */
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
