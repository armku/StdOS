#ifndef __OLED_H
    #define __OLED_H			  	 
    #include "sys.h"
    #include "stdlib.h"	    	
    //OLED模式设置
    //0:4线串行模式
    //1:并行8080模式
    #define OLED_MODE 0
    #define SIZE 16
    #define XLevelL		0x00
    #define XLevelH		0x10
    #define Max_Column	128
    #define Max_Row		64
    #define Brightness	0xFF 
    #define X_WIDTH 	128
    #define Y_WIDTH 	64	    						  
    //-----------------OLED端口定义----------------  					   
    #define OLED_CS_Clr()  GPIO_ResetBits(GPIOD,GPIO_Pin_3)//CS
    #define OLED_CS_Set()  GPIO_SetBits(GPIOD,GPIO_Pin_3)

    #define OLED_RST_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_4)//RES
    #define OLED_RST_Set() GPIO_SetBits(GPIOD,GPIO_Pin_4)

    #define OLED_DC_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_5)//DC
    #define OLED_DC_Set() GPIO_SetBits(GPIOD,GPIO_Pin_5)

    #define OLED_WR_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_14)
    #define OLED_WR_Set() GPIO_SetBits(GPIOG,GPIO_Pin_14)

    #define OLED_RD_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_13)
    #define OLED_RD_Set() GPIO_SetBits(GPIOG,GPIO_Pin_13)



    //PC0~7,作为数据线
    #define DATAOUT(x) GPIO_Write(GPIOC,x);//输出  
    //使用4线串行接口时使用 

    #define OLED_SCLK_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_6)//CLK
    #define OLED_SCLK_Set() GPIO_SetBits(GPIOD,GPIO_Pin_6)

    #define OLED_SDIN_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_7)//DIN
    #define OLED_SDIN_Set() GPIO_SetBits(GPIOD,GPIO_Pin_7)


    #define OLED_CMD  0	//写命令
    #define OLED_DATA 1	//写数据


    //OLED控制用函数
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
#endif
