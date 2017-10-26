#include "Drivers\SSD1309.h"
#include "bmp.h"

#ifdef DEBUG
	SSD1309 ssd1309;
    void ssd1309routin(void *param)
    {
        static int cnt;
        static byte t;
        if (++t > 100)
        {
            t = 0;
        }
        ssd1309.OLED_Clear();
        switch (++cnt)
        {
            case 1:
                ssd1309.OLED_ShowCHinese(0, 0, 0); //中
                ssd1309.OLED_ShowCHinese(18, 0, 1); //景
                ssd1309.OLED_ShowCHinese(36, 0, 2); //园
                ssd1309.OLED_ShowCHinese(54, 0, 3); //电
                ssd1309.OLED_ShowCHinese(72, 0, 4); //子
                ssd1309.OLED_ShowCHinese(90, 0, 5); //科
                ssd1309.OLED_ShowCHinese(108, 0, 6); //技
                ssd1309.OLED_ShowString(0, 3, (byte*)"1.54' OLED TEST");
                //OLED_ShowString(8,2,"ZHONGJINGYUAN");  
                //	OLED_ShowString(20,4,"2014/05/01");  
                ssd1309.OLED_ShowString(0, 6, (byte*)"ASCII:");
                ssd1309.OLED_ShowString(63, 6, (byte*)"CODE:");
                ssd1309.OLED_ShowChar(48, 6, t); //显示ASCII字符	  
                ssd1309.OLED_ShowNum(103, 6, t, 3, 16); //显示ASCII字符的码值 
                break;
            case 2:
                ssd1309.OLED_ShowString(30, 0, (byte*)"OLED TEST");
                ssd1309.OLED_ShowString(8, 2, (byte*)"ZHONGJINGYUAN");
                ssd1309.OLED_ShowString(20, 4, (byte*)"2014/05/01");
                ssd1309.OLED_ShowString(0, 6, (byte*)"ASCII:");
                ssd1309.OLED_ShowString(63, 6, (byte*)"CODE:");
                ssd1309.OLED_ShowCHinese(0, 0, 0); //中
                ssd1309.OLED_ShowCHinese(18, 0, 1); //景
                ssd1309.OLED_ShowCHinese(36, 0, 2); //园
                ssd1309.OLED_ShowCHinese(54, 0, 3); //电
                ssd1309.OLED_ShowCHinese(72, 0, 4); //子
                ssd1309.OLED_ShowCHinese(90, 0, 5); //科
                ssd1309.OLED_ShowCHinese(108, 0, 6); //技
                break;
            case 3:
                ssd1309.OLED_DrawBMP(0, 0, 128, 8, BMP1); //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用) 
                break;
            case 4:
                ssd1309.OLED_DrawBMP(0, 0, 128, 8, BMP2);
				cnt=0;
                break;
            default:
                cnt = 0;
                break;
        }
    }

    void ssd1309Test()
    {
        ssd1309.OLED_Init(); //初始化OLED  
        ssd1309.OLED_Clear();

        Sys.AddTask(ssd1309routin, 0, 100, 1000, "ssd1309routin");
    }
#endif
