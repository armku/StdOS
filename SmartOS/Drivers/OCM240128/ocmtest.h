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
	void chk_busy(byte autowr);    //测状态
	void wr_data(byte dat);       //写数据
	void wr_comm(byte comm);       //写命令
	void wr_auto(byte dat);               //自动写数据
	void wr_xd(uint dat, byte comm);       //写一个16进制数据和一个命令
	void wr_td(byte datl, byte dath, byte comm);  //写两个数据和一个命令
	void wr_od(byte dat, byte comm);       //写一个数据和一个命令
	void delay1(uint ms);
public:
	Port wr;  //Data Write into T6963C,L有效
	Port rd;  //Data Read from T6963C,L有效
	Port ce;  //使能信号，L有效
	Port cd;  //当wr=L,cd=H:写命令,cd=L:写数据;当rd=L,cd=H:读状态,cd=L:读数据
	Port rst;  //Lcm reset,低有效
	Port fs;  //字体选择,H:6*8点阵;L:8*8点阵

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
