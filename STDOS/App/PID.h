#ifndef _PID_H
#define _PID_H

#include "Sys.h"

class PIDbasePosition//位置型PID
{
public:
	void Init();
	float PID_realize(float speed);
protected:
	float SetSpeed; //定义设定值
	float ActualSpeed; //定义实际值
	float err; //定义偏差值
	float err_last; //定义上一个偏差值
	float Kp, Ki, Kd; //定义比例、积分、微分系数
	float voltage; //定义电压值（控制执行器的变量）
	float integral; //定义积分值
private:

};

class PIDPositionInc //增量型 D PID
{
public:
	void PID_init();
	float PID_realize(float speed);
protected:
	float SetSpeed; //定义设定值
	float ActualSpeed; //定义实际值
	float err; //定义偏差值
	float err_next; //定义上一个偏差值
	float err_last; //定义最上前的偏差值
	float Kp, Ki, Kd; //定义比例、积分、微分系数
private:

};

class PIDJIFENLISAN //积分分离的 D PID
{
public:
	void PID_init();
	float PID_realize(float speed);

protected:
	float SetSpeed; //定义设定值
	float ActualSpeed; //定义实际值
	float err; //定义偏差值
	float err_next; //定义上一个偏差值
	float err_last; //定义最上前的偏差值
	float Kp, Ki, Kd; //定义比例、积分、微分系数


	float integral;
	float voltage;
private:

};

class PIDKangBaohejifen //抗积分饱和的 D PID
{
public:
	void PID_init();
	float PID_realize(float speed);
protected:
	float SetSpeed; //定义设定值
	float ActualSpeed; //定义实际值
	float err; //定义偏差值
	float err_last; //定义上一个偏差值
	float Kp, Ki, Kd; //定义比例、积分、微分系数
	float voltage; //定义电压值（控制执行器的变量）
	float integral; //定义积分值
	float umax;
	float umin;
private:

};

class PIDtixingjifen //梯形积分的 D 
{
public:

protected:

private:

};

class PIDBIANJIFEN //变积分
{
public:

protected:

private:
};

#endif // _PID_H
