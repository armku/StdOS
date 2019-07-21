#ifndef _GPS_S1216_H
#define _GPS_S1216_H

#include "Buffer.h"
//GPS NMEA-0183协议重要参数结构体定义 
//卫星信息
__packed typedef struct
{
	uint8_t num;		//卫星编号
	uint8_t eledeg;	//卫星仰角
	uint16_t azideg;	//卫星方位角
	uint8_t sn;		//信噪比		   
}nmea_slmsg;
//北斗 NMEA-0183协议重要参数结构体定义 
//卫星信息
__packed typedef struct
{
	uint8_t beidou_num;		//卫星编号
	uint8_t beidou_eledeg;	//卫星仰角
	uint16_t beidou_azideg;	//卫星方位角
	uint8_t beidou_sn;		//信噪比		   
}beidou_nmea_slmsg;

//UTC时间信息
__packed typedef struct
{
	uint16_t year;	//年份
	uint8_t month;	//月份
	uint8_t date;	//日期
	uint8_t hour; 	//小时
	uint8_t min; 	//分钟
	uint8_t sec; 	//秒钟
}nmea_utc_time;
//NMEA 0183 协议解析后数据存放结构体
__packed typedef struct
{
	uint8_t svnum;					//可见GPS卫星数
	uint8_t beidou_svnum;					//可见GPS卫星数
	nmea_slmsg slmsg[12];		//最多12颗GPS卫星
	beidou_nmea_slmsg beidou_slmsg[12];		//暂且算最多12颗北斗卫星
	nmea_utc_time utc;			//UTC时间
	uint latitude;				//纬度 分扩大100000倍,实际要除以100000
	uint8_t nshemi;					//北纬/南纬,N:北纬;S:南纬				  
	uint longitude;			    //经度 分扩大100000倍,实际要除以100000
	uint8_t ewhemi;					//东经/西经,E:东经;W:西经
	uint8_t gpssta;					//GPS状态:0,未定位;1,非差分定位;2,差分定位;6,正在估算.				  
	uint8_t posslnum;				//用于定位的GPS卫星数,0~12.
	uint8_t possl[12];				//用于定位的卫星编号
	uint8_t fixmode;					//定位类型:1,没有定位;2,2D定位;3,3D定位
	uint16_t pdop;					//位置精度因子 0~500,对应实际值0~50.0
	uint16_t hdop;					//水平精度因子 0~500,对应实际值0~50.0
	uint16_t vdop;					//垂直精度因子 0~500,对应实际值0~50.0 

	int altitude;			 	//海拔高度,放大了10倍,实际除以10.单位:0.1m	 
	uint16_t speed;					//地面速率,放大了1000倍,实际除以10.单位:0.001公里/小时	 
}nmea_msg;
////////////////////////////////////////////////////////////////////////////////////////////////////
//SkyTra S1216F8 配置波特率结构体
__packed typedef struct
{
	uint16_t sos;            //启动序列，固定为0XA0A1
	uint16_t PL;             //有效数据长度0X0004； 
	uint8_t id;             //ID，固定为0X05
	uint8_t com_port;       //COM口，固定为0X00，即COM1   
	uint8_t Baud_id;       //波特率（0~8,4800,9600,19200,38400,57600,115200,230400,460800,921600）
	uint8_t Attributes;     //配置数据保存位置 ,0保存到SRAM，1保存到SRAM&FLASH，2临时保存
	uint8_t CS;             //校验值
	uint16_t end;            //结束符:0X0D0A  
}SkyTra_baudrate;

//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 配置位置更新率结构体
__packed typedef struct
{
	uint16_t sos;            //启动序列，固定为0XA0A1
	uint16_t PL;             //有效数据长度0X0003； 
	uint8_t id;             //ID，固定为0X0E
	uint8_t rate;           //取值范围:1, 2, 4, 5, 8, 10, 20, 25, 40, 50
	uint8_t Attributes;     //配置数据保存位置 ,0保存到SRAM，1保存到SRAM&FLASH，2临时保存
	uint8_t CS;             //校验值
	uint16_t end;            //结束符:0X0D0A  
}SkyTra_PosRate;
//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 配置输出脉冲(PPS)宽度结构体
__packed typedef struct
{
	uint16_t sos;            //启动序列，固定为0XA0A1
	uint16_t PL;             //有效数据长度0X0007； 
	uint8_t id;             //ID，固定为0X65
	uint8_t Sub_ID;         //0X01
	uint width;        //1~100000(us)
	uint8_t Attributes;     //配置数据保存位置 ,0保存到SRAM，1保存到SRAM&FLASH，2临时保存
	uint8_t CS;             //校验值
	uint16_t end;            //结束符:0X0D0A 
}SkyTra_pps_width;

class GPS_S1216
{
public:
	GPS_S1216();
	void Analysis(uint8_t *buf);
	void Show();
	nmea_msg gpsx;	//GPS信息
private:
	uint8_t NMEA_Comma_Pos(uint8_t *buf, uint8_t cx);
	uint NMEA_Pow(uint8_t m, uint8_t n);
	int NMEA_Str2num(uint8_t *buf, uint8_t*dx);
	uint8_t SkyTra_Cfg_Ack_Check();
	void NMEA_GPGSV_Analysis(uint8_t *buf);
	void NMEA_BDGSV_Analysis(uint8_t *buf);
	void NMEA_GNGGA_Analysis(uint8_t *buf);
	void NMEA_GNGSA_Analysis(uint8_t *buf);
	void NMEA_GNRMC_Analysis(uint8_t *buf);
	void NMEA_GNVTG_Analysis(uint8_t *buf);
public:
	uint8_t SkyTra_Cfg_Prt(uint baud_id, uint8_t* buf, int& len);
	uint8_t SkyTra_Cfg_Tp(uint width, uint8_t* buf, int& len);
	uint8_t SkyTra_Cfg_Rate(uint8_t Frep, uint8_t* buf, int& len);
};

#endif // !_GPS_H
