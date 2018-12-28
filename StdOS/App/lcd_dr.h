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
            void DisplayChar4x8(uint16_t x, uint16_t y, uint8_t ch);
            void DisplayString4x8(uint16_t x, uint16_t y, char *text);
            void DisplayChar5x8(uint16_t x, uint16_t y, uint8_t ch);
            void DisplayString5x8(uint16_t x, uint16_t y, char *text);
            void DisplayChar6x12(uint16_t x, uint16_t y, uint8_t ch);
			void DisplayChar6x16(uint16_t x, uint16_t y, uint8_t ch);
			void DisplayChar8x16(uint16_t x, uint16_t y, uint8_t ch);
			void DisplayChar8x16Bolt(uint16_t x, uint16_t y, uint8_t ch);
            void DisplayString8x16(uint16_t x, uint16_t y, char *text);

            void DisplayLattice(uint16_t x, uint16_t y, uint8_t *dp, Font font); //��ʾ����ͼ��

            void DisplayHEX(uint16_t x, uint16_t y, uint8_t da, Font font = FONT_8X16);
            void DisplayStr(uint16_t x, uint16_t y, char *text, Font font = FONT_8X16); //�ַ�����ʾ        
            void DispVal(uint16_t x, uint16_t y, float val, int8_t wei = 0, Font font = FONT_5X8); //��ʾ��ֵ float����
            void DispValu(uint16_t x, uint16_t y, uint16_t val, int wei =  - 1, uint8_t reversebit = 0, Font font = FONT_8X16); //��ʾ��ֵ �̶�λ�÷���

        private:
            uint16_t SearchhzIndex(uint8_t lo, uint8_t hi, Font font); //���Һ��ֱ���λ��
    };
#endif
