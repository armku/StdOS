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
                    /* 摇杆LEFT键按下, 控制LED显示流动 */
                    printf("键码：%d KEY_1_DOWN\r\n", ucKeyCode);
                    break;
                case KEY_2_DOWN:
                    /* 摇杆RIGHT键按下 */
                    printf("键码：%d KEY_2_DOWN\r\n", ucKeyCode);
                    break;
                case KEY_9_DOWN:
                    /* 摇杆UP键按下 */
                    printf("键码：%d KEY_9_DOWN\r\n", ucKeyCode);
                    break;
                case KEY_1_UP:
                    /* 摇杆DOWN键按下 */
                    printf("键码：%d KEY_1_UP\r\n", ucKeyCode);
                    break;
                case KEY_1_LONG:
                    /* 摇杆OK键按下 */
                    printf("键码：%d KEY_1_LONG\r\n", ucKeyCode);
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
     *	函 数 名: IsKeyDownX
     *	功能说明: 判断按键是否按下
     *	形    参: 无
     *	返 回 值: 返回值1 表示按下，0表示未按下
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

    byte IsKeyDown9() /* K1 K2组合键 */
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

        /* 判断按键按下的函数 */
        keytest.SetKeyDetectFunc(IsKeyDown1, 0);
        keytest.SetKeyDetectFunc(IsKeyDown2, 1);
        keytest.SetKeyDetectFunc(IsKeyDown9, 2); //组合按键

        Sys.AddTask(readkeyroutin, &keytest, 0, 10, "keyrread");
        Sys.AddTask(keycoderoutin, &keytest, 6, 10, "keyroutin");
    }
#endif
