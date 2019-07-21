#ifndef _Port_H_
#define _Port_H_

#include "Kernel\Sys.h"

/******** �˿ڴ򿪹ر����� ********/
/*
Port::Open
	#Port::Opening
		OutputPort::OnOpen
			#Port::OnOpen
		#OutputPort::OpenPin

Port::Close
	#Port::OnClose
*/


/******************************** Port ********************************/

// �˿ڻ���
// ���ڹ���һ���˿ڣ�ͨ��PinBit��ʶ�������Щ���š�
// �����ʼ��ʱ��ͨ��SetPort���ö˿ڣ���������״̬��Ȼ��Configͨ��gpio�ṹ�����ö˿ڣ��˿�����ʱ�ָ�����״̬
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

// �˿���
class Port
{
public:
	enum GPIO_AF
	{
		AF_0 = 0,
		AF_1 = 1,
		AF_2 = 2,
		AF_3 = 3,
		AF_4 = 4,
		AF_5 = 5,
		AF_6 = 6,
		AF_7 = 7
	};

    Pin		_Pin;		// ����
	bool	Opened;		// �Ƿ��Ѿ���
	byte    Index;		// ������������ţ��������ֶ����Ŵ���
	void*	State;		// �û�״̬����

	Port();
#ifndef TINY
	//virtual ~Port();
#endif

    Port& Set(Pin pin);	// ��������
	bool Empty() const;

	bool Open();
	void Close();
	void Clear();

	void AFConfig(GPIO_AF GPIO_AF) const;
	static void RemapConfig(uint param, bool sta);

    virtual bool Read() const;

	String ToString() const;

protected:
    // ���ù���
    //virtual void OnOpen(void* param);
	//virtual void OnClose();

private:
	void Opening();












public:
	virtual Port& SetPin(Pin pin);	// ��������
	void pinMode(GPIOMode_T mode);//���ùܽ�ģʽ

	bool ReadInput() const;// Read/ReadInput ���������ڣ�ǰ�߶�������߶����룬�ڿ�©�����ʱ���кܴ�����
	void Write(bool value) const;
	Port& operator=(bool value) { Write(value); return *this; }
	Port& operator=(Port& port) { Write(port.Read()); return *this; }
	
	operator bool() const { return Read(); }
public:
	int Invert;//�Ƿ���
};

#endif //_Port_H_
