#include "AT24CXX.h"

#define macI2C_WR	0		/* д����bit */
#define macI2C_RD	1		/* ������bit */


AT24CXX::AT24CXX(){}
AT24CXX::~AT24CXX(){}

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
    this->IIC.SetPin(pinsck, pinsda);
    this->deviceType = devtype;
    this->Address = devaddr;
    this->pageSize = this->jsPageSize(devtype);
    this->writedelaynms = wnms;
}

/*
 *********************************************************************************************************
 *	�� �� ��: Read
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
 *	�� �� ��: Write
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
    this->IIC.Start();

    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    this->IIC.WriteByte(this->Address | macI2C_WR); /* �˴���дָ�� */

    /* ��3�����ȴ�ACK */
    if (this->IIC.WaitAck() != 0)
    {
        goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
    }
    if (this->deviceType > AT24C16)
    {
        /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
        this->IIC.WriteByte((byte)((address) >> 8));

        /* ��5�����ȴ�ACK */
        if (this->IIC.WaitAck() != 0)
        {
            goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
        }
    }
    /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
    this->IIC.WriteByte((byte)address);

    /* ��5�����ȴ�ACK */
    if (this->IIC.WaitAck() != 0)
    {
        goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
    }

    /* ��6������������I2C���ߡ�ǰ��Ĵ����Ŀ����EEPROM���͵�ַ�����濪ʼ��ȡ���� */
    this->IIC.Start();

    /* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    this->IIC.WriteByte(this->Address | macI2C_RD); /* �˴��Ƕ�ָ�� */

    /* ��8��������ACK */
    if (this->IIC.WaitAck() != 0)
    {
        goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
    }

    /* ��9����ѭ����ȡ���� */

    ret = this->IIC.ReadByte(); /* ��1���ֽ� */


    this->IIC.Ack(false); /* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */


    /* ����I2C����ֹͣ�ź� */
    this->IIC.Stop();
    return ret; /* ִ�гɹ� */

    cmd_Readbytefail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    this->IIC.Stop();

    return ret;
}

bool AT24CXX::Write(ushort address, byte da)
{
    uint m;

    /*���ڣ�������ֹͣ�źţ������ڲ�д������*/
    this->IIC.Stop();

    /* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms 			
    CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
     */
    for (m = 0; m < 1000; m++)
    {
        /* ��1��������I2C���������ź� */
        this->IIC.Start();

        /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
        this->IIC.WriteByte(this->Address | macI2C_WR); /* �˴���дָ�� */

        /* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
        if (this->IIC.WaitAck() == 0)
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
        this->IIC.WriteByte((byte)((address >> 8)));

        /* ��5�����ȴ�ACK */
        if (this->IIC.WaitAck() != 0)
        {
            goto cmd_Writebytefail; /* EEPROM������Ӧ�� */
        }
    }
    /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
    this->IIC.WriteByte((byte)address);

    /* ��5�����ȴ�ACK */
    if (this->IIC.WaitAck() != 0)
    {
        goto cmd_Writebytefail; /* EEPROM������Ӧ�� */
    }



    /* ��6������ʼд������ */
    this->IIC.WriteByte(da);

    /* ��7��������ACK */
    if (this->IIC.WaitAck() != 0)
    {
        goto cmd_Writebytefail; /* EEPROM������Ӧ�� */
    }

    /* ����ִ�гɹ�������I2C����ֹͣ�ź� */
    this->IIC.Stop();
    Sys.Sleep(this->writedelaynms);
    return true;

    cmd_Writebytefail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    this->IIC.Stop();
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
    return size;
}

int AT24CXX::writePage(byte *buf, ushort bufpos, ushort addr, uint size) //ҳ��д
{
    uint i, m;
    ushort usAddr;

    usAddr = addr;

    /*���ڣ�������ֹͣ�źţ������ڲ�д������*/
    this->IIC.Stop();

    /* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms 			
    CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
     */
    for (m = 0; m < 1000; m++)
    {
        /* ��1��������I2C���������ź� */
        this->IIC.Start();

        /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
        this->IIC.WriteByte(this->Address | macI2C_WR); /* �˴���дָ�� */

        /* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
        if (this->IIC.WaitAck() == 0)
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
        this->IIC.WriteByte((byte)((usAddr >> 8)));

        /* ��5�����ȴ�ACK */
        if (this->IIC.WaitAck() != 0)
        {
            goto cmd_Writefail; /* EEPROM������Ӧ�� */
        }
    }
    /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
    this->IIC.WriteByte((byte)usAddr);

    /* ��5�����ȴ�ACK */
    if (this->IIC.WaitAck() != 0)
    {
        goto cmd_Writefail; /* EEPROM������Ӧ�� */
    }

    for (i = 0; i < size; i++)
    {

        /* ��6������ʼд������ */
        this->IIC.WriteByte(buf[bufpos + i]);

        /* ��7��������ACK */
        if (this->IIC.WaitAck() != 0)
        {
            goto cmd_Writefail; /* EEPROM������Ӧ�� */
        }

        usAddr++; /* ��ַ��1 */
    }

    /* ����ִ�гɹ�������I2C����ֹͣ�ź� */
    this->IIC.Stop();
    return 0;

    cmd_Writefail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    this->IIC.Stop();
    return 1;
}

