#include "Kernel\Sys.h"
#include "Port.h"

#include <typeinfo>
using namespace ::std;

/******************************** Port ********************************/

#if DEBUG
// �������ţ���Ĺ���Ҫʹ��ʱ���ᱨ�������Ƿ񱣻��ɹ�
static bool Port_Reserve(Pin pin, bool flag);
#endif

// �˿ڻ�������
#define REGION_Port 1
#ifdef REGION_Port

Port::Port()
{
	this->_Pin = P0;
	this->Opened = false;
	this->Invert=false;
}


#ifndef TINY
Port::~Port()
{
	Close();
}
#endif


String Port::ToString() const
{
	String str;
	str += 'P';
	if (_Pin == P0)
	{
		str += '0';
	}
	else
	{
		str += (char)('A' + (_Pin >> 4));
		str += (byte)(_Pin & 0x0F);
	}
	return str;
}

// ��һ���ų�ʼ��
Port& Port::Set(Pin pin)
{
	// ������Ų��䣬��������
	if (pin == _Pin) return *this;

#ifndef TINY
	// �ͷ��������ŵı���
	if (_Pin != P0) Close();
#endif

	_Pin = pin;

	return *this;
}

bool Port::Empty() const
{
	if (_Pin != P0) return false;

	return true;
}

void Port::Clear()
{
	_Pin = P0;
}



void Port::Close()
{
	if (!Opened) return;
	if (_Pin == P0) return;

	OnClose();

#if DEBUG
	// ��������
	auto name = typeid(*this).name();
	while (*name >= '0' && *name <= '9') name++;
	debug_printf("%s", name);
	Port_Reserve(_Pin, false);
	debug_printf("\r\n");
#endif

	Opened = false;
}

WEAK void Port::Opening() { OnOpen(nullptr); }
WEAK void Port::OnOpen(void* param) {}

WEAK void Port::OnClose() {}

WEAK void Port::RemapConfig(uint param, bool sta) {}
WEAK void Port::AFConfig(GPIO_AF GPIO_AF) const {}
#endif

// �˿����ű���
#if DEBUG
// ���ű���λ����¼ÿ�������Ƿ��Ѿ�����������ֹ���ģ��ʹ��
// !!!ע�⣬����ȫ�㣬������ܻᱻ����zeroinit������copy�����µò�����ʼ��
static ushort Reserved[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF };

// �������ţ���Ĺ���Ҫʹ��ʱ���ᱨ�������Ƿ񱣻��ɹ�
bool Port_Reserve(Pin pin, bool flag)
{
	debug_printf("::");
	int port = pin >> 4, bit = 1 << (pin & 0x0F);
	if (flag) {
		if (Reserved[port] & bit) {
			// ��������Ѿ�����������ʾ���ܶ�ط�����ReservePin����д��־���õ�False��ֱ�����쳣
			debug_printf("P%c%d �ѱ���", _PIN_NAME(pin));
			return false; // already reserved
		}
		Reserved[port] |= bit;

		debug_printf("�� P%c%d", _PIN_NAME(pin));
	}
	else {
		Reserved[port] &= ~bit;

		debug_printf("�ر� P%c%d", _PIN_NAME(pin));
	}

	return true;
}

/*// �����Ƿ񱻱���
bool Port::IsBusy(Pin pin)
{
	int port = pin >> 4, sh = pin & 0x0F;
	return (Reserved[port] >> sh) & 1;
}*/
#endif

/******************************** OutputPort ********************************/

// ����˿�
#define REGION_Output 1
#ifdef REGION_Output

OutputPort::OutputPort() : Port() { }
OutputPort::OutputPort(Pin pin) : OutputPort(pin, 2) { }
OutputPort::OutputPort(Pin pin, byte invert, bool openDrain, byte speed) : Port()
{
	OpenDrain = openDrain;
	Speed = speed;
	Invert = invert;

	if (pin != P0)
	{
		Set(pin);
		//Open();
	}
}

OutputPort& OutputPort::Init(Pin pin, bool invert)
{
	Port::Set(pin);

	Invert = invert;

	return *this;
}

void OutputPort::OnOpen(void* param)
{
	TS("OutputPort::OnOpen");

#if DEBUG
	debug_printf(" %dM", Speed);
	if (OpenDrain)
		debug_printf(" ��©");
	else
		debug_printf(" ����");
	bool fg = false;
#endif

	// ���ݵ����������ȡ��ʼ״̬���Զ��ж��Ƿ���
	bool rs = Port::Read();
	if (Invert > 1)
	{
		Invert = rs;
#if DEBUG
		fg = true;
#endif
	}

#if DEBUG
	if (Invert)
	{
		if (fg)
			debug_printf(" �Զ�����");
		else
			debug_printf(" ����");
	}
#endif

#if DEBUG
	debug_printf(" ��ʼ��ƽ=%d \r\n", rs);
#endif

	Port::OnOpen(param);

	//OpenPin(param);
}

