#ifndef _Port_H_
#define _Port_H_

#include "Core\Type.h"
#include "Platform\Pin.h"
#include "port/std_port_gpio.h"

// �˿ڻ���
class Port:public mcuGpio
{
public:	
    bool Open();
};
// �����
class OutputPort : public Port
{
public:
    bool OpenDrain	= false;	// �Ƿ�©���

    OutputPort();
    OutputPort(Pin pin, uint8_t invert, bool openDrain = false);
	
    void Write(bool value) ;
	// Read/ReadInput ���������ڣ�ǰ�߶�������߶����룬�ڿ�©�����ʱ���кܴ�����
    uint8_t ReadInput();

    static void Write(Pin pin, bool value);

    OutputPort& operator=(bool value) { Write(value); return *this; }
    OutputPort& operator=(OutputPort& port) { Write(port.Read()); return *this; }
    operator bool()  { return Read(); }

protected:
    virtual void OnOpen(void* param);
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
	    
    bool	Floating	= true;	// �Ƿ񸡿�����
    PuPd	Pull		= UP;	// ������������
	
	InputPort(){}

protected:
    virtual void OnOpen(void* param);
};

#endif //_Port_H_
