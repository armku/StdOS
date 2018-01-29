#include "Sys.h"
#include "Drivers\GPS_S1216.h"
#include "Device\SerialPort.h"

#define _GPSTEST
#ifdef _GPSTEST
GPS_S1216 gps1216;
#define USART3_MAX_RECV_LEN		600					//�����ջ����ֽ���
byte USART1_TX_BUF[USART3_MAX_RECV_LEN]; 					//����1,���ͻ�����
uint OnUsart3Read(ITransport *transport, Buffer &bs, void *para, void *para2)
{
//	bs.Show(true);
	gps1216.Analysis((byte*)bs.GetBuffer());//�����ַ���
	gps1216.Show();				//��ʾ��Ϣ	
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
	if (gps1216.SkyTra_Cfg_Rate(5, USART3_TX_BUF, len) != 0)	//���ö�λ��Ϣ�����ٶ�Ϊ5Hz,˳���ж�GPSģ���Ƿ���λ. 
	{
		printf("%s", "S1216F8-BD Setting...");
		do
		{
			usart3_init(36, 9600);			//��ʼ������3������Ϊ9600
			gps1216.SkyTra_Cfg_Prt(3, USART3_TX_BUF, len);			//��������ģ��Ĳ�����Ϊ38400			
			usart3_init(36, 38400);			//��ʼ������3������Ϊ38400
			key = gps1216.SkyTra_Cfg_Tp(100000, USART3_TX_BUF, len);	//������Ϊ100ms
		} while (gps1216.SkyTra_Cfg_Rate(5, USART3_TX_BUF, len) != 0 && key != 0);//����SkyTraF8-BD�ĸ�������Ϊ5Hz
		printf("%s", "S1216F8-BD Set Done!!");
		delay_ms(500);
	}
#endif
}
#if 0
//����һ�����ݸ�SkyTraF8-BD������ͨ������3����
//dbuf�����ݻ����׵�ַ
//len��Ҫ���͵��ֽ���
void SendBuffer(byte* dbuf, ushort len)
{
	ushort j;
	for (j = 0; j < len; j++)//ѭ����������
	{
		while ((USART3->SR & 0X40) == 0);//ѭ������,ֱ���������   
		USART3->DR = dbuf[j];
	}
}

const uint BAUD_id[9] = { 4800,9600,19200,38400,57600,115200,230400,460800,921600 };//ģ��֧�ֲ���������
																					//����SkyTra_GPS/����ģ�鲨����
																					//baud_id:0~8����Ӧ������,4800/9600/19200/38400/57600/115200/230400/460800/921600	  
																					//����ֵ:0,ִ�гɹ�;����,ִ��ʧ��(���ﲻ�᷵��0��)
