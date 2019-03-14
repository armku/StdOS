/*
各种控制方式公式如下：

Position PID:               Output= KP*ET[CountT] +KI*Sum(ET[CountT]) +KD*vT；

Position PID Inc：    Delta Output= KP*vT +KI*ET[CountT] +KD*aT；

Inc PID：             Delta Output= KP*ET[CountT] +KI*Sum(ET[CountT]) +KD*vT；

PreF_v:               Delta Output= FdK*(ET[CountT]+vT*PreK)；

PreF_av:              Delta Output= FdK*(ET[CountT]+PreK*(vT+PreKv*(aT+PreKa*(a1T+PreKa1*(a2T+PreKa2*a2T_adj)))))；

PreF_adv:             Delta Output= FdK*(ET[CountT]+vT*PreK(T))；


ET[CountT]: 温度误差(在PID 模式下等于设置温度减去当前温度,在预测式负反馈模式下则相反）

vT=ET[CountT]-ET[CountT-1] ：升温速率

aT=ET[CountT]-2ET[CountT-1]+ET[CountT-2]) ： 升温加速度

Sum(ET[CountT]) 积分量


下面的代码为控制部分的核心代码示例：

PID:=KP*vT +KI*ET[CountT] +KD*aT;

if (PID>0) then if  ((pwmpointer+PID)<=pwmmax)  then pwmpointer:=pwmpointer+PID;

if (PID<0) then if  (pwmpointer+PID>=0) then pwmpointer:=pwmpointer+PID;

if pwmpointer<0  then pwmpointer:=0;

if pwmpointer>pwmmax  then pwmpointer:=pwmmax;

CurPow[CountT]:=pwmpointer*MaxP/pwmmax;   //Curent Power

MaxP： 最大加热功率   pwmmax:  PWM 最大值



使用方法：

　１)PID 模式下填写下列三个参数： 　　KP，KI， KD(均为实数)；

 　２)线性预测负反馈模式下填写下列两个参数: FdK，PreK（反馈深度与预测深度）；

  ３)填写设定温度，当前环境温度，系统在50℃时的自由升温速率；
  填入系统的延时估计值,  当前环境温度的波动幅度和周期，
  填入控制周期(仅在线性预测负反馈模式下有效)，
  填入最大温度估计值（系统自由加热可达到的最大温度值）和需要的仿真时间；

  ４)提前量为控制模式在达到设定温度前提前开启控制模式的时间;

  ５)当手绘环境温度选项开启后，你可以用鼠标在绘图区绘制环境温度曲线 ;

  　６)红色线为系统温度值, 蓝色线为系统加热功率, 黄色线为环境温度,黑色线为设定温度，绿色线为调整量变化曲线；

   　７)点击主菜单中的 “运行”按键按给定的参数进行仿真.

	　 8) 在非线性预测负反馈模式下，点击主菜单中的 “自动匹配”按键会软件会尝试自动寻找最佳参数（仅非线性预测负反馈模式下可用，
	  得到非线性预测负反馈模式和位置式PID增量形式的参数，效果不是很理想，需要的可以将就用一下！）.

	  9) 玩得开心!


	  其它事项:

	  在批量仿真页面可以一次运行多个仿真（用于寻找最佳参数）:

	  1) 在主菜单下选择仿真模式;

	  2) 在批量仿真页面,FdK(KP)、PreK(KI)、PreKv(KD)三个参数可选则一个填入起始值，并填写每步增量值和最大值后点击运行进行批量仿真;

	  3) 批量仿真的结果会在下面列出;

	  4) 使用保存按键可以保存批量仿真结果供分析用.


	  预测式负反馈控制方式的解释:

	  预测一段时间以后系统的温度值,使用预测的值(而不是当前值)来进行运算调整输出所需的改变量；

	  公式: 输出增量= 反馈深度*( 当前的温度误差值 + 预测深度 *升温速率),

	  这里面( 当前的温度误差值 + 预测深度 *升温速率)就是在预测深度(一段时间，单位S)以后的温度误差估计值，此处为线性预测，当公式内含比

	  升温速率更高阶的项目时即为非线性预测，一般场合使用线性预测就可以了，但进过仔细调整的非线性预测在某些场合亦会有明显改善.
*/
#ifndef _PID_H
#define _PID_H

#include "Sys.h"

class PID
{
public:
	PID(){CountT=20;}
	float SV;//设定值
	float PV;//当前值

	int CountT;// = 20;
	float ET[21];//温度误差(在PID 模式下等于设置温度减去当前温度,在预测式负反馈模式下则相反）
	float CurPow[21];
	float vT;//升温速率
	float aT;//升温加速度
	float Sum;//积分量

	float KP;
	float KI;
	float KD;

	float FdK;//反馈深度
	float PreK;//预测深度

	float MaxP;//最大加热功率 
	float pwmpointer;
	float pwmmax;//PWM 最大值
	void addPV(float pv);//增加当前值

	float PIDPosition();//位置式PID
	float PIDPositionInc();//位置式PID增量方式
	float PIDInc();//增量式PID
	float PIDPreF_v();//线性预测负反馈
	float PIDPreF_av();//非线性预测负反馈
	float PIDPreF_adv();//分段线性预测

private:
	float sum();//统计误差总和


};

#if 0
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
#endif
#endif // _PID_H
