#include "AT24CXX.h"
#include "stdio.h"
#include "Sys.h"

#define macI2C_WR	0		/* д����bit */
#define macI2C_RD	1		/* ������bit */


AT24CXX::AT24CXX(){}
AT24CXX::~AT24CXX(){}

void AT24CXX::Init(){

}

bool AT24CXX::Write(uint addr, const Buffer &bs)const
{
    return false;
}

bool AT24CXX::Read(uint addr, Buffer &bs)const
{
    return false;
}

AT24CXX::AT24CXX(Pin pinsck, Pin pinsda, EW24XXType devtype, byte devaddr, uint wnms)
{
    this->IIC = new SoftI2C();
	this->IIC->SetPin(pinsck,pinsda);
    this->deviceType = devtype;
    this->Address = devaddr;
    this->pageSize = this->jsPageSize(devtype);
    this->writedelaynms = wnms;
}

byte AT24CXX::CheckOk()
{
    if (this->checkDevice() == 0)
    {
        return 1;
    }
    else
    {
        /* ʧ�ܺ��мǷ���I2C����ֹͣ�ź� */
        this->IIC->Stop();
        return 0;
    }
}

byte AT24CXX::checkDevice()
{
    byte ucAck;

    this->IIC->Start(); /* ���������ź� */
    /* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
    this->IIC->WriteByte((this->Address) | macI2C_WR);
    ucAck = this->IIC->WaitAck(); /*����豸��ACKӦ�� */

    this->IIC->Stop(); /* ����ֹͣ�ź� */

    return ucAck;
}

/*
 *********************************************************************************************************
 *	�� �� ��: ee_ReadBytes
 *	����˵��: �Ӵ���EEPROMָ����ַ����ʼ��ȡ��������
 *	��    �Σ�addr : ��ʼ��ַ
 *			 size : ���ݳ��ȣ���λΪ�ֽ�
 *			 pBuffer : ��Ŷ��������ݵĻ�����ָ��
 *	�� �� ֵ: 1 ��ʾʧ�ܣ�0��ʾ�ɹ�
 *********************************************************************************************************
 */
int AT24CXX::Read(uint addr, void *pBuffer, int size, ushort bufpos)
{
    return this->bufwr(addr, (byte*)pBuffer, size, bufpos, 0);
}

/*
 *********************************************************************************************************
 *	�� �� ��: ee_WriteBytes
 *	����˵��: ����EEPROMָ����ַд���������ݣ�����ҳд�������д��Ч��
 *	��    �Σ�addr : ��ʼ��ַ
 *			 size : ���ݳ��ȣ���λΪ�ֽ�
 *			 pBuffer : ��Ŷ��������ݵĻ�����ָ��
 *	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
 *********************************************************************************************************
 */
int AT24CXX::Write(uint addr, void *pBuffer, int size, ushort bufpos)
{
    int i = 0;
    for (i = 0; i < size; i++)
    {
        if (this->Read(addr + i) != ((byte*)pBuffer)[bufpos + i])
        {
            break;
        }
    }
    if (i >= size)
    {
        //�洢������ͬ������Ҫд��
        return size;
    }
    return this->bufwr(addr, (byte*)pBuffer, size, bufpos, 1);
}

byte AT24CXX::Read(ushort address)
{
    byte ret = 0;


    /* ��1��������I2C���������ź� */
    this->IIC->Start();

    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    this->IIC->WriteByte(this->Address | macI2C_WR); /* �˴���дָ�� */

    /* ��3�����ȴ�ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
    }
    if (this->deviceType > AT24C16)
    {
        /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
        this->IIC->WriteByte((byte)((address) >> 8));

        /* ��5�����ȴ�ACK */
        if (this->IIC->WaitAck() != 0)
        {
            goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
        }
    }
    /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
    this->IIC->WriteByte((byte)address);

    /* ��5�����ȴ�ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
    }

    /* ��6������������I2C���ߡ�ǰ��Ĵ����Ŀ����EEPROM���͵�ַ�����濪ʼ��ȡ���� */
    this->IIC->Start();

    /* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    this->IIC->WriteByte(this->Address | macI2C_RD); /* �˴��Ƕ�ָ�� */

    /* ��8��������ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
    }

    /* ��9����ѭ����ȡ���� */

    ret = this->IIC->ReadByte(); /* ��1���ֽ� */


    this->IIC->Ack(false); /* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */


    /* ����I2C����ֹͣ�ź� */
    this->IIC->Stop();
    return ret; /* ִ�гɹ� */

    cmd_Readbytefail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    this->IIC->Stop();

    return ret;
}

