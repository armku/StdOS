#ifndef _BSP_KEY_H
    #define _BSP_KEY_H
    #include "Sys.h"
    #define KEY_COUNT    3	   					/* 按键个数, 2个独立建 + 1个组合键 */

	/* 按键ID, 主要用于bsp_KeyState()函数的入口参数 */
    typedef enum
    {
        KID_K1 = 0, KID_K2, KID_K3
    } KEY_ID_E;    
    /*
    按键滤波时间50ms, 单位10ms。
    只有连续检测到50ms状态不变才认为有效，包括弹起和按下两种事件
    即使按键电路不做硬件滤波，该滤波机制也可以保证可靠地检测到按键事件
     */
    #define KEY_FILTER_TIME   5
    #define KEY_LONG_TIME     100			/* 单位10ms， 持续1秒，认为长按事件 */    
	class Key;
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
            void Init();
            void Clear();
            void Push(byte da);
            byte Pop();
            byte Pop2();
        private:
			const int _bufSize=10;
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
		
			void SetKeyDetectFunc( byte (*func) (),byte pos=0);
        public:
            
        private:
            void DetectKey(byte i);
		private:
			KEY_FIFO s_tKey;     
			KEY_T s_tBtn[KEY_COUNT];
    };
#endif
