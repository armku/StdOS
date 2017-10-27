/*
ssd1106代码兼容1309，可以直接驱动ssd1309;1309代码不兼容1106
*/
#include "SSD1106.h"

//初始化SSD1306					    
void SSD1106::Init()
{
    this->_res = 1;
    Sys.Sleep(100);
    this->_res = 0;
    Sys.Sleep(100);
    this->_res = 1;
	
	this->_wrcmd(0xAE);//--turn off oled panel
	this->_wrcmd(0x02);//---set low column address
	this->_wrcmd(0x10);//---set high column address
	this->_wrcmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	this->_wrcmd(0x81);//--set contrast control register
	this->_wrcmd(0xCF); // Set SEG Output Current Brightness
	this->_wrcmd(0xA1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	this->_wrcmd(0xC8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	this->_wrcmd(0xA6);//--set normal display
	this->_wrcmd(0xA8);//--set multiplex ratio(1 to 64)
	this->_wrcmd(0x3f);//--1/64 duty
	this->_wrcmd(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	this->_wrcmd(0x00);//-not offset
	this->_wrcmd(0xd5);//--set display clock divide ratio/oscillator frequency
	this->_wrcmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	this->_wrcmd(0xD9);//--set pre-charge period
	this->_wrcmd(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	this->_wrcmd(0xDA);//--set com pins hardware configuration
	this->_wrcmd(0x12);
	this->_wrcmd(0xDB);//--set vcomh
	this->_wrcmd(0x40);//Set VCOM Deselect Level
	this->_wrcmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	this->_wrcmd(0x02);//
	this->_wrcmd(0x8D);//--set Charge Pump enable/disable
	this->_wrcmd(0x14);//--set(0x10) disable
	this->_wrcmd(0xA4);// Disable Entire Display On (0xa4/0xa5)
	this->_wrcmd(0xA6);// Disable Inverse Display On (0xa6/a7) 
	this->_wrcmd(0xAF);//--turn on oled panel
	this->_wrcmd(0xAF); /*display ON*/ 
	
    this->Clear();
    this->SetPos(0, 0);
}
