#ifndef __WaitHandle_H__
#define __WaitHandle_H__

// �ȴ����
class WaitHandle
{
public:
	uint	TaskID;	// �����������
	void*	State;	// �û�����
	bool	Result;	// ���

	WaitHandle();
	
	bool WaitOne(int ms);	// �ȴ�һ��
	
	//void Reset();
	void Set();	// ���ý��
	
private:
};

#endif
