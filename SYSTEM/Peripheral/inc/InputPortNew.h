#pragma once

#include "Port.h"

// �����
class InputPortNew: public Port
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

        InputPortNew()
        {
                Init();
        }
        InputPortNew(Pin pin, bool floating = true, PuPd_TypeDef pupd = PuPd_UP)
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
