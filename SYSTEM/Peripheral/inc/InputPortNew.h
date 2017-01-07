#pragma once

#include "Port.h"

// �����
class InputPortNew: public Port
{
    public:
		//����Ϊ2017-01-07
		typedef enum
        {
                NOPULL = 0x00,
				UP 	= 0x01,//�������� 
				DOWN 	= 0x02,//�������� 
        } PuPd;
		//enum class Trigger	//ǿ����ö��
		typedef enum	//2017-01-07
		{
			Rising	=	0x01,	//������
			Falling	=	0x02,	//�½���
			Both	=	0x03	//�����½���
		}Trigger;

        // ��ȡί��
        typedef void(*IOReadHandler)(InputPortNew * port, bool down, void *param); 

        ushort	ShakeTime;//	=	0; 		// ����ʱ��.����
		ushort	PressTime;//	=	0;		//����ʱ�䡣����
		bool	Invert	;//	=	2; 		// �Ƿ������������Ĭ��2��ʾ�Զ����		
        PuPd Pull;//	=	PuPd;//������������
		Trigger	Mode	;//	=	Both;	//����ģʽ���������½���
		bool	HardEvent;//	=	false;	//�Ƿ�ʹ��Ӳ���¼���Ĭ��false
		
		//Delegate2<InputPortNew&,bool> Press;//�����¼�
		
		//����Ϊ2017-01-07
        bool Floating; // �Ƿ񸡿�����
        

        InputPortNew()
        {
                Init();
        }
        InputPortNew(Pin pin, bool floating = true, PuPd pupd = UP)
        {
                Init(floating, pupd);
                Set(pin);
        }

        virtual ~InputPortNew();

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
        void Init(bool floating = true, PuPd pupd = UP)
        {
                Pull = pupd;
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
