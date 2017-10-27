#include "Drivers\SSD1309.h"
#include "Drivers\SSD1106.h"
#include "Fonts\ssd1309bmp.h"

#ifdef DEBUG
	#if 0
	SSD1309 ssd1309;
	#else
	SSD1106 ssd1309;
	#endif
    void ssd1309routin(void *param)
    {
        static int cnt;
        static byte t;
        if (++t > 100)
        {
            t = 0;
        }
        ssd1309.Clear(0);
        switch (++cnt)
        {
            case 1:
                ssd1309.ShowString(0, 0, "�о�԰���ӿƼ�",2); 
                ssd1309.ShowString(0, 3, "1.54' OLED TEST");
                ssd1309.ShowString(0, 6, "ASCII:");
                ssd1309.ShowString(63, 6, "CODE:");
				t='P';
                ssd1309.ShowChar(48, 6, t); //��ʾASCII�ַ�	  
                ssd1309.ShowNum(103, 6, t, 3, 16); //��ʾASCII�ַ�����ֵ 
                break;
            case 2:
                ssd1309.ShowString(30, 0, "OLED TEST");
                ssd1309.ShowString(8, 2, "ZHONGJINGYUAN");
                ssd1309.ShowString(20, 4, "2014/05/01");
                ssd1309.ShowString(0, 6, "ASCII:");
                ssd1309.ShowString(63, 6, "CODE:"); 
                break;
            case 3:
                ssd1309.DrawBMP(0, 0, 128, 8, BMP1); //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������) 
                break;
            case 4:
                ssd1309.DrawBMP(0, 0, 128, 8, BMP2);
				break;
            case 5:
                ssd1309.Clear(0XFF);
				cnt=0;
                break;
            default:
                cnt = 0;
                break;
        }
    }

    void ssd1309Test()
    {
		#ifdef STM32F1
		ssd1309.SetPinSpi(PD6,PD7,PD5,PD4,PD3);
		#elif defined STM32F4
		ssd1309.SetPinSpi(PH15,PH2,PC13,PI11,PI8);
		#endif
        ssd1309.Init(); //��ʼ��OLED 

        Sys.AddTask(ssd1309routin, 0, 100, 1000, "ssd1309routin");
    }
#endif
