#ifndef _Port_H_
#define _Port_H_

#include "Core\Type.h"
#include "Platform\Pin.h"

/******************************** Port ********************************/

// �˿ڻ���
class Port
{
public:	
    Pin		_Pin;		// ����  
	bool	Opened;		// �Ƿ��Ѿ���

	Port();

    Port& Set(Pin pin);	// ��������
	bool Empty() const;

	bool Open();
	void Close();

	static void RemapConfig(uint32_t param, bool sta);

    virtual bool Read() const;

protected:
    // ���ù���
    virtual void OnOpen(void* param);
};

/******************************** OutputPort ********************************/

// �����
class OutputPort : public Port
{
public:
    uint8_t Invert		= 2;		// �Ƿ������������Ĭ��2��ʾ�Զ����
    bool OpenDrain	= false;	// �Ƿ�©���
    uint8_t Speed		= 50;		// �ٶ�

    OutputPort();
    OutputPort(Pin pin);
    OutputPort(Pin pin, uint8_t invert, bool openDrain = false, uint8_t speed = 50);
	
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

/******************************** AlternatePort ********************************/

// ���������
class AlternatePort : public OutputPort
{
public:
	AlternatePort();
    AlternatePort(Pin pin);
    AlternatePort(Pin pin, uint8_t invert, bool openDrain = false, uint8_t speed = 50);

protected:
    virtual void OpenPin(void* param);
};

/******************************** InputPort ********************************/

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
    InputPort(Pin pin, bool floating = true, PuPd pull = UP);

	// ��ȡ״̬
    virtual bool Read() const;
    operator bool() const { return Read(); }

protected:
    virtual void OnOpen(void* param);
};

#endif //_Port_H_