WEAK bool OutputPort::ReadInput() const
{
	if (Empty()) return false;

	bool v = Port::Read();
	if (Invert)return !v;
	return v;
}

void OutputPort::Up(int ms) const
{
	if (Empty()) return;

	//Write(true);
	//Sys.Sleep(ms);
	//Write(false);
}

void OutputPort::Down(int ms) const
{
	if (Empty()) return;

	//Write(false);
	//Sys.Sleep(ms);
	//Write(true);
}

void OutputPort::Blink(int times, int ms) const
{
	if (Empty()) return;

	bool flag = true;
	for (int i = 0; i < times; i++)
	{
		//Write(flag);
		flag = !flag;
		//Sys.Sleep(ms);
	}
	//Write(false);
}

/******************************** AlternatePort ********************************/

AlternatePort::AlternatePort() : OutputPort(P0, false, false) { }
AlternatePort::AlternatePort(Pin pin) : OutputPort(P0, false, false)
{
	if (pin != P0)
	{
		Set(pin);
		//Open();
	}
}
AlternatePort::AlternatePort(Pin pin, byte invert, bool openDrain, byte speed)
	: OutputPort(P0, invert, openDrain, speed)
{
	if (pin != P0)
	{
		Set(pin);
		//Open();
	}
}

//WEAK void AlternatePort::OpenPin(void* param) { OutputPort::OpenPin(param); }

#endif

/******************************** InputPort ********************************/

// ����˿�
#define REGION_Input 1
#ifdef REGION_Input

InputPort::InputPort() : InputPort(P0) { }
InputPort::InputPort(Pin pin, bool floating, PuPd pull) : Port()
{
	_task = 0;

	_Value = 0;

	_Start = 0;
	PressTime = 0;
	_Last = 0;

	if (pin != P0)
	{
		Set(pin);
		//Open();
	}
}

InputPort::~InputPort()
{
	Sys.RemoveTask(_task);
}

InputPort& InputPort::Init(Pin pin, bool invert)
{
	Port::Set(pin);

	Invert = invert;

	return *this;
}

// ��ȡ�����������ţ������Ϊtrue�򷵻�true����ҪΪ��һ���ŷ���
bool InputPort::Read() const
{
	bool v = Port::Read();
	if (Invert)return !v;
	return v;
}

#if DEBUG
int InputPort_Total = 0;
int InputPort_Error = 0;
#endif
void InputPort::OnPress(bool down)
{
	auto v = down ? Rising : Falling;

	// ��GD32F103VE�ϣ�����PE13����5%���Ҽ��ʴ���PE14�ĵ����жϣ���ʾ����û�м�⵽PE14�����в���
	// ���������������0801����0802��û�����֣�����������PE9/PE14
	if (_Value == v) return;

	/*
	������ע�⣺
	��Щ��ť���ܻ����110����Ҳ���ǰ��µ�ʱ��1���������������ʱ��������1��0������������
	*/

#if DEBUG
	InputPort_Total++;
#endif
	int	now =0;// (int)Sys.Ms();
	// ��һ�δ�������һ��̫���������������Ե�
	if (_Last > 0 && ShakeTime > 0 && now - _Last < ShakeTime)
	{
		// ������һ��׼��ִ�еĶ�������ȡ����һ���¼�
		_Value = 0;
#if DEBUG
		InputPort_Error++;
#endif
		return;
	}
	_Last = now;

	//_Value |= v;
	// ���ܻ����㣬������ѯʱ����������������ԭ��δ���
	_Value = v;

	if (down)
		_Start = now;
	else
		PressTime = (ushort)(now - _Start);

	if (HardEvent || !_IRQ)
		Press(*this, down);
	else
		// �ڶ���ʱ���ڣ������һ���źŵ��������л��᳷��
		Sys.SetTask(_task, true, ShakeTime);
}

void InputPort::InputTask(void* param)
{
	auto port = (InputPort*)param;
	byte v = port->_Value;
	port->_Value = 0;
	if (!v) return;

	//v	&= port->Mode;
	if (v & Rising)		port->Press(*port, true);
	if (v & Falling)	port->Press(*port, false);
}

void InputPort::InputNoIRQTask(void* param)
{
	auto port = (InputPort*)param;
	auto val = port->Read();

	port->OnPress(val);
}

