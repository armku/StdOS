#ifndef _LCD_DR_H
#define _LCD_DR_H

    #include <stdint.h>
    #include "Drivers\ST7565R.h"

    typedef enum
    {
        FONT_4X8, FONT_5X8, FONT_8X8, FONT_6X12, FONT_12X12, FONT_6X16,FONT_8X16,FONT_8X16BOLT, FONT_16X16, FONT_16X32, FONT_32X32
    } Font;

    class CLcd_DR: public CLcd
    {
        public:
            CLcd_DR();
            void DisplayChar4x8(ushort x, ushort y, byte ch);
            void DisplayString4x8(ushort x, ushort y, char *text);
            void DisplayChar5x8(ushort x, ushort y, byte ch);
            void DisplayString5x8(ushort x, ushort y, char *text);
            void DisplayChar6x12(ushort x, ushort y, byte ch);
			void DisplayChar6x16(ushort x, ushort y, byte ch);
			void DisplayChar8x16(ushort x, ushort y, byte ch);
			void DisplayChar8x16Bolt(ushort x, ushort y, byte ch);
            void DisplayString8x16(ushort x, ushort y, char *text);

            void DisplayLattice(ushort x, ushort y, byte *dp, Font font); //��ʾ����ͼ��

            void DisplayHEX(ushort x, ushort y, byte da, Font font = FONT_8X16);
            void DisplayStr(ushort x, ushort y, char *text, Font font = FONT_8X16); //�ַ�����ʾ        
            void DispVal(ushort x, ushort y, float val, int8_t wei = 0, Font font = FONT_5X8); //��ʾ��ֵ float����
            void DispValu(ushort x, ushort y, ushort val, int wei =  - 1, byte reversebit = 0, Font font = FONT_8X16); //��ʾ��ֵ �̶�λ�÷���

        private:
            ushort SearchhzIndex(byte lo, byte hi, Font font); //���Һ��ֱ���λ��
    };
#endif