int AT24CXX::readPage(byte *buf, ushort bufpos, ushort addr, uint size) //ҳ�ڶ�
{
    uint i;

    /* ��1��������I2C���������ź� */
    this->IIC.Start();

    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    this->IIC.WriteByte(this->Address | macI2C_WR); /* �˴���дָ�� */

    /* ��3�����ȴ�ACK */
    if (this->IIC.WaitAck() != 0)
    {
        goto cmd_Readfail; /* EEPROM������Ӧ�� */
    }
    if (this->deviceType > AT24C16)
    {
        /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
        this->IIC.WriteByte((byte)((addr) >> 8));

        /* ��5�����ȴ�ACK */
        if (this->IIC.WaitAck() != 0)
        {
            goto cmd_Readfail; /* EEPROM������Ӧ�� */
        }
    }
    /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
    this->IIC.WriteByte((byte)addr);

    /* ��5�����ȴ�ACK */
    if (this->IIC.WaitAck() != 0)
    {
        goto cmd_Readfail; /* EEPROM������Ӧ�� */
    }

    /* ��6������������I2C���ߡ�ǰ��Ĵ����Ŀ����EEPROM���͵�ַ�����濪ʼ��ȡ���� */
    this->IIC.Start();

    /* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    this->IIC.WriteByte(this->Address | macI2C_RD); /* �˴��Ƕ�ָ�� */

    /* ��8��������ACK */
    if (this->IIC.WaitAck() != 0)
    {
        goto cmd_Readfail; /* EEPROM������Ӧ�� */
    }

    /* ��9����ѭ����ȡ���� */
    for (i = 0; i < size; i++)
    {
        buf[bufpos + i] = this->IIC.ReadByte(); /* ��1���ֽ� */

        /* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
        if (i != size - 1)
        {
            this->IIC.Ack(true); /* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
        }
        else
        {
            this->IIC.Ack(false); /* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
        }
    }
    /* ����I2C����ֹͣ�ź� */
    this->IIC.Stop();
    return 0; /* ִ�гɹ� */

    cmd_Readfail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    this->IIC.Stop();
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

byte AT24CXX::checkDevice()
{
    byte ucAck;

    this->IIC.Start(); /* ���������ź� */
    /* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
    this->IIC.WriteByte((this->Address) | macI2C_WR);
    ucAck = this->IIC.WaitAck(); /*����豸��ACKӦ�� */

    this->IIC.Stop(); /* ����ֹͣ�ź� */

    return ucAck;
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
        this->IIC.Stop();
        return 0;
    }
}

#if 0
    #define EE_SIZE				256			  /* 24xx02������ */    
       
    void AT24C02Test()
    {
		AT24CXX at2402(PB6, PB7, AT24C02);
        printf("\r\n");

        ushort i;
        byte write_buf[EE_SIZE];
        byte read_buf[EE_SIZE];

        /*-----------------------------------------------------------------------------------*/
        if (at2402.CheckOk() == 0)
        {
            /* û�м�⵽EEPROM */
            printf("û�м�⵽����EEPROM!\r\n");

            while (1)
                ;
            /* ͣ�� */
        }
        /*------------------------------------------------------------------------------------*/
        /* �����Ի����� */
        for (i = 0; i < EE_SIZE; i++)
        {
            write_buf[i] = i;
        }
        /*------------------------------------------------------------------------------------*/
        if (at2402.Write(0,write_buf, EE_SIZE) == 0)
        {
            printf("дeeprom����\r\n");
            return ;
        }
        else
        {
            printf("дeeprom�ɹ���\r\n");
        }

        /*д��֮����Ҫ�ʵ�����ʱ��ȥ������Ȼ�����*/
        Sys.Sleep(10);
        /*-----------------------------------------------------------------------------------*/
        if (at2402.Read(0,read_buf, EE_SIZE) == 0)
        {
            printf("��eeprom����\r\n");
            return ;
        }
        else
        {
            printf("��eeprom�ɹ����������£�\r\n");
        }
        /*-----------------------------------------------------------------------------------*/
        for (i = 0; i < EE_SIZE; i++)
        {
            if (read_buf[i] != write_buf[i])
            {
                printf("0x%02X ", read_buf[i]);
                printf("����:EEPROM������д������ݲ�һ��");
                return ;
            }
            printf(" %02X", read_buf[i]);

            if ((i &15) == 15)
            {
                printf("\r\n");
            }
        }
        printf("eeprom��д���Գɹ�\r\n");
    }
#endif
