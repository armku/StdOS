#include <string.h>
#include <stdio.h>
#include "GPS.h"
#include "Sys.h"
//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 配置输出信息结构体
__packed typedef struct
{
	ushort sos;            //启动序列，固定为0XA0A1
	ushort PL;             //有效数据长度0X0009； 
	byte id;             //ID，固定为0X08
	byte GGA;            //1~255（s）,0:disable
	byte GSA;            //1~255（s）,0:disable
	byte GSV;            //1~255（s）,0:disable
	byte GLL;            //1~255（s）,0:disable
	byte RMC;            //1~255（s）,0:disable
	byte VTG;            //1~255（s）,0:disable
	byte ZDA;            //1~255（s）,0:disable
	byte Attributes;     //配置数据保存位置 ,0保存到SRAM，1保存到SRAM&FLASH，2临时保存
	byte CS;             //校验值
	ushort end;            //结束符:0X0D0A  
}SkyTra_outmsg;
//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 ACK结构体
__packed typedef struct
{
	ushort sos;            //启动序列，固定为0XA0A1
	ushort PL;             //有效数据长度0X0002； 
	byte id;             //ID，固定为0X83
	byte ACK_ID;         //ACK ID may further consist of message ID and message sub-ID which will become 3 bytes of ACK message
	byte CS;             //校验值
	ushort end;            //结束符 
}SkyTra_ACK;
//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 NACK结构体
__packed typedef struct
{
	ushort sos;            //启动序列，固定为0XA0A1
	ushort PL;             //有效数据长度0X0002； 
	byte id;             //ID，固定为0X84
	byte NACK_ID;         //ACK ID may further consist of message ID and message sub-ID which will become 3 bytes of ACK message
	byte CS;             //校验值
	ushort end;            //结束符 
}SkyTra_NACK;
GPS_S1216::GPS_S1216()
{
	this->gpsx.ewhemi = 'E';
	this->gpsx.nshemi = 'N';
}

