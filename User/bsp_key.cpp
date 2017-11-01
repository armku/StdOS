#include "stm32f10x.h"
#include "bsp_key.h"

Key keytest;
/* �����ڶ�Ӧ��RCCʱ�� */
#define RCC_ALL_KEY 	(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC)

#define GPIO_PORT_K1    GPIOC
#define GPIO_PIN_K1	    GPIO_Pin_13

#define GPIO_PORT_K2    GPIOA
#define GPIO_PIN_K2	    GPIO_Pin_0

static KEY_T s_tBtn[KEY_COUNT];
static KEY_FIFO_T s_tKey; /* ����FIFO����,�ṹ�� */

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
    if ((GPIO_PORT_K1->IDR &GPIO_PIN_K1) == 0 && (GPIO_PORT_K2->IDR &GPIO_PIN_K2) == 0)
        return 1;
    else
        return 0;
}

byte IsKeyDown2()
{
    if ((GPIO_PORT_K1->IDR &GPIO_PIN_K1) != 0 && (GPIO_PORT_K2->IDR &GPIO_PIN_K2) != 0)
        return 1;
    else
        return 0;
}

byte IsKeyDown9() /* K1 K2��ϼ� */
{
    if ((GPIO_PORT_K1->IDR &GPIO_PIN_K1) == 0 && (GPIO_PORT_K2->IDR &GPIO_PIN_K2) != 0)
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
void Key::bsp_InitKey(void)
{
    bsp_InitKeyVar(); /* ��ʼ���������� */
    bsp_InitKeyHard(); /* ��ʼ������Ӳ�� */
}

/*
 *********************************************************************************************************
 *	�� �� ��: bsp_PutKey
 *	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
 *	��    ��:  _KeyCode : ��������
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void Key::bsp_PutKey(byte _KeyCode)
{
    s_tKey.Buf[s_tKey.Write] = _KeyCode;

    if (++s_tKey.Write >= KEY_FIFO_SIZE)
    {
        s_tKey.Write = 0;
    }
}

/*
 *********************************************************************************************************
 *	�� �� ��: bsp_GetKey
 *	����˵��: �Ӱ���FIFO��������ȡһ����ֵ��
 *	��    ��:  ��
 *	�� �� ֵ: ��������
 *********************************************************************************************************
 */
byte Key::bsp_GetKey(void)
{
    byte ret;

    if (s_tKey.Read == s_tKey.Write)
    {
        return KEY_NONE;
    }
    else
    {
        ret = s_tKey.Buf[s_tKey.Read];

        if (++s_tKey.Read >= KEY_FIFO_SIZE)
        {
            s_tKey.Read = 0;
        }
        return ret;
    }
}

/*
 *********************************************************************************************************
 *	�� �� ��: bsp_GetKey2
 *	����˵��: �Ӱ���FIFO��������ȡһ����ֵ�������Ķ�ָ�롣
 *	��    ��:  ��
 *	�� �� ֵ: ��������
 *********************************************************************************************************
 */
byte Key::bsp_GetKey2(void)
{
    byte ret;

    if (s_tKey.Read2 == s_tKey.Write)
    {
        return KEY_NONE;
    }
    else
    {
        ret = s_tKey.Buf[s_tKey.Read2];

        if (++s_tKey.Read2 >= KEY_FIFO_SIZE)
        {
            s_tKey.Read2 = 0;
        }
        return ret;
    }
}

/*
 *********************************************************************************************************
 *	�� �� ��: bsp_GetKeyState
 *	����˵��: ��ȡ������״̬
 *	��    ��:  _ucKeyID : ����ID����0��ʼ
 *	�� �� ֵ: 1 ��ʾ���£� 0 ��ʾδ����
 *********************************************************************************************************
 */
byte Key::bsp_GetKeyState(KEY_ID_E _ucKeyID)
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
void Key::bsp_SetKeyParam(byte _ucKeyID, ushort _LongTime, byte _RepeatSpeed)
{
    s_tBtn[_ucKeyID].LongTime = _LongTime; /* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
    s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed; /* �����������ٶȣ�0��ʾ��֧������ */
    s_tBtn[_ucKeyID].RepeatCount = 0; /* ���������� */
}


/*
 *********************************************************************************************************
 *	�� �� ��: bsp_ClearKey
 *	����˵��: ��հ���FIFO������
 *	��    �Σ���
 *	�� �� ֵ: ��������
 *********************************************************************************************************
 */
void Key::bsp_ClearKey()
{
    s_tKey.Read = s_tKey.Write;
}

/*
 *********************************************************************************************************
 *	�� �� ��: bsp_InitKeyHard
 *	����˵��: ���ð�����Ӧ��GPIO
 *	��    ��:  ��
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void Key::bsp_InitKeyHard()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ��1������GPIOʱ�� */
    RCC_APB2PeriphClockCmd(RCC_ALL_KEY, ENABLE);

    /* ��2�����������еİ���GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; /* ���븡��ģʽ */

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K1;
    GPIO_Init(GPIO_PORT_K1, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K2;
    GPIO_Init(GPIO_PORT_K2, &GPIO_InitStructure);
}

/*
 *********************************************************************************************************
 *	�� �� ��: bsp_InitKeyVar
 *	����˵��: ��ʼ����������
 *	��    ��:  ��
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void Key::bsp_InitKeyVar()
{
    byte i;

    /* �԰���FIFO��дָ������ */
    s_tKey.Read = 0;
    s_tKey.Write = 0;
    s_tKey.Read2 = 0;

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
void Key::bsp_DetectKey(byte i)
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
                bsp_PutKey((byte)(3 *i + 1));
            }

            if (pBtn->LongTime > 0)
            {
                if (pBtn->LongCount < pBtn->LongTime)
                {
                    /* ���Ͱ�ť�������µ���Ϣ */
                    if (++pBtn->LongCount == pBtn->LongTime)
                    {
                        /* ��ֵ���밴��FIFO */
                        bsp_PutKey((byte)(3 *i + 3));
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
                            bsp_PutKey((byte)(3 *i + 1));
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
                bsp_PutKey((byte)(3 *i + 2));
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
void Key::bsp_KeyScan()
{
    byte i;

    for (i = 0; i < KEY_COUNT; i++)
    {
        bsp_DetectKey(i);
    }
}

#ifdef DEBUG
    void readkeyroutin(void *param)
    {
        keytest.bsp_KeyScan();
    }
    void keycoderoutin(void *param)
    {
        int aa = keytest.bsp_GetKey();
        printf("���룺%d\r\n", aa);
    }

    void keyTest()
    {
        keytest.bsp_InitKey();
        Sys.AddTask(readkeyroutin, 0, 0, 10, "readkeyroutin");
        Sys.AddTask(keycoderoutin, 0, 0, 1000, "keycoderoutin");
    }
#endif
