#ifndef _SSD1309_H
    #define _SSD1309_H
    #include "Sys.h"
    #include "Port.h"	
	
    class SSD1309
    {
        public:
            void SetPinSpi(Pin sclk,Pin sdin,Pin dc,Pin res=P0,Pin cs=P0);
            void DisplayOn();
            void DisplayOff();
            void Init();
            void Clear(char ch=0x00);
            void ShowChar(byte x, byte y, byte chr);
            void ShowNum(byte x, byte y, uint num, byte len, byte size);
            void ShowString(byte x, byte y, char *p,byte interval=0);
            void ShowCHinese(byte x, byte y, char *hz);
			void DrawBMP(byte x0, byte y0, byte x1, byte y1, byte BMP[]);
        protected:
			void SetPos(byte x, byte y);            
			void _wrcmd(byte cmd);//Ð´ÃüÁî
			void _wrdata(byte da);//Ð´Êý¾Ý			       
		private:
			uint oled_pow(byte m, byte n);		
			uint16_t SearchhzIndex(byte lo, byte hi); //²éÕÒºº×Ö±àÂëÎ»ÖÃ
			void ShowCHinese11(byte x, byte y, byte no);     
			void WRByte(byte dat, byte cmd);
		protected:
            //SPI
			Port _cs;
            Port _res;
            Port _dc;
            Port _sclk;
            Port _sdin;
			//8080
			Port _wr;
            Port _rd;            
    };
#endif
