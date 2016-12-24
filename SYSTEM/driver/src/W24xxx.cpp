#include "stdio.h"
#include "W24xxx.h"
#include "delay.h"

#define macI2C_WR	0		/* д����bit */
#define macI2C_RD	1		/* ������bit */

CW24xxx::CW24xxx(PinPort pinsck, PinPort pinsda, EW24XXType devtype, uint8_t devaddr, uint32_t wnms)
{
    this->pi2c = new CI2CSoft(pinsck, pinsda);
    this->deviceType = devtype;
    this->devAddr = devaddr;
    this->pageSize = this->jsPageSize(devtype);
    this->writedelaynms = wnms;
}

CW24xxx::~CW24xxx(){

}
uint8_t CW24xxx::CheckOk()
{
    if (this->checkDevice() == 0)
    {
        return 1;
    }
    else
    {
        /* ʧ�ܺ��мǷ���I2C����ֹͣ�ź� */
        this->pi2c->Stop();
        return 0;
    }
}

uint8_t CW24xxx::checkDevice()
{
    uint8_t ucAck;

    this->pi2c->Start(); /* ���������ź� */
    /* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
    this->pi2c->WriteByte((this->devAddr) | macI2C_WR);
    ucAck = this->pi2c->WaitAck(); /*����豸��ACKӦ�� */

    this->pi2c->Stop(); /* ����ֹͣ�ź� */

    return ucAck;
}

/*
 *********************************************************************************************************
 *	�� �� ��: ee_ReadBytes
 *	����˵��: �Ӵ���EEPROMָ����ַ����ʼ��ȡ��������
 *	��    �Σ�_usAddress : ��ʼ��ַ
 *			 _usSize : ���ݳ��ȣ���λΪ�ֽ�
 *			 _pReadBuf : ��Ŷ��������ݵĻ�����ָ��
 *	�� �� ֵ: 1 ��ʾʧ�ܣ�0��ʾ�ɹ�
 *********************************************************************************************************
 */
uint8_t CW24xxx::ReadBytes(uint8_t *_pReadBuf, uint16_t bufpos, uint16_t _usAddress, uint32_t _usSize)
{
    return this->bufwr(_pReadBuf, bufpos, _usAddress, _usSize, 0);
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
uint8_t CW24xxx::WriteBytes(uint8_t *_pWriteBuf, uint16_t bufpos, uint16_t _usAddress, uint32_t _usSize)
{
    return this->bufwr(_pWriteBuf, bufpos, _usAddress, _usSize, 1);
}

uint8_t CW24xxx::ReadByte(uint16_t address)
{
    uint8_t ret = 0;


    /* ��1��������I2C���������ź� */
    this->pi2c->Start();

    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    this->pi2c->WriteByte(this->devAddr | macI2C_WR); /* �˴���дָ�� */

    /* ��3�����ȴ�ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
    }
    if (this->deviceType > AT24C16)
    {
        /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
        this->pi2c->WriteByte((uint8_t)((address) >> 8));

        /* ��5�����ȴ�ACK */
        if (this->pi2c->WaitAck() != 0)
        {
            goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
        }
    }
    /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
    this->pi2c->WriteByte((uint8_t)address);

    /* ��5�����ȴ�ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
    }

    /* ��6������������I2C���ߡ�ǰ��Ĵ����Ŀ����EEPROM���͵�ַ�����濪ʼ��ȡ���� */
    this->pi2c->Start();

    /* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    this->pi2c->WriteByte(this->devAddr | macI2C_RD); /* �˴��Ƕ�ָ�� */

    /* ��8��������ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
    }

    /* ��9����ѭ����ȡ���� */

    ret = this->pi2c->ReadByte(); /* ��1���ֽ� */


    this->pi2c->NAck(); /* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */


    /* ����I2C����ֹͣ�ź� */
    this->pi2c->Stop();
    return ret; /* ִ�гɹ� */

    cmd_Readbytefail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    this->pi2c->Stop();

    return ret;
}

uint8_t CW24xxx::WriteByte(uint16_t address, uint8_t da)
{
    uint32_t m;

    /*���ڣ�������ֹͣ�źţ������ڲ�д������*/
    this->pi2c->Stop();

    /* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms 			
    CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
     */
    for (m = 0; m < 1000; m++)
    {
        /* ��1��������I2C���������ź� */
        this->pi2c->Start();

        /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
        this->pi2c->WriteByte(this->devAddr | macI2C_WR); /* �˴���дָ�� */

        /* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
        if (this->pi2c->WaitAck() == 0)
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
        this->pi2c->WriteByte((uint8_t)((address >> 8)));

        /* ��5�����ȴ�ACK */
        if (this->pi2c->WaitAck() != 0)
        {
            goto cmd_Writebytefail; /* EEPROM������Ӧ�� */
        }
    }
    /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
    this->pi2c->WriteByte((uint8_t)address);

    /* ��5�����ȴ�ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Writebytefail; /* EEPROM������Ӧ�� */
    }



    /* ��6������ʼд������ */
    this->pi2c->WriteByte(da);

    /* ��7��������ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Writebytefail; /* EEPROM������Ӧ�� */
    }

    /* ����ִ�гɹ�������I2C����ֹͣ�ź� */
    this->pi2c->Stop();
    delay_ms(this->writedelaynms);
    return 0;

    cmd_Writebytefail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    this->pi2c->Stop();
    return 0;
}

