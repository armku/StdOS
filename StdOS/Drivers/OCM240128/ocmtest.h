#ifndef _OCMTEST_H
#define _OCMTEST_H

class ocmtest
{
public:
	//	void wr_comm(uint8_t comm);
	//	void wr_data(uint8_t dat);
	//	void chk_busy(uint8_t autowr);
	void delay(uint32_t us);
	void lcdtest();
	void disp_eng(uint8_t const* eng);
	void disp_chn(uint32_t addr, uint8_t xl, uint8_t yl, uint8_t row_xl, uint8_t row_yl, uint8_t const* chn);
	void disp_img(uint32_t addr, uint8_t xl, uint8_t yl, uint8_t const* img);
	void disp_dz(uint8_t data1, uint8_t data2);
	void clrram(void);
	void init_lcd(void);
	void chk_busy(uint8_t autowr);    //测状态
	void wr_data(uint8_t dat);       //写数据
	void wr_comm(uint8_t comm);       //写命令
	void wr_auto(uint8_t dat);               //自动写数据
	void wr_xd(uint32_t dat, uint8_t comm);       //写一个16进制数据和一个命令
	void wr_td(uint8_t datl, uint8_t dath, uint8_t comm);  //写两个数据和一个命令
	void wr_od(uint8_t dat, uint8_t comm);       //写一个数据和一个命令
	void delay1(uint32_t ms);
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

	uint8_t data_ora;
};

#endif // !_OCMTEST_H
