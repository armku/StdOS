#ifndef _BSP_KEY_H
    #define _BSP_KEY_H
    #include "Sys.h"
    #define KEY_COUNT    5	   					/* 按键个数, 4个独立建 + 1个组合键 */
	/*
	KEY_NONE,
  KEY_OK,
  KEY_LEFT,
  KEY_UP,
  KEY_DOWN,
  KEY_OK_C,
  KEY_LEFT_C,
  KEY_UP_C,
  KEY_DOWN_C,  
  KEY_OK_LEFT, 
	*/

    /* 按键ID, 主要用于bsp_KeyState()函数的入口参数 */
    typedef enum
    {
        KID_KOK = 0, KID_KLEFT, KID_KDOWN,KID_KUP
    } KEY_ID_E;
    typedef enum
    {
		KEY_NONE = 0,
		
        KEYDOWN_OK,		// 按下
		KEYUP_OK,		// 弹起
		KEYLONG_OK,		// 长按
		
		KEYDOWN_LEFT,  
		KEYUP_LEFT,
		KEYLONG_LEFT,		
		
		KEYDOWN_DOWN,  
		KEYUP_DOWN,
		KEYLONG_DOWN,
		
        KEYDOWN_UP,  
		KEYUP_UP,
		KEYLONG_UP,
						
        KEYDOWN_CFG,// = KEY_LEFT | KEY_MENU,  // 进入参数配置界面
		KEYUP_CFG,
		KEYLONG_CFG,		
		        	
    } KEY_ENUM;
    class KEY_T /*每个按键对应1个全局的结构体变量。
     */
    {
        public:
            byte(*IsKeyDownFunc)(); /* 按键按下的判断函数,1表示按下 */

            byte Count; /* 滤波器计数器 */
            ushort LongCount; /* 长按计数器 */
            ushort LongTime; /* 按键按下持续时间, 0表示不检测长按 */
            byte State; /* 按键当前状态（按下还是弹起） */
            byte RepeatSpeed; /* 连续按键周期 */
            byte RepeatCount; /* 连续按键计数器 */			
        public:
            //void CheckKey();			
    };
    class KEY_FIFO
    {
        public:
			KEY_FIFO(){_bufSize=10;}
			void Init();
            void Clear();
            void Push(byte da);
            byte Pop();
            byte Pop2();
        private:
            int _bufSize;// = 10;
            byte Buf[10]; /* 键值缓冲区 */
            byte Read; /* 缓冲区读指针1 */
            byte Write; /* 缓冲区写指针 */
            byte Read2; /* 缓冲区读指针2 */
    };

    class Key
    {
        public:
            void InitKeyVar();
            void KeyScan();
            void PutKey(byte _KeyCode);
            byte GetKeyState(KEY_ID_E _ucKeyID);
            void SetKeyParam(byte _ucKeyID, ushort _LongTime, byte _RepeatSpeed);
            byte GetKeyCode();

            void SetKeyDetectFunc(byte(*func)(), byte pos = 0);
        public:

        private:
            void DetectKey(byte i);
        private:
            KEY_FIFO s_tKey;
            KEY_T s_tBtn[KEY_COUNT];
			byte flagLongkey;//长按标记，长按下时不发送弹起指令
    };
#endif
