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
	void chk_busy(uint8_t autowr);    //��״̬
	void wr_data(uint8_t dat);       //д����
	void wr_comm(uint8_t comm);       //д����
	void wr_auto(uint8_t dat);               //�Զ�д����
	void wr_xd(uint32_t dat, uint8_t comm);       //дһ��16�������ݺ�һ������
	void wr_td(uint8_t datl, uint8_t dath, uint8_t comm);  //д�������ݺ�һ������
	void wr_od(uint8_t dat, uint8_t comm);       //дһ�����ݺ�һ������
	void delay1(uint32_t ms);
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

	uint8_t data_ora;
};

#endif // !_OCMTEST_H
