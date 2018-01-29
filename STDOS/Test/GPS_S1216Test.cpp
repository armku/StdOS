#include "Sys.h"
#include "Drivers\GPS_S1216.h"
#include "Device\SerialPort.h"

#define _GPSTEST
#ifdef _GPSTEST
GPS_S1216 gps1216;
#define USART3_MAX_RECV_LEN		600					//最大接收缓存字节数
byte USART1_TX_BUF[USART3_MAX_RECV_LEN]; 					//串口1,发送缓存区
uint OnUsart3Read(ITransport *transport, Buffer &bs, void *para, void *para2)
{
//	bs.Show(true);
	gps1216.Analysis((byte*)bs.GetBuffer());//分析字符串
	gps1216.Show();				//显示信息	
	return 0;
}

SerialPort *sp3;
byte com3rx[500], com3tx[500];
void GPSTest()
{
	sp3 = new SerialPort(COM3);
	sp3->Rx.SetBuf(com3rx, ArrayLength(com3rx));
	sp3->Tx.SetBuf(com3tx, ArrayLength(com3tx));
	sp3->Register(OnUsart3Read);
	sp3->SetBaudRate(38400);
	sp3->Open();
#if 0
	printf("%s", "S1216F8-BD TEST");
	printf("%s", "KEY0:Upload NMEA Data SW");
	printf("%s", "NMEA Data Upload:OFF");
	int len;
	if (gps1216.SkyTra_Cfg_Rate(5, USART3_TX_BUF, len) != 0)	//设置定位信息更新速度为5Hz,顺便判断GPS模块是否在位. 
	{
		printf("%s", "S1216F8-BD Setting...");
		do
		{
			usart3_init(36, 9600);			//初始化串口3波特率为9600
			gps1216.SkyTra_Cfg_Prt(3, USART3_TX_BUF, len);			//重新设置模块的波特率为38400			
			usart3_init(36, 38400);			//初始化串口3波特率为38400
			key = gps1216.SkyTra_Cfg_Tp(100000, USART3_TX_BUF, len);	//脉冲宽度为100ms
		} while (gps1216.SkyTra_Cfg_Rate(5, USART3_TX_BUF, len) != 0 && key != 0);//配置SkyTraF8-BD的更新速率为5Hz
		printf("%s", "S1216F8-BD Set Done!!");
		delay_ms(500);
	}
#endif
}
#if 0
//发送一批数据给SkyTraF8-BD，这里通过串口3发送
//dbuf：数据缓存首地址
//len：要发送的字节数
void SendBuffer(byte* dbuf, ushort len)
{
	ushort j;
	for (j = 0; j < len; j++)//循环发送数据
	{
		while ((USART3->SR & 0X40) == 0);//循环发送,直到发送完毕   
		USART3->DR = dbuf[j];
	}
}

const uint BAUD_id[9] = { 4800,9600,19200,38400,57600,115200,230400,460800,921600 };//模块支持波特率数组
																					//配置SkyTra_GPS/北斗模块波特率
																					//baud_id:0~8，对应波特率,4800/9600/19200/38400/57600/115200/230400/460800/921600	  
																					//返回值:0,执行成功;其他,执行失败(这里不会返回0了)
