#ifndef _SSD1309_H
    #define _SSD1309_H
    #include "Sys.h"
    #include "Port.h"

    class SSD1309
    {
        public:
            void SetPin(Pin cs, Pin res, Pin dc, Pin wr, Pin rd, Pin sclk, Pin sdin);
            void DisplayOn();
            void DisplayOff();
            void Init();
            void Clear();
//            void DrawPoint(byte x, byte y, byte t);
//            void Fill(byte x1, byte y1, byte x2, byte y2, byte dot);
            void ShowChar(byte x, byte y, byte chr);
            void ShowNum(byte x, byte y, uint num, byte len, byte size);
            void ShowString(byte x, byte y, byte *p);
            void SetPos(unsigned char x, unsigned char y);
            void ShowCHinese(byte x, byte y, byte no);
            void DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
        private:
			void WRByte(byte dat, byte cmd);            
		private:
            uint oled_pow(byte m, byte n);

            OutputPort _cs;
            OutputPort _res;
            OutputPort _dc;
            OutputPort _wr;
            OutputPort _rd;
            OutputPort _sclk;
            OutputPort _sdin;
    };
#endif
