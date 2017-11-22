#ifndef __BSP_ESP8266_H
    #define __BSP_ESP8266_H

    #include "stm32f10x.h"    
    #include <stdio.h>
    #include <stdbool.h>

    #if defined ( __CC_ARM   )
        #pragma anon_unions
    #endif 
    /******************************* ESP8266 外部全局变量声明 ***************************/
    #define RX_BUF_MAX_LEN     1024                                     //最大接收缓存字节数

    extern struct STRUCT_USARTx_Fram  //串口数据帧的处理结构体
    {
        char Data_RX_BUF[RX_BUF_MAX_LEN];
        union
        {
            __IO u16 InfAll;
            struct 
            {
                __IO u16 FramLength: 15; // 14:0 
                __IO u16 FramFinishFlag: 1; // 15 
            } InfBit;
        };
    }
    strEsp8266_Fram_Record;

    /*********************************************** ESP8266 函数宏定义 *******************************************/
    #define macESP8266_CH_ENABLE()                 GPIO_SetBits ( GPIOG, GPIO_Pin_13 )
    #define macESP8266_CH_DISABLE()                GPIO_ResetBits ( GPIOG, GPIO_Pin_13 )
    #define macESP8266_RST_HIGH_LEVEL()            GPIO_SetBits ( GPIOG, GPIO_Pin_14 )
    #define macESP8266_RST_LOW_LEVEL()             GPIO_ResetBits ( GPIOG, GPIO_Pin_14 )

    /****************************************** ESP8266 函数声明 ***********************************************/
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
            bool Cmd(char *cmd, char *reply1, char *reply2, u32 waittime);
            void Test();
            bool NetModeChoose(ENUMNetModeTypeDef enumMode);
            bool JoinAP(char *pSSID, char *pPassWord);
            bool BuildAP(char *pSSID, char *pPassWord, ENUMAPPsdModeTypeDef enunPsdMode);
            bool EnableMultipleId(FunctionalState enumEnUnvarnishTx);
            bool LinkServer(ENUMNetProTypeDef enumE, char *ip, char *ComNum, ENUMIDNOTypeDef id);
            bool StartOrShutServer(FunctionalState enumMode, char *pPortNum, char *pTimeOver);
            uint8_t GetLinkStatus();
            uint8_t GetIdLinkStatus();
            uint8_t InquireApIp(char *pApIp, uint8_t ucArrayLength);
            bool UnvarnishSend();
            void ExitUnvarnishSend();
            bool SendString(FunctionalState enumEnUnvarnishTx, char *pStr, u32 ulStrLength, ENUMIDNOTypeDef ucId);
            char *ReceiveString(FunctionalState enumEnUnvarnishTx);
        private:
            void GPIOConfig();
            void USARTConfig();
            void USARTNVICConfiguration();
		
			char *itoa(int value, char *string, int radix);
			void USART_printf(char *Data, ...);
    };    
#endif
