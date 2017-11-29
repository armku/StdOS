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

    typedef struct
    {
        char RxBuf[RX_BUF_MAX_LEN];
        int Length;
        int FlagFinish;
    } Fram_T;
	// 最好打开 Soket 前 不注册中断，以免AT指令乱入到中断里面去  然后信息不对称
    class Esp8266 : public WiFiInterface
    {
        public:
//			AT		At;		// AT操作对象
		ITransport*	Port;	// 传输口 代替AT指令

//			NetworkType	WorkMode;	// 工作模式

//			IDataPort*	Led;	// 指示灯

//			OutputPort	_Power;	// 电源
			OutputPort	_Reset;	// 重置
//			OutputPort	_LowPower;	// 低功耗

//			// 多个硬件socket
//			void*	Sockets[5];

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
//			// 启用DNS
//			virtual bool EnableDNS();
//			// 启用DHCP
//			virtual bool EnableDHCP();

			/******************************** 基础AT指令 ********************************/
			bool Test(int times = 10, int interval = 500);
			bool Reset(bool soft);
//			String GetVersion();
			bool Sleep(uint ms);
//			bool Echo(bool open);
			// 恢复出厂设置，将擦写所有保存到Flash的参数，恢复为默认参数。会导致模块重启
//			bool Restore();

			/******************************** WiFi功能指令 ********************************/
				// 获取模式
//			NetworkType GetMode();
//			// 设置模式。需要重启
//			bool SetMode(NetworkType mode);

			// 连接AP相关
//			String GetJoinAP();
			bool JoinAP(const String& ssid, const String& pass);
			bool UnJoinAP();
//			bool SetAutoConn(bool enable);

//			String LoadAPs();
//			String GetAP();
//			bool SetAP(const String& ssid, const String& pass, byte channel, byte ecn = 0, byte maxConnect = 4, bool hidden = false);
			// 查询连接到AP的Stations信息。无法查询DHCP接入
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

			/******************************** 发送指令 ********************************/
				// 设置无线组网密码。匹配令牌协议
//			bool SetWiFi(const Pair& args, Stream& result);
			// 获取无线名称。
//			bool GetWiFi(const Pair& args, Stream& result);
			// 获取可用APs
//			String* APs;
//			void GetAPsTask();
//			bool GetAPs(const Pair& args, Stream& result);

		private:
//			uint		_task;		// 调度任务
			//ByteArray	_Buffer;	// 待处理数据包
			//IPEndPoint	_Remote;	// 当前数据包远程地址

			// 打开与关闭
//			virtual bool OnOpen();
//			virtual void OnClose();
//			// 检测连接
//			virtual bool OnLink(uint retry);

//			bool CheckReady();
//			void OpenAP();

			// 处理收到的数据包
			void Process();

			// 数据到达
			void OnReceive(Buffer& bs);
		
		
		
		
		
		
		
		
		
		bool SendCmdNew(char *cmd, char *reply1, char *reply2, int waittime);
		bool SendCmd(char *cmd, char *reply1, char *reply2, int waittime);
		
		
		
		
		
		
		
		
		
		
		
		public:
            typedef enum
            {
                    STA, AP, STA_AP
            } ENUMNetModeTypeDef;
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
            void Init();            
            void SetPin(Pin pinChEn, Pin pinReset); //设置引脚
			
            void ChipEnable(bool en = true); //设置芯片有效
            
			bool NetModeChoose(ENUMNetModeTypeDef enumMode);
            bool JoinAP(char *ssid, char *pass);
            bool BuildAP(char *pSSID, char *pPassWord, ENUMAPPsdModeTypeDef enunPsdMode);
            bool EnableMultipleId(bool enumEnUnvarnishTx);
            bool LinkServer(ENUMNetProTypeDef enumE, char *ip, char *ComNum, ENUMIDNOTypeDef id);
            bool StartOrShutServer(bool enumMode, char *pPortNum, char *pTimeOver);
            int GetLinkStatus();
            int GetIdLinkStatus();
            int InquireApIp(char *pApIp, int ucArrayLength);
            bool UnvarnishSend();
            void ExitUnvarnishSend();
            bool SendString(bool enumEnUnvarnishTx, char *pStr, int ulStrLength, ENUMIDNOTypeDef ucId);
            char *ReceiveString(bool enumEnUnvarnishTx);
        private:
            char *itoa(int value, char *string, int radix);
            void USART_printf(char *Data, ...);
        private:			
			void SendData(char *buf,int len);
        public:
            volatile bool FlagTcpClosed; //是否断开连接
			int RunStep;//运行步骤

            OutputPort portEnable; // 芯片使能
    };

    void Delay_ms(int ms);
    extern Fram_T strEsp8266_Fram_Record;
#endif