void InputPort::OnOpen(void* param)
{
	TS("InputPort::OnOpen");

	// �������Ӳ���¼�����Ĭ��ʹ��20ms����
	if (!HardEvent && ShakeTime == 0) ShakeTime = 20;
#if DEBUG
	debug_printf(" ����=%dms", ShakeTime);
	if (Floating)
		debug_printf(" ����");
	else if (Pull == UP)
		debug_printf(" ������");
	else if (Pull == DOWN)
		debug_printf(" �½���");
	//if(Mode & Rising)	debug_printf(" ����");
	//if(Mode & Falling)	debug_printf(" ����");

	bool fg = false;
#endif

	//Port::OnOpen(param);
	//OpenPin(param);

	// ���ݵ����������ȡ��ʼ״̬���Զ��ж��Ƿ���
	bool rs = Port::Read();
	if (Invert > 1)
	{
		Invert = rs;
#if DEBUG
		fg = true;
#endif

		// ���ð�����ʼ״̬�����⿪ʼ��ѯʱ����һ���󴥷�
		_Value = Falling;
	}

#if DEBUG
	if (Invert)
	{
		if (fg)
			debug_printf(" �Զ�����");
		else
			debug_printf(" ����");
	}
#endif

#if DEBUG
	debug_printf(" ��ʼ��ƽ=%d \r\n", rs);
#endif
}

void InputPort::OnClose()
{
	Port::OnClose();

	//ClosePin();
}

// ������ѯʱ������Ĭ��100ms�������ⲿ�޸�
uint InputPort_Polling = 100;

bool InputPort::UsePress()
{
	assert(_Pin != P0, "����ע���������������");

	_IRQ =false;// OnRegister();

	if (!_task && !HardEvent)
	{
		// ���Ӳ���ж�ע��ʧ�ܣ������10ms��ʱ��ȡ
		if (_IRQ)
			_task = Sys.AddTask(InputTask, this, -1, -1, "�����¼�");
		else
		{
			// ���ð�����ʼ״̬�����⿪ʼ��ѯʱ����һ���󴥷�
			//_Value = Read() ? Rising : Falling;
			_task = Sys.AddTask(InputNoIRQTask, this, InputPort_Polling, InputPort_Polling, "������ѯ");
		}
	}

	return true;
}

#endif

/******************************** AnalogInPort ********************************/

void AnalogInPort::OnOpen(void* param)
{
#if DEBUG
	debug_printf("\r\n");
#endif

	//Port::OnOpen(param);

	//OpenPin(param);
}






#include "Port.h"
#include "stm32f10x.h"

#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

GPIO_TypeDef* GPIO_TypeDefGroup[] = { GPIOA ,GPIOB,GPIOC ,GPIOD ,GPIOE ,GPIOF,GPIOG };

GPIO_TypeDef *IndexToGroup(byte index)
{
#if defined STM32F0
	return ((GPIO_TypeDef*)(GPIOA_BASE + (index << 10)));
#elif defined STM32F1
	return GPIO_TypeDefGroup[index];
#elif defined STM32F4
	return ((GPIO_TypeDef*)(GPIOA_BASE + (index << 10)));
#endif
}



Port &Port::SetPin(Pin pin)
{
	if (this->_Pin != pin)
	{
		if (this->_Pin != P0)
			this->Close();
		this->_Pin = pin;

	}
	return  *this;
}

//���ùܽ�ģʽ
void Port::pinMode(GPIOMode_T mode)
{
	if (this->Empty())
		return;
	//***�����ȿ���ʱ�ӣ���������
#if defined STM32F0
	// ��ʱ��
	int gi = _Pin >> 4;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA << gi, ENABLE);
#elif defined STM32F1
	// ��ʱ��
	int gi = _Pin >> 4;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << gi, ENABLE);
	// PA15/PB3/PB4 ��Ҫ�ر�JTAG
	switch (_Pin)
	{
	case PA15:
	case PB3:
	case PB4:
		//debug_printf("Close JTAG for P%c%d\r\n", _PIN_NAME(_Pin));
		// PA15��jtag�ӿ��е�һԱ ��Ҫʹ�� ���뿪��remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		break;
	}
#elif defined STM32F4
	// ��ʱ��
	int gi = _Pin >> 4;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA << gi, ENABLE);
#endif

#if defined STM32F0
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
#elif defined STM32F1
	GPIO_InitTypeDef gpio;
	// �ر�Ҫ���أ���Щ�ṹ���Ա������Ϊû�г�ʼ������ɴ��
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = 1 << (this->_Pin & 0x0F);

	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = (GPIOMode_TypeDef)mode;

	GPIO_Init(IndexToGroup(this->_Pin >> 4), &gpio);
#elif defined STM32F4
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
#endif

	this->Opened = true;
}



bool Port::ReadInput()const
{
	if (this->Empty())
		return false;
	else
		return Port::Read();
}

void Port::Write(bool value)const
{
	if (this->Empty())
		return;
#if defined STM32F0	
	if (value)
	{
		GPIO_SetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
	}
	else
	{
		GPIO_ResetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
	}
#elif defined STM32F1
	if (this->Invert == false)
	{
		if (value)
		{
			GPIO_SetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
		}
		else
		{
			GPIO_ResetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
		}
	}
	else
	{
		if (value)
		{
			GPIO_ResetBits(_GROUP(this->_Pin), _PORT(this->_Pin));			
		}
		else
		{
			GPIO_SetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
		}
	}
#elif defined STM32F4
	if (value)
	{
		GPIO_SetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
	}
	else
	{
		GPIO_ResetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
	}
#endif
}
