#pragma once 

#include "Sys.h"
#include "stm32f10x.h"

#define _force_inline 
#ifdef STM32F4
    #define GPIO_MAX_SPEED 100
#else 
    #define GPIO_MAX_SPEED 50
#endif 

// �˿ڻ���
// ���ڹ���һ���˿ڣ�ͨ��PinBit��ʶ�������Щ���š�
// �����ʼ��ʱ��ͨ��SetPort���ö˿ڣ���������״̬��Ȼ��Configͨ��gpio�ṹ�����ö˿ڣ��˿�����ʱ�ָ�����״̬
class Port
{
    public:
        GPIO_TypeDef *Group; // �����
        Pin _Pin; // ���
        ushort PinBit; // ��������λ��ÿ������һ��λ

        Port &Set(Pin pin); // �������ţ���Ӧ�����á�
        bool Empty()const
        {
                return _Pin == P0;
        }

        virtual void Config(); // ȷ������,ȷ���ö����ڲ��Ĳ������г�ʼ��

        // ��������
        _force_inline static GPIO_TypeDef* IndexToGroup(byte index);
        _force_inline static byte GroupToIndex(GPIO_TypeDef* group);

        #if DEBUG
            static bool Reserve(Pin pin, bool flag); // �������ţ���Ĺ���Ҫʹ��ʱ���ᱨ�������Ƿ񱣻��ɹ�
            static bool IsBusy(Pin pin); // �����Ƿ񱻱���
        #endif 		
    protected:
        Port();
        virtual ~Port();

        // ���ù��̣���Config���ã����GPIO_Init
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
        #if DEBUG
            virtual bool OnReserve(Pin pin, bool flag);
        #endif 

    private:
        #if defined(STM32F1)
            ulong InitState; // �������ų�ʼ״̬��������ʱ��ԭ
        #endif 
};

// �����
class OutputPort: public Port
{
    public:
        bool OpenDrain; // �Ƿ�©���
        bool Invert; // �Ƿ����������
        uint Speed; // �ٶ�

        OutputPort()
        {
                Init();
        }
        // ��ͨ���һ����ÿ�©�������Ҫ����
        OutputPort(Pin pin, bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED)
        {
                Init(invert, openDrain, speed);
                Set(pin);
        }

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

        OutputPort &operator = (bool value)
        {
                Write(value);
                return  *this;
        }
        OutputPort &operator = (OutputPort &port)
        {
                Write(port.Read());
                return  *this;
        }
        operator bool()
        {
                return Read();
        }

    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);

        void Init(bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED)
        {
                OpenDrain = openDrain;
                Speed = speed;
                Invert = invert;
        }

        #if DEBUG
            virtual bool OnReserve(Pin pin, bool flag);
        #endif 
};

// ���������
class AlternatePort: public OutputPort
{
    public:
        AlternatePort(): OutputPort()
        {
                Init(false, false);
        }
        // �������һ������������������Ҫ����
        AlternatePort(Pin pin, bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED): OutputPort()
        {
                Init(invert, openDrain, speed);
                Set(pin);
        }

    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);

        #if DEBUG
            virtual bool OnReserve(Pin pin, bool flag);
        #endif 
};
#if 0

// �����
class InputPort: public Port
{
    public:
        typedef enum
        {
                PuPd_NOPULL = 0x00, PuPd_UP = 0x01, PuPd_DOWN = 0x02
        } PuPd_TypeDef;

        // ��ȡί��
        typedef void(*IOReadHandler)(Pin pin, bool down, void *param);

        uint ShakeTime; // ����ʱ��
        PuPd_TypeDef PuPd; // ������������
        bool Floating; // �Ƿ񸡿�����
        bool Invert; // �Ƿ����������

        InputPort()
        {
                Init();
        }
        InputPort(Pin pin, bool floating = true, PuPd_TypeDef pupd = PuPd_UP)
        {
                Init(floating, pupd);
                Set(pin);
        }

        virtual ~InputPort();

        ushort ReadGroup(); // �����ȡ
        bool Read(); // ��ȡ״̬
        static bool Read(Pin pin); // ��ȡĳ������

        void Register(IOReadHandler handler, void *param = NULL); // ע���¼�

        operator bool()
        {
                return Read();
        }

    protected:
        // ��������ΪInit��������Ϊ���캯������Ҫ����Ϊ�ù��캯���ᵼ����ʵ����һ������Ȼ������������Ǹ��¶�������ִ��
        void Init(bool floating = true, PuPd_TypeDef pupd = PuPd_UP)
        {
                PuPd = pupd;
                Floating = floating;

                _Registed = false;
                //ShakeTime = 20;
                // ��ЩӦ�õ��������Ҫ���ߵ������ȣ����ʱ����Ҫ�������
                ShakeTime = 0;
                Invert = false;
        }

        virtual void OnConfig(GPIO_InitTypeDef &gpio);

        #if DEBUG
            virtual bool OnReserve(Pin pin, bool flag);
        #endif 

    private:
        bool _Registed;

        void RegisterInput(int groupIndex, int pinIndex, IOReadHandler handler, void *param);
        void UnRegisterInput(int pinIndex);
};
#endif
// ģ�����������
class AnalogInPort: public Port
{
    public:
        AnalogInPort(Pin pin)
        {
                Set(pin);
        }

    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
};

// ����˿ڻỰ�ࡣ��ʼ��ʱ�򿪶˿ڣ���������������ʱ�رա�������������ö˿�Ϊ����
class PortScope
{
    private:
        OutputPort *_port;
        bool _value;

    public:
        PortScope(OutputPort *port, bool value = true)
        {
                _port = port;
                if (_port)
                {
                        // ������ֵ��������ʱ����Ҫ��ԭ
                        _value = port->Read();
                        *_port = value;
                }
        }

        ~PortScope()
        {
                if (_port)
                     *_port = _value;
        }
};

