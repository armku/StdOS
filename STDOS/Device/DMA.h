#ifndef __DMA_H__
#define __DMA_H__

// DMA
class DMA
{
private:
	byte _index;	// �ڼ�����ʱ������0��ʼ
	bool _started;

public:
	DMA(byte index);
	~DMA();

    int Retry;  // �ȴ����Դ�����Ĭ��200
    int Error;  // �������

	bool Start();	// ��ʼ
	bool WaitForStop();	// ֹͣ

	//typedef void (*TimerHandler)(Timer* tim, void* param);
	//void Register(TimerHandler handler, void* param = nullptr);

private:
	//static void OnHandler(ushort num, void* param);
	//TimerHandler _Handler;
	//void* _Param;
};

#endif
