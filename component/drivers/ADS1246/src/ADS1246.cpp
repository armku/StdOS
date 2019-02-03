#include "ADS1246.h"
#include "../../../StdOS/Kernel/Sys.h"

//�½���оƬ�������ݣ�������оƬ�������
//ADS1246�Ĵ����б�
#define ADC_REG_BCS         0x00  		//��·����Դ�Ĵ���
#define ADC_REG_VBIAS       0x01  		//ƫѹ�Ĵ���
#define ADC_REG_MUX         0x02  		//��·���ƼĴ���  ϵͳ������
#define ADC_REG_SYS0        0x03  		//ϵͳ�Ĵ���0  ѡ��PGA�������
#define ADC_REG_OFC0        0x04  		//ƫ��У׼ϵ���Ĵ���0
#define ADC_REG_OFC1        0x05  		//ƫ��У׼ϵ���Ĵ���1 
#define ADC_REG_OFC2        0x06  		//ƫ��У׼ϵ���Ĵ���2
#define ADC_REG_FSC0        0x07  		//������У׼ϵ���Ĵ���0  
#define ADC_REG_FSC1        0x08  		//������У׼ϵ���Ĵ���1   
#define ADC_REG_FSC2        0x09  		//������У׼ϵ���Ĵ���2   
#define ADC_REG_ID          0x0A   		//ID�Ĵ���    0x08

//ADS1246֧�ֵ������б�
#define ADC_GAIN_1          0x00   		// 
#define ADC_GAIN_2          0x10   		// 
#define ADC_GAIN_4          0x20   		// 
#define ADC_GAIN_8          0x30   		// 
#define ADC_GAIN_16         0x40  		//  
#define ADC_GAIN_32         0x50   		// 
#define ADC_GAIN_64         0x60   		// 
#define ADC_GAIN_128        0x70   		// 

//ADS1246֧�ֵ�ת�������б�
#define ADC_SPS_5           0x00    	//
#define ADC_SPS_10          0x01   		// 
#define ADC_SPS_20          0x02   		// 
#define ADC_SPS_40          0x03   		// 
#define ADC_SPS_80          0x04   		// 
#define ADC_SPS_160         0x05   		// 
#define ADC_SPS_320         0x06   		// 
#define ADC_SPS_640         0x07   		// 
#define ADC_SPS_1000        0x08   		// 
#define ADC_SPS_2000        0x09   		// 

//ADS1246ת��ģʽ����
#define ADC_MODE_SINGLECOV      0x00        //����ת��ģʽ  
#define ADC_MODE_CONTINUOUS     0x01        //����ת��ģʽ 

//ADS1246�������б�
#define ADC_CMD_WAKEUP      0x00            //�˳�˯��ģʽ  
#define ADC_CMD_SLEEP       0x02            //����˯��ģʽ  
#define ADC_CMD_SYNC        0x04            //ͬ��ADCת��  
#define ADC_CMD_RESET       0x06            //оƬ��λ  
#define ADC_CMD_NOP			0xFF            //�ղ���  
#define ADC_CMD_RDATA       0x12            //���ζ�ȡ����  
#define ADC_CMD_RDATAC      0x14            //������ȡ����  
#define ADC_CMD_SDATAC      0x16            //ֹͣ������ȡ  
#define ADC_CMD_RREG        0x20            //���Ĵ���  
#define ADC_CMD_WREG        0x40            //д�Ĵ���  
#define ADC_CMD_SYSOCAL     0x60            //ϵͳƫ��У׼  
#define ADC_CMD_SYSGCAL     0x61            //ϵͳ����У׼  
#define ADC_CMD_SELFOCAL    0x62            //ϵͳ��У׼  
#define ADC_CMD_RESTRICTED  0xF1            //  
#if ADS1246SPISOFT
ADS1246::ADS1246(SpiSoft *spi, mcuGpio& pinrd, Pin pinreset,ADSPEEDTYPE spd)
#else
ADS1246::ADS1246(Spi *spi, mcuGpio& pinrd, Pin pinreset,ADSPEEDTYPE spd)
#endif
{
	this->ppinreset.Invert=false;
	
	this->ppinreset.Set(pinreset);
	
	this->pspi=spi;
	this->ppinrd=&pinrd;
    this->ppinreset=0;
	this->speed=spd;
	this->online=false;
}

