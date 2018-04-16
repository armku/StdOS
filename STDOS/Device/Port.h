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
class Port
{
public:	
    Pin		_Pin;		// ����   4
	bool	Opened;		// �Ƿ��Ѿ��� 8 5
	uint8_t    Index;		// ������������ţ��������ֶ����Ŵ���
	void*	State;		// �û�״̬����

	Port();

    Port& Set(Pin pin);	// ��������
	bool Empty() const;

	bool Open();
	void Close();
	void Clear();

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
    uint8_t Invert		= 2;		// �Ƿ������������Ĭ��2��ʾ�Զ����  12
    bool OpenDrain	= false;	// �Ƿ�©��� 13
    uint8_t Speed		= 50;		// �ٶ� 14

    OutputPort();
    OutputPort(Pin pin);
    OutputPort(Pin pin, uint8_t invert, bool openDrain = false, uint8_t speed = 50);

	OutputPort& Init(Pin pin, bool invert);

    void Write(bool value) const;
	// ����һ��ʱ�������
	void Up(int ms) const;
	void Down(int ms) const;
	// ��˸���
	void Blink(int times, int ms) const;

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

    // ��ȡί��
    typedef void (*IOReadHandler)(InputPort* port, bool down, void* param);

    uint8_t	Invert		= 2;	// �Ƿ������������Ĭ��2��ʾ�Զ���� 16
    bool	Floating	= true;	// �Ƿ񸡿�����
    PuPd	Pull		= UP;	// ������������
	
	InputPort(){}
    InputPort(Pin pin, bool floating = true, PuPd pull = UP);

	InputPort& Init(Pin pin, bool invert);

	// ��ȡ״̬
    virtual bool Read() const;
    operator bool() const { return Read(); }

protected:
    virtual void OnOpen(void* param);
};

#endif //_Port_H_
