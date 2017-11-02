#ifndef _BSP_USART_FIFO_H_
    #define _BSP_USART_FIFO_H_
    #include "Sys.h"
    /* ����˿ں� */
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
			bool Full();//��������
			bool Empty();//��������			
		private:
			uint8_t *Buf; /* ���ͻ����� */
			uint16_t BufSize; /* ���ͻ�������С */
			__IO uint16_t Write; /* ���ͻ�����дָ�� */
			__IO uint16_t Read; /* ���ͻ�������ָ�� */
			__IO uint16_t Count; /* �ȴ����͵����ݸ��� */
			
			
	};   
    class UART_T
    {
        public:
            USART_TypeDef *uart; /* STM32�ڲ������豸ָ�� */
		
			FIFO tx;
			FIFO rx;

            void(*SendBefor)(void); /* ��ʼ����֮ǰ�Ļص�����ָ�루��Ҫ����RS485�л�������ģʽ�� */
            void(*SendOver)(void); /* ������ϵĻص�����ָ�루��Ҫ����RS485������ģʽ�л�Ϊ����ģʽ�� */
            void(*ReciveNew)(uint8_t _byte); /* �����յ����ݵĻص�����ָ�� */
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
