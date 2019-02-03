#include <string.h>
#include <stdio.h>
#include "GPS_S1216.h"
#include "../../../../StdOS/Kernel/Sys.h"
//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 ���������Ϣ�ṹ��
__packed typedef struct
{
	uint16_t sos;            //�������У��̶�Ϊ0XA0A1
	uint16_t PL;             //��Ч���ݳ���0X0009�� 
	uint8_t id;             //ID���̶�Ϊ0X08
	uint8_t GGA;            //1~255��s��,0:disable
	uint8_t GSA;            //1~255��s��,0:disable
	uint8_t GSV;            //1~255��s��,0:disable
	uint8_t GLL;            //1~255��s��,0:disable
	uint8_t RMC;            //1~255��s��,0:disable
	uint8_t VTG;            //1~255��s��,0:disable
	uint8_t ZDA;            //1~255��s��,0:disable
	uint8_t Attributes;     //�������ݱ���λ�� ,0���浽SRAM��1���浽SRAM&FLASH��2��ʱ����
	uint8_t CS;             //У��ֵ
	uint16_t end;            //������:0X0D0A  
}SkyTra_outmsg;
//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 ACK�ṹ��
__packed typedef struct
{
	uint16_t sos;            //�������У��̶�Ϊ0XA0A1
	uint16_t PL;             //��Ч���ݳ���0X0002�� 
	uint8_t id;             //ID���̶�Ϊ0X83
	uint8_t ACK_ID;         //ACK ID may further consist of message ID and message sub-ID which will become 3 bytes of ACK message
	uint8_t CS;             //У��ֵ
	uint16_t end;            //������ 
}SkyTra_ACK;
//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 NACK�ṹ��
__packed typedef struct
{
	uint16_t sos;            //�������У��̶�Ϊ0XA0A1
	uint16_t PL;             //��Ч���ݳ���0X0002�� 
	uint8_t id;             //ID���̶�Ϊ0X84
	uint8_t NACK_ID;         //ACK ID may further consist of message ID and message sub-ID which will become 3 bytes of ACK message
	uint8_t CS;             //У��ֵ
	uint16_t end;            //������ 
}SkyTra_NACK;
GPS_S1216::GPS_S1216()
{
	this->gpsx.ewhemi = 'E';
	this->gpsx.nshemi = 'N';
}

