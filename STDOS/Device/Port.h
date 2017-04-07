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
/*
�����
 */
class OutputPort: public Port
{
    public:
        bool OpenDrain; // �Ƿ�©���
        bool Invert; // �Ƿ����������
        uint Speed; // �ٶ�

        OutputPort();
        // ��ͨ���һ����ÿ�©�������Ҫ����
        OutputPort(Pin pin, bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED);

        // ����д�����а���������
        void Write(bool value);
        void WriteGroup(ushort value); // ����д��
        void Up(uint ms); // ����һ��ʱ�������
        void Blink(uint times, uint ms); // ��˸���

        ushort ReadGroup(); // �����ȡ
        // ��ȡָ���������š��������մ�С����0xFF��ʾ�����Ϊtrue�򷵻�true
        bool Read(byte index);
        bool Read(); // Read() ReadReal() ��������  ǰ�߶����  һ��������    �ڿ�©�����ʱ���кܴ�����
        bool ReadInput();

        static bool Read(Pin pin);
        static void Write(Pin pin, bool value);

        OutputPort &operator = (bool value);
        OutputPort &operator = (OutputPort &port);
        operator bool();

    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
        void Init(bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED);
};
/*
����˿ڻỰ�ࡣ��ʼ��ʱ�򿪶˿ڣ���������������ʱ�رա�������������ö˿�Ϊ����
 */
class PortScope
{
    private:
        OutputPort *_port;
        bool _value;
    public:
        PortScope(OutputPort *port, bool value = true);

        ~PortScope();
};
/*
���������
 */
class AlternatePort: public OutputPort
{
    public:
        AlternatePort();
        // �������һ������������������Ҫ����
        AlternatePort(Pin pin, bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED);
    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
};
/*
ģ�����������
 */
class AnalogInPort: public Port
{
    public:
        AnalogInPort(Pin pin);

    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
};
// �����
class InputPort: public Port
{
    public:
        //����Ϊ2017-01-07
        typedef enum
        {
                NOPULL = 0x00, UP = 0x01,  //�������� 
                DOWN = 0x02,  //�������� 
        } PuPd;
        //enum class Trigger	//ǿ����ö��
        typedef enum  //2017-01-07
        {
                Rising = 0x01,  //������
                Falling = 0x02,  //�½���
                Both = 0x03  //�����½���
        } Trigger;
        // ��ȡί��
        typedef void(*IOReadHandler)(Pin port, bool down, void *param);
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