byte GPS_S1216::SkyTra_Cfg_Prt(uint baud_id, byte* buf, int& len)
{
	SkyTra_baudrate *cfg_prt = (SkyTra_baudrate *)buf;
	cfg_prt->sos = 0XA1A0;		//引导序列(小端模式)
	cfg_prt->PL = 0X0400;			//有效数据长度(小端模式)
	cfg_prt->id = 0X05;		    //配置波特率的ID 
	cfg_prt->com_port = 0X00;			//操作串口1
	cfg_prt->Baud_id = baud_id;	 	////波特率对应编号
	cfg_prt->Attributes = 1; 		  //保存到SRAM&FLASH
	cfg_prt->CS = cfg_prt->id^cfg_prt->com_port^cfg_prt->Baud_id^cfg_prt->Attributes;
	cfg_prt->end = 0X0A0D;        //发送结束符(小端模式)
	SendBuffer((byte*)cfg_prt, sizeof(SkyTra_baudrate));//发送数据给SkyTra   
	delay_ms(200);				//等待发送完成 
	usart3_init(36, BAUD_id[baud_id]);	//重新初始化串口3  
	return SkyTra_Cfg_Ack_Check();//这里不会反回0,因为UBLOX发回来的应答在串口重新初始化的时候已经被丢弃了.
}
//配置SkyTra_GPS模块的时钟脉冲宽度
//width:脉冲宽度1~100000(us)
//返回值:0,发送成功;其他,发送失败.
byte GPS_S1216::SkyTra_Cfg_Tp(uint width, byte* buf, int& len)
{
	uint temp = width;
	SkyTra_pps_width *cfg_tp = (SkyTra_pps_width *)buf;
	temp = (width >> 24) | ((width >> 8) & 0X0000FF00) | ((width << 8) & 0X00FF0000) | ((width << 24) & 0XFF000000);//小端模式
	cfg_tp->sos = 0XA1A0;		    //cfg header(小端模式)
	cfg_tp->PL = 0X0700;        //有效数据长度(小端模式)
	cfg_tp->id = 0X65;			    //cfg tp id
	cfg_tp->Sub_ID = 0X01;			//数据区长度为20个字节.
	cfg_tp->width = temp;		  //脉冲宽度,us
	cfg_tp->Attributes = 0X01;  //保存到SRAM&FLASH	
	cfg_tp->CS = cfg_tp->id^cfg_tp->Sub_ID ^ (cfg_tp->width >> 24) ^ (cfg_tp->width >> 16) & 0XFF ^ (cfg_tp->width >> 8) & 0XFF ^ cfg_tp->width & 0XFF ^ cfg_tp->Attributes;    	//用户延时为0ns
	cfg_tp->end = 0X0A0D;       //发送结束符(小端模式)
	SendBuffer((byte*)cfg_tp, sizeof(SkyTra_pps_width));//发送数据给NEO-6M  
	return SkyTra_Cfg_Ack_Check();
}
//配置SkyTraF8-BD的更新速率	    
//Frep:（取值范围:1,2,4,5,8,10,20,25,40,50）测量时间间隔，单位为Hz，最大不能大于50Hz
//返回值:0,发送成功;其他,发送失败.
byte GPS_S1216::SkyTra_Cfg_Rate(byte Frep, byte* buf, int& len)
{
	SkyTra_PosRate *cfg_rate = (SkyTra_PosRate *)buf;
	cfg_rate->sos = 0XA1A0;	    //cfg header(小端模式)
	cfg_rate->PL = 0X0300;			//有效数据长度(小端模式)
	cfg_rate->id = 0X0E;	      //cfg rate id
	cfg_rate->rate = Frep;	 	  //更新速率
	cfg_rate->Attributes = 0X01;	   	//保存到SRAM&FLASH	.
	cfg_rate->CS = cfg_rate->id^cfg_rate->rate^cfg_rate->Attributes;//脉冲间隔,us
	cfg_rate->end = 0X0A0D;       //发送结束符(小端模式)
	SendBuffer((byte*)cfg_rate, sizeof(SkyTra_PosRate));//发送数据给NEO-6M 
	return SkyTra_Cfg_Ack_Check();
}
///////////////////////////////////////////UBLOX 配置代码/////////////////////////////////////
////检查CFG配置执行情况
////返回值:0,ACK成功
////       1,接收超时错误
////       2,没有找到同步字符
////       3,接收到NACK应答
byte GPS_S1216::SkyTra_Cfg_Ack_Check()
{
	ushort len = 0, i;
	byte rval = 0;
	while ((USART3_RX_STA & 0X8000) == 0 && len < 100)//等待接收到应答   
	{
		len++;
		delay_ms(5);
	}
	if (len < 100)   	//超时错误.
	{
		len = USART3_RX_STA & 0X7FFF;	//此次接收到的数据长度 
		for (i = 0; i < len; i++)
		{
			if (USART3_RX_BUF[i] == 0X83)
				break;
			else if (USART3_RX_BUF[i] == 0X84)
			{
				rval = 3;
				break;
			}
		}
		if (i == len)rval = 2;						//没有找到同步字符
	}
	else
		rval = 1;								//接收超时错误
	USART3_RX_STA = 0;							//清除接收
	return rval;
}
#endif
#endif // _GPSTEST
