#ifndef _Port_H_
#define _Port_H_

#include "Core\Type.h"
#include "Platform\Pin.h"
#include "port/std_port_gpio.h"

// �˿ڻ���
class Port:public mcuGpio
{
public:	
    Port();
    Port& Set(Pin pin);	// ��������
	bool Open();
	virtual bool Read() const;
protected:
    // ���ù���
    virtual void OnOpen(void* param);
};
// �����
class OutputPort : public Port
{
public:
    uint8_t Invert		= 2;		// �Ƿ������������Ĭ��2��ʾ�Զ����
    bool OpenDrain	= false;	// �Ƿ�©���

    OutputPort();
    OutputPort(Pin pin, uint8_t invert, bool openDrain = false);
	
    void Write(bool value) const;
	// Read/ReadInput ���������ڣ�ǰ�߶�������߶����룬�ڿ�©�����ʱ���кܴ�����
    virtual bool Read() const;
	bool ReadInput() const;

    static void Write(Pin pin, bool value);

    OutputPort& operator=(bool value) { Write(value); return *this; }
    OutputPort& operator=(OutputPort& port) { Write(port.Read()); return *this; }
    operator bool() const { return Read(); }

protected:
    virtual void OnOpen(void* param);
	virtual void OpenPin(void* param);
};
// ���������
class AlternatePort : public OutputPort
{
public:
	AlternatePort();
protected:
    virtual void OpenPin(void* param);
};
// �����
class InputPort : public Port
{
public:
    typedef enum
    {
        NOPULL	= 0x00,
        UP		= 0x01,	// ��������
        DOWN	= 0x02,	// ��������
    }PuPd;
	    
    uint8_t	Invert		= 2;	// �Ƿ������������Ĭ��2��ʾ�Զ����
    bool	Floating	= true;	// �Ƿ񸡿�����
    PuPd	Pull		= UP;	// ������������
	
	InputPort(){}
	// ��ȡ״̬
    virtual bool Read() const;
    operator bool() const { return Read(); }

protected:
    virtual void OnOpen(void* param);
};

#endif //_Port_H_
