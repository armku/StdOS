#ifndef _PID_H
#define _PID_H

#include "Sys.h"

class PIDbasePosition//λ����PID
{
public:
	void Init();
	float PID_realize(float speed);
protected:
	float SetSpeed; //�����趨ֵ
	float ActualSpeed; //����ʵ��ֵ
	float err; //����ƫ��ֵ
	float err_last; //������һ��ƫ��ֵ
	float Kp, Ki, Kd; //������������֡�΢��ϵ��
	float voltage; //�����ѹֵ������ִ�����ı�����
	float integral; //�������ֵ
private:

};

class PIDPositionInc //������ D PID
{
public:
	void PID_init();
	float PID_realize(float speed);
protected:
	float SetSpeed; //�����趨ֵ
	float ActualSpeed; //����ʵ��ֵ
	float err; //����ƫ��ֵ
	float err_next; //������һ��ƫ��ֵ
	float err_last; //��������ǰ��ƫ��ֵ
	float Kp, Ki, Kd; //������������֡�΢��ϵ��
private:

};

class PIDJIFENLISAN //���ַ���� D PID
{
public:
	void PID_init();
	float PID_realize(float speed);

protected:
	float SetSpeed; //�����趨ֵ
	float ActualSpeed; //����ʵ��ֵ
	float err; //����ƫ��ֵ
	float err_next; //������һ��ƫ��ֵ
	float err_last; //��������ǰ��ƫ��ֵ
	float Kp, Ki, Kd; //������������֡�΢��ϵ��


	float integral;
	float voltage;
private:

};

class PIDKangBaohejifen //�����ֱ��͵� D PID
{
public:
	void PID_init();
	float PID_realize(float speed);
protected:
	float SetSpeed; //�����趨ֵ
	float ActualSpeed; //����ʵ��ֵ
	float err; //����ƫ��ֵ
	float err_last; //������һ��ƫ��ֵ
	float Kp, Ki, Kd; //������������֡�΢��ϵ��
	float voltage; //�����ѹֵ������ִ�����ı�����
	float integral; //�������ֵ
	float umax;
	float umin;
private:

};

class PIDtixingjifen //���λ��ֵ� D 
{
public:

protected:

private:

};

class PIDBIANJIFEN //�����
{
public:

protected:

private:
};

#endif // _PID_H
