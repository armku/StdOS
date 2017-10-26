#include "Drivers\SSD1309.h"
#include "bmp.h"

#ifdef DEBUG
    void ssd1309routin(void *param)
    {
        static int cnt;
        static byte t;
        if (++t > 100)
        {
            t = 0;
        }
        OLED_Clear();
        switch (++cnt)
        {
            case 1:
                OLED_ShowCHinese(0, 0, 0); //��
                OLED_ShowCHinese(18, 0, 1); //��
                OLED_ShowCHinese(36, 0, 2); //԰
                OLED_ShowCHinese(54, 0, 3); //��
                OLED_ShowCHinese(72, 0, 4); //��
                OLED_ShowCHinese(90, 0, 5); //��
                OLED_ShowCHinese(108, 0, 6); //��
                OLED_ShowString(0, 3, (byte*)"1.54' OLED TEST");
                //OLED_ShowString(8,2,"ZHONGJINGYUAN");  
                //	OLED_ShowString(20,4,"2014/05/01");  
                OLED_ShowString(0, 6, (byte*)"ASCII:");
                OLED_ShowString(63, 6, (byte*)"CODE:");
                OLED_ShowChar(48, 6, t); //��ʾASCII�ַ�	  
                OLED_ShowNum(103, 6, t, 3, 16); //��ʾASCII�ַ�����ֵ 
                break;
            case 2:
                OLED_ShowString(30, 0, (byte*)"OLED TEST");
                OLED_ShowString(8, 2, (byte*)"ZHONGJINGYUAN");
                OLED_ShowString(20, 4, (byte*)"2014/05/01");
                OLED_ShowString(0, 6, (byte*)"ASCII:");
                OLED_ShowString(63, 6, (byte*)"CODE:");
                OLED_ShowCHinese(0, 0, 0); //��
                OLED_ShowCHinese(18, 0, 1); //��
                OLED_ShowCHinese(36, 0, 2); //԰
                OLED_ShowCHinese(54, 0, 3); //��
                OLED_ShowCHinese(72, 0, 4); //��
                OLED_ShowCHinese(90, 0, 5); //��
                OLED_ShowCHinese(108, 0, 6); //��
                break;
            case 3:
                OLED_DrawBMP(0, 0, 128, 8, BMP1); //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������) 
                break;
            case 4:
                OLED_DrawBMP(0, 0, 128, 8, BMP2);
				cnt=0;
                break;
            default:
                cnt = 0;
                break;
        }
    }

    void ssd1309Test()
    {
        OLED_Init(); //��ʼ��OLED  
        OLED_Clear();

        Sys.AddTask(ssd1309routin, 0, 100, 1000, "ssd1309routin");
    }
#endif