bool AT24CXX::Write(ushort address, byte da)
{
    uint m;

    /*���ڣ�������ֹͣ�źţ������ڲ�д������*/
    this->IIC->Stop();

    /* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms 			
    CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
     */
    for (m = 0; m < 1000; m++)
    {
        /* ��1��������I2C���������ź� */
        this->IIC->Start();

        /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
        this->IIC->WriteByte(this->Address | macI2C_WR); /* �˴���дָ�� */

        /* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
        if (this->IIC->WaitAck() == 0)
        {
            break;
        }
    }
    if (m == 1000)
    {
        goto cmd_Writebytefail; /* EEPROM����д��ʱ */
    }
    if (this->deviceType > AT24C16)
    {
        /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
        this->IIC->WriteByte((byte)((address >> 8)));

        /* ��5�����ȴ�ACK */
        if (this->IIC->WaitAck() != 0)
        {
            goto cmd_Writebytefail; /* EEPROM������Ӧ�� */
        }
    }
    /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
    this->IIC->WriteByte((byte)address);

    /* ��5�����ȴ�ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Writebytefail; /* EEPROM������Ӧ�� */
    }



    /* ��6������ʼд������ */
    this->IIC->WriteByte(da);

    /* ��7��������ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Writebytefail; /* EEPROM������Ӧ�� */
    }

    /* ����ִ�гɹ�������I2C����ֹͣ�ź� */
    this->IIC->Stop();
    Sys.Sleep(this->writedelaynms);
    return true;

    cmd_Writebytefail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    this->IIC->Stop();
    return true;
}

int AT24CXX::bufwr(ushort addr, byte *buf, uint size, ushort bufpos, byte wr) //��д���в���1д 0��
{
    uint curAddr;
    uint pageStart; //ҳ����ʼ��ַ
    uint bytesLeave; //��ʣ�����ֽڶ�ȡ
    ushort bufaddr;

    pageStart = addr % this->pageSize;
    bytesLeave = size;
    curAddr = addr;
    bufaddr = bufpos;
    if (this->pinWP)
    {
        *this->pinWP = 0;
    }
    if (pageStart)
    {
        //��ȡ����ҳ��ʼ��ַ������
        //һ���ܶ���
        if ((pageStart + bytesLeave) < this->pageSize)
        {
            if (wr)
            {
                this->writePage(buf, bufaddr, curAddr, bytesLeave);
            }
            else
            {
                this->readPage(buf, bufaddr, curAddr, bytesLeave);
            }
            bytesLeave -= bytesLeave;
            if (wr)
            {
                Sys.Sleep(this->writedelaynms);
            }
            return 0;
        }
        //һ�ζ�ȡ����
        else
        {
            if (wr)
            {
                this->writePage(buf, bufaddr, curAddr, this->pageSize - pageStart);
            }
            else
            {
                this->readPage(buf, bufaddr, curAddr, this->pageSize - pageStart);
            }
            bytesLeave -= (this->pageSize - pageStart);
            curAddr += (this->pageSize - pageStart);
            bufaddr += (this->pageSize - pageStart);
            if (wr)
            {
                Sys.Sleep(this->writedelaynms);
            }
        }
    }

    while (bytesLeave > 0)
    {
        if (bytesLeave > this->pageSize)
        {
            if (wr)
            {
                this->writePage(buf, bufaddr, curAddr, this->pageSize);
            }
            else
            {
                this->readPage(buf, bufaddr, curAddr, this->pageSize);
            }
            bytesLeave -= this->pageSize;
            curAddr += this->pageSize;
            bufaddr += this->pageSize;
            if (wr)
            {
                Sys.Sleep(this->writedelaynms);
            }
        }
        else
        {
            if (wr)
            {
                this->writePage(buf, bufaddr, curAddr, bytesLeave);
            }
            else
            {
                this->readPage(buf, bufaddr, curAddr, bytesLeave);
            }

            curAddr += bytesLeave;
            bufaddr += bytesLeave;
            bytesLeave -= bytesLeave;
            if (wr)
            {
                Sys.Sleep(this->writedelaynms);
            }
        }
    }
    if (this->pinWP)
    {
        *this->pinWP = 1;
    }
    return 0;
}

