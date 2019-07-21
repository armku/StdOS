#ifndef _GPS_S1216_H
#define _GPS_S1216_H

#include "Buffer.h"
//GPS NMEA-0183协议重要参数结构体定义 
//卫星信息
__packed typedef struct
{
	byte num;		//卫星编号
	byte eledeg;	//卫星仰角
	ushort azideg;	//卫星方位角
	byte sn;		//信噪比		   
}nmea_slmsg;
//北斗 NMEA-0183协议重要参数结构体定义 
//卫星信息
__packed typedef struct
{
	byte beidou_num;		//卫星编号
	byte beidou_eledeg;	//卫星仰角
	ushort beidou_azideg;	//卫星方位角
	byte beidou_sn;		//信噪比		   
}beidou_nmea_slmsg;

//UTC时间信息
__packed typedef struct
{
	ushort year;	//年份
	byte month;	//月份
	byte date;	//日期
	byte hour; 	//小时
	byte min; 	//分钟
	byte sec; 	//秒钟
}nmea_utc_time;
//NMEA 0183 协议解析后数据存放结构体
__packed typedef struct
{
	byte svnum;					//可见GPS卫星数
	byte beidou_svnum;					//可见GPS卫星数
	nmea_slmsg slmsg[12];		//最多12颗GPS卫星
	beidou_nmea_slmsg beidou_slmsg[12];		//暂且算最多12颗北斗卫星
	nmea_utc_time utc;			//UTC时间
	uint latitude;				//纬度 分扩大100000倍,实际要除以100000
	byte nshemi;					//北纬/南纬,N:北纬;S:南纬				  
	uint longitude;			    //经度 分扩大100000倍,实际要除以100000
	byte ewhemi;					//东经/西经,E:东经;W:西经
	byte gpssta;					//GPS状态:0,未定位;1,非差分定位;2,差分定位;6,正在估算.				  
	byte posslnum;				//用于定位的GPS卫星数,0~12.
	byte possl[12];				//用于定位的卫星编号
	byte fixmode;					//定位类型:1,没有定位;2,2D定位;3,3D定位
	ushort pdop;					//位置精度因子 0~500,对应实际值0~50.0
	ushort hdop;					//水平精度因子 0~500,对应实际值0~50.0
	ushort vdop;					//垂直精度因子 0~500,对应实际值0~50.0 

	int altitude;			 	//海拔高度,放大了10倍,实际除以10.单位:0.1m	 
	ushort speed;					//地面速率,放大了1000倍,实际除以10.单位:0.001公里/小时	 
}nmea_msg;
////////////////////////////////////////////////////////////////////////////////////////////////////
//SkyTra S1216F8 配置波特率结构体
__packed typedef struct
{
	ushort sos;            //启动序列，固定为0XA0A1
	ushort PL;             //有效数据长度0X0004； 
	byte id;             //ID，固定为0X05
	byte com_port;       //COM口，固定为0X00，即COM1   
	byte Baud_id;       //波特率（0~8,4800,9600,19200,38400,57600,115200,230400,460800,921600）
	byte Attributes;     //配置数据保存位置 ,0保存到SRAM，1保存到SRAM&FLASH，2临时保存
	byte CS;             //校验值
	ushort end;            //结束符:0X0D0A  
}SkyTra_baudrate;

//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 配置位置更新率结构体
__packed typedef struct
{
	ushort sos;            //启动序列，固定为0XA0A1
	ushort PL;             //有效数据长度0X0003； 
	byte id;             //ID，固定为0X0E
	byte rate;           //取值范围:1, 2, 4, 5, 8, 10, 20, 25, 40, 50
	byte Attributes;     //配置数据保存位置 ,0保存到SRAM，1保存到SRAM&FLASH，2临时保存
	byte CS;             //校验值
	ushort end;            //结束符:0X0D0A  
}SkyTra_PosRate;
//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 配置输出脉冲(PPS)宽度结构体
__packed typedef struct
{
	ushort sos;            //启动序列，固定为0XA0A1
	ushort PL;             //有效数据长度0X0007； 
	byte id;             //ID，固定为0X65
	byte Sub_ID;         //0X01
	uint width;        //1~100000(us)
	byte Attributes;     //配置数据保存位置 ,0保存到SRAM，1保存到SRAM&FLASH，2临时保存
	byte CS;             //校验值
	ushort end;            //结束符:0X0D0A 
}SkyTra_pps_width;

class GPS_S1216
{
public:
	GPS_S1216();
	void Analysis(byte *buf);
	void Show();
	nmea_msg gpsx;	//GPS信息
private:
	byte NMEA_Comma_Pos(byte *buf, byte cx);
	uint NMEA_Pow(byte m, byte n);
	int NMEA_Str2num(byte *buf, byte*dx);
	byte SkyTra_Cfg_Ack_Check();
	void NMEA_GPGSV_Analysis(byte *buf);
	void NMEA_BDGSV_Analysis(byte *buf);
	void NMEA_GNGGA_Analysis(byte *buf);
	void NMEA_GNGSA_Analysis(byte *buf);
	void NMEA_GNRMC_Analysis(byte *buf);
	void NMEA_GNVTG_Analysis(byte *buf);
public:
	byte SkyTra_Cfg_Prt(uint baud_id, byte* buf, int& len);
	byte SkyTra_Cfg_Tp(uint width, byte* buf, int& len);
	byte SkyTra_Cfg_Rate(byte Frep, byte* buf, int& len);
};

#endif // !_GPS_H
