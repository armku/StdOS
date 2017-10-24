#ifndef __BSP_ESP8266_H
    #define __BSP_ESP8266_H
	#include "Type.h"
    #include "stm32f10x.h"

    #if defined ( __CC_ARM   )
        #pragma anon_unions
    #endif 
    /******************************* ESP8266 数据类型定义 ***************************/
    typedef enum
    {
        STA, AP, STA_AP
    } ENUM_Net_ModeTypeDef;
    typedef enum
    {
        enumTCP, enumUDP, 
    } ENUM_NetPro_TypeDef;
    typedef enum
    {
        Multiple_ID_0 = 0, Multiple_ID_1 = 1, Multiple_ID_2 = 2, Multiple_ID_3 = 3, Multiple_ID_4 = 4, Single_ID_0 = 5, 
    } ENUM_ID_NO_TypeDef;
    typedef enum
    {
        OPEN = 0, WEP = 1, WPA_PSK = 2, WPA2_PSK = 3, WPA_WPA2_PSK = 4, 
    } ENUM_AP_PsdMode_TypeDef;
    /******************************* ESP8266 外部全局变量声明 ***************************/
    #define RX_BUF_MAX_LEN     1024                                   //最大接收缓存字节数
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
    class ESP8266
    {
        public:
            void Init(void); //初始化WiFi模块使用的接口和外设
            void Rst(void);
            void AT_Test(void);
            uint8_t Get_LinkStatus(void);
            uint8_t Get_IdLinkStatus(void);
            bool UnvarnishSend(void);
            void ExitUnvarnishSend(void);
            void GPIO_Config(void);
            void USART_Config(void);
            void USART_NVIC_Configuration(void);
            bool Cmd(char *cmd, char *reply1, char *reply2, u32 waittime);
            bool Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode);
            bool JoinAP(char *pSSID, char *pPassWord);
            bool BuildAP(char *pSSID, char *pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode);
            bool Enable_MultipleId(FunctionalState enumEnUnvarnishTx);
            bool Link_Server(ENUM_NetPro_TypeDef enumE, char *ip, char *ComNum, ENUM_ID_NO_TypeDef id);
            bool StartOrShutServer(FunctionalState enumMode, char *pPortNum, char *pTimeOver);
            uint8_t Inquire_ApIp(char *pApIp, uint8_t ucArrayLength);
            bool SendString(FunctionalState enumEnUnvarnishTx, char *pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId);
            char *ReceiveString(FunctionalState enumEnUnvarnishTx);
    };    
#endif
