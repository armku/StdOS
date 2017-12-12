#ifndef __BSP_ESP8266_H
    #define __BSP_ESP8266_H

    #include "Device\Port.h"

	#include "Net\Socket.h"
	#include "Net\NetworkInterface.h"
	#include "Net\ITransport.h"

//	#include "Message\DataStore.h"
//	#include "Message\Pair.h"

	#include "App\AT.h"
	
	#include "SerialPort.h"

    #define RX_BUF_MAX_LEN     1024 
	
	enum class EspCmdType
	{
		ENONE,//û��ָ��
		ETEST,//����ָ��
		ESetMode,//���ù���ģʽ
		EJoinAP,
		EEnableMultipleId,
		ELinkServer,
		EUnvarnishSend,
		EExitUnvarnishSend,
		EStartOrShutServer,
		EGetIdLinkStatus,
		EGetLinkStatus,
	};

    typedef struct
    {
        char RxBuf[RX_BUF_MAX_LEN];
        int Length;
        int FlagFinish;
    } Fram_T;
	// ��ô� Soket ǰ ��ע���жϣ�����ATָ�����뵽�ж�����ȥ  Ȼ����Ϣ���Գ�
    class Esp8266 : public WiFiInterface
    {
        public:
			char bufrcv1[20];//���յ���Ӧ�ַ���1
			char bufrcv2[20];//���յ���Ӧ�ַ���2
			char bufrcvcnt;//���յ���Ӧ�ַ�������
			void SetRcv(char *rcv1, char *rcv2,int rcvcnt);//���ý��յ��ַ���
			AT		At;		// AT��������
		SerialPort*	Port;	// ����� ����ATָ��

			NetworkType	WorkMode;	// ����ģʽ

//			IDataPort*	Led;	// ָʾ��

//			OutputPort	_Power;	// ��Դ
			OutputPort	_Reset;	// ����
//			OutputPort	_LowPower;	// �͹���

//			// ���Ӳ��socket
//			void*	Sockets[5];
			EspCmdType cmdType;//ָ������

			Esp8266();
			virtual ~Esp8266();

			void Init(ITransport* port);
			void Init(COM idx, int baudrate = 115200);
			void Set(Pin power = P0, Pin rst = P0, Pin low = P0);

			virtual bool Config();
			void SetLed(Pin led);
			void SetLed(OutputPort& led);
			void RemoveLed();

			//virtual const String ToString() const { return String("Esp8266"); }
//			virtual Socket* CreateSocket(NetType type);
//			// ����DNS
//			virtual bool EnableDNS();
//			// ����DHCP
//			virtual bool EnableDHCP();

			/******************************** ����ATָ�� ********************************/
			bool Test(int times = 10, int interval = 500);
			bool Reset(bool soft);
//			String GetVersion();
			bool Sleep(uint ms);
//			bool Echo(bool open);
			// �ָ��������ã�����д���б��浽Flash�Ĳ������ָ�ΪĬ�ϲ������ᵼ��ģ������
//			bool Restore();

			/******************************** WiFi����ָ�� ********************************/
				// ��ȡģʽ
//			NetworkType GetMode();
			// ����ģʽ����Ҫ����
			bool SetMode(NetworkType mode);

			// ����AP���
//			String GetJoinAP();
			bool JoinAP(const String& ssid, const String& pass);
			bool UnJoinAP();
//			bool SetAutoConn(bool enable);

//			String LoadAPs();
//			String GetAP();
//			bool SetAP(const String& ssid, const String& pass, byte channel, byte ecn = 0, byte maxConnect = 4, bool hidden = false);
			// ��ѯ���ӵ�AP��Stations��Ϣ���޷���ѯDHCP����
//			String LoadStations();

//			bool GetDHCP(bool* sta, bool* ap);
//			bool SetDHCP(NetworkType mode, bool enable);

//			MacAddress GetMAC(bool sta);
//			bool SetMAC(bool sta, const MacAddress& mac);

//			IPAddress GetIP(bool sta);

			/******************************** TCP/IP ********************************/
//			String GetStatus();
//			bool GetMux();
//			bool SetMux(bool enable);

//			bool Update();

//			bool Ping(const IPAddress& ip);

//			bool SetIPD(bool enable);

			/******************************** ����ָ�� ********************************/
				// ���������������롣ƥ������Э��
//			bool SetWiFi(const Pair& args, Stream& result);
			// ��ȡ�������ơ�
//			bool GetWiFi(const Pair& args, Stream& result);
			// ��ȡ����APs
//			String* APs;
//			void GetAPsTask();
//			bool GetAPs(const Pair& args, Stream& result);

//		private:
			uint		_task;		// ��������
			//ByteArray	_Buffer;	// ���������ݰ�
			//IPEndPoint	_Remote;	// ��ǰ���ݰ�Զ�̵�ַ

			// ����ر�
//			virtual bool OnOpen();
//			virtual void OnClose();
//			// �������
//			virtual bool OnLink(uint retry);

//			bool CheckReady();
//			void OpenAP();

			// �����յ������ݰ�
			void Process();

			// ���ݵ���
			void OnReceive(Buffer& bs);
			
			void Routin();//ѭ������
		
		
			
		
		
		
		
		
		
		public:
            
            typedef enum
            {
                    enumTCP, enumUDP, 
            } ENUMNetProTypeDef;


            typedef enum
            {
                    MultipleID0 = 0, MultipleID1 = 1, MultipleID2 = 2, MultipleID3 = 3, MultipleID4 = 4, SingleID0 = 5, 
            } ENUMIDNOTypeDef;

            typedef enum
            {
                    OPEN = 0, WEP = 1, WPA_PSK = 2, WPA2_PSK = 3, WPA_WPA2_PSK = 4, 
            } ENUMAPPsdModeTypeDef;
                     
            void SetPin(Pin pinChEn, Pin pinReset); //��������
			
            void ChipEnable(bool en = true); //����оƬ��Ч			
            bool JoinAP(char *ssid, char *pass);
            bool EnableMultipleId(bool enumEnUnvarnishTx);
            bool LinkServer(ENUMNetProTypeDef enumE, char *ip, char *ComNum, ENUMIDNOTypeDef id);
            int GetLinkStatus();
            bool UnvarnishSend();
            void ExitUnvarnishSend();
        public:
            volatile bool FlagTcpClosed; //�Ƿ�Ͽ�����
			int RunStep;//���в���

            OutputPort portEnable; // оƬʹ��
    };

    void Delay_ms(int ms);
    extern Fram_T strEsp8266_Fram_Record;
#endif
