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
#include "stm32f10x.h"

#ifdef STM32F4
    #define GPIO_MAX_SPEED 100
#else 
    #define GPIO_MAX_SPEED 50
#endif 

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
	Port();
	#ifndef TINY
	virtual ~Port();
	#endif
	Port& Set(Pin pin);	// ��������
	bool Empty() const;
	bool Open();
	void Close();
	void Clear();
	void AFConfig(GPIO_AF GPIO_AF) const;
	static void RemapConfig(uint param, bool sta);

    //virtual bool Read() const;

	String ToString() const;
	protected:
    // ���ù���
    virtual void OnOpen(void* param);
	virtual void OnClose();

private:
	void Opening();
	






    public:
        GPIO_TypeDef *Group; // �����
		
        ushort PinBit; // ��������λ��ÿ������һ��λ

       
       
        virtual void Config(); // ȷ������,ȷ���ö����ڲ��Ĳ������г�ʼ��
        // ��������
        _force_inline static GPIO_TypeDef *IndexToGroup(byte index);
        _force_inline static byte GroupToIndex(GPIO_TypeDef *group);

    protected:
       
       
        // ���ù��̣���Config���ã����GPIO_Init
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
    private:
        #if defined(STM32F1)
            UInt64 InitState; // �������ų�ʼ״̬��������ʱ��ԭ
        #endif 
};
/******************************** OutputPort ********************************/

// �����
class OutputPort : public Port
{
public:
    byte Invert		= 2;		// �Ƿ������������Ĭ��2��ʾ�Զ����
    bool OpenDrain	= false;	// �Ƿ�©���
    byte Speed		= 50;		// �ٶ�
	OutputPort();
    OutputPort(Pin pin);
    OutputPort(Pin pin, byte invert, bool openDrain = false, byte speed = 50);
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

private:		
    public:          
        void WriteGroup(ushort value); // ����д��
        ushort ReadGroup(); // �����ȡ
        // ��ȡָ���������š��������մ�С����0xFF��ʾ�����Ϊtrue�򷵻�true
        bool Read(byte index);
        static bool Read(Pin pin); 
        operator bool();
    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
        void Init(bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED);
};

/******************************** AlternatePort ********************************/

// ���������
class AlternatePort : public OutputPort
{
public:
	AlternatePort();
    AlternatePort(Pin pin);
    AlternatePort(Pin pin, byte invert, bool openDrain = false, byte speed = 50);

protected:
    //virtual void OnOpen();
	virtual void OpenPin(void* param);

private:
	public:       
    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
};

/******************************** InputPort ********************************/

// �����
class InputPort : public Port
{
	typedef enum
    {
        NOPULL	= 0x00,
        UP		= 0x01,	// ��������
        DOWN	= 0x02,	// ��������
    }PuPd;
    typedef enum
    {
        Rising	= 0x01,	// ������
        Falling	= 0x02,	// �½���
		Both	= 0x03	// �����½���
    }Trigger;

    // ��ȡί��
    typedef void (*IOReadHandler)(InputPort* port, bool down, void* param);

    ushort	ShakeTime	= 0;	// ���� ����ʱ�䡣����
	ushort	PressTime	= 0;	// ��ȡ ����ʱ�䡣����
    byte	Invert		= 2;	// �Ƿ������������Ĭ��2��ʾ�Զ����
    bool	Floating	= true;	// �Ƿ񸡿�����
    PuPd	Pull		= UP;	// ������������
	//Trigger	Mode		= Both;	// ����ģʽ���������½���
	bool	HardEvent	= false;// �Ƿ�ʹ��Ӳ���¼���Ĭ��false

	Delegate2<InputPort&, bool>	Press;	// �����¼�

	InputPort();
    InputPort(Pin pin, bool floating = true, PuPd pull = UP);
    virtual ~InputPort();

	InputPort& Init(Pin pin, bool invert);

