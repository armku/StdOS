/*
���ֿ��Ʒ�ʽ��ʽ���£�

Position PID:               Output= KP*ET[CountT] +KI*Sum(ET[CountT]) +KD*vT��

Position PID Inc��    Delta Output= KP*vT +KI*ET[CountT] +KD*aT��

Inc PID��             Delta Output= KP*ET[CountT] +KI*Sum(ET[CountT]) +KD*vT��

PreF_v:               Delta Output= FdK*(ET[CountT]+vT*PreK)��

PreF_av:              Delta Output= FdK*(ET[CountT]+PreK*(vT+PreKv*(aT+PreKa*(a1T+PreKa1*(a2T+PreKa2*a2T_adj)))))��

PreF_adv:             Delta Output= FdK*(ET[CountT]+vT*PreK(T))��


ET[CountT]: �¶����(��PID ģʽ�µ��������¶ȼ�ȥ��ǰ�¶�,��Ԥ��ʽ������ģʽ�����෴��

vT=ET[CountT]-ET[CountT-1] ����������

aT=ET[CountT]-2ET[CountT-1]+ET[CountT-2]) �� ���¼��ٶ�

Sum(ET[CountT]) ������


����Ĵ���Ϊ���Ʋ��ֵĺ��Ĵ���ʾ����

PID:=KP*vT +KI*ET[CountT] +KD*aT;

if (PID>0) then if  ((pwmpointer+PID)<=pwmmax)  then pwmpointer:=pwmpointer+PID;

if (PID<0) then if  (pwmpointer+PID>=0) then pwmpointer:=pwmpointer+PID;

if pwmpointer<0  then pwmpointer:=0;

if pwmpointer>pwmmax  then pwmpointer:=pwmmax;

CurPow[CountT]:=pwmpointer*MaxP/pwmmax;   //Curent Power

MaxP�� �����ȹ���   pwmmax:  PWM ���ֵ



ʹ�÷�����

����)PID ģʽ����д�������������� ����KP��KI�� KD(��Ϊʵ��)��

 ����)����Ԥ�⸺����ģʽ����д������������: FdK��PreK�����������Ԥ����ȣ���

  ��)��д�趨�¶ȣ���ǰ�����¶ȣ�ϵͳ��50��ʱ�������������ʣ�
  ����ϵͳ����ʱ����ֵ,  ��ǰ�����¶ȵĲ������Ⱥ����ڣ�
  �����������(��������Ԥ�⸺����ģʽ����Ч)��
  ��������¶ȹ���ֵ��ϵͳ���ɼ��ȿɴﵽ������¶�ֵ������Ҫ�ķ���ʱ�䣻

  ��)��ǰ��Ϊ����ģʽ�ڴﵽ�趨�¶�ǰ��ǰ��������ģʽ��ʱ��;

  ��)���ֻ滷���¶�ѡ����������������ڻ�ͼ�����ƻ����¶����� ;

  ����)��ɫ��Ϊϵͳ�¶�ֵ, ��ɫ��Ϊϵͳ���ȹ���, ��ɫ��Ϊ�����¶�,��ɫ��Ϊ�趨�¶ȣ���ɫ��Ϊ�������仯���ߣ�

   ����)������˵��е� �����С������������Ĳ������з���.

	�� 8) �ڷ�����Ԥ�⸺����ģʽ�£�������˵��е� ���Զ�ƥ�䡱����������᳢���Զ�Ѱ����Ѳ�������������Ԥ�⸺����ģʽ�¿��ã�
	  �õ�������Ԥ�⸺����ģʽ��λ��ʽPID������ʽ�Ĳ�����Ч�����Ǻ����룬��Ҫ�Ŀ��Խ�����һ�£���.

	  9) ��ÿ���!


	  ��������:

	  ����������ҳ�����һ�����ж�����棨����Ѱ����Ѳ�����:

	  1) �����˵���ѡ�����ģʽ;

	  2) ����������ҳ��,FdK(KP)��PreK(KI)��PreKv(KD)����������ѡ��һ��������ʼֵ������дÿ������ֵ�����ֵ�������н�����������;

	  3) ��������Ľ�����������г�;

	  4) ʹ�ñ��水�����Ա���������������������.


	  Ԥ��ʽ���������Ʒ�ʽ�Ľ���:

	  Ԥ��һ��ʱ���Ժ�ϵͳ���¶�ֵ,ʹ��Ԥ���ֵ(�����ǵ�ǰֵ)��������������������ĸı�����

	  ��ʽ: �������= �������*( ��ǰ���¶����ֵ + Ԥ����� *��������),

	  ������( ��ǰ���¶����ֵ + Ԥ����� *��������)������Ԥ�����(һ��ʱ�䣬��λS)�Ժ���¶�������ֵ���˴�Ϊ����Ԥ�⣬����ʽ�ں���

	  �������ʸ��߽׵���Ŀʱ��Ϊ������Ԥ�⣬һ�㳡��ʹ������Ԥ��Ϳ����ˣ���������ϸ�����ķ�����Ԥ����ĳЩ������������Ը���.
*/
#ifndef _PID_H
#define _PID_H

#include "Sys.h"

class PID
{
public:
	PID(){CountT=20;}
	float SV;//�趨ֵ
	float PV;//��ǰֵ

	int CountT;// = 20;
	float ET[21];//�¶����(��PID ģʽ�µ��������¶ȼ�ȥ��ǰ�¶�,��Ԥ��ʽ������ģʽ�����෴��
	float CurPow[21];
	float vT;//��������
	float aT;//���¼��ٶ�
	float Sum;//������

	float KP;
	float KI;
	float KD;

	float FdK;//�������
	float PreK;//Ԥ�����

	float MaxP;//�����ȹ��� 
	float pwmpointer;
	float pwmmax;//PWM ���ֵ
	void addPV(float pv);//���ӵ�ǰֵ

	float PIDPosition();//λ��ʽPID
	float PIDPositionInc();//λ��ʽPID������ʽ
	float PIDInc();//����ʽPID
	float PIDPreF_v();//����Ԥ�⸺����
	float PIDPreF_av();//������Ԥ�⸺����
	float PIDPreF_adv();//�ֶ�����Ԥ��

private:
	float sum();//ͳ������ܺ�


};

#if 0
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
#endif
#endif // _PID_H
