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

	Port();
	Port& SetPin(Pin pin);	// ��������
	void SetPinMode(GPIOMode_T mode);//���ùܽ�ģʽ

	bool ReadInput() const;// Read/ReadInput ���������ڣ�ǰ�߶�������߶����룬�ڿ�©�����ʱ���кܴ�����
	static void Write(Pin pin, bool value);
	void Write(bool value) const;
	Port& operator=(bool value) { Write(value); return *this; }
	Port& operator=(Port& port) { Write(port.Read()); return *this; }
	
	bool Empty() const;

	bool Open();
	void Close();

	virtual bool Read() const;
	operator bool() const { return Read(); }
};

#endif //_Port_H_