//从buf里面得到第cx个逗号所在的位置
//返回值:0~0XFE,代表逗号所在位置的偏移.
//       0XFF,代表不存在第cx个逗号							  
byte GPS_S1216::NMEA_Comma_Pos(byte *buf, byte cx)
{
	byte *p = buf;
	while (cx)
	{
		if (*buf == '*' || *buf<' ' || *buf>'z')
			return 0XFF;//遇到'*'或者非法字符,则不存在第cx个逗号
		if (*buf == ',')cx--;
		buf++;
	}
	return buf - p;
}
//m^n函数
//返回值:m^n次方.
uint GPS_S1216::NMEA_Pow(byte m, byte n)
{
	uint result = 1;
	while (n--)
		result *= m;
	return result;
}
//str转换为数字,以','或者'*'结束
//buf:数字存储区
//dx:小数点位数,返回给调用函数
//返回值:转换后的数值
int GPS_S1216::NMEA_Str2num(byte *buf, byte*dx)
{
	byte *p = buf;
	uint ires = 0, fres = 0;
	byte ilen = 0, flen = 0, i;
	byte mask = 0;
	int res;
	while (1) //得到整数和小数的长度
	{
		if (*p == '-')
		{
			mask |= 0X02;
			p++;
		}//是负数
		if (*p == ',' || (*p == '*'))
			break;//遇到结束了
		if (*p == '.')
		{
			mask |= 0X01;
			p++;
		}//遇到小数点了
		else if (*p > '9' || (*p < '0'))	//有非法字符
		{
			ilen = 0;
			flen = 0;
			break;
		}
		if (mask & 0X01)
			flen++;
		else ilen++;
		p++;
	}
	if (mask & 0X02)buf++;	//去掉负号
	for (i = 0; i < ilen; i++)	//得到整数部分数据
	{
		ires += NMEA_Pow(10, ilen - 1 - i)*(buf[i] - '0');
	}
	if (flen > 5)
		flen = 5;	//最多取5位小数
	*dx = flen;	 		//小数点位数
	for (i = 0; i < flen; i++)	//得到小数部分数据
	{
		fres += NMEA_Pow(10, flen - 1 - i)*(buf[ilen + 1 + i] - '0');
	}
	res = ires * NMEA_Pow(10, flen) + fres;
	if (mask & 0X02)
		res = -res;
	return res;
}
//分析GPGSV信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void GPS_S1216::NMEA_GPGSV_Analysis(byte *buf)
{
	byte *p, *p1, dx;
	byte len, i, j, slx = 0;
	byte posx;
	p = buf;
	p1 = (byte*)strstr((const char *)p, "$GPGSV");
	len = p1[7] - '0';								//得到GPGSV的条数
	posx = NMEA_Comma_Pos(p1, 3); 					//得到可见卫星总数
	if (posx != 0XFF) this->gpsx.svnum = NMEA_Str2num(p1 + posx, &dx);
	for (i = 0; i < len; i++)
	{
		p1 = (byte*)strstr((const char *)p, "$GPGSV");
		for (j = 0; j < 4; j++)
		{
			posx = NMEA_Comma_Pos(p1, 4 + j * 4);
			if (posx != 0XFF)
				this->gpsx.slmsg[slx].num = NMEA_Str2num(p1 + posx, &dx);	//得到卫星编号
			else
				break;
			posx = NMEA_Comma_Pos(p1, 5 + j * 4);
			if (posx != 0XFF)
				this->gpsx.slmsg[slx].eledeg = NMEA_Str2num(p1 + posx, &dx);//得到卫星仰角 
			else
				break;
			posx = NMEA_Comma_Pos(p1, 6 + j * 4);
			if (posx != 0XFF)
				this->gpsx.slmsg[slx].azideg = NMEA_Str2num(p1 + posx, &dx);//得到卫星方位角
			else
				break;
			posx = NMEA_Comma_Pos(p1, 7 + j * 4);
			if (posx != 0XFF)
				this->gpsx.slmsg[slx].sn = NMEA_Str2num(p1 + posx, &dx);	//得到卫星信噪比
			else
				break;
			slx++;
		}
		p = p1 + 1;//切换到下一个GPGSV信息
	}
}
//分析BDGSV信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void GPS_S1216::NMEA_BDGSV_Analysis(byte *buf)
{
	byte *p, *p1, dx;
	byte len, i, j, slx = 0;
	byte posx;
	p = buf;
	p1 = (byte*)strstr((const char *)p, "$BDGSV");
	len = p1[7] - '0';								//得到BDGSV的条数
	posx = NMEA_Comma_Pos(p1, 3); 					//得到可见北斗卫星总数
	if (posx != 0XFF)
		this->gpsx.beidou_svnum = NMEA_Str2num(p1 + posx, &dx);
	for (i = 0; i < len; i++)
	{
		p1 = (byte*)strstr((const char *)p, "$BDGSV");
		for (j = 0; j < 4; j++)
		{
			posx = NMEA_Comma_Pos(p1, 4 + j * 4);
			if (posx != 0XFF)
				this->gpsx.beidou_slmsg[slx].beidou_num = NMEA_Str2num(p1 + posx, &dx);	//得到卫星编号
			else
				break;
			posx = NMEA_Comma_Pos(p1, 5 + j * 4);
			if (posx != 0XFF)
				this->gpsx.beidou_slmsg[slx].beidou_eledeg = NMEA_Str2num(p1 + posx, &dx);//得到卫星仰角 
			else
				break;
			posx = NMEA_Comma_Pos(p1, 6 + j * 4);
			if (posx != 0XFF)
				this->gpsx.beidou_slmsg[slx].beidou_azideg = NMEA_Str2num(p1 + posx, &dx);//得到卫星方位角
			else
				break;
			posx = NMEA_Comma_Pos(p1, 7 + j * 4);
			if (posx != 0XFF)
				this->gpsx.beidou_slmsg[slx].beidou_sn = NMEA_Str2num(p1 + posx, &dx);	//得到卫星信噪比
			else
				break;
			slx++;
		}
		p = p1 + 1;//切换到下一个BDGSV信息
	}
}
//分析GNGGA信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void GPS_S1216::NMEA_GNGGA_Analysis(byte *buf)
{
	byte *p1, dx;
	byte posx;
	p1 = (byte*)strstr((const char *)buf, "$GNGGA");
	posx = NMEA_Comma_Pos(p1, 6);								//得到GPS状态
	if (posx != 0XFF)
		this->gpsx.gpssta = NMEA_Str2num(p1 + posx, &dx);
	posx = NMEA_Comma_Pos(p1, 7);								//得到用于定位的卫星数
	if (posx != 0XFF)
		this->gpsx.posslnum = NMEA_Str2num(p1 + posx, &dx);
	posx = NMEA_Comma_Pos(p1, 9);								//得到海拔高度
	if (posx != 0XFF)
		this->gpsx.altitude = NMEA_Str2num(p1 + posx, &dx);
}
//分析GNGSA信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void GPS_S1216::NMEA_GNGSA_Analysis(byte *buf)
{
	byte *p1, dx;
	byte posx;
	byte i;
	p1 = (byte*)strstr((const char *)buf, "$GNGSA");
	posx = NMEA_Comma_Pos(p1, 2);								//得到定位类型
	if (posx != 0XFF)
		this->gpsx.fixmode = NMEA_Str2num(p1 + posx, &dx);
	for (i = 0; i < 12; i++)										//得到定位卫星编号
	{
		posx = NMEA_Comma_Pos(p1, 3 + i);
		if (posx != 0XFF)
			this->gpsx.possl[i] = NMEA_Str2num(p1 + posx, &dx);
		else
			break;
	}
	posx = NMEA_Comma_Pos(p1, 15);								//得到PDOP位置精度因子
	if (posx != 0XFF)
		this->gpsx.pdop = NMEA_Str2num(p1 + posx, &dx);
	posx = NMEA_Comma_Pos(p1, 16);								//得到HDOP位置精度因子
	if (posx != 0XFF)
		this->gpsx.hdop = NMEA_Str2num(p1 + posx, &dx);
	posx = NMEA_Comma_Pos(p1, 17);								//得到VDOP位置精度因子
	if (posx != 0XFF)
		this->gpsx.vdop = NMEA_Str2num(p1 + posx, &dx);
}
//分析GNRMC信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void GPS_S1216::NMEA_GNRMC_Analysis(byte *buf)
{
	byte *p1, dx;
	byte posx;
	uint temp;
	float rs;
	p1 = (byte*)strstr((const char *)buf, "$GNRMC");//"$GNRMC",经常有&和GNRMC分开的情况,故只判断GPRMC.
	posx = NMEA_Comma_Pos(p1, 1);								//得到UTC时间
	if (posx != 0XFF)
	{
		temp = NMEA_Str2num(p1 + posx, &dx) / NMEA_Pow(10, dx);	 	//得到UTC时间,去掉ms
		this->gpsx.utc.hour = temp / 10000;
		this->gpsx.utc.min = (temp / 100) % 100;
		this->gpsx.utc.sec = temp % 100;
	}
	posx = NMEA_Comma_Pos(p1, 3);								//得到纬度
	if (posx != 0XFF)
	{
		temp = NMEA_Str2num(p1 + posx, &dx);
		this->gpsx.latitude = temp / NMEA_Pow(10, dx + 2);	//得到°
		rs = temp % NMEA_Pow(10, dx + 2);				//得到'		 
		this->gpsx.latitude = this->gpsx.latitude*NMEA_Pow(10, 5) + (rs*NMEA_Pow(10, 5 - dx)) / 60;//转换为° 
	}
	posx = NMEA_Comma_Pos(p1, 4);								//南纬还是北纬 
	if (posx != 0XFF)this->gpsx.nshemi = *(p1 + posx);
	posx = NMEA_Comma_Pos(p1, 5);								//得到经度
	if (posx != 0XFF)
	{
		temp = NMEA_Str2num(p1 + posx, &dx);
		this->gpsx.longitude = temp / NMEA_Pow(10, dx + 2);	//得到°
		rs = temp % NMEA_Pow(10, dx + 2);				//得到'		 
		this->gpsx.longitude = this->gpsx.longitude*NMEA_Pow(10, 5) + (rs*NMEA_Pow(10, 5 - dx)) / 60;//转换为° 
	}
	posx = NMEA_Comma_Pos(p1, 6);								//东经还是西经
	if (posx != 0XFF)
		this->gpsx.ewhemi = *(p1 + posx);
	posx = NMEA_Comma_Pos(p1, 9);								//得到UTC日期
	if (posx != 0XFF)
	{
		temp = NMEA_Str2num(p1 + posx, &dx);		 				//得到UTC日期
		this->gpsx.utc.date = temp / 10000;
		this->gpsx.utc.month = (temp / 100) % 100;
		this->gpsx.utc.year = 2000 + temp % 100;
	}
}
//分析GNVTG信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void GPS_S1216::NMEA_GNVTG_Analysis(byte *buf)
{
	byte *p1, dx;
	byte posx;
	p1 = (byte*)strstr((const char *)buf, "$GNVTG");
	posx = NMEA_Comma_Pos(p1, 7);								//得到地面速率
	if (posx != 0XFF)
	{
		this->gpsx.speed = NMEA_Str2num(p1 + posx, &dx);
		if (dx < 3)
			this->gpsx.speed *= NMEA_Pow(10, 3 - dx);	 	 		//确保扩大1000倍
	}
}
//提取NMEA-0183信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void GPS_S1216::Analysis(byte *buf)
{
	this->NMEA_GPGSV_Analysis(buf);	//GPGSV解析
	this->NMEA_BDGSV_Analysis(buf);	//BDGSV解析
	this->NMEA_GNGGA_Analysis(buf);	//GNGGA解析 	
	this->NMEA_GNGSA_Analysis(buf);	//GPNSA解析
	this->NMEA_GNRMC_Analysis(buf);	//GPNMC解析
	this->NMEA_GNVTG_Analysis(buf);	//GPNTG解析
}

