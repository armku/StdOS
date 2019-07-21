#ifndef _BSP_KEY_H
    #define _BSP_KEY_H
    #include "Sys.h"
    #define KEY_COUNT    5	   					/* ��������, 4�������� + 1����ϼ� */
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

    /* ����ID, ��Ҫ����bsp_KeyState()��������ڲ��� */
    typedef enum
    {
        KID_KOK = 0, KID_KLEFT, KID_KDOWN,KID_KUP
    } KEY_ID_E;
    typedef enum
    {
		KEY_NONE = 0,
		
        KEYDOWN_OK,		// ����
		KEYUP_OK,		// ����
		KEYLONG_OK,		// ����
		
		KEYDOWN_LEFT,  
		KEYUP_LEFT,
		KEYLONG_LEFT,		
		
		KEYDOWN_DOWN,  
		KEYUP_DOWN,
		KEYLONG_DOWN,
		
        KEYDOWN_UP,  
		KEYUP_UP,
		KEYLONG_UP,
						
        KEYDOWN_CFG,// = KEY_LEFT | KEY_MENU,  // ����������ý���
		KEYUP_CFG,
		KEYLONG_CFG,		
		        	
    } KEY_ENUM;
    class KEY_T /*ÿ��������Ӧ1��ȫ�ֵĽṹ�������
     */
    {
        public:
            byte(*IsKeyDownFunc)(); /* �������µ��жϺ���,1��ʾ���� */

            byte Count; /* �˲��������� */
            ushort LongCount; /* ���������� */
            ushort LongTime; /* �������³���ʱ��, 0��ʾ����ⳤ�� */
            byte State; /* ������ǰ״̬�����»��ǵ��� */
            byte RepeatSpeed; /* ������������ */
            byte RepeatCount; /* �������������� */			
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
            byte Buf[10]; /* ��ֵ������ */
            byte Read; /* ��������ָ��1 */
            byte Write; /* ������дָ�� */
            byte Read2; /* ��������ָ��2 */
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
			byte flagLongkey;//������ǣ�������ʱ�����͵���ָ��
    };
#endif