byte GPS_S1216::SkyTra_Cfg_Prt(uint baud_id, byte* buf, int& len)
{
	SkyTra_baudrate *cfg_prt = (SkyTra_baudrate *)buf;
	cfg_prt->sos = 0XA1A0;		//��������(С��ģʽ)
	cfg_prt->PL = 0X0400;			//��Ч���ݳ���(С��ģʽ)
	cfg_prt->id = 0X05;		    //���ò����ʵ�ID 
	cfg_prt->com_port = 0X00;			//��������1
	cfg_prt->Baud_id = baud_id;	 	////�����ʶ�Ӧ���
	cfg_prt->Attributes = 1; 		  //���浽SRAM&FLASH
	cfg_prt->CS = cfg_prt->id^cfg_prt->com_port^cfg_prt->Baud_id^cfg_prt->Attributes;
	cfg_prt->end = 0X0A0D;        //���ͽ�����(С��ģʽ)
	SendBuffer((byte*)cfg_prt, sizeof(SkyTra_baudrate));//�������ݸ�SkyTra   
	delay_ms(200);				//�ȴ�������� 
	usart3_init(36, BAUD_id[baud_id]);	//���³�ʼ������3  
	return SkyTra_Cfg_Ack_Check();//���ﲻ�ᷴ��0,��ΪUBLOX��������Ӧ���ڴ������³�ʼ����ʱ���Ѿ���������.
}
//����SkyTra_GPSģ���ʱ��������
//width:������1~100000(us)
//����ֵ:0,���ͳɹ�;����,����ʧ��.
byte GPS_S1216::SkyTra_Cfg_Tp(uint width, byte* buf, int& len)
{
	uint temp = width;
	SkyTra_pps_width *cfg_tp = (SkyTra_pps_width *)buf;
	temp = (width >> 24) | ((width >> 8) & 0X0000FF00) | ((width << 8) & 0X00FF0000) | ((width << 24) & 0XFF000000);//С��ģʽ
	cfg_tp->sos = 0XA1A0;		    //cfg header(С��ģʽ)
	cfg_tp->PL = 0X0700;        //��Ч���ݳ���(С��ģʽ)
	cfg_tp->id = 0X65;			    //cfg tp id
	cfg_tp->Sub_ID = 0X01;			//����������Ϊ20���ֽ�.
	cfg_tp->width = temp;		  //������,us
	cfg_tp->Attributes = 0X01;  //���浽SRAM&FLASH	
	cfg_tp->CS = cfg_tp->id^cfg_tp->Sub_ID ^ (cfg_tp->width >> 24) ^ (cfg_tp->width >> 16) & 0XFF ^ (cfg_tp->width >> 8) & 0XFF ^ cfg_tp->width & 0XFF ^ cfg_tp->Attributes;    	//�û���ʱΪ0ns
	cfg_tp->end = 0X0A0D;       //���ͽ�����(С��ģʽ)
	SendBuffer((byte*)cfg_tp, sizeof(SkyTra_pps_width));//�������ݸ�NEO-6M  
	return SkyTra_Cfg_Ack_Check();
}
//����SkyTraF8-BD�ĸ�������	    
//Frep:��ȡֵ��Χ:1,2,4,5,8,10,20,25,40,50������ʱ��������λΪHz������ܴ���50Hz
//����ֵ:0,���ͳɹ�;����,����ʧ��.
byte GPS_S1216::SkyTra_Cfg_Rate(byte Frep, byte* buf, int& len)
{
	SkyTra_PosRate *cfg_rate = (SkyTra_PosRate *)buf;
	cfg_rate->sos = 0XA1A0;	    //cfg header(С��ģʽ)
	cfg_rate->PL = 0X0300;			//��Ч���ݳ���(С��ģʽ)
	cfg_rate->id = 0X0E;	      //cfg rate id
	cfg_rate->rate = Frep;	 	  //��������
	cfg_rate->Attributes = 0X01;	   	//���浽SRAM&FLASH	.
	cfg_rate->CS = cfg_rate->id^cfg_rate->rate^cfg_rate->Attributes;//������,us
	cfg_rate->end = 0X0A0D;       //���ͽ�����(С��ģʽ)
	SendBuffer((byte*)cfg_rate, sizeof(SkyTra_PosRate));//�������ݸ�NEO-6M 
	return SkyTra_Cfg_Ack_Check();
}
///////////////////////////////////////////UBLOX ���ô���/////////////////////////////////////
////���CFG����ִ�����
////����ֵ:0,ACK�ɹ�
////       1,���ճ�ʱ����
////       2,û���ҵ�ͬ���ַ�
////       3,���յ�NACKӦ��
byte GPS_S1216::SkyTra_Cfg_Ack_Check()
{
	ushort len = 0, i;
	byte rval = 0;
	while ((USART3_RX_STA & 0X8000) == 0 && len < 100)//�ȴ����յ�Ӧ��   
	{
		len++;
		delay_ms(5);
	}
	if (len < 100)   	//��ʱ����.
	{
		len = USART3_RX_STA & 0X7FFF;	//�˴ν��յ������ݳ��� 
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
		if (i == len)rval = 2;						//û���ҵ�ͬ���ַ�
	}
	else
		rval = 1;								//���ճ�ʱ����
	USART3_RX_STA = 0;							//�������
	return rval;
}
#endif
#endif // _GPSTEST