int AT24CXX::writePage(byte *buf, ushort bufpos, ushort addr, uint size) //ҳ��д
{
    uint i, m;
    ushort usAddr;

    usAddr = addr;

    /*���ڣ�������ֹͣ�źţ������ڲ�д������*/
    this->IIC->Stop();

    /* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms 			
    CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
     */
    for (m = 0; m < 1000; m++)
    {
        /* ��1��������I2C���������ź� */
        this->IIC->Start();

        /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
        this->IIC->WriteByte(this->Address | macI2C_WR); /* �˴���дָ�� */

        /* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
        if (this->IIC->WaitAck() == 0)
        {
            break;
        }
    }
    if (m == 1000)
    {
        goto cmd_Writefail; /* EEPROM����д��ʱ */
    }
    if (this->deviceType > AT24C16)
    {
        /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
        this->IIC->WriteByte((byte)((usAddr >> 8)));

        /* ��5�����ȴ�ACK */
        if (this->IIC->WaitAck() != 0)
        {
            goto cmd_Writefail; /* EEPROM������Ӧ�� */
        }
    }
    /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
    this->IIC->WriteByte((byte)usAddr);

    /* ��5�����ȴ�ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Writefail; /* EEPROM������Ӧ�� */
    }

    for (i = 0; i < size; i++)
    {

        /* ��6������ʼд������ */
        this->IIC->WriteByte(buf[bufpos + i]);

        /* ��7��������ACK */
        if (this->IIC->WaitAck() != 0)
        {
            goto cmd_Writefail; /* EEPROM������Ӧ�� */
        }

        usAddr++; /* ��ַ��1 */
    }

    /* ����ִ�гɹ�������I2C����ֹͣ�ź� */
    this->IIC->Stop();
    return 0;

    cmd_Writefail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    this->IIC->Stop();
    return 1;
}

int AT24CXX::readPage(byte *buf, ushort bufpos, ushort addr, uint size) //ҳ�ڶ�
{
    uint i;

    /* ��1��������I2C���������ź� */
    this->IIC->Start();

    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    this->IIC->WriteByte(this->Address | macI2C_WR); /* �˴���дָ�� */

    /* ��3�����ȴ�ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Readfail; /* EEPROM������Ӧ�� */
    }
    if (this->deviceType > AT24C16)
    {
        /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
        this->IIC->WriteByte((byte)((addr) >> 8));

        /* ��5�����ȴ�ACK */
        if (this->IIC->WaitAck() != 0)
        {
            goto cmd_Readfail; /* EEPROM������Ӧ�� */
        }
    }
    /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
    this->IIC->WriteByte((byte)addr);

    /* ��5�����ȴ�ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Readfail; /* EEPROM������Ӧ�� */
    }

    /* ��6������������I2C���ߡ�ǰ��Ĵ����Ŀ����EEPROM���͵�ַ�����濪ʼ��ȡ���� */
    this->IIC->Start();

    /* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    this->IIC->WriteByte(this->Address | macI2C_RD); /* �˴��Ƕ�ָ�� */

    /* ��8��������ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Readfail; /* EEPROM������Ӧ�� */
    }

    /* ��9����ѭ����ȡ���� */
    for (i = 0; i < size; i++)
    {
        buf[bufpos + i] = IIC->ReadByte(); /* ��1���ֽ� */

        /* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
        if (i != size - 1)
        {
            this->IIC->Ack(true); /* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
        }
        else
        {
            this->IIC->Ack(false); /* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
        }
    }
    /* ����I2C����ֹͣ�ź� */
    this->IIC->Stop();
    return 0; /* ִ�гɹ� */

    cmd_Readfail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    this->IIC->Stop();
    return 1;
}

