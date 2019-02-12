/*
	夏普1051，串口型PM2.5传感器驱动。
	通信波特率 2400 。采集间隔:大于10ms
	如果输出数值为0，查看下设备电源，测试采用面包板时一直输出为0，当用手将电源线直连时问题解决
	updata函数需要改进
	未测试
*/

#ifndef _SHARP_PM2_5_H_
#define _SHARP_PM2_5_H_

#include "stm32f10x.h"
#include "../../../../Bsp/Porting.h"

class HardwareSerial0_T;

class Sharp_PM_2_5
{
private:
	HardwareSerial0_T &mCom;
	float UD;//粉尘浓度 单位 ug/m^3
	bool Check(u8 *Data, int Lenth, u8 CheckSum);

public:
	Sharp_PM_2_5(HardwareSerial0_T &com);
	bool Update();
	float GetConcentration();
};

#define  COEFFICIENT_A 800  //电压与浓度的比例系数

#endif
