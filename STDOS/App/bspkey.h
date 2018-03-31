#ifndef _BSP_KEY_H
    #define _BSP_KEY_H
    #include "Sys.h"
    #define KEY_COUNT    4	   					/* 按键个数, 2个独立建 + 1个组合键 */
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
		
		KEYDOWN_OK_LEFT,
		KEYUP_OK_LEFT,
		KEYLONG_OK_LEFT,
		
		KEYDOWN_OK_UP,
		KEYUP_OK_UP,
		KEYLONG_OK_UP,
		
	
        KEYDOWN_CFG,// = KEY_LEFT | KEY_MENU,  // 进入参数配置界面
		KEYUP_CFG,
		KEYLONG_CFG,		
		
        KEYDOWN_CAL,// = KEY_UP | KEY_MENU,  // 校验界面        
		KEYUP_CAL,
		KEYLONG_CAL,
		
		KEYDOWN_DEMOCAL,// = KEY_UP | KEY_MENU,  // 校验界面        
		KEYUP_DEMOCAL,
		KEYLONG_DEMOCAL,		
    } KEY_ENUM;
    class KEY_T /*每个按键对应1个全局的结构体变量。
     */
    {
        public:
            uint8_t(*IsKeyDownFunc)(); /* 按键按下的判断函数,1表示按下 */

            uint8_t Count; /* 滤波器计数器 */
            uint16_t LongCount; /* 长按计数器 */
            uint16_t LongTime; /* 按键按下持续时间, 0表示不检测长按 */
            uint8_t State; /* 按键当前状态（按下还是弹起） */
            uint8_t RepeatSpeed; /* 连续按键周期 */
            uint8_t RepeatCount; /* 连续按键计数器 */			
        public:
            //void CheckKey();			
    };
    class KEY_FIFO
    {
        public:
            void Init();
            void Clear();
            void Push(uint8_t da);
            uint8_t Pop();
            uint8_t Pop2();
        private:
            const int _bufSize = 10;
            uint8_t Buf[10]; /* 键值缓冲区 */
            uint8_t Read; /* 缓冲区读指针1 */
            uint8_t Write; /* 缓冲区写指针 */
            uint8_t Read2; /* 缓冲区读指针2 */
    };

    class Key
    {
        public:
            void InitKeyVar();
            void KeyScan();
            void PutKey(uint8_t _KeyCode);
            uint8_t GetKeyState(KEY_ID_E _ucKeyID);
            void SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t _RepeatSpeed);
            uint8_t GetKeyCode();

            void SetKeyDetectFunc(uint8_t(*func)(), uint8_t pos = 0);
        public:

        private:
            void DetectKey(uint8_t i);
        private:
            KEY_FIFO s_tKey;
            KEY_T s_tBtn[KEY_COUNT];
			uint8_t flagLongkey;//长按标记，长按下时不发送弹起指令
    };
#endif
