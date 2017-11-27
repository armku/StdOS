#ifndef __BSP_ESP8266_H
    #define __BSP_ESP8266_H
	
	#include "Port.h"
        
    #define RX_BUF_MAX_LEN     1024 

    typedef struct   
    {
        char RxBuf[RX_BUF_MAX_LEN];
        int Length;
		int FlagFinish;
    }Fram_T;
    	
    class Esp8266
    {
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
            void Rst();
			void SetPin(Pin pinChEn);//设置引脚
			void ChipEnable(bool en=true);//设置芯片有效
            bool Cmd(char *cmd, char *reply1, char *reply2, int waittime);
            void Test();
            bool NetModeChoose(ENUMNetModeTypeDef enumMode);
            bool JoinAP(char *pSSID, char *pPassWord);
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
            void GPIOConfig();
            void USARTConfig();
            void USARTNVICConfiguration();
		
			char *itoa(int value, char *string, int radix);
			void USART_printf(char *Data, ...);
		private:
			public:
			volatile bool FlagTcpClosed;//是否断开连接
			
			OutputPort pChEnable;
			
    };    
	
	void Delay_ms(int ms);
	extern Fram_T strEsp8266_Fram_Record;
#endif
