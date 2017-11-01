#include "stm32f10x.h"
#include "bsp_key.h"
#include "Port.h"

Key keytest;

OutputPort key11(PC13);
OutputPort key22(PA0);

static KEY_T s_tBtn[KEY_COUNT];
KEY_FIFO_T s_tKey; /* ����FIFO����,�ṹ�� */

/*
 *********************************************************************************************************
 *	�� �� ��: IsKeyDownX
 *	����˵��: �жϰ����Ƿ���
 *	��    ��: ��
 *	�� �� ֵ: ����ֵ1 ��ʾ���£�0��ʾδ����
 *********************************************************************************************************
 */
/* ������ STM32-V4 ������ */
/* Ϊ������3���¼�:��K1��������, K2�������£� K1��K2ͬʱ���� */
byte IsKeyDown1()
{
    if (key11 == 0 && key22 == 0)
        return 1;
    else
        return 0;
}

byte IsKeyDown2()
{
    if (key11 != 0 && key22 != 0)
        return 1;
    else
        return 0;
}

byte IsKeyDown9() /* K1 K2��ϼ� */
{
    if (key11 == 0 && key22 != 0)
        return 1;
    else
        return 0;
}

/*
 *********************************************************************************************************
 *	�� �� ��: bsp_InitKey
 *	����˵��: ��ʼ������. �ú����� bsp_Init() ���á�
 *	��    ��:  ��
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void Key::InitKey(void)
{
    InitKeyVar(); /* ��ʼ���������� */
}

/*
 *********************************************************************************************************
 *	�� �� ��: bsp_PutKey
 *	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
 *	��    ��:  _KeyCode : ��������
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void Key::PutKey(byte _KeyCode)
{
	s_tKey.Push(_KeyCode);
}
/*
 *********************************************************************************************************
 *	�� �� ��: bsp_GetKeyState
 *	����˵��: ��ȡ������״̬
 *	��    ��:  _ucKeyID : ����ID����0��ʼ
 *	�� �� ֵ: 1 ��ʾ���£� 0 ��ʾδ����
 *********************************************************************************************************
 */
byte Key::GetKeyState(KEY_ID_E _ucKeyID)
{
    return s_tBtn[_ucKeyID].State;
}

/*
 *********************************************************************************************************
 *	�� �� ��: bsp_SetKeyParam
 *	����˵��: ���ð�������
 *	��    �Σ�_ucKeyID : ����ID����0��ʼ
 *			_LongTime : �����¼�ʱ��
 *			 _RepeatSpeed : �����ٶ�
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void Key::SetKeyParam(byte _ucKeyID, ushort _LongTime, byte _RepeatSpeed)
{
    s_tBtn[_ucKeyID].LongTime = _LongTime; /* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
    s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed; /* �����������ٶȣ�0��ʾ��֧������ */
    s_tBtn[_ucKeyID].RepeatCount = 0; /* ���������� */
}
/*
 *********************************************************************************************************
 *	�� �� ��: bsp_InitKeyVar
 *	����˵��: ��ʼ����������
 *	��    ��:  ��
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void Key::InitKeyVar()
{
    byte i;

    /* �԰���FIFO��дָ������ */
    s_tKey.Init();

    /* ��ÿ�������ṹ���Ա������һ��ȱʡֵ */
    for (i = 0; i < KEY_COUNT; i++)
    {
        s_tBtn[i].LongTime = KEY_LONG_TIME; /* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
        s_tBtn[i].Count = KEY_FILTER_TIME / 2; /* ����������Ϊ�˲�ʱ���һ�� */
        s_tBtn[i].State = 0; /* ����ȱʡ״̬��0Ϊδ���� */
        //s_tBtn[i].KeyCodeDown = 3 * i + 1;				/* �������µļ�ֵ���� */
        //s_tBtn[i].KeyCodeUp   = 3 * i + 2;				/* ��������ļ�ֵ���� */
        //s_tBtn[i].KeyCodeLong = 3 * i + 3;				/* �������������µļ�ֵ���� */
        s_tBtn[i].RepeatSpeed = 0; /* �����������ٶȣ�0��ʾ��֧������ */
        s_tBtn[i].RepeatCount = 0; /* ���������� */
    }

    /* �����Ҫ��������ĳ�������Ĳ����������ڴ˵������¸�ֵ */
    /* ���磬����ϣ������1���³���1����Զ��ط���ͬ��ֵ */
    s_tBtn[KID_JOY_U].LongTime = 100;
    s_tBtn[KID_JOY_U].RepeatSpeed = 5; /* ÿ��50ms�Զ����ͼ�ֵ */

    /* �жϰ������µĺ��� */
    s_tBtn[0].IsKeyDownFunc = IsKeyDown1;
    s_tBtn[1].IsKeyDownFunc = IsKeyDown2;

    /* ��ϼ� */
    s_tBtn[2].IsKeyDownFunc = IsKeyDown9;
}

