#include "bsp_key.h"
#include "Port.h"

void KEY_FIFO::Init()
{
    this->Write = 0;
    this->Read = 0;
    this->Read2 = 0;
}

void KEY_FIFO::Clear()
{
    this->Read = this->Write;
}

void KEY_FIFO::Push(byte da)
{
    this->Buf[this->Write] = da;
    if (++this->Write >= this->_bufSize)
    {
        this->Write = 0;
    }
}

byte KEY_FIFO::Pop()
{
    byte ret;

    if (this->Read == this->Write)
    {
        return KEY_NONE;
    }
    else
    {
        ret = this->Buf[this->Read];

        if (++this->Read >= this->_bufSize)
        {
            this->Read = 0;
        }
        return ret;
    }
}

byte KEY_FIFO::Pop2()
{
    byte ret;

    if (this->Read2 == this->Write)
    {
        return KEY_NONE;
    }
    else
    {
        ret = this->Buf[this->Read2];

        if (++this->Read2 >= this->_bufSize)
        {
            this->Read2 = 0;
        }
        return ret;
    }
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
    
    //���û�г�ʼ�������������򱨴�
    if (s_tBtn[i].IsKeyDownFunc == 0)
    {
    printf("Fault : DetectButton(), s_tBtn[i].IsKeyDownFunc undefine");
    }
     

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
        this->DetectKey(i);
    }
}
byte Key::GetKeyCode()
{
	return this->s_tKey.Pop();
}

#ifdef DEBUG
	
    void readkeyroutin(void *param)
    {
		Key * key=(Key*)param;
        key->KeyScan();
    }
    void keycoderoutin(void *param)
    {
		Key * key=(Key*)param;
        int ucKeyCode = key->GetKeyCode();
        if (ucKeyCode != KEY_NONE)
        {
            switch (ucKeyCode)
            {
                case KEY_1_DOWN:
                    /* ҡ��LEFT������, ����LED��ʾ���� */
                    printf("���룺%d KEY_1_DOWN\r\n", ucKeyCode);
                    break;
                case KEY_2_DOWN:
                    /* ҡ��RIGHT������ */
                    printf("���룺%d KEY_2_DOWN\r\n", ucKeyCode);
                    break;
                case KEY_9_DOWN:
                    /* ҡ��UP������ */
                    printf("���룺%d KEY_9_DOWN\r\n", ucKeyCode);
                    break;
                case KEY_1_UP:
                    /* ҡ��DOWN������ */
                    printf("���룺%d KEY_1_UP\r\n", ucKeyCode);
                    break;
                case KEY_1_LONG:
                    /* ҡ��OK������ */
                    printf("���룺%d KEY_1_LONG\r\n", ucKeyCode);
                    break;
                default:
                    break;
            }
        }
    }
    OutputPort key11(PA0);
    OutputPort key22(PC13);
    /*
     *********************************************************************************************************
     *	�� �� ��: IsKeyDownX
     *	����˵��: �жϰ����Ƿ���
     *	��    ��: ��
     *	�� �� ֵ: ����ֵ1 ��ʾ���£�0��ʾδ����
     *********************************************************************************************************
     */
    byte IsKeyDown1()
    {
        if (key11 != 0 && key22 != 0)
            return 1;
        else
            return 0;
    }

    byte IsKeyDown2()
    {
        if (key11 == 0 && key22 == 0)
            return 1;
        else
            return 0;
    }

    byte IsKeyDown9() /* K1 K2��ϼ� */
    {
        if (key11 != 0 && key22 == 0)
            return 1;
        else
            return 0;
    }
	Key keytest;
    void keyTest()
    {
        key11.OpenDrain = false;
        key22.OpenDrain = false;

        key11.Invert = 0;
        key22.Invert = 0;

        key11.Open();
        key22.Open();
		
		keytest.InitKeyVar();

        /* �жϰ������µĺ��� */
        keytest.s_tBtn[0].IsKeyDownFunc = IsKeyDown1;
        keytest.s_tBtn[1].IsKeyDownFunc = IsKeyDown2;

        /* ��ϼ� */
        keytest.s_tBtn[2].IsKeyDownFunc = IsKeyDown9;
        
        Sys.AddTask(readkeyroutin, &keytest, 0, 10, "readkeyroutin");
        Sys.AddTask(keycoderoutin, &keytest, 6, 10, "keycoderoutin");
    }
#endif
