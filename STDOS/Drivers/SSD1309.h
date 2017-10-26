#ifndef _SSD1309_H
    #define _SSD1309_H
    #include "Sys.h"

    class SSD1309
    {
        public:
            //OLED¿ØÖÆÓÃº¯Êý
            void OLED_WR_Byte(byte dat, byte cmd);
            void OLED_Display_On(void);
            void OLED_Display_Off(void);
            void OLED_Init(void);
            void OLED_Clear(void);
            void OLED_DrawPoint(byte x, byte y, byte t);
            void OLED_Fill(byte x1, byte y1, byte x2, byte y2, byte dot);
            void OLED_ShowChar(byte x, byte y, byte chr);
            void OLED_ShowNum(byte x, byte y, uint num, byte len, byte size);
            void OLED_ShowString(byte x, byte y, byte *p);
            void OLED_Set_Pos(unsigned char x, unsigned char y);
            void OLED_ShowCHinese(byte x, byte y, byte no);
            void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
        private:
            uint oled_pow(byte m, byte n);

    };
#endif
