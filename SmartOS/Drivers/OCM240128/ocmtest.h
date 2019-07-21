#ifndef _OCMTEST_H
#define _OCMTEST_H

class ocmtest
{
public:
	void SetPin(Pin ce = P0, Pin wr = P0, Pin rd = P0, Pin fs = P0, Pin led = P0, Pin cd = P0);
	void SetDataPin(Pin d0, Pin d1, Pin d2, Pin d3, Pin d4, Pin d5, Pin d6, Pin d7);
	void writedata(byte da);
	//	void wr_comm(byte comm);
	//	void wr_data(byte dat);
	//	void chk_busy(byte autowr);
	void delay(uint us);
	void lcdtest();
	void disp_eng(byte const* eng);
	void disp_chn(uint addr, byte xl, byte yl, byte row_xl, byte row_yl, byte const* chn);
	void disp_img(uint addr, byte xl, byte yl, byte const* img);
	void disp_dz(byte data1, byte data2);
	void clrram(void);
	void init_lcd(void);
	void chk_busy(byte autowr);    //��״̬
	void wr_data(byte dat);       //д����
	void wr_comm(byte comm);       //д����
	void wr_auto(byte dat);               //�Զ�д����
	void wr_xd(uint dat, byte comm);       //дһ��16�������ݺ�һ������
	void wr_td(byte datl, byte dath, byte comm);  //д�������ݺ�һ������
	void wr_od(byte dat, byte comm);       //дһ�����ݺ�һ������
	void delay1(uint ms);
public:
	Port wr;  //Data Write into T6963C,L��Ч
	Port rd;  //Data Read from T6963C,L��Ч
	Port ce;  //ʹ���źţ�L��Ч
	Port cd;  //��wr=L,cd=H:д����,cd=L:д����;��rd=L,cd=H:��״̬,cd=L:������
	Port rst;  //Lcm reset,����Ч
	Port fs;  //����ѡ��,H:6*8����;L:8*8����

	Port bf0;
	Port bf1;
	Port bf3;

	//byte data_ora;
	Port data0;
	Port data1;
	Port data2;
	Port data3;
	Port data4;
	Port data5;
	Port data6;
	Port data7;
};

#endif // !_OCMTEST_H
