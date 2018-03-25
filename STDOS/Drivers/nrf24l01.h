#ifndef _NRF24L01_H
    #define _NRF24L01_H

	#include "Kernel\Sys.h"
	#include "Device\Spi.h"
	#include "Net\ITransport.h"
	//#include "Device\Power.h"
	//#include "Message\DataStore.h"
	//#include "APP\FlushPort.h"

    #define MAX_RT      0x10 //�ﵽ����ط������жϱ�־λ
    #define TX_DS		0x20 //��������жϱ�־λ	  // 
    #define RX_DR		0x40 //���յ������жϱ�־λ
	
	class NRF24L01 
	{
		public:
			uint8_t Channel;		// ͨѶƵ��������Ƶ�ʺţ���2400MHZ�����ϼ�
			uint8_t Local[5];		// ���ص�ַ
			uint8_t Remote[5];		// Զ�̵�ַ
			bool DynPayload;	// ��̬����
			bool AutoAnswer;	// �Զ�Ӧ��Ĭ������
			uint16_t Speed;		// ��Ƶ�����ʣ���λkbps��Ĭ��250kbps����ѡ1000kbps/2000kbps���ٶ�Խ�ʹ���ԽԶ
			uint8_t RadioPower;	// ���书�ʡ���8�������0x07����7dBm�����
			bool Master;		// �Ƿ����ڵ㡣

			uint16_t	Error;		// �������������������������Զ�����

			NRF24L01();
			virtual ~NRF24L01();
			void Init(Spi* spi, Pin ce = P0, Pin irq = P0, Pin power = P0);

			bool Check();
			bool Config();		// ��ɻ��������趨��Ĭ�ϳ�ʼ��Ϊ����ģʽ
			bool GetPower();	// ��ȡ��ǰ��Դ״̬
			bool SetPowerMode(bool on);	// ���õ�ǰ��Դ״̬�������Ƿ�ɹ�
			bool GetMode();		// ��ȡ��ǰģʽ�Ƿ����ģʽ
			bool SetMode(bool isReceive, const Buffer& addr);	// �л��շ�ģʽ�������������趨
			void SetAddress();	// ���õ�ַ
			
			// ��Դ�ȼ�����������͹���ģʽ��ʱ����
			//virtual void ChangePower(int level);

			typedef int (*FuncBufInt)(const Buffer&);
			FuncBufInt	FixData;// �������ݵ�ί��

			uint8_t Status;
			uint8_t FifoStatus;
			void ShowStatus();

			//IDataPort*	Led;	// ���ݵ�
			void SetLed(OutputPort& led);
			void SetLed(Pin led);

			//virtual String ToString() const { return String("R24"); }
			virtual String& ToStr(String& str) const { return str + "R24"; }

		private:

//			virtual bool OnOpen();
//			virtual void OnClose();

//			virtual bool OnWrite(const Buffer& bs);
//			virtual uint32_t OnRead(Buffer& bs);

//			// �������ݵ����¼�
//			//virtual uint32_t OnReceive(Buffer& bs, void* param);
//			virtual bool OnWriteEx(const Buffer& bs, const void* opt);
			
			bool SendTo(const Buffer& bs, const Buffer& addr);

			Spi*		_spi;
			OutputPort	_CE;
			InputPort	Irq;
			OutputPort	_Power;	// ���ÿ���2401��Դ������  ֱ�ӽ��ж�2401��ͨ�ϵ����������������setPower��Ч

			uint8_t WriteBuf(uint8_t reg, const Buffer& bs);
			uint8_t ReadBuf(uint8_t reg, Buffer& bs);
			uint8_t ReadReg(uint8_t reg);
			uint8_t WriteReg(uint8_t reg, uint8_t dat);

			void AddError();

			// ��������
			static void ReceiveTask(void* param);
			uint32_t _tidOpen;
			uint32_t _tidRecv;
			void OnIRQ(InputPort& port, bool down);
			void OnIRQ();










		public:
			void TX_Mode();
			void RX_Mode();
			uint8_t Rx_Dat(uint8_t *rxbuf);
			uint8_t Tx_Dat(uint8_t *txbuf);
			OutputPort _CSN;
		private:
					
			uint8_t ReadBuf(uint8_t reg, uint8_t *pBuf, uint8_t bytes);
			uint8_t WriteBuf(uint8_t reg, uint8_t *pBuf, uint8_t bytes);
	};

#endif
