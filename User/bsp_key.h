#ifndef _BSP_KEY_H
    #define _BSP_KEY_H
    #include "Sys.h"
    #define KEY_COUNT    3	   					/* ��������, 2�������� + 1����ϼ� */

    /* ����ID, ��Ҫ����bsp_KeyState()��������ڲ��� */
    typedef enum
    {
        KID_K1 = 0, KID_K2, KID_K3, KID_JOY_U
    } KEY_ID_E;
    /*
    �����ֵ����, ���밴���´���ʱÿ�����İ��¡�����ͳ����¼�

    �Ƽ�ʹ��enum, ����#define��ԭ��
    (1) ����������ֵ,�������˳��ʹ���뿴���������
    (2) �������ɰ����Ǳ����ֵ�ظ���
     */
    typedef enum
    {
        KEY_NONE = 0,  /* 0 ��ʾ�����¼� */

        KEY_1_DOWN,  /* 1������ */
        KEY_1_UP,  /* 1������ */
        KEY_1_LONG,  /* 1������ */

        KEY_2_DOWN,  /* 2������ */
        KEY_2_UP,  /* 2������ */
        KEY_2_LONG,  /* 2������ */

        /* ��ϼ� */
        KEY_9_DOWN,  /* 9������ */
        KEY_9_UP,  /* 9������ */
        KEY_9_LONG,  /* 9������ */
    } KEY_ENUM;
    /*
    �����˲�ʱ��50ms, ��λ10ms��
    ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
    ��ʹ������·����Ӳ���˲������˲�����Ҳ���Ա�֤�ɿ��ؼ�⵽�����¼�
     */
    #define KEY_FILTER_TIME   5
    #define KEY_LONG_TIME     100			/* ��λ10ms�� ����1�룬��Ϊ�����¼� */    
    class KEY_T /*ÿ��������Ӧ1��ȫ�ֵĽṹ�������
     */
    {
        public:
            /* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
            byte(*IsKeyDownFunc)(); /* �������µ��жϺ���,1��ʾ���� */

            byte Count; /* �˲��������� */
            ushort LongCount; /* ���������� */
            ushort LongTime; /* �������³���ʱ��, 0��ʾ����ⳤ�� */
            byte State; /* ������ǰ״̬�����»��ǵ��� */
            byte RepeatSpeed; /* ������������ */
            byte RepeatCount; /* �������������� */
    };
    class KEY_FIFO_T
    {
        public:
            void Init();
            void Clear();
            void Push(byte da);
            byte Pop();
            byte Pop2();
        private:
			const int _bufSize=10;
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
        public:
            KEY_T s_tBtn[KEY_COUNT];
        private:
            void DetectKey(byte i);
		private:
			KEY_FIFO_T s_tKey; /* ����FIFO����,�ṹ�� */            
    };
#endif