uint8_t ADS1246::ReadReg(uint8_t RegAddr)
{
    uint8_t ret = 0;
    uint8_t Cmd;

    this->pspi->Start();


    Cmd = ADC_CMD_RREG | RegAddr;
    this->pspi->Write(Cmd);
    this->pspi->Write(0);
    ret = this->pspi->Write(0X00);
    this->pspi->Write(0xff); //����NOP
    this->pspi->Stop();

    return ret;

}

void ADS1246::WriteReg(uint8_t RegAddr, uint8_t da)
{
    uint8_t Cmd;
    this->pspi->Start();

    Cmd = ADC_CMD_WREG | RegAddr;
    this->pspi->Write(Cmd);
    this->pspi->Write(0);
    this->pspi->Write(da);
    this->pspi->Write(0xff); //����NOP
    this->pspi->Stop();
}

/*---------------------------------------------------------
���ݶ�ȡ
7F FF FF +MAX
00 00 01 +MIN
00 00 00 0
FF FF FF -MIN
80 00 00 -MAX
---------------------------------------------------------*/
int ADS1246::decodead(uint8_t *da)
{
    int ret = 0;
    uint32_t ret1 = 0;
    if (da[0] < 0x80)
    {
        ret1 = da[0];
        ret1 <<= 8;
        ret1 |= da[1];
        ret1 <<= 8;
        ret1 |= da[2];
        ret = ret1;
    }
    else
    {
        ret1 = da[0];
        ret1 <<= 8;
        ret1 |= da[1];
        ret1 <<= 8;
        ret1 |= da[2];
        ret1 ^= 0x00ffffff;
        ret1 += 1;
        ret = ret1;
        ret =  - ret;
    }
    return ret;
}

int ADS1246::Read(void) //����-1,��ʾת��δ���
{
    uint8_t Cmd[3];
    int Ret = 0;
	this->readCnt++;
	this->readCntCheck++;

    Cmd[0] = ADC_CMD_RDATA;
    this->pspi->Start();

	this->flagOK=true;
    if (this->ppinrd->Read())
    {
		this->flagOK=false;
        return  - 1;
    }	
    this->pspi->Write(Cmd[0]);	
    Cmd[0] = this->pspi->Write(0xff);
    Cmd[1] = this->pspi->Write(0xff);
    Cmd[2] = this->pspi->Write(0xff);
    this->pspi->Write(0xff); //����NOP	
    this->pspi->Stop();

    Ret = decodead(Cmd);
	if(this->readCnt>30)
	{
		this->online=true;
	}

    return Ret;
}

void ADS1246::Init(void)
{
	this->pspi->Open();
    this->pspi->Stop();
    this->ppinreset=0;
    delay_ms(40);
    this->ppinreset=1;
    delay_ms(20);
    this->pspi->Start();
    this->WriteReg(ADC_REG_ID, 0x08); //DOUT����DRDY����   0X4A 00 08
    delay_ms(40);
    this->WriteReg(ADC_REG_SYS0, this->speed | ADC_GAIN_1); //���������ٶ�
    this->pspi->Stop();


    //���жϣ�ת������ж�
}
void ADS1246::Reset(void)
{
	this->SetSpeed(this->speed);
}
//����ADת���ٶ�
void  ADS1246::SetSpeed(ADSPEEDTYPE spd)
{
	this->pspi->Start();
    this->WriteReg(ADC_REG_SYS0, spd | ADC_GAIN_1); //���������ٶ�
    this->pspi->Stop();
}

//AD��飬��������0
uint8_t ADS1246::Check(void)
{
    uint8_t ret = 0;
    if (0x08 != this->ReadReg(ADC_REG_ID))
    {
        ret++;
    }
    if ((ADC_SPS_10 | ADC_GAIN_1) != this->ReadReg(ADC_REG_SYS0))
    {
        ret++;
    }
    return ret;
}

//��ȡADת���Ƿ�����
bool ADS1246::GetFlag(void)
{
	bool ret=this->flagOK;
	this->flagOK=false;
	return ret;
}
//����ٶ�,����������
int  ADS1246::CheckSpeed(int checkTimems)
{
	checkTimems=checkTimems;
	int ret=this->readCntCheck;
	this->readCntCheck=0;
	return ret;
}
//����
bool ADS1246::Online()
{
	return this->online;
}
