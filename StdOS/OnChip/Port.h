#ifndef _Port_H_
#define _Port_H_

#include "Core\Type.h"
#include "Platform\Pin.h"

typedef enum
{
	GPIO_AIN = 0x0,//ģ������ģʽ
	GPIO_IN_FLOATING = 0x04,//��������ģʽ
	GPIO_IPD = 0x28,//��������ģʽ
	GPIO_IPU = 0x48, //��������ģʽ
	GPIO_Out_OD = 0x14,//��©���ģʽ
	GPIO_Out_PP = 0x10,//ͨ���������ģʽ
	GPIO_AF_OD = 0x1C,//���ù��ܿ�©���
	GPIO_AF_PP = 0x18//���ù����������
}GPIOMode_T;

// �˿ڻ���
class Port
{
public:	
    Pin		_Pin;		// ����  
	bool	Opened;		// �Ƿ��Ѿ���

	void SetPinMode(GPIOMode_T mode);//���ùܽ�ģʽ

	//InputPort
	typedef enum
	{
		NOPULL = 0x00,
		UP = 0x01,	// ��������
		DOWN = 0x02,	// ��������
	}PuPd;
	bool	Floating;	// �Ƿ񸡿�����
	PuPd	Pull;	// ������������
	// END InputPort
	//Port
	bool OpenDrain;	// �Ƿ�©���
	//end Port
	//Port
	/*operator bool() const { return Read(); }*/
	bool ReadInput() const;// Read/ReadInput ���������ڣ�ǰ�߶�������߶����룬�ڿ�©�����ʱ���кܴ�����
	static void Write(Pin pin, bool value);
	void Write(bool value) const;
	Port& operator=(bool value) { Write(value); return *this; }
	Port& operator=(Port& port) { Write(port.Read()); return *this; }
	//end Port
	virtual void OpenPinAlternatePort(void* param);

	Port();

    Port& Set(Pin pin);	// ��������
	bool Empty() const;

	bool Open();
	void Close();

	virtual bool Read() const;
	operator bool() const { return Read(); }	
protected:
    // ���ù���
    virtual void OnOpen(void* param);
	virtual void OnOpenInputPort(void* param);
	virtual void OnOpenOutputPort(void* param);
	virtual void OpenPinOutputPort(void* param);
};

#endif //_Port_H_
