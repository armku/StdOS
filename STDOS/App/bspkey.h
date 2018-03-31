#ifndef _BSP_KEY_H
    #define _BSP_KEY_H
    #include "Sys.h"
    #define KEY_COUNT    4	   					/* ��������, 2�������� + 1����ϼ� */
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
		
		KEYDOWN_OK_LEFT,
		KEYUP_OK_LEFT,
		KEYLONG_OK_LEFT,
		
		KEYDOWN_OK_UP,
		KEYUP_OK_UP,
		KEYLONG_OK_UP,
		
	
        KEYDOWN_CFG,// = KEY_LEFT | KEY_MENU,  // ����������ý���
		KEYUP_CFG,
		KEYLONG_CFG,		
		
        KEYDOWN_CAL,// = KEY_UP | KEY_MENU,  // У�����        
		KEYUP_CAL,
		KEYLONG_CAL,
		
		KEYDOWN_DEMOCAL,// = KEY_UP | KEY_MENU,  // У�����        
		KEYUP_DEMOCAL,
		KEYLONG_DEMOCAL,		
    } KEY_ENUM;
    class KEY_T /*ÿ��������Ӧ1��ȫ�ֵĽṹ�������
     */
    {
        public:
            uint8_t(*IsKeyDownFunc)(); /* �������µ��жϺ���,1��ʾ���� */

            uint8_t Count; /* �˲��������� */
            uint16_t LongCount; /* ���������� */
            uint16_t LongTime; /* �������³���ʱ��, 0��ʾ����ⳤ�� */
            uint8_t State; /* ������ǰ״̬�����»��ǵ��� */
            uint8_t RepeatSpeed; /* ������������ */
            uint8_t RepeatCount; /* �������������� */			
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
            uint8_t Buf[10]; /* ��ֵ������ */
            uint8_t Read; /* ��������ָ��1 */
            uint8_t Write; /* ������дָ�� */
            uint8_t Read2; /* ��������ָ��2 */
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
			uint8_t flagLongkey;//������ǣ�������ʱ�����͵���ָ��
    };
#endif
