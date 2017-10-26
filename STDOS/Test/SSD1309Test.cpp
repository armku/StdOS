#include "Drivers\SSD1309.h"
#include "oled.h"
#include "bmp.h"

#ifdef DEBUG

    void ssd1309Test()
    {
        byte t;
        OLED_Init(); //��ʼ��OLED  
        OLED_Clear();
        //		/*
        OLED_ShowString(30, 0, (byte*)"OLED TEST");
        OLED_ShowString(8, 2, (byte*)"ZHONGJINGYUAN");
        OLED_ShowString(20, 4, (byte*)"2014/05/01");
        OLED_ShowString(0, 6, (byte*)"ASCII:");
        OLED_ShowString(63, 6, (byte*)"CODE:");
        t = ' ';
        OLED_ShowCHinese(0, 0, 0); //��
        OLED_ShowCHinese(18, 0, 1); //��
        OLED_ShowCHinese(36, 0, 2); //԰
        OLED_ShowCHinese(54, 0, 3); //��
        OLED_ShowCHinese(72, 0, 4); //��
        OLED_ShowCHinese(90, 0, 5); //��
        OLED_ShowCHinese(108, 0, 6); //��
        //		*/	
        t = ' ';
        while (1)
        {
            OLED_Clear();
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
            t++;
            if (t > '~')
                t = ' ';
            OLED_ShowNum(103, 6, t, 3, 16); //��ʾASCII�ַ�����ֵ 	


            delay_ms(8000);
            delay_ms(8000);
            delay_ms(8000);
            OLED_Clear();
            delay_ms(8000);
            delay_ms(8000);
            delay_ms(8000);
            OLED_DrawBMP(0, 0, 128, 8, BMP1); //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)
            delay_ms(8000);
            delay_ms(8000);
            delay_ms(8000);
            OLED_DrawBMP(0, 0, 128, 8, BMP2);
            delay_ms(8000);
            delay_ms(8000);
            delay_ms(8000);
        }
    }
#endif