//显示GPS定位信息 
void GPS_S1216::Show()
{
	const byte*fixmode_tbl[4] = { "Fail","Fail"," 2D "," 3D " };	//fix mode字符串 
	float tp;
	tp = this->gpsx.longitude;
	debug_printf("经度:%.5f %1c ", tp /= 100000, this->gpsx.ewhemi);	//得到经度字符串
	tp = this->gpsx.latitude;
	debug_printf("纬度:%.5f %1c ", tp /= 100000, this->gpsx.nshemi);	//得到纬度字符串
	tp = this->gpsx.altitude;
	debug_printf("海拔:%.1fm ", tp /= 10);	    			//得到高度字符串
	tp = this->gpsx.speed;
	debug_printf("速度:%.3fkm/h ", tp /= 1000);		    		//得到速度字符串	
	if (this->gpsx.fixmode <= 3)														//定位状态
	{
		debug_printf("Fix Mode:%s", fixmode_tbl[this->gpsx.fixmode]);
	}
	debug_printf("GPS+BD有效:%02d ", this->gpsx.posslnum);	 		//用于定位的GPS卫星数
	debug_printf("GPS可见:%02d ", this->gpsx.svnum % 100);	 		//可见GPS卫星数
	debug_printf("BD可见e:%02d ", this->gpsx.beidou_svnum % 100);	 		//可见北斗卫星数
	debug_printf("UTC:%04d-%02d-%02d ", this->gpsx.utc.year, this->gpsx.utc.month, this->gpsx.utc.date);	//显示UTC日期
	debug_printf("%02d:%02d:%02d", this->gpsx.utc.hour, this->gpsx.utc.min, this->gpsx.utc.sec);	//显示UTC时间
}
