#pragma once 

#include "Type.h"

typedef enum
{
    AIN = 0x0,  //模拟输入 
    INPUT = 0x04,  //输入
    INPUT_PD = 0x28,  //浮空输入
    INPUT_PU = 0x48,  //推挽式输入
    OUTPUT_OD = 0x14,  //开漏输出
    OUTPUT_PP = 0x10,  //推挽式输出
    AF_OD = 0x1C,  //开漏复用输出
    AF_PP = 0x18  //推挽复用输出
} PIN_MODE;
/* 针脚 ------------------------------------------------------------------*/
//引脚定义
typedef enum
{
	PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15,
	PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15,
	PC0,PC1,PC2,PC3,PC4,PC5,PC6,PC7,PC8,PC9,PC10,PC11,PC12,PC13,PC14,PC15,
	PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7,PD8,PD9,PD10,PD11,PD12,PD13,PD14,PD15,
	PE0,PE1,PE2,PE3,PE4,PE5,PE6,PE7,PE8,PE9,PE10,PE11,PE12,PE13,PE14,PE15,
	PF0,PF1,PF2,PF3,PF4,PF5,PF6,PF7,PF8,PF9,PF10,PF11,PF12,PF13,PF14,PF15,
	PG0,PG1,PG2,PG3,PG4,PG5,PG6,PG7,PG8,PG9,PG10,PG11,PG12,PG13,PG14,PG15,
	P0 = 0XFFFF
}Pin;

class Port
{
    public:
		Port();
        Port(Pin pin);		
		void Write(const bool value);//写入值，true：高电平，false：低电平
		void operator = (const bool value);//写入值，true:高电平 false:低电平
		void operator = (const byte value);//写入值，其他:高电平 0:低电平
        void SetModeAIN();
        void SetModeIN_FLOATING();
        void SetModeINPUT_IPD();
        void SetModeINPUT_IPU();
        void SetModeOut_OD();
        void SetModeOut_PP();
        void SetModeAF_OD();
        void SetModeAF_PP();
        void Set(); //设置高电平
		void Set(Pin pin); //设置引脚
        void Reset(); //复位引脚        
        byte Read(void); 
		static bool ReadPinPort(Pin pin);//读取端口状态
    protected:
        Pin pin; //引脚号
        byte pinbit;
        void SetMode(PIN_MODE mode); //引脚模式		
};