	// ��ȡ״̬
    virtual bool Read() const;

	bool UsePress();
	void OnPress(bool down);

    operator bool() const { return Read(); }

protected:
    virtual void OnOpen(void* param);
	virtual void OnClose();

private:
	bool	_IRQ	= false;

	uint	_task	= 0;	// ��������
	int		_Start	= 0;	// ��ʼ����ʱ��
	int		_Last	= 0;	// ���һ�δ���ʱ��
	static void InputTask(void* param);
	static void InputNoIRQTask(void* param);

private:
	void OpenPin(void* param);
	void ClosePin();
	bool OnRegister();
	byte	_Value;	// ��ǰֵ
	
	
	
	
	
    public:
               
        ushort ShakeTime; //	=	0; 		// ����ʱ��.����
        ushort PressTime; //	=	0;		//����ʱ�䡣����
        bool Invert; //	=	2; 		// �Ƿ������������Ĭ��2��ʾ�Զ����		
        PuPd Pull; //	=	PuPd;//������������
        Trigger Mode; //	=	Both;	//����ģʽ���������½���
        bool HardEvent; //	=	false;	//�Ƿ�ʹ��Ӳ���¼���Ĭ��false
        //Delegate2<InputPortNew&,bool> Press;//�����¼�
        //����Ϊ2017-01-07
        bool Floating; // �Ƿ񸡿�����
        InputPort();
        InputPort(Pin pin, bool floating = true, PuPd pupd = UP);
        virtual ~InputPort();
        ushort ReadGroup(); // �����ȡ
        bool Read(); // ��ȡ״̬
        static bool Read(Pin pin); // ��ȡĳ������
        void Register(IOReadHandler handler, void *param = NULL); // ע���¼�
        operator bool();
    protected:
        // ��������ΪInit��������Ϊ���캯������Ҫ����Ϊ�ù��캯���ᵼ����ʵ����һ������Ȼ������������Ǹ��¶�������ִ��
        void Init(bool floating = true, PuPd pupd = UP);
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
    private:
        bool _Registed;
        void RegisterInput(int groupIndex, int pinIndex, IOReadHandler handler, void *param);
        void UnRegisterInput(int pinIndex);
};
/******************************** AnalogInPort ********************************/

// ģ�������
class AnalogInPort : public Port
{
public:
    AnalogInPort() : Port() { }
    AnalogInPort(Pin pin) : Port() { Set(pin); Open(); }

protected:
    virtual void OnOpen(void* param);

private:
	void OpenPin(void* param);

    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
};


/******************************** PortScope ********************************/

// ����˿ڻỰ�ࡣ��ʼ��ʱ�򿪶˿ڣ���������������ʱ�رա�������������ö˿�Ϊ����
class PortScope
{
private:
	OutputPort* _port;
	bool _value;

public:
	PortScope(OutputPort* port, bool value = true)
	{
		_port = port;
		if(_port)
		{
			// ������ֵ��������ʱ����Ҫ��ԭ
			_value = port->Read();
			*_port = value;
		}
	}

	~PortScope()
	{
		if(_port) *_port = _value;
	}
	private:
        OutputPort *_port;
        bool _value;
};

//�ж��ߴ򿪡��ر�
void SetEXIT(int pinIndex, bool enable);

/*
����ڷ���ԭ��
1���ж�ʱ��ͨ��ѭ����ȡ�����⼫����жϴ�����
ʵ���ϲ�֪���Ƿ���Ч����̫�����޴Ӳ��ԣ������ӿ��ܵ��¶�ʧ���»����¼������������Ƿ���Ҫ�Ľ���
2�����º͵����¼�����ͬʱ������_Value��Ȼ������һ����������������֧�ֺ��뼶�����ӳ�
3����Ϊ_Value����ͬʱ���ڰ��º͵���������ʱ��Ҫͬʱ�������ߡ�
*/

#endif //_Port_H_

