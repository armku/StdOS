#include "Drivers/bspkey.h"
#include "Port.h"

#ifdef DEBUG

    void readkeyroutin(void *param)
    {
        Key *key = (Key*)param;
        key->KeyScan();
    }
    void keycoderoutin(void *param)
    {
        Key *key = (Key*)param;
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
        keytest.SetKeyDetectFunc(IsKeyDown1, 0);
        keytest.SetKeyDetectFunc(IsKeyDown2, 1);
        keytest.SetKeyDetectFunc(IsKeyDown9, 2); //��ϰ���

        Sys.AddTask(readkeyroutin, &keytest, 0, 10, "keyrread");
        Sys.AddTask(keycoderoutin, &keytest, 6, 10, "keyroutin");
    }
#endif
