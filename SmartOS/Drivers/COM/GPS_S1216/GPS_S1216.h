#ifndef _GPS_S1216_H
#define _GPS_S1216_H

#include "Buffer.h"
//GPS NMEA-0183Э����Ҫ�����ṹ�嶨�� 
//������Ϣ
__packed typedef struct
{
	uint8_t num;		//���Ǳ��
	uint8_t eledeg;	//��������
	uint16_t azideg;	//���Ƿ�λ��
	uint8_t sn;		//�����		   
}nmea_slmsg;
//���� NMEA-0183Э����Ҫ�����ṹ�嶨�� 
//������Ϣ
__packed typedef struct
{
	uint8_t beidou_num;		//���Ǳ��
	uint8_t beidou_eledeg;	//��������
	uint16_t beidou_azideg;	//���Ƿ�λ��
	uint8_t beidou_sn;		//�����		   
}beidou_nmea_slmsg;

//UTCʱ����Ϣ
__packed typedef struct
{
	uint16_t year;	//���
	uint8_t month;	//�·�
	uint8_t date;	//����
	uint8_t hour; 	//Сʱ
	uint8_t min; 	//����
	uint8_t sec; 	//����
}nmea_utc_time;
//NMEA 0183 Э����������ݴ�Žṹ��
__packed typedef struct
{
	uint8_t svnum;					//�ɼ�GPS������
	uint8_t beidou_svnum;					//�ɼ�GPS������
	nmea_slmsg slmsg[12];		//���12��GPS����
	beidou_nmea_slmsg beidou_slmsg[12];		//���������12�ű�������
	nmea_utc_time utc;			//UTCʱ��
	uint latitude;				//γ�� ������100000��,ʵ��Ҫ����100000
	uint8_t nshemi;					//��γ/��γ,N:��γ;S:��γ				  
	uint longitude;			    //���� ������100000��,ʵ��Ҫ����100000
	uint8_t ewhemi;					//����/����,E:����;W:����
	uint8_t gpssta;					//GPS״̬:0,δ��λ;1,�ǲ�ֶ�λ;2,��ֶ�λ;6,���ڹ���.				  
	uint8_t posslnum;				//���ڶ�λ��GPS������,0~12.
	uint8_t possl[12];				//���ڶ�λ�����Ǳ��
	uint8_t fixmode;					//��λ����:1,û�ж�λ;2,2D��λ;3,3D��λ
	uint16_t pdop;					//λ�þ������� 0~500,��Ӧʵ��ֵ0~50.0
	uint16_t hdop;					//ˮƽ�������� 0~500,��Ӧʵ��ֵ0~50.0
	uint16_t vdop;					//��ֱ�������� 0~500,��Ӧʵ��ֵ0~50.0 

	int altitude;			 	//���θ߶�,�Ŵ���10��,ʵ�ʳ���10.��λ:0.1m	 
	uint16_t speed;					//��������,�Ŵ���1000��,ʵ�ʳ���10.��λ:0.001����/Сʱ	 
}nmea_msg;
////////////////////////////////////////////////////////////////////////////////////////////////////
//SkyTra S1216F8 ���ò����ʽṹ��
__packed typedef struct
{
	uint16_t sos;            //�������У��̶�Ϊ0XA0A1
	uint16_t PL;             //��Ч���ݳ���0X0004�� 
	uint8_t id;             //ID���̶�Ϊ0X05
	uint8_t com_port;       //COM�ڣ��̶�Ϊ0X00����COM1   
	uint8_t Baud_id;       //�����ʣ�0~8,4800,9600,19200,38400,57600,115200,230400,460800,921600��
	uint8_t Attributes;     //�������ݱ���λ�� ,0���浽SRAM��1���浽SRAM&FLASH��2��ʱ����
	uint8_t CS;             //У��ֵ
	uint16_t end;            //������:0X0D0A  
}SkyTra_baudrate;

//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 ����λ�ø����ʽṹ��
__packed typedef struct
{
	uint16_t sos;            //�������У��̶�Ϊ0XA0A1
	uint16_t PL;             //��Ч���ݳ���0X0003�� 
	uint8_t id;             //ID���̶�Ϊ0X0E
	uint8_t rate;           //ȡֵ��Χ:1, 2, 4, 5, 8, 10, 20, 25, 40, 50
	uint8_t Attributes;     //�������ݱ���λ�� ,0���浽SRAM��1���浽SRAM&FLASH��2��ʱ����
	uint8_t CS;             //У��ֵ
	uint16_t end;            //������:0X0D0A  
}SkyTra_PosRate;
//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 �����������(PPS)��Ƚṹ��
__packed typedef struct
{
	uint16_t sos;            //�������У��̶�Ϊ0XA0A1
	uint16_t PL;             //��Ч���ݳ���0X0007�� 
	uint8_t id;             //ID���̶�Ϊ0X65
	uint8_t Sub_ID;         //0X01
	uint width;        //1~100000(us)
	uint8_t Attributes;     //�������ݱ���λ�� ,0���浽SRAM��1���浽SRAM&FLASH��2��ʱ����
	uint8_t CS;             //У��ֵ
	uint16_t end;            //������:0X0D0A 
}SkyTra_pps_width;

class GPS_S1216
{
public:
	GPS_S1216();
	void Analysis(uint8_t *buf);
	void Show();
	nmea_msg gpsx;	//GPS��Ϣ
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
