/*
Һ���� 128��64��������
���Ͻ�����Ϊ��0��0�������½�����Ϊ(127,63)
 */
#pragma once

#include "Type.h"
#include "OutputPort.h"

class CLcd
{
    public:
        CLcd(Pin pinrs, Pin pinsclk, Pin pinsid, Pin pinres, Pin pincs);
		CLcd();    
        //ֱ�Ӳ�����Ļ
        void Init(); //��ʼ��
        void Cls(ushort x0 = 0, ushort y0 = 0, ushort width = 128, ushort height = 64); //����ָ��λ��
        void Test(byte data1, byte data2);
        void Point(ushort x0, ushort y0, ushort color = 1); //����,Ϊ���еĻ�������
        void Line(ushort x0, ushort y0, ushort x1, ushort y1);
        void Rect(ushort x0, ushort y0, ushort width, ushort height); //����
		void RectSolid(ushort x0, ushort y0, ushort width, ushort height); //ʵ�ľ���
        void Circle(ushort x0, ushort y0, ushort r);//��Բ
		void Display4x8(ushort x, ushort y, byte *dp);
		void Display5x8(ushort x, ushort y, byte *dp);
		void Display8x16(ushort x, ushort y, byte *dp);
		void Display12x12(ushort x, ushort y, byte *dp);
		void Display16x16(ushort x, ushort y, byte *dp);
		void Display16x32(ushort x, ushort y, byte *dp);
		void Display32x32(ushort x, ushort y, byte *dp);
        void Flush(); //��ʾ������ˢ�µ���        
		struct
		{
			ushort x;
			ushort y;
			ushort width;
			ushort height;
			bool enable;
		}ShadowRect[3];//3����Ӱ����ʵ���ض�������
		void ShadowReset();//��Ӱ���ָ�λ
		void ShadowOn(ushort x,ushort y,ushort width,ushort height,byte id=0);//��Ӱ���ã�Ĭ������ͨ��0
    protected:
        byte Interface_Table[8][128]; //��ʾ������
		ushort readPoint(ushort x,ushort y);//��ȡ��ɫ
        void writeData(byte data1);
        void writeCMD(byte data1);
        void SetAddress(byte page, byte column); //д���ַ ҳ ��    
		bool inShadow(ushort x,ushort y);//�Ƿ�����Ӱ����
    private:
        OutputPort pPinRS;
        OutputPort pPinsclk;
        OutputPort pPinsid;
        OutputPort pPinres;
        OutputPort pPincs;
};