/*
 *********************************************************************************************************
 *	�� �� ��: bsp_DetectKey
 *	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
 *	��    ��:  �����ṹ����ָ��
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void Key::DetectKey(byte i)
{
    KEY_T *pBtn;

    /*
    ���û�г�ʼ�������������򱨴�
    if (s_tBtn[i].IsKeyDownFunc == 0)
    {
    printf("Fault : DetectButton(), s_tBtn[i].IsKeyDownFunc undefine");
    }
     */

    pBtn = &s_tBtn[i];
    if (pBtn->IsKeyDownFunc())
    {
        if (pBtn->Count < KEY_FILTER_TIME)
        {
            pBtn->Count = KEY_FILTER_TIME;
        }
        else if (pBtn->Count < 2 *KEY_FILTER_TIME)
        {
            pBtn->Count++;
        }
        else
        {
            if (pBtn->State == 0)
            {
                pBtn->State = 1;

                /* ���Ͱ�ť���µ���Ϣ */
                PutKey((byte)(3 *i + 1));
            }

            if (pBtn->LongTime > 0)
            {
                if (pBtn->LongCount < pBtn->LongTime)
                {
                    /* ���Ͱ�ť�������µ���Ϣ */
                    if (++pBtn->LongCount == pBtn->LongTime)
                    {
                        /* ��ֵ���밴��FIFO */
                        PutKey((byte)(3 *i + 3));
                    }
                }
                else
                {
                    if (pBtn->RepeatSpeed > 0)
                    {
                        if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
                        {
                            pBtn->RepeatCount = 0;
                            /* ��������ÿ��10ms����1������ */
                            PutKey((byte)(3 *i + 1));
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (pBtn->Count > KEY_FILTER_TIME)
        {
            pBtn->Count = KEY_FILTER_TIME;
        }
        else if (pBtn->Count != 0)
        {
            pBtn->Count--;
        }
        else
        {
            if (pBtn->State == 1)
            {
                pBtn->State = 0;

                /* ���Ͱ�ť�������Ϣ */
                PutKey((byte)(3 *i + 2));
            }
        }

        pBtn->LongCount = 0;
        pBtn->RepeatCount = 0;
    }
}

/*
 *********************************************************************************************************
 *	�� �� ��: bsp_KeyScan
 *	����˵��: ɨ�����а���������������systick�ж������Եĵ���
 *	��    ��:  ��
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void Key::KeyScan()
{
    byte i;

    for (i = 0; i < KEY_COUNT; i++)
    {
        DetectKey(i);
    }
}

#ifdef DEBUG
    void readkeyroutin(void *param)
    {
        keytest.KeyScan();
    }
    void keycoderoutin(void *param)
    {
        int aa = s_tKey.Pop();
        printf("���룺%d\r\n", aa);
    }

    void keyTest()
    {
		key11.OpenDrain=false;
		key22.OpenDrain=false;
		
		key11.Invert=0;
		key22.Invert=0;
		
		key11.Open();
		key22.Open();
		
        keytest.InitKey();
        Sys.AddTask(readkeyroutin, 0, 0, 10, "readkeyroutin");
        Sys.AddTask(keycoderoutin, 0, 0, 1000, "keycoderoutin");
    }
#endif