uint8_t CW24xxx::bufwr(uint8_t *buf, uint16_t bufpos, uint16_t addr, uint32_t len, uint8_t wr) //��д���в���1д 0��
{
    uint32_t curAddr;
    uint32_t pageStart; //ҳ����ʼ��ַ
    uint32_t bytesLeave; //��ʣ�����ֽڶ�ȡ
    uint16_t bufaddr;

    pageStart = addr % this->pageSize;
    bytesLeave = len;
    curAddr = addr;
    bufaddr = bufpos;

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
                delay_ms(this->writedelaynms);
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
                delay_ms(this->writedelaynms);
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
                delay_ms(this->writedelaynms);
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
                delay_ms(this->writedelaynms);
            }
        }
    }
    return 0;
}

uint8_t CW24xxx::writePage(uint8_t *buf, uint16_t bufpos, uint16_t addr, uint32_t len) //ҳ��д
{
    uint32_t i, m;
    uint16_t usAddr;

    usAddr = addr;

    /*���ڣ�������ֹͣ�źţ������ڲ�д������*/
    this->pi2c->Stop();

    /* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms 			
    CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
     */
    for (m = 0; m < 1000; m++)
    {
        /* ��1��������I2C���������ź� */
        this->pi2c->Start();

        /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
        this->pi2c->WriteByte(this->devAddr | macI2C_WR); /* �˴���дָ�� */

        /* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
        if (this->pi2c->WaitAck() == 0)
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
        this->pi2c->WriteByte((uint8_t)((usAddr >> 8)));

        /* ��5�����ȴ�ACK */
        if (this->pi2c->WaitAck() != 0)
        {
            goto cmd_Writefail; /* EEPROM������Ӧ�� */
        }
    }
    /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
    this->pi2c->WriteByte((uint8_t)usAddr);

    /* ��5�����ȴ�ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Writefail; /* EEPROM������Ӧ�� */
    }

    for (i = 0; i < len; i++)
    {

        /* ��6������ʼд������ */
        this->pi2c->WriteByte(buf[bufpos + i]);

        /* ��7��������ACK */
        if (this->pi2c->WaitAck() != 0)
        {
            goto cmd_Writefail; /* EEPROM������Ӧ�� */
        }

        usAddr++; /* ��ַ��1 */
    }

    /* ����ִ�гɹ�������I2C����ֹͣ�ź� */
    this->pi2c->Stop();
    return 0;

    cmd_Writefail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    this->pi2c->Stop();
    return 1;
}

uint8_t CW24xxx::readPage(uint8_t *buf, uint16_t bufpos, uint16_t addr, uint32_t len) //ҳ�ڶ�
{
    uint32_t i;

    /* ��1��������I2C���������ź� */
    this->pi2c->Start();

    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    this->pi2c->WriteByte(this->devAddr | macI2C_WR); /* �˴���дָ�� */

    /* ��3�����ȴ�ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Readfail; /* EEPROM������Ӧ�� */
    }
    if (this->deviceType > AT24C16)
    {
        /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
        this->pi2c->WriteByte((uint8_t)((addr) >> 8));

        /* ��5�����ȴ�ACK */
        if (this->pi2c->WaitAck() != 0)
        {
            goto cmd_Readfail; /* EEPROM������Ӧ�� */
        }
    }
    /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
    this->pi2c->WriteByte((uint8_t)addr);

    /* ��5�����ȴ�ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Readfail; /* EEPROM������Ӧ�� */
    }

    /* ��6������������I2C���ߡ�ǰ��Ĵ����Ŀ����EEPROM���͵�ַ�����濪ʼ��ȡ���� */
    this->pi2c->Start();

    /* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    this->pi2c->WriteByte(this->devAddr | macI2C_RD); /* �˴��Ƕ�ָ�� */

    /* ��8��������ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Readfail; /* EEPROM������Ӧ�� */
    }

    /* ��9����ѭ����ȡ���� */
    for (i = 0; i < len; i++)
    {
        buf[bufpos + i] = pi2c->ReadByte(); /* ��1���ֽ� */

        /* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
        if (i != len - 1)
        {
            this->pi2c->Ack(); /* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
        }
        else
        {
            this->pi2c->NAck(); /* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
        }
    }
    /* ����I2C����ֹͣ�ź� */
    this->pi2c->Stop();
    return 0; /* ִ�гɹ� */

    cmd_Readfail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    this->pi2c->Stop();
    return 1;
}

uint16_t CW24xxx::jsPageSize(uint32_t type) //����洢ҳ��С
{
    uint16_t ret = 8;
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

#if EEPTEST
    /*
     * ��д����
     */
    void CW24xxx::Test(void)
    {
        uint32_t i;
        const uint32_t testsize = 8;
        const uint32_t testaddress = 0;
        uint8_t write_buf[testsize + testaddress];
        uint8_t read_buf[testsize + testaddress];

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
        if (this->WriteBytes(write_buf, 0, testaddress, testsize))
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
        if (this->ReadBytes(read_buf, 0, testaddress, testsize))
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
