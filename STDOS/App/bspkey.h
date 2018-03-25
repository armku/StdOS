#ifndef _BSP_KEY_H
    #define _BSP_KEY_H
    #include "Sys.h"
    #define KEY_COUNT    3	   					/* �������� */

    /* ����ID, ��Ҫ����bsp_KeyState()��������ڲ��� */
    typedef enum
    {
        KID_K1 = 0, KID_K2, KID_K3
    } KEY_ID_E;		
	typedef enum
	{
		KEY_NONE = 0,  /* 0 ��ʾ�����¼� */

		KEYDOWN_1,  /* 1������ */
		KEYUP_1,  /* 1������ */
		KEYLONG_1,  /* 1������ */

		KEYDOWN_2,  /* 2������ */
		KEYUP_2,  /* 2������ */
		KEYLONG_2,  /* 2������ */

		/* ��ϼ� */
		KEYDOWN_9,  /* 9������ */
		KEYUP_9,  /* 9������ */
		KEYLONG_9,  /* 9������ */
	} KEY_ENUM;
    class KEY_T /*ÿ��������Ӧ1��ȫ�ֵĽṹ�������
     */
    {
        public:
            byte(*IsKeyDownFunc)(); /* �������µ��жϺ���,1��ʾ���� */

            byte Count; /* �˲��������� */
            uint16_t LongCount; /* ���������� */
            uint16_t LongTime; /* �������³���ʱ��, 0��ʾ����ⳤ�� */
            byte State; /* ������ǰ״̬�����»��ǵ��� */
            byte RepeatSpeed; /* ������������ */
            byte RepeatCount; /* �������������� */
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
            const int _bufSize = 10;
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
            void SetKeyParam(byte _ucKeyID, uint16_t _LongTime, byte _RepeatSpeed);
            byte GetKeyCode();

            void SetKeyDetectFunc(byte(*func)(), byte pos = 0);
        public:

        private:
            void DetectKey(byte i);
        private:
            KEY_FIFO s_tKey;
            KEY_T s_tBtn[KEY_COUNT];
    };
#endif
