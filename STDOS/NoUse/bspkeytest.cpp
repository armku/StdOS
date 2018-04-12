#include "App/bspkey.h"
#include "Port.h"

//#define _BSPKEY_TEST_H

#ifdef _BSPKEY_TEST_H

    void readkeyroutin(void *param)
    {
        Key *key = (Key*)param;
        key->KeyScan();
    }
    void keyDeal(void *param)
    {
        Key *key = (Key*)param;
        int ucKeyCode = key->GetKeyCode();
        if (ucKeyCode != KEY_NONE)
        {
            switch (ucKeyCode)
            {
                case KEYDOWN_1:
                    /* ҡ��LEFT������, ����LED��ʾ���� */
                    debug_printf("���룺%d KEY_1_DOWN\r\n", ucKeyCode);
                    break;
                case KEYDOWN_2:
                    /* ҡ��RIGHT������ */
                    debug_printf("���룺%d KEY_2_DOWN\r\n", ucKeyCode);
                    break;
                case KEYDOWN_9:
                    /* ҡ��UP������ */
                    debug_printf("���룺%d KEY_9_DOWN\r\n", ucKeyCode);
                    break;
                case KEYUP_1:
                    /* ҡ��DOWN������ */
                    debug_printf("���룺%d KEY_1_UP\r\n", ucKeyCode);
                    break;
                case KEYLONG_1:
                    /* ҡ��OK������ */
                    debug_printf("���룺%d KEY_1_LONG\r\n", ucKeyCode);
                    break;
                default:
                    break;
            }
        }
    }
    OutputPort key11(PA0);
    OutputPort key22(PC13);
    //����˵��: �жϰ����Ƿ���,����ֵ1 ��ʾ���£�0��ʾδ����
    uint8_t IsKeyDown1()
    {
        if (key11 != 0 && key22 != 0)
            return 1;
        else
            return 0;
    }

    uint8_t IsKeyDown2()
    {
        if (key11 == 0 && key22 == 0)
            return 1;
        else
            return 0;
    }

    uint8_t IsKeyDown9()
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
        keytest.SetKeyDetectFunc(IsKeyDown9, 2);

        Sys.AddTask(readkeyroutin, &keytest, 0, 10, "keyrread");
        Sys.AddTask(keyDeal, &keytest, 6, 10, "keyDeal");
    }
#endif