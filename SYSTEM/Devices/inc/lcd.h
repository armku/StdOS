/*
液晶屏 128×64（长×宽）
左上角坐标为（0，0），右下角坐标为(127,63)
 */
#pragma once

#include "Type.h"
#include "OutputPort.h"

class CLcd
{
    public:
        CLcd(Pin pinrs, Pin pinsclk, Pin pinsid, Pin pinres, Pin pincs);
		CLcd();
    public:
        //直接操作屏幕
        void Init(); //初始化
        void Cls(ushort x0 = 0, ushort y0 = 0, ushort width = 128, ushort height = 64); //清理指定位置
        void Test(byte data1, byte data2);
        void Point(ushort x0, ushort y0, ushort color = 1,ushort reverse=0); //画点,为所有的基础操作
        void Line(ushort x0, ushort y0, ushort x1, ushort y1, ushort color = 1);
        void Rect(ushort x0, ushort y0, ushort width, ushort height, ushort color = 1); //矩形
		void RectSolid(ushort x0, ushort y0, ushort width, ushort height, ushort color = 1); //实心矩形
        void Circle(ushort x0, ushort y0, ushort r, ushort color = 1);//画圆
		void Display4x8(ushort x, ushort y, byte *dp,ushort color=1,ushort reverse=0);
		void Display5x8(ushort x, ushort y, byte *dp,ushort color=1,ushort reverse=0);
		void Display8x16(ushort x, ushort y, byte *dp,ushort color=1,ushort reverse=0);
		void Display12x12(ushort x, ushort y, byte *dp,ushort color=1,ushort reverse=0);
		void Display16x16(ushort x, ushort y, byte *dp,ushort color=1,ushort reverse=0);
		void Display16x32(ushort x, ushort y, byte *dp,ushort color=1,ushort reverse=0);
		void Display32x32(ushort x, ushort y, byte *dp,ushort color=1,ushort reverse=0);
        void Flush(); //显示缓冲区刷新到屏        
    protected:
        byte Interface_Table[8][128]; //显示缓冲区
        void writeData(byte data1);
        void writeCMD(byte data1);
        void SetAddress(byte page, byte column); //写入地址 页 列        
    private:
        OutputPort pPinRS;
        OutputPort pPinsclk;
        OutputPort pPinsid;
        OutputPort pPinres;
        OutputPort pPincs;
};