ushort AT24CXX::jsPageSize(uint type) //����洢ҳ��С
{
    ushort ret = 8;
    switch (type)
    {
        case AT24C02:
        case AT24C04:
            ret = 8;
            break;
        case AT24C08:
        case AT24C16:
            ret = 16;
            break;
        case AT24C32:
        case AT24C64:
            ret = 32;
            break;
        case AT24C128:
        case AT24C256:
            ret = 64;
            break;
        case AT24C512:
        case AT24C1024:
            ret = 128;
            break;
        case AT24C2048:
            ret = 256;
            break;
        default:
            break;
    }
    return ret;
}

#ifdef DEBUG
    /*
     * ��д����
     */
    void AT24CXX::Test()
    {
        uint i;
        const uint testsize = 8;
        const uint testaddress = 0;
        byte write_buf[testsize + testaddress];
        byte read_buf[testsize + testaddress];

        /*-----------------------------------------------------------------------------------*/
        if (this->CheckOk() == 0)
        {
            /* û�м�⵽EEPROM */
            printf("û�м�⵽����EEPROM!\r\n");

            return ;
        }
        /*------------------------------------------------------------------------------------*/
        /* �����Ի����� */
        for (i = 0; i < testsize; i++)
        {
            write_buf[i] = i;
            read_buf[i] = 0;
        }
        printf("ԭʼ����\r\n");
        for (i = 0; i < testsize; i++)
        {
            printf("%02X ", write_buf[i]);

            if ((i &15) == 15)
            {
                printf("\r\n");
            }
        }
        printf("\r\n");
        /*------------------------------------------------------------------------------------*/
        if (this->Write(testaddress, write_buf, testsize, 0))
        {
            printf("дeeprom����\r\n");
            return ;
        }
        else
        {
            printf("дeeprom�ɹ���\r\n");
        }
        /*д��֮����Ҫ�ʵ�����ʱ��ȥ������Ȼ�����*/

        /*-----------------------------------------------------------------------------------*/
        if (this->Read(testaddress, read_buf, testsize, 0))
        {
            printf("��eeprom����\r\n");
            return ;
        }
        else
        {
            printf("��eeprom�ɹ����������£�\r\n");
        }

        /*-----------------------------------------------------------------------------------*/
        for (i = 0; i < testsize; i++)
        {
            if (read_buf[i] != write_buf[i])
            {
                //printf("0X%02X ", read_buf[i]);
                //printf("����:EEPROM������д������ݲ�һ��");
                //return ;
            }
            printf("%02X ", read_buf[i]);

            if ((i &15) == 15)
            {
                printf("\r\n");
            }
        }
        printf("eeprom��д���Գɹ�\r\n");
    }
#endif
#if 1
	#include "stm32f10x.h"
	#define EE_DEV_ADDR			0xA0		/* 24xx02���豸��ַ */
#define EE_PAGE_SIZE		8			  /* 24xx02��ҳ���С */
#define EE_SIZE				256			  /* 24xx02������ */
#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */
/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define GPIO_PORT_I2C	GPIOB			/* GPIO�˿� */
#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define I2C_SCL_PIN		GPIO_Pin_6			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_PIN		GPIO_Pin_7			/* ���ӵ�SDA�����ߵ�GPIO */

/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 0	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
	#define I2C_SCL_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 1 */
	#define I2C_SCL_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 1 */
	#define I2C_SDA_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 0 */
	
	#define I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define I2C_SCL_1()  GPIO_PORT_I2C->BSRR = I2C_SCL_PIN				/* SCL = 1 */
	#define I2C_SCL_0()  GPIO_PORT_I2C->BRR = I2C_SCL_PIN				/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_PORT_I2C->BSRR = I2C_SDA_PIN				/* SDA = 1 */
	#define I2C_SDA_0()  GPIO_PORT_I2C->BRR = I2C_SDA_PIN				/* SDA = 0 */
	
	#define I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#endif

