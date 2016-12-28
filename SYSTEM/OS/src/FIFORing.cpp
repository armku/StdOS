/*
�ο���http://www.amobbs.com/thread-5604139-1-1.html

*/
#include "FIFORing.h"


CFIFORing::CFIFORing()
{
    this->Reset();
}
CFIFORing::~CFIFORing()
{

}

//������д�뻺����
//����������ʱ����λ flagFull ��־����ʾ��������
//����������������������д�����ݣ�������д������ݱ����
//data: д�뻺����������
//����ֵ�̶�Ϊ0
void CFIFORing::Push(byte data)
{
    //writeIndex &= (NMAX - 1);
    this->buffer[writeIndex] = data;
    this->writeIndex++;

    //ȷ��ָ��һֱ����Ч��Χ�ڣ�дָ��Խ��ʱ�����´�0��ʼд����������Խ���־
    if(this->writeIndex == NMAX)
    {
        this->writeIndex = 0;
        this->flagOverflow = 1;
    }

    //FIFO��������д��һ�����ݣ�������д�������Ҫ�����
    //���Զ�ָ��Ҫ������дָ�����
    if(this->flagFull)
    {
        this->readIndex = this->writeIndex;
    }

    //���дָ��Խ�磬�����ָ����ȣ���˵��д��
    if(this->flagOverflow & (this->writeIndex == this->readIndex))
    {
        this->flagFull = 1;
    }
    else
    {
        this->flagFull = 0;
    }

    //д��һ�����ݣ��򻺳�����һ���ǲ��յ�
    this->flagEmpty = 0;

}

//�ӻ�������ȡ��1�ֽ�����
//�������������һ�����ݱ�ȡ����ʱ����λ flagEmpty ��־����ʾ��������
//�������ѿգ��ֽ��ж����ݲ��������������������Ч��
//����ֵΪ����������
byte CFIFORing::Pop()
{
    byte ucData;

    //readIndex &= (NMAX - 1);
    ucData = this->buffer[readIndex];
    this->readIndex++;

    //ȷ��ָ��һֱ����Ч��Χ�ڣ���ָ��Խ��ʱ�����´�0��ʼ�����������Խ���־
    if(this->readIndex == NMAX)
    {
        this->readIndex = 0;
        this->flagOverflow = 0;
    }

    //FIFO�ѿգ��ֶ�һ�����ݣ����������������Ч��
    //����дָ��Ҫ���Ŷ�ָ��仯�������´�д�������λ���ǲ���ȷ��
    if(this->flagEmpty)
    {
        this->writeIndex = this->readIndex;
    }

    //��Խ���־Ϊ0���Ҷ�дָ����ȣ���˵���Ѿ�������
    if((!this->flagOverflow) & (this->writeIndex == this->readIndex))
    {
        this->flagEmpty = 1;
    }
    else
    {
        this->flagEmpty = 0;
    }

    //����һ�����ݣ��򻺳�����һ���ǲ�����
    this->flagFull = 0;

    return ucData;
}
ushort CFIFORing::Pop(byte *buf,ushort pos,ushort len)//����ָ�����ȵ�����
{
	ushort readlen=0;
	
	if(len>this->GetLength())
	{
		readlen=GetLength();		
	}
	else
	{
		readlen=len;
	}
	for(int i=0;i<readlen;i++)
	{
		buf[pos+i]=this->Pop();
	}

	return readlen;	
}

//��ȡ�����������ݸ���
ushort CFIFORing::GetLength()
{
    ushort ucDataNum;

    if(this->flagOverflow)
    {
        ucDataNum = NMAX + this->writeIndex - this->readIndex;
    }
    else
    {
        ucDataNum = this->writeIndex - this->readIndex;
    }

    return 	ucDataNum;
}
void CFIFORing::Reset()//��λ
{
	this->writeIndex = 0;    //д������������ǰ����д���λ��
    this->readIndex = 0;	//��������������ǰ�������ݵ�λ��
    this->flagOverflow = 0;   //Խ���־
    this->flagFull = 0;		//����������־
    this->flagEmpty = 1;		//�������ձ�־
}
