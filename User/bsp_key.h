#ifndef _BSP_KEY_H
#define _BSP_KEY_H
	#include "Sys.h"
	 #define KEY_COUNT    3	   					/* ��������, 8�������� + 2����ϼ� */

    /* ����Ӧ�ó���Ĺ��������������� */
    #define KEY_DOWN_K1		KEY_1_DOWN
    #define KEY_UP_K1		KEY_1_UP
    #define KEY_LONG_K1		KEY_1_LONG

    #define KEY_DOWN_K2		KEY_2_DOWN
    #define KEY_UP_K2		KEY_2_UP
    #define KEY_LONG_K2		KEY_2_LONG

    #define SYS_DOWN_K1K2	KEY_9_DOWN		/* K1 K2 ��ϼ� */
    #define SYS_UP_K1K2	    KEY_9_UP
    #define SYS_LONG_K1K2	KEY_9_LONG

    /* ����ID, ��Ҫ����bsp_KeyState()��������ڲ��� */
    typedef enum
    {
        KID_K1 = 0, KID_K2, KID_K3, KID_JOY_U
    } KEY_ID_E;

    /*
    �����˲�ʱ��50ms, ��λ10ms��
    ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
    ��ʹ������·����Ӳ���˲������˲�����Ҳ���Ա�֤�ɿ��ؼ�⵽�����¼�
     */
    #define KEY_FILTER_TIME   5
    #define KEY_LONG_TIME     100			/* ��λ10ms�� ����1�룬��Ϊ�����¼� */

    /*
    ÿ��������Ӧ1��ȫ�ֵĽṹ�������
     */
    typedef struct
    {
        /* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
        byte(*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

        byte Count; /* �˲��������� */
        ushort LongCount; /* ���������� */
        ushort LongTime; /* �������³���ʱ��, 0��ʾ����ⳤ�� */
        byte State; /* ������ǰ״̬�����»��ǵ��� */
        byte RepeatSpeed; /* ������������ */
        byte RepeatCount; /* �������������� */
    } KEY_T;

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

    /* ����FIFO�õ����� */
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
            byte Buf[KEY_FIFO_SIZE]; /* ��ֵ������ */
            byte Read; /* ��������ָ��1 */
            byte Write; /* ������дָ�� */
            byte Read2; /* ��������ָ��2 */
    };

    class Key
    {
        public:
            /* ���ⲿ���õĺ������� */
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
