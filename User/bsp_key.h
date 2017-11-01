#ifndef _BSP_KEY_H
#define _BSP_KEY_H
	#include "Sys.h"
	 #define KEY_COUNT    3	   					/* 按键个数, 8个独立建 + 2个组合键 */

    /* 根据应用程序的功能重命名按键宏 */
    #define KEY_DOWN_K1		KEY_1_DOWN
    #define KEY_UP_K1		KEY_1_UP
    #define KEY_LONG_K1		KEY_1_LONG

    #define KEY_DOWN_K2		KEY_2_DOWN
    #define KEY_UP_K2		KEY_2_UP
    #define KEY_LONG_K2		KEY_2_LONG

    #define SYS_DOWN_K1K2	KEY_9_DOWN		/* K1 K2 组合键 */
    #define SYS_UP_K1K2	    KEY_9_UP
    #define SYS_LONG_K1K2	KEY_9_LONG

    /* 按键ID, 主要用于bsp_KeyState()函数的入口参数 */
    typedef enum
    {
        KID_K1 = 0, KID_K2, KID_K3, KID_JOY_U
    } KEY_ID_E;

    /*
    按键滤波时间50ms, 单位10ms。
    只有连续检测到50ms状态不变才认为有效，包括弹起和按下两种事件
    即使按键电路不做硬件滤波，该滤波机制也可以保证可靠地检测到按键事件
     */
    #define KEY_FILTER_TIME   5
    #define KEY_LONG_TIME     100			/* 单位10ms， 持续1秒，认为长按事件 */

    /*
    每个按键对应1个全局的结构体变量。
     */
    typedef struct
    {
        /* 下面是一个函数指针，指向判断按键手否按下的函数 */
        byte(*IsKeyDownFunc)(void); /* 按键按下的判断函数,1表示按下 */

        byte Count; /* 滤波器计数器 */
        ushort LongCount; /* 长按计数器 */
        ushort LongTime; /* 按键按下持续时间, 0表示不检测长按 */
        byte State; /* 按键当前状态（按下还是弹起） */
        byte RepeatSpeed; /* 连续按键周期 */
        byte RepeatCount; /* 连续按键计数器 */
    } KEY_T;

    /*
    定义键值代码, 必须按如下次序定时每个键的按下、弹起和长按事件

    推荐使用enum, 不用#define，原因：
    (1) 便于新增键值,方便调整顺序，使代码看起来舒服点
    (2) 编译器可帮我们避免键值重复。
     */
    typedef enum
    {
        KEY_NONE = 0,  /* 0 表示按键事件 */

        KEY_1_DOWN,  /* 1键按下 */
        KEY_1_UP,  /* 1键弹起 */
        KEY_1_LONG,  /* 1键长按 */

        KEY_2_DOWN,  /* 2键按下 */
        KEY_2_UP,  /* 2键弹起 */
        KEY_2_LONG,  /* 2键长按 */

        /* 组合键 */
        KEY_9_DOWN,  /* 9键按下 */
        KEY_9_UP,  /* 9键弹起 */
        KEY_9_LONG,  /* 9键长按 */
    } KEY_ENUM;

    /* 按键FIFO用到变量 */
    #define KEY_FIFO_SIZE	10
    class KEY_FIFO_T
    {
		public:
			void Init()
			{
				this->Write=0;
				this->Read=0;
				this->Read2=0;
			}
			void Clear()
			{
				this->Read=this->Write;
			}
			void Push(byte da)
			{
				this->Buf[this->Write]=da;
				if (++this->Write >= KEY_FIFO_SIZE)
				{
					this->Write = 0;
				}				
			}
			byte Pop()
			{
				byte ret;

				if (this->Read == this->Write)
				{
					return KEY_NONE;
				}
				else
				{
					ret = this->Buf[this->Read];

					if (++this->Read >= KEY_FIFO_SIZE)
					{
						this->Read = 0;
					}
					return ret;
				}
			}
			byte Pop2()
			{
				byte ret;

				if (this->Read2 == this->Write)
				{
					return KEY_NONE;
				}
				else
				{
					ret = this->Buf[this->Read2];

					if (++this->Read2 >= KEY_FIFO_SIZE)
					{
						this->Read2 = 0;
					}
					return ret;
				}
			}
        public:
            byte Buf[KEY_FIFO_SIZE]; /* 键值缓冲区 */
            byte Read; /* 缓冲区读指针1 */
            byte Write; /* 缓冲区写指针 */
            byte Read2; /* 缓冲区读指针2 */
    };

    class Key
    {
        public:
            /* 供外部调用的函数声明 */
            void bsp_InitKey();
            void bsp_KeyScan();
            void bsp_PutKey(byte _KeyCode);
            byte bsp_GetKey();
            byte bsp_GetKey2();
            byte bsp_GetKeyState(KEY_ID_E _ucKeyID);
            void bsp_SetKeyParam(byte _ucKeyID, ushort _LongTime, byte _RepeatSpeed);
            void bsp_ClearKey();
        private:
            void bsp_InitKeyHard();
            void bsp_InitKeyVar();
            void bsp_DetectKey(byte i);
    };
    extern Key keytest;


#endif