//��buf����õ���cx���������ڵ�λ��
//����ֵ:0~0XFE,����������λ�õ�ƫ��.
//       0XFF,�������ڵ�cx������							  
uint8_t GPS_S1216::NMEA_Comma_Pos(uint8_t *buf, uint8_t cx)
{
	uint8_t *p = buf;
	while (cx)
	{
		if (*buf == '*' || *buf<' ' || *buf>'z')
			return 0XFF;//����'*'���߷Ƿ��ַ�,�򲻴��ڵ�cx������
		if (*buf == ',')cx--;
		buf++;
	}
	return buf - p;
}
//m^n����
//����ֵ:m^n�η�.
uint32_t GPS_S1216::NMEA_Pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while (n--)
		result *= m;
	return result;
}
//strת��Ϊ����,��','����'*'����
//buf:���ִ洢��
//dx:С����λ��,���ظ����ú���
//����ֵ:ת�������ֵ
int GPS_S1216::NMEA_Str2num(uint8_t *buf, uint8_t*dx)
{
	uint8_t *p = buf;
	uint32_t ires = 0, fres = 0;
	uint8_t ilen = 0, flen = 0, i;
	uint8_t mask = 0;
	int res;
	while (1) //�õ�������С���ĳ���
	{
		if (*p == '-')
		{
			mask |= 0X02;
			p++;
		}//�Ǹ���
		if (*p == ',' || (*p == '*'))
			break;//����������
		if (*p == '.')
		{
			mask |= 0X01;
			p++;
		}//����С������
		else if (*p > '9' || (*p < '0'))	//�зǷ��ַ�
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
	if (mask & 0X02)buf++;	//ȥ������
	for (i = 0; i < ilen; i++)	//�õ�������������
	{
		ires += NMEA_Pow(10, ilen - 1 - i)*(buf[i] - '0');
	}
	if (flen > 5)
		flen = 5;	//���ȡ5λС��
	*dx = flen;	 		//С����λ��
	for (i = 0; i < flen; i++)	//�õ�С����������
	{
		fres += NMEA_Pow(10, flen - 1 - i)*(buf[ilen + 1 + i] - '0');
	}
	res = ires * NMEA_Pow(10, flen) + fres;
	if (mask & 0X02)
		res = -res;
	return res;
}
//����GPGSV��Ϣ
//gpsx:nmea��Ϣ�ṹ��
//buf:���յ���GPS���ݻ������׵�ַ
void GPS_S1216::NMEA_GPGSV_Analysis(uint8_t *buf)
{
	uint8_t *p, *p1, dx;
	uint8_t len, i, j, slx = 0;
	uint8_t posx;
	p = buf;
	p1 = (uint8_t*)strstr((const char *)p, "$GPGSV");
	len = p1[7] - '0';								//�õ�GPGSV������
	posx = NMEA_Comma_Pos(p1, 3); 					//�õ��ɼ���������
	if (posx != 0XFF) this->gpsx.svnum = NMEA_Str2num(p1 + posx, &dx);
	for (i = 0; i < len; i++)
	{
		p1 = (uint8_t*)strstr((const char *)p, "$GPGSV");
		for (j = 0; j < 4; j++)
		{
			posx = NMEA_Comma_Pos(p1, 4 + j * 4);
			if (posx != 0XFF)
				this->gpsx.slmsg[slx].num = NMEA_Str2num(p1 + posx, &dx);	//�õ����Ǳ��
			else
				break;
			posx = NMEA_Comma_Pos(p1, 5 + j * 4);
			if (posx != 0XFF)
				this->gpsx.slmsg[slx].eledeg = NMEA_Str2num(p1 + posx, &dx);//�õ��������� 
			else
				break;
			posx = NMEA_Comma_Pos(p1, 6 + j * 4);
			if (posx != 0XFF)
				this->gpsx.slmsg[slx].azideg = NMEA_Str2num(p1 + posx, &dx);//�õ����Ƿ�λ��
			else
				break;
			posx = NMEA_Comma_Pos(p1, 7 + j * 4);
			if (posx != 0XFF)
				this->gpsx.slmsg[slx].sn = NMEA_Str2num(p1 + posx, &dx);	//�õ����������
			else
				break;
			slx++;
		}
		p = p1 + 1;//�л�����һ��GPGSV��Ϣ
	}
}
//����BDGSV��Ϣ
//gpsx:nmea��Ϣ�ṹ��
//buf:���յ���GPS���ݻ������׵�ַ
void GPS_S1216::NMEA_BDGSV_Analysis(uint8_t *buf)
{
	uint8_t *p, *p1, dx;
	uint8_t len, i, j, slx = 0;
	uint8_t posx;
	p = buf;
	p1 = (uint8_t*)strstr((const char *)p, "$BDGSV");
	len = p1[7] - '0';								//�õ�BDGSV������
	posx = NMEA_Comma_Pos(p1, 3); 					//�õ��ɼ�������������
	if (posx != 0XFF)
		this->gpsx.beidou_svnum = NMEA_Str2num(p1 + posx, &dx);
	for (i = 0; i < len; i++)
	{
		p1 = (uint8_t*)strstr((const char *)p, "$BDGSV");
		for (j = 0; j < 4; j++)
		{
			posx = NMEA_Comma_Pos(p1, 4 + j * 4);
			if (posx != 0XFF)
				this->gpsx.beidou_slmsg[slx].beidou_num = NMEA_Str2num(p1 + posx, &dx);	//�õ����Ǳ��
			else
				break;
			posx = NMEA_Comma_Pos(p1, 5 + j * 4);
			if (posx != 0XFF)
				this->gpsx.beidou_slmsg[slx].beidou_eledeg = NMEA_Str2num(p1 + posx, &dx);//�õ��������� 
			else
				break;
			posx = NMEA_Comma_Pos(p1, 6 + j * 4);
			if (posx != 0XFF)
				this->gpsx.beidou_slmsg[slx].beidou_azideg = NMEA_Str2num(p1 + posx, &dx);//�õ����Ƿ�λ��
			else
				break;
			posx = NMEA_Comma_Pos(p1, 7 + j * 4);
			if (posx != 0XFF)
				this->gpsx.beidou_slmsg[slx].beidou_sn = NMEA_Str2num(p1 + posx, &dx);	//�õ����������
			else
				break;
			slx++;
		}
		p = p1 + 1;//�л�����һ��BDGSV��Ϣ
	}
}
//����GNGGA��Ϣ
//gpsx:nmea��Ϣ�ṹ��
//buf:���յ���GPS���ݻ������׵�ַ
void GPS_S1216::NMEA_GNGGA_Analysis(uint8_t *buf)
{
	uint8_t *p1, dx;
	uint8_t posx;
	p1 = (uint8_t*)strstr((const char *)buf, "$GNGGA");
	posx = NMEA_Comma_Pos(p1, 6);								//�õ�GPS״̬
	if (posx != 0XFF)
		this->gpsx.gpssta = NMEA_Str2num(p1 + posx, &dx);
	posx = NMEA_Comma_Pos(p1, 7);								//�õ����ڶ�λ��������
	if (posx != 0XFF)
		this->gpsx.posslnum = NMEA_Str2num(p1 + posx, &dx);
	posx = NMEA_Comma_Pos(p1, 9);								//�õ����θ߶�
	if (posx != 0XFF)
		this->gpsx.altitude = NMEA_Str2num(p1 + posx, &dx);
}
//����GNGSA��Ϣ
//gpsx:nmea��Ϣ�ṹ��
//buf:���յ���GPS���ݻ������׵�ַ
void GPS_S1216::NMEA_GNGSA_Analysis(uint8_t *buf)
{
	uint8_t *p1, dx;
	uint8_t posx;
	uint8_t i;
	p1 = (uint8_t*)strstr((const char *)buf, "$GNGSA");
	posx = NMEA_Comma_Pos(p1, 2);								//�õ���λ����
	if (posx != 0XFF)
		this->gpsx.fixmode = NMEA_Str2num(p1 + posx, &dx);
	for (i = 0; i < 12; i++)										//�õ���λ���Ǳ��
	{
		posx = NMEA_Comma_Pos(p1, 3 + i);
		if (posx != 0XFF)
			this->gpsx.possl[i] = NMEA_Str2num(p1 + posx, &dx);
		else
			break;
	}
	posx = NMEA_Comma_Pos(p1, 15);								//�õ�PDOPλ�þ�������
	if (posx != 0XFF)
		this->gpsx.pdop = NMEA_Str2num(p1 + posx, &dx);
	posx = NMEA_Comma_Pos(p1, 16);								//�õ�HDOPλ�þ�������
	if (posx != 0XFF)
		this->gpsx.hdop = NMEA_Str2num(p1 + posx, &dx);
	posx = NMEA_Comma_Pos(p1, 17);								//�õ�VDOPλ�þ�������
	if (posx != 0XFF)
		this->gpsx.vdop = NMEA_Str2num(p1 + posx, &dx);
}
//����GNRMC��Ϣ
//gpsx:nmea��Ϣ�ṹ��
//buf:���յ���GPS���ݻ������׵�ַ
void GPS_S1216::NMEA_GNRMC_Analysis(uint8_t *buf)
{
	uint8_t *p1, dx;
	uint8_t posx;
	uint32_t temp;
	float rs;
	p1 = (uint8_t*)strstr((const char *)buf, "$GNRMC");//"$GNRMC",������&��GNRMC�ֿ������,��ֻ�ж�GPRMC.
	posx = NMEA_Comma_Pos(p1, 1);								//�õ�UTCʱ��
	if (posx != 0XFF)
	{
		temp = NMEA_Str2num(p1 + posx, &dx) / NMEA_Pow(10, dx);	 	//�õ�UTCʱ��,ȥ��ms
		this->gpsx.utc.hour = temp / 10000;
		this->gpsx.utc.min = (temp / 100) % 100;
		this->gpsx.utc.sec = temp % 100;
	}
	posx = NMEA_Comma_Pos(p1, 3);								//�õ�γ��
	if (posx != 0XFF)
	{
		temp = NMEA_Str2num(p1 + posx, &dx);
		this->gpsx.latitude = temp / NMEA_Pow(10, dx + 2);	//�õ���
		rs = temp % NMEA_Pow(10, dx + 2);				//�õ�'		 
		this->gpsx.latitude = this->gpsx.latitude*NMEA_Pow(10, 5) + (rs*NMEA_Pow(10, 5 - dx)) / 60;//ת��Ϊ�� 
	}
	posx = NMEA_Comma_Pos(p1, 4);								//��γ���Ǳ�γ 
	if (posx != 0XFF)this->gpsx.nshemi = *(p1 + posx);
	posx = NMEA_Comma_Pos(p1, 5);								//�õ�����
	if (posx != 0XFF)
	{
		temp = NMEA_Str2num(p1 + posx, &dx);
		this->gpsx.longitude = temp / NMEA_Pow(10, dx + 2);	//�õ���
		rs = temp % NMEA_Pow(10, dx + 2);				//�õ�'		 
		this->gpsx.longitude = this->gpsx.longitude*NMEA_Pow(10, 5) + (rs*NMEA_Pow(10, 5 - dx)) / 60;//ת��Ϊ�� 
	}
	posx = NMEA_Comma_Pos(p1, 6);								//������������
	if (posx != 0XFF)
		this->gpsx.ewhemi = *(p1 + posx);
	posx = NMEA_Comma_Pos(p1, 9);								//�õ�UTC����
	if (posx != 0XFF)
	{
		temp = NMEA_Str2num(p1 + posx, &dx);		 				//�õ�UTC����
		this->gpsx.utc.date = temp / 10000;
		this->gpsx.utc.month = (temp / 100) % 100;
		this->gpsx.utc.year = 2000 + temp % 100;
	}
}
//����GNVTG��Ϣ
//gpsx:nmea��Ϣ�ṹ��
//buf:���յ���GPS���ݻ������׵�ַ
void GPS_S1216::NMEA_GNVTG_Analysis(uint8_t *buf)
{
	uint8_t *p1, dx;
	uint8_t posx;
	p1 = (uint8_t*)strstr((const char *)buf, "$GNVTG");
	posx = NMEA_Comma_Pos(p1, 7);								//�õ���������
	if (posx != 0XFF)
	{
		this->gpsx.speed = NMEA_Str2num(p1 + posx, &dx);
		if (dx < 3)
			this->gpsx.speed *= NMEA_Pow(10, 3 - dx);	 	 		//ȷ������1000��
	}
}
//��ȡNMEA-0183��Ϣ
//gpsx:nmea��Ϣ�ṹ��
//buf:���յ���GPS���ݻ������׵�ַ
void GPS_S1216::Analysis(uint8_t *buf)
{
	this->NMEA_GPGSV_Analysis(buf);	//GPGSV����
	this->NMEA_BDGSV_Analysis(buf);	//BDGSV����
	this->NMEA_GNGGA_Analysis(buf);	//GNGGA���� 	
	this->NMEA_GNGSA_Analysis(buf);	//GPNSA����
	this->NMEA_GNRMC_Analysis(buf);	//GPNMC����
	this->NMEA_GNVTG_Analysis(buf);	//GPNTG����
}

//��ʾGPS��λ��Ϣ 
void GPS_S1216::Show()
{
	const uint8_t*fixmode_tbl[4] = { "Fail","Fail"," 2D "," 3D " };	//fix mode�ַ��� 
	float tp;
	tp = this->gpsx.longitude;
	debug_printf("����:%.5f %1c ", tp /= 100000, this->gpsx.ewhemi);	//�õ������ַ���
	tp = this->gpsx.latitude;
	debug_printf("γ��:%.5f %1c ", tp /= 100000, this->gpsx.nshemi);	//�õ�γ���ַ���
	tp = this->gpsx.altitude;
	debug_printf("����:%.1fm ", tp /= 10);	    			//�õ��߶��ַ���
	tp = this->gpsx.speed;
	debug_printf("�ٶ�:%.3fkm/h ", tp /= 1000);		    		//�õ��ٶ��ַ���	
	if (this->gpsx.fixmode <= 3)														//��λ״̬
	{
		debug_printf("��λģʽ:%s", fixmode_tbl[this->gpsx.fixmode]);
	}
	debug_printf("GPS+BD��Ч:%02d ", this->gpsx.posslnum);	 		//���ڶ�λ��GPS������
	debug_printf("GPS�ɼ�:%02d ", this->gpsx.svnum % 100);	 		//�ɼ�GPS������
	debug_printf("BD�ɼ�:%02d ", this->gpsx.beidou_svnum % 100);	 		//�ɼ�����������
	debug_printf("UTC:%04d-%02d-%02d ", this->gpsx.utc.year, this->gpsx.utc.month, this->gpsx.utc.date);	//��ʾUTC����
	debug_printf("%02d:%02d:%02d", this->gpsx.utc.hour, this->gpsx.utc.min, this->gpsx.utc.sec);	//��ʾUTCʱ��
}
