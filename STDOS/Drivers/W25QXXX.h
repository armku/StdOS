#ifndef _W25QXXX_H
#define _W25QXXX_H

	#include "Kernel\Sys.h"
	#include "Device\Spi.h"

// W25Q64�洢Flash
class W25Q64
{
private:
    Spi* _spi;  // �ڲ�Spi����

    // ���ò�����ַ
    void SetAddr(uint addr);
    // �ȴ��������
	bool WaitForEnd();

    // ��ȡ���
	uint ReadID();
public:
    uint ID;            // оƬID
    ushort PageSize;    // ҳ��С
    ushort Retry;       // �ȴ����Դ���

    W25Q64(Spi* spi);
    ~W25Q64();

    // ��������
    bool Erase(uint sector);
    // ����ҳ
	bool ErasePage(uint pageAddr);

    // д��һҳ
	bool WritePage(uint addr, byte* buf, uint count);
    // ��ȡһҳ
	bool ReadPage(uint addr, byte* buf, uint count);
    // д������
	bool Write(uint addr, byte* buf, uint count);
    // ��ȡ����
	bool Read(uint addr, byte* buf, uint count);
};

/*
AT45DB041�е����ݰ�ҳ���,���湲2048ҳ,ÿҳ 264�ֽ�,����������Ϊ528K�ֽڣ�Լ4M���أ�������������е����ݵ��粻��ʧ��
������������,AT45DB041������������Ϊ264�ֽڵ����ݻ��档������������������ⲿ�������ݽ���ʱ�Ļ�������,Ҳ�����ݴ�һЩ��ʱ���ݡ�
�����д����Ѹ��,���������ݻᶪʧ��AT45DB041���ݶ�д���ô��з�ʽ,��д�ٶȿ�,��ҳ������Ĵ���ʱ��Ϊ80 s����,���Ҽ���CMOS��TTL����������

���ݶ�ȡ 
�����棺ͨ����ͬ�Ĳ������ѡ���ȡ���������е�����һ����������54H����������1,��������56H����������2����
Ϊ��ʵ�ֶ�����Ĳ���,��8λ�������������15λ������,9λ��ַ���8λ�����롣����9λ��ַ�루BFA8-BFA0��������ָ��Ҫ��ȡ�����ֽڡ�
���ֻ�ʱ��SCK�Ŀ�����,��SI��������롢��ַ�롢�����롣�����������,��SCK�ֻ�ʱ�ӵĿ�����,�����е����ݾͿ��Դ�SO������
�����������ĩβʱ,�豸�������ӻ���Ŀ����ض�����������������/CS���뱣�ֵ͵�ƽ�������������,/CS��һ���ӵ͵��ߵĵ�ƽ���佫��ֹ��������
����ҳ������Ĵ��ͣ�һҳ���ݿ��Դ����洫������1��2��
8λ�����루53H���ڻ���1,55H���ڻ���2�������4λ����λ,11λ����ָ��Ŀ��ҳ�ĵ�ַλ��PA10-PA0��,��9λ������,���ֻ�ʱ��SCK�Ŀ����´�SI���롣
����һ������/CS�ܽű��뱣�ֵ͵�λ�������������,��/CS�ϼ�⵽һ���ɵ͵��ߵĵ�ƽ�����,����ҳ�е����ݿ�ʼ���������档
�����ݴ��͵Ĺ�����,״̬�Ĵ�����ָʾ״̬æ�� 

����д�� 
д���棺��SCK�Ŀ�����,8λ�Ĳ����루84H���ڻ���1,87H���ڻ���2���������15λ�������9λ��ַ�루BFA8-BFA0����SI����,
����9λ��ַ��ָ���˻����б�д�����ֽ�,���ݽ����ŵ�ַ�����롣/CS��һ���ɵ͵��ߵ�����,������д���������
�������ò����Ļ���������ҳ�Ĵ��ͣ�8λ�Ĳ����루83H���ڻ��� 1,86H���ڻ���2�������4λ����λ,11λָ��Ҫд�������ҳ�ĵ�ַ�루PA10-PA0��,
��9λ������,���ֻ�ʱ��SCK�Ŀ����´�SI���롣��/CS�ϼ�⵽һ���ɵ͵��ߵ�����ʱ,���������Ȳ�����ѡ�е�����ҳ,Ȼ�󽫴����ڻ����е�����д���ҳ��
������д��Ĺ��̶����Զ����е�,��һ���̲�����20ms�������ʱ����,״̬�Ĵ�����ָʾ״̬æ��״̬�Ĵ���ֵ��ȡ:��״̬�Ĵ�����ֵ��������ȷ���豸��æ��״̬��
æ��״̬ͨ���Ĵ����ĵ�7λָʾ�������7λΪ1,���豸λ�ڿ���״̬���ɽ�����һ��ָ����Ϊ0,���豸æ��������8λ������57H��,��SO�Ͽ��Զ���1�ֽڵ�״̬�Ĵ�����ֵ��
*/
    
    class W25QXXX
    {
        public:
            W25QXXX(Spi *spi, Pin pincs = P0);
            void Write(uint addr, byte *pBuffer, int size);
            void Read(uint addr, byte *pBuffer, int size);
        public:
            void Init(void);
            void SectorErase(uint SectorAddr);
            void BulkErase(void);
            void WritePage(uint addr, byte *pBuffer, int size);
            uint ReadID(void);
            uint ReadDeviceID(void);
            void StartReadSequence(uint addr);
            void PowerDown(void);
            void WAKEUP(void);

            void WriteEnable(void);
            void WaitForWriteEnd(void);
        private:
            Spi *pSpi;
            OutputPort *pcs; //Ƭѡ��
    };
#endif
