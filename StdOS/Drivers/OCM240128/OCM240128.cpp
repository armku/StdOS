#include "OCM240128.h"
#include "Sys.h"
OCM240128::OCM240128()
{
	

}

void OCM240128::SetPin(Pin ce, Pin wr, Pin rd, Pin fs, Pin led,Pin cd)
{
	this->pince.SetPin(ce);
	this->pinwr.SetPin(wr);
	this->pinrd.SetPin(rd);
	/*this->pinfs.SetPin(fs);*/
	this->pinled.SetPin(led);
	this->pincd.SetPin(cd);
		
	this->pince.pinMode(GPIO_Out_PP);
	this->pinwr.pinMode(GPIO_Out_PP);
	this->pinrd.pinMode(GPIO_Out_PP);
	/*this->pinfs.pinMode(GPIO_Out_PP);*/
	this->pinled.pinMode(GPIO_Out_PP);
	this->pincd.pinMode(GPIO_Out_PP);
}
void OCM240128::SetDataPin(Pin d0, Pin d1, Pin d2, Pin d3, Pin d4, Pin d5, Pin d6, Pin d7)
{
	this->data0.SetPin(d0);
	this->data1.SetPin(d1);
	this->data2.SetPin(d2);
	this->data3.SetPin(d3);
	this->data4.SetPin(d4);
	this->data5.SetPin(d5);
	this->data6.SetPin(d6);
	this->data7.SetPin(d7);

	this->data0.pinMode(GPIO_Out_PP);
	this->data1.pinMode(GPIO_Out_PP);
	this->data2.pinMode(GPIO_Out_PP);
	this->data3.pinMode(GPIO_Out_PP);
	this->data4.pinMode(GPIO_Out_PP);
	this->data5.pinMode(GPIO_Out_PP);
	this->data6.pinMode(GPIO_Out_PP);
	this->data7.pinMode(GPIO_Out_PP);
}
void OCM240128::writedata(uint8_t da)
{
	this->data0 = da & (1 << 0);
	this->data1 = da & (1 << 1);
	this->data2 = da & (1 << 2);
	this->data3 = da & (1 << 3);
	this->data4 = da & (1 << 4);
	this->data5 = da & (1 << 5);
	this->data6 = da & (1 << 6);
	this->data7 = da & (1 << 7);
}
/************************************************************************************************
@f_name: void LCD12864_Init(void)
@brief:	 液晶初始化
@param:	 None
@return: None
************************************************************************************************/
void OCM240128::Init()
{
	//this->pinfs = 0;
	this->pince = 0;
	this->pinwr = 1;
	this->pinrd = 1;

	this->wcmd2(0x00, 0x00, 0x40);
	this->wcmd2(0x20, 0x00, 0x41);
	this->wcmd2(0x00, 0x08, 0x42);
	this->wcmd2(0x20, 0x00, 0x43);
	this->wcmd(0xa1); //光标形状
	this->wcmd(0x80);
	this->wcmd(0x98);
	this->pince = 1;
	/*this->pinled = 1;*/
}

void OCM240128::Clr()
{
	uint16_t i;
	this->wcmd2(0x00, 0x00, 0x24);
	this->wcmd(0xb0);
	for (i = 0; i < 8192; i++)
	{
		this->wdata(0);
	}
	this->wcmd(0xb2);
	this->wcmd(0x98); //禁止光标闪动
}

void OCM240128::DispDot8x16(uint8_t x, uint8_t y, uint8_t *text, uint8_t mode)
{
	uint16_t add = y * 0x20 + x + 0x800;
	uint8_t i = add;
	uint8_t j = add >> 8;
	for (int k = 0; k < 16; k++)
	{
		this->wcmd2(i, j, 0x24);
		if (mode)
			this->wcmd(text[k], 0xc0);
		else
			this->wcmd(~text[k], 0xc0);
		add = add + 0x20;
		i = add;
		j = add >> 8;
	}
}

