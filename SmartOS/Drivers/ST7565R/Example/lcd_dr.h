#ifndef _LCD_DR_H
#define _LCD_DR_H

    #include <stdint.h>
    #include "Drivers\ST7565R\ST7565R.h"

    typedef enum
    {
        FONT_4X8, FONT_5X8, FONT_8X8, FONT_6X12, FONT_12X12, FONT_6X16,FONT_8X16,FONT_8X16BOLT, FONT_16X16, FONT_16X32, FONT_32X32
    } Font;

    class CLcd_DR: public CLcd
    {
        public:
            CLcd_DR();
            void DisplayChar4x8(uint16_t x, uint16_t y, byte ch);
            void DisplayString4x8(uint16_t x, uint16_t y, char *text);
            void DisplayChar5x8(uint16_t x, uint16_t y, byte ch);
            void DisplayString5x8(uint16_t x, uint16_t y, char *text);
            void DisplayChar6x12(uint16_t x, uint16_t y, byte ch);
			void DisplayChar6x16(uint16_t x, uint16_t y, byte ch);
			void DisplayChar8x16(uint16_t x, uint16_t y, byte ch);
			void DisplayChar8x16Bolt(uint16_t x, uint16_t y, byte ch);
            void DisplayString8x16(uint16_t x, uint16_t y, char *text);

            void DisplayLattice(uint16_t x, uint16_t y, byte *dp, Font font); //显示点阵图形

            void DisplayHEX(uint16_t x, uint16_t y, byte da, Font font = FONT_8X16);
            void DisplayStr(uint16_t x, uint16_t y, char *text, Font font = FONT_8X16); //字符串显示        
            void DispVal(uint16_t x, uint16_t y, float val, int8_t wei = 0, Font font = FONT_5X8); //显示数值 float类型
            void DispValu(uint16_t x, uint16_t y, uint16_t val, int wei =  - 1, byte reversebit = 0, Font font = FONT_8X16); //显示数值 固定位置反显

        private:
            uint16_t SearchhzIndex(byte lo, byte hi, Font font); //查找汉字编码位置
    };
#endif
