#ifndef __Environment_H__
#define __Environment_H__

// ��������
class TEnvironment
{
public:
	TEnvironment();

	// ��ȡϵͳ�����󾭹��ĺ�����
	UInt64 TickCount() const;

	// ��ȡϵͳ�����󾭹��ĺ�����
	UInt64 Ms() const;

	// ��ȡ��ǰ������ϵĴ�������
	int ProcessorCount() const;
};

// ȫ�ֻ�������
extern const TEnvironment Environment;

#endif