static void i2c_CfgGpio(void);

/*
*********************************************************************************************************
*	�� �� ��: i2c_Delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*��
	 	�����ʱ����ͨ��������AX-Pro�߼������ǲ��Եõ��ġ�
		CPU��Ƶ72MHzʱ�����ڲ�Flash����, MDK���̲��Ż�
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
        
    IAR���̱���Ч�ʸߣ���������Ϊ7
	*/
	for (i = 0; i < 10; i++);
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C���������ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		i2c_Delay();
		I2C_SCL_1();
		i2c_Delay();	
		I2C_SCL_0();
		if (i == 7)
		{
			 I2C_SDA_1(); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		i2c_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_0();
		i2c_Delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0();
	i2c_Delay();
	return re;
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	I2C_SDA_0();	/* CPU����SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	I2C_SDA_1();	/* CPU����SDA = 1 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();	
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_CfgGpio
*	����˵��: ����I2C���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_CfgGpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_I2C_PORT, ENABLE);	/* ��GPIOʱ�� */

	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* ��©��� */
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);

	/* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
	i2c_Stop();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*********************************************************************************************************
*/
uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	i2c_CfgGpio();		/* ����GPIO */

	
	i2c_Start();		/* ���������ź� */

	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte(_Address | I2C_WR);
	ucAck = i2c_WaitAck();	/* ����豸��ACKӦ�� */

	i2c_Stop();			/* ����ֹͣ�ź� */

	return ucAck;
}
/*
*********************************************************************************************************
*	�� �� ��: ee_CheckOk
*	����˵��: �жϴ���EERPOM�Ƿ�����
*	��    �Σ���
*	�� �� ֵ: 1 ��ʾ������ 0 ��ʾ������
*********************************************************************************************************
*/
uint8_t ee_CheckOk(void)
{
	if (i2c_CheckDevice(EE_DEV_ADDR) == 0)
	{
		return 1;
	}
	else
	{
		/* ʧ�ܺ��мǷ���I2C����ֹͣ�ź� */
		i2c_Stop();		
		return 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: ee_ReadBytes
*	����˵��: �Ӵ���EEPROMָ����ַ����ʼ��ȡ��������
*	��    �Σ�_usAddress : ��ʼ��ַ
*			 _usSize : ���ݳ��ȣ���λΪ�ֽ�
*			 _pReadBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;
	
	/* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */
	
	/* ��1��������I2C���������ź� */
	i2c_Start();
	
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(EE_DEV_ADDR | I2C_WR);	/* �˴���дָ�� */
	
	/* ��3��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}

	/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
	i2c_SendByte((uint8_t)_usAddress);
	
	/* ��5��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
	/* ��6������������I2C���ߡ�ǰ��Ĵ����Ŀ����EEPROM���͵�ַ�����濪ʼ��ȡ���� */
	i2c_Start();
	
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(EE_DEV_ADDR | I2C_RD);	/* �˴��Ƕ�ָ�� */
	
	/* ��8��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}	
	
	/* ��9����ѭ����ȡ���� */
	for (i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = i2c_ReadByte();	/* ��1���ֽ� */
		
		/* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
		if (i != _usSize - 1)
		{
			i2c_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
		}
		else
		{
			i2c_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
		}
	}
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 1;	/* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: ee_WriteBytes
*	����˵��: ����EEPROMָ����ַд���������ݣ�����ҳд�������д��Ч��
*	��    �Σ�_usAddress : ��ʼ��ַ
*			 _usSize : ���ݳ��ȣ���λΪ�ֽ�
*			 _pWriteBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;
	
	/* 
		д����EEPROM�������������������ȡ�ܶ��ֽڣ�ÿ��д����ֻ����ͬһ��page��
		����24xx02��page size = 8
		�򵥵Ĵ�����Ϊ�����ֽ�д����ģʽ��ûд1���ֽڣ������͵�ַ
		Ϊ���������д��Ч��: ����������page wirte������
	*/

	usAddr = _usAddress;	
	for (i = 0; i < _usSize; i++)
	{
		/* �����͵�1���ֽڻ���ҳ���׵�ַʱ����Ҫ���·��������źź͵�ַ */
		if ((i == 0) || (usAddr & (EE_PAGE_SIZE - 1)) == 0)
		{
			/*���ڣ�������ֹͣ�źţ������ڲ�д������*/
			i2c_Stop();
			
			/* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms 			
				CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
			*/
			for (m = 0; m < 100; m++)
			{				
				/* ��1��������I2C���������ź� */
				i2c_Start();
				
				/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
				i2c_SendByte(EE_DEV_ADDR | I2C_WR);	/* �˴���дָ�� */
				
				/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
				if (i2c_WaitAck() == 0)
				{
					break;
				}
			}
			if (m  == 1000)
			{
				goto cmd_fail;	/* EEPROM����д��ʱ */
			}
		
			/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
			i2c_SendByte((uint8_t)usAddr);
			
			/* ��5��������ACK */
			if (i2c_WaitAck() != 0)
			{
				goto cmd_fail;	/* EEPROM������Ӧ�� */
			}
		}
	
		/* ��6������ʼд������ */
		i2c_SendByte(_pWriteBuf[i]);
	
		/* ��7��������ACK */
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* EEPROM������Ӧ�� */
		}

		usAddr++;	/* ��ַ��1 */		
	}
	
	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
	i2c_Stop();
	return 1;

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 0;
}

void ee_Erase(void)
{
	uint16_t i;
	uint8_t buf[EE_SIZE];
	
	/* ��仺���� */
	for (i = 0; i < EE_SIZE; i++)
	{
		buf[i] = 0xFF;
	}
	
	/* дEEPROM, ��ʼ��ַ = 0�����ݳ���Ϊ 256 */
	if (ee_WriteBytes(buf, 0, EE_SIZE) == 0)
	{
		printf("����eeprom����\r\n");
		return;
	}
	else
	{
		printf("����eeprom�ɹ���\r\n");
	}
}


/*--------------------------------------------------------------------------------------------------*/

static void ee_Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
void AT24C02Test()
{
//	AT24CXX at2402(PB6,PB7,AT24C02);
//	i2c_CfgGpio();
//	at2402.Init();	
//	printf("\r\n");
//	at2402.Test();
	uint16_t i;
	uint8_t write_buf[EE_SIZE];
  uint8_t read_buf[EE_SIZE];
  
/*-----------------------------------------------------------------------------------*/  
  if (ee_CheckOk() == 0)
	{
		/* û�м�⵽EEPROM */
		printf("û�м�⵽����EEPROM!\r\n");
				
		while (1);	/* ͣ�� */
	}
/*------------------------------------------------------------------------------------*/  
  /* �����Ի����� */
	for (i = 0; i < EE_SIZE; i++)
	{		
		write_buf[i] = i;
	}
/*------------------------------------------------------------------------------------*/  
  if (ee_WriteBytes(write_buf, 0, EE_SIZE) == 0)
	{
		printf("дeeprom����\r\n");
		return;
	}
	else
	{		
		printf("дeeprom�ɹ���\r\n");
	}
  
  /*д��֮����Ҫ�ʵ�����ʱ��ȥ������Ȼ�����*/
  ee_Delay(0x0FFFFF);
	//Sys.Sleep(999);
/*-----------------------------------------------------------------------------------*/
  if (ee_ReadBytes(read_buf, 0, EE_SIZE) == 0)
	{
		printf("��eeprom����\r\n");
		return;
	}
	else
	{		
		printf("��eeprom�ɹ����������£�\r\n");
	}
/*-----------------------------------------------------------------------------------*/  
  for (i = 0; i < EE_SIZE; i++)
	{
		if(read_buf[i] != write_buf[i])
		{
			printf("0x%02X ", read_buf[i]);
			printf("����:EEPROM������д������ݲ�һ��");
			return;
		}
    printf(" %02X", read_buf[i]);
		
		if ((i & 15) == 15)
		{
			printf("\r\n");	
		}		
	}
  printf("eeprom��д���Գɹ�\r\n");
}
#endif
