#ifndef _JTW8953_H_
#define _JTW8953_H_

class I2C;
class InputPort;

// ������������оƬ
class JTW8953
{
public:
	I2C* 		IIC;		// I2Cͨ�ſ�
	InputPort*	Port;		// �ж�����
	ushort		Address;	// �豸��ַ

	JTW8953();
	~JTW8953();

	void Init();
	void SetSlide(bool slide);
	// д���λ����
	bool WriteKey(ushort index, byte data);

	// ��������
	bool SetConfig(const Buffer& bs) const;

	bool Write(const Buffer& bs) const;

	bool Read(Buffer& bs) const;

	static void JTW8953Test();

private:
};

#endif
