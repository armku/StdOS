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
        //直接操作屏幕
        void Init(); //初始化
        void Cls(ushort x0 = 0, ushort y0 = 0, ushort width = 128, ushort height = 64); //清理指定位置
        void Test(byte data1, byte data2);
        void Point(ushort x0, ushort y0, ushort color = 1); //画点,为所有的基础操作
        void Line(ushort x0, ushort y0, ushort x1, ushort y1);
        void Rect(ushort x0, ushort y0, ushort width, ushort height); //矩形
		void RectSolid(ushort x0, ushort y0, ushort width, ushort height); //实心矩形
        void Circle(ushort x0, ushort y0, ushort r);//画圆
		void Display4x8(ushort x, ushort y, byte *dp);
		void Display5x8(ushort x, ushort y, byte *dp);
		void Display8x16(ushort x, ushort y, byte *dp);
		void Display12x12(ushort x, ushort y, byte *dp);
		void Display16x16(ushort x, ushort y, byte *dp);
		void Display16x32(ushort x, ushort y, byte *dp);
		void Display32x32(ushort x, ushort y, byte *dp);
        void Flush(); //显示缓冲区刷新到屏        
		struct
		{
			ushort x;
			ushort y;
			ushort width;
			ushort height;
			bool enable;
		}ShadowRect[3];//3组阴影区域，实现特定动作。
		void ShadowReset();//阴影部分复位
		void ShadowOn(ushort x,ushort y,ushort width,ushort height,byte id=0);//阴影启用，默认启用通道0
    protected:
        byte Interface_Table[8][128]; //显示缓冲区
		ushort readPoint(ushort x,ushort y);//读取颜色
        void writeData(byte data1);
        void writeCMD(byte data1);
        void SetAddress(byte page, byte column); //写入地址 页 列    
		bool inShadow(ushort x,ushort y);//是否在阴影区域
    private:
        OutputPort pPinRS;
        OutputPort pPinsclk;
        OutputPort pPinsid;
        OutputPort pPinres;
        OutputPort pPincs;
};
