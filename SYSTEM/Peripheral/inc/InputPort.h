#pragma once

#include "Port.h"
#define REGOLD	1	//旧版注册机制
// 输入口
class InputPort: public Port
{
    public:
		//以下为2017-01-07
		typedef enum
        {
                NOPULL = 0x00,
				UP 	= 0x01,//上拉电阻 
				DOWN 	= 0x02,//下拉电阻 
        } PuPd;
		//enum class Trigger	//强类型枚举
		typedef enum	//2017-01-07
		{
			Rising	=	0x01,	//上升沿
			Falling	=	0x02,	//下降沿
			Both	=	0x03	//上升下降沿
		}Trigger;

        // 读取委托
        typedef void(*IOReadHandler)(InputPort * port, bool down, void *param); 

        ushort	ShakeTime;//	=	0; 		// 抖动时间.毫秒
		ushort	PressTime;//	=	0;		//长按时间。毫秒
		bool	Invert	;//	=	2; 		// 是否倒置输入输出。默认2表示自动检测		
        PuPd Pull;//	=	PuPd;//上拉下拉电阻
		Trigger	Mode	;//	=	Both;	//触发模式，上升沿下降沿
		bool	HardEvent;//	=	false;	//是否使用硬件事件，默认false
		
		//Delegate2<InputPortNew&,bool> Press;//按下事件
		
		//以上为2017-01-07
        bool Floating; // 是否浮空输入
        

        InputPort()
        {
                Init();
        }
        InputPort(Pin pin, bool floating = true, PuPd pupd = UP)
        {
                Init(floating, pupd);
                Set(pin);
        }

        virtual ~InputPort();

        ushort ReadGroup(); // 整组读取
        bool Read(); // 读取状态
        static bool Read(Pin pin); // 读取某个引脚

        void Register(IOReadHandler handler, void *param = NULL); // 注册事件
		void RegisterAdd();//附加测试功能

        operator bool()
        {
                return Read();
        }

    protected:
        // 函数命名为Init，而不作为构造函数，主要是因为用构造函数会导致再实例化一个对象，然后这个函数在那个新对象里面执行
        void Init(bool floating = true, PuPd pupd = UP)
        {
                Pull = pupd;
                Floating = floating;

                _Registed = false;
                //ShakeTime = 20;
                // 有些应用的输入口需要极高的灵敏度，这个时候不需要抖动检测
                ShakeTime = 0;
                Invert = false;
        }

        virtual void OnConfig(GPIO_InitTypeDef &gpio);
    private:
        bool _Registed;        
            void RegisterInput(int groupIndex, int pinIndex, IOReadHandler handler, void *param);
            void UnRegisterInput(int pinIndex);
        
};
//回调函数定义 down指示按下还是弹起触发
typedef void(*BtnInCallbackOld)(Pin pin, bool down);

class InputPortOld
{
    public:
		InputPortOld();
        InputPortOld(Pin pin);
        void InitOld();
         
		Pin _Pin; // 针脚
   
        void On(); //打开中断
        void Off(); //关闭中断		
        void RegisterOld(BtnInCallbackOld handler); // 注册回调		
    private:
        void Exti0_state(bool onoff);
        uint eXTI_Line; //中断线
        byte nVIC_IRQChannel; //中断向量
        byte gPIO_PinSource; //中断线
        byte gPIO_PortSourceGPIO; //端口	
	public:        	
		void Write(const bool value);//写入值，true：高电平，false：低电平			
        void SetModeAIN();
        void SetModeIN_FLOATING();
        void SetModeINPUT_IPD();
        void SetModeINPUT_IPU();
        void SetModeOut_OD();
        void SetModeOut_PP();
        void SetModeAF_OD();
        void SetModeAF_PP();
        void Set(); //设置引脚
        void Reset(); //复位引脚        
        byte Read(void); 
		static bool ReadPinPort(Pin pin);//读取端口状态
    protected:         
        void SetMode(PIN_MODE mode); //引脚模式		
};
