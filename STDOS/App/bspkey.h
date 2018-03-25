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
    };
#endif
