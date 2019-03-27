#ifndef _Port_H_
#define _Port_H_

#include "Core\Type.h"
#include "Platform\Pin.h"

// �˿ڻ���
class Port
{
public:	
    Pin		_Pin;		// ����  
	bool	Opened;		// �Ƿ��Ѿ���

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
// �����
//class Port : public Port
//{
//public:  
//    
//};

#endif //_Port_H_