void OCM240128::DispDot16x16(uint8_t x, uint8_t y, uint8_t *text, uint8_t mode)
{
	uint16_t add = y * 0x20 + x + 0x800;
	uint8_t i = add;
	uint8_t j = add >> 8;
	for (int k = 0; k < 32; k = k + 2)
		{
			this->wcmd2(i, j, 0x24);
			if (mode)
			{
				this->wcmd(text[k], 0xc0);
				this->wcmd(text[k + 1], 0xc0);
			}
			else
			{
				this->wcmd(~text[k], 0xc0);
				this->wcmd(~text[k + 1], 0xc0);
			}			
		}
}

//画横线
void OCM240128::Draw_hline(uint8_t x, uint8_t y, uint16_t count)
{
	uint16_t add;
	uint8_t i, j;
	add = y * 0x20 + x + 0x800;
	i = add;
	j = add >> 8;
	this->wcmd2(i, j, 0x24);
	for (i = 0; i < count; i++)
	{
		this->wcmd(0xff, 0xc0);
	}
}
void OCM240128::Test()
{
	this->data0 = 0;
	this->data1 = 0;
	this->data2 = 0;
	this->data3 = 0;
	this->data4 = 0;
	this->data5 = 0;
	this->data6 = 0;
	this->data7 = 0;
	
	this->pincd = 0;
	this->pinrd = 0;
	this->pince = 0;
	this->pinwr = 0;

}

//画竖线
void OCM240128::Draw_vline(uint8_t x, uint8_t y, uint16_t count)
{
	uint16_t add;
	uint8_t i, j, k;
	add = y * 0x20 + x + 0x800;
	i = add;
	j = add >> 8;
	for (k = 0; k < count; k++)
	{
		this->wcmd2(i, j, 0x24);
		this->wcmd(0x01, 0xc0);
		add = add + 0x20;
		i = add;
		j = add >> 8;
	}
}
/************************************************************************************************
@f_name: uint8_t LCD12864_busy(void)
@brief:	 检测忙状态
@param:	 None
@return: None
************************************************************************************************/
void OCM240128::busycheck(uint8_t autowr)
{
#if 0
	//	LCD_DataPort_In();
	//	this->pincd = 1;
	//	this->pinwr = 1;
	//	this->pinrd = 0;
	//	if (autowr)
	//	{
	//		while (this->pinsta3==0)
	//		{
	//			
	//		}
	//	}
	//	else
	//	{
	//		while((this->pinsta0==0)||(this->pinsta1==0))
	//		{
	//			delayMicroseconds(10);
	//		};
	//	}
	//	this->pinrd = 1;
	//	LCD_DataPort_Out();
#else
	delayMicroseconds(10);
#endif
}
/************************************************************************************************
@f_name: void LCD12684_Wcmd_L(uint8_t dat)
@brief:	 写命令
@param:	 uint8_t dat  输入指令
@return: None
************************************************************************************************/
void OCM240128::wcmd(uint8_t cmd)
{
	this->busycheck(0);
	this->pincd = 1;
	this->pinrd = 1;
	this->writedata(cmd);
	this->pinwr = 0;
	this->pinwr = 1;
}

/************************************************************************************************
@f_name: void LCD12684_Wcmd_R(uint8_t dat)
@brief:	 写数据
@param:	 uint8_t dat  输入指令
@return: None
************************************************************************************************/
void OCM240128::wdata(uint8_t dat)
{
	this->busycheck(0);
	this->pincd = 0;
	this->pinrd = 1;
	this->writedata(dat);
	this->pinwr = 0;
	this->pinwr = 1;
}
/************************************************************************************************
@f_name: void LCD12684_Wdat_L(uint8_t dat)
@brief:	 写一个数据和一个命令
@param:	 uint8_t dat 输入数据
@return: None
************************************************************************************************/
void OCM240128::wcmd(uint8_t dat, uint8_t cmd)
{
	this->wdata(dat);
	this->wcmd(cmd);
}

/************************************************************************************************
@f_name: void LCD12684_Wdat_R(uint8_t dat)
@brief:	 写两个数据一个命令
@param:	 uint8_t dat 输入数据
@return: None
************************************************************************************************/
void OCM240128::wcmd2(uint8_t dat1, uint8_t dat2, uint8_t cmd)
{
	this->wdata(dat1);
	this->wdata(dat2);
	this->wcmd(cmd);
}
