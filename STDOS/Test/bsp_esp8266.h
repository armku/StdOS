#ifndef  __BSP_ESP8266_H
#define	 __BSP_ESP8266_H
#include "stm32f10x.h"
#include "common.h"
#include <stdio.h>

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif
/******************************* ESP8266 数据类型定义 ***************************/
typedef enum{
	STA,
  AP,
  STA_AP  
} ENUM_Net_ModeTypeDef;
typedef enum{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;
typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;
typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;
/******************************* ESP8266 外部全局变量声明 ***************************/
#define RX_BUF_MAX_LEN     1024                                     //最大接收缓存字节数

extern struct  STRUCT_USARTx_Fram                                  //串口数据帧的处理结构体
{
	char  Data_RX_BUF [ RX_BUF_MAX_LEN ];
	
  union {
    __IO u16 InfAll;
    struct {
		  __IO u16 FramLength       :15;                               // 14:0 
		  __IO u16 FramFinishFlag   :1;                                // 15 
	  } InfBit;
  }; 
	
} strEsp8266_Fram_Record;

class ESP8266
{
    public:
        void ESP8266_Init(void);//初始化WiFi模块使用的接口和外设
        void ESP8266_Rst(void);
        void ESP8266_AT_Test(void);
        uint8_t ESP8266_Get_LinkStatus(void);
        uint8_t ESP8266_Get_IdLinkStatus(void);
        bool ESP8266_UnvarnishSend(void);
        void ESP8266_ExitUnvarnishSend(void);
		void ESP8266_GPIO_Config(void);
		void ESP8266_USART_Config(void);
		void ESP8266_USART_NVIC_Configuration(void);
};
/*********************************************** ESP8266 函数宏定义 *******************************************/
#define     macESP8266_Usart( fmt, ... )           USART_printf ( USART3, fmt, ##__VA_ARGS__ ) 
#define     macPC_Usart( fmt, ... )                printf ( fmt, ##__VA_ARGS__ )
//#define     macPC_Usart( fmt, ... )                
#define     macESP8266_CH_ENABLE()                 GPIO_SetBits ( GPIOG, GPIO_Pin_13 )
#define     macESP8266_CH_DISABLE()                GPIO_ResetBits ( GPIOG, GPIO_Pin_13 )
#define     macESP8266_RST_HIGH_LEVEL()            GPIO_SetBits ( GPIOG, GPIO_Pin_14 )
#define     macESP8266_RST_LOW_LEVEL()             GPIO_ResetBits ( GPIOG, GPIO_Pin_14 )
/****************************************** ESP8266 函数声明 ***********************************************/
bool                     ESP8266_Cmd                         ( char * cmd, char * reply1, char * reply2, u32 waittime );
bool                     ESP8266_Net_Mode_Choose             ( ENUM_Net_ModeTypeDef enumMode );
bool                     ESP8266_JoinAP                      ( char * pSSID, char * pPassWord );
bool                     ESP8266_BuildAP                     ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode );
bool                     ESP8266_Enable_MultipleId           ( FunctionalState enumEnUnvarnishTx );
bool                     ESP8266_Link_Server                 ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
bool                     ESP8266_StartOrShutServer           ( FunctionalState enumMode, char * pPortNum, char * pTimeOver );
uint8_t                  ESP8266_Inquire_ApIp                ( char * pApIp, uint8_t ucArrayLength );
bool                     ESP8266_SendString                  ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId );
char *                   ESP8266_ReceiveString               ( FunctionalState enumEnUnvarnishTx );
#endif
