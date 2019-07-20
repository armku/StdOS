#include "bspkey.h"
#include "Sys.h"
/*
�����˲�ʱ��50ms, ��λ10ms��
ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
��ʹ������·����Ӳ���˲������˲�����Ҳ���Ա�֤�ɿ��ؼ�⵽�����¼�
 */
#define KEY_FILTER_TIME   5
#define KEY_LONG_TIME     100			/* ��λ10ms�� ����1�룬��Ϊ�����¼� */ 

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

void KEY_FIFO::Push(uint8_t da)
{
    this->Buf[this->Write] = da;
    if (++this->Write >= this->_bufSize)
    {
        this->Write = 0;
    }
}

uint8_t KEY_FIFO::Pop()
{
    uint8_t ret;

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

uint8_t KEY_FIFO::Pop2()
{
    uint8_t ret;

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
void Key::PutKey(uint8_t _KeyCode)
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
uint8_t Key::GetKeyState(KEY_ID_E _ucKeyID)
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
void Key::SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t _RepeatSpeed)
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
    uint8_t i;

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
    s_tBtn[KID_KOK].LongTime = 100;
    s_tBtn[KID_KOK].RepeatSpeed = 5; /* ÿ��50ms�Զ����ͼ�ֵ */
	
	s_tBtn[KID_KLEFT].LongTime = 100;
    s_tBtn[KID_KLEFT].RepeatSpeed = 5; /* ÿ��50ms�Զ����ͼ�ֵ */
	
	s_tBtn[KID_KDOWN].LongTime = 100;
    s_tBtn[KID_KDOWN].RepeatSpeed = 5; /* ÿ��50ms�Զ����ͼ�ֵ */
	
	s_tBtn[KID_KUP].LongTime = 100;
    s_tBtn[KID_KUP].RepeatSpeed = 5; /* ÿ��50ms�Զ����ͼ�ֵ */
}

/*
 *********************************************************************************************************
 *	�� �� ��: bsp_DetectKey
 *	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
 *	��    ��:  �����ṹ����ָ��
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void Key::DetectKey(uint8_t i)
{
    KEY_T *pBtn;

    //���û�г�ʼ�������������򱨴�
	#ifdef DEBUG
    if (s_tBtn[i].IsKeyDownFunc == 0)
    {
        debug_printf("Fault : DetectButton(), s_tBtn[i].IsKeyDownFunc undefine");
    }
	#endif

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
                PutKey((uint8_t)(3 *i + 1));
            }

            if (pBtn->LongTime > 0)
            {
                if (pBtn->LongCount < pBtn->LongTime)
                {
                    /* ���Ͱ�ť�������µ���Ϣ */
                    if (++pBtn->LongCount == pBtn->LongTime)
                    {
                        /* ��ֵ���밴��FIFO */
                        PutKey((uint8_t)(3 *i + 3));
						this->flagLongkey = 1;
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
                            PutKey((uint8_t)(3 *i + 1));
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
				if (!this->flagLongkey)
				{
					PutKey((uint8_t)(3 * i + 2));
				}
				this->flagLongkey = 0;
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
    uint8_t i;

    for (i = 0; i < KEY_COUNT; i++)
    {
        this->DetectKey(i);
    }
}

uint8_t Key::GetKeyCode()
{
    return this->s_tKey.Pop();
}

void Key::SetKeyDetectFunc(uint8_t(*func)(), uint8_t pos)
{
    this->s_tBtn[pos].IsKeyDownFunc = func;
}
