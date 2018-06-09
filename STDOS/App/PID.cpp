#include "PID.h"
#include "stdio.h"
#include "math.h"

//���ӵ�ǰֵ
void  PID::addPV(float pv)
{
	for (int i = 0; i < this->CountT; i++)
	{
		this->ET[i] = this->ET[i + 1];
	}
	this->PV = pv;
	this->ET[CountT] = this->SV - this->PV;
	this->vT = this->ET[CountT] - this->ET[CountT - 1];
	this->aT = this->ET[CountT] - 2 * this->ET[CountT - 1] + this->ET[CountT - 2];
	this->Sum = this->sum();
}
//λ��ʽPID
float PID::PIDPosition()
{
	float Output = KP * ET[CountT] + KI * Sum + KD * vT;
	return Output;
}
//λ��ʽPID������ʽ
float PID::PIDPositionInc()
{
	float DeltaOutput = KP * vT + KI * ET[CountT] + KD * aT;
	float PID = DeltaOutput;
	if (PID > 0)
	{
		if((pwmpointer + PID) <= pwmmax)
			pwmpointer = pwmpointer + PID;
	}
	else
	{
		if(pwmpointer + PID >= 0)
			pwmpointer = pwmpointer + PID;
	}
	if( pwmpointer<0)
		 pwmpointer  = 0;
	if (pwmpointer>pwmmax)
		 pwmpointer  = pwmmax;

	CurPow[CountT] = pwmpointer * MaxP / pwmmax;   //Curent Power
	return DeltaOutput;
}
//����ʽPID
float PID::PIDInc()
{
	float DeltaOutput = KP * ET[CountT] + KI * sum() + KD * vT;
	return DeltaOutput;

}
//����Ԥ�⸺����
float PID::PIDPreF_v()
{
	float DeltaOutput =  FdK * (ET[CountT] + vT * PreK);
	return DeltaOutput;
}
//������Ԥ�⸺����
float PID::PIDPreF_av()
{
	float DeltaOutput = 0;// FdK * (ET[CountT] + PreK * (vT + PreKv * (aT + PreKa * (a1T + PreKa1 * (a2T + PreKa2 * a2T_adj)))));
	return DeltaOutput;
}
//�ֶ�����Ԥ��
float PID::PIDPreF_adv()
{
	float DeltaOutput = 0;// FdK * (ET[CountT] + vT * PreK(T));
	return DeltaOutput;
}

//ͳ������ܺ�
float PID::sum()
{
	float ret = 0;
	for (int i = 0; i < this->CountT + 1; i++)
	{
		ret += this->ET[i];
	}
	return ret;
}

#if 0
void PIDbasePosition::Init()
{
	printf("PID_init begin \n");
	this->SetSpeed = 0.0;
	this->ActualSpeed = 0.0;
	this->err = 0.0;
	this->err_last = 0.0;
	this->voltage = 0.0;
	this->integral = 0.0;
	this->Kp = 0.2;
	this->Ki = 0.015;
	this->Kd = 0.2;
	printf("PID_init end \n");
}
float PIDbasePosition::PID_realize(float speed)
{
	this->SetSpeed = speed;
	this->err = this->SetSpeed - this->ActualSpeed;
	this->integral += this->err;
	this->voltage = this->Kp*this->err + this->Ki*this->integral + this->Kd*(this->err - this->err_last);
	this->err_last = this->err;
	this->ActualSpeed = this->voltage*1.0;
	return this->ActualSpeed;
}
//test
void PIDbasePositionTest()
{
	PIDbasePosition pid;
	printf("System begin \n");
	pid.Init();
	int count = 0;
	while (count < 1000)
	{
		float speed = pid.PID_realize(200.0);
		printf("%f\n", speed);
		count++;
	}
}
void PIDPositionInc::PID_init()
{
	this->SetSpeed = 0.0;
	this->ActualSpeed = 0.0;
	this->err = 0.0;
	this->err_last = 0.0;
	this->err_next = 0.0;
	this->Kp = 0.2;
	this->Ki = 0.015;
	this->Kd = 0.2;
}
float PIDPositionInc::PID_realize(float speed)
{
	this->SetSpeed = speed;
	this->err = this->SetSpeed - this->ActualSpeed;
	float		incrementSpeed = this->Kp*(this->err - this->err_next) + this->Ki*this->err + this->Kd*(this->err - 2 * this->err_next + this->err_last);
	this->ActualSpeed += incrementSpeed;
	this->err_last = this->err_next;
	this->err_next = this->err;
	return this->ActualSpeed;
}
int PIDPositionIncTest()
{
	PIDPositionInc pid;
	pid.PID_init();
	int count = 0;
	while (count < 1000)
	{
		float speed = pid.PID_realize(200.0);
		printf("%f\n", speed);
		count++;
	}
	return 0;
}

void PIDJIFENLISAN::PID_init()
{
	this->SetSpeed = 0.0;
	this->ActualSpeed = 0.0;
	this->err = 0.0;
	this->err_last = 0.0;
	this->err_next = 0.0;
	this->Kp = 0.2;
	this->Ki = 0.015;
	this->Kd = 0.2;

	this->Kp = 0.2;
	this->Ki = 0.04;
	this->Kd = 0.2; //��ʼ������
}
float PIDJIFENLISAN::PID_realize(float speed)
{
	this->SetSpeed = speed;
	this->err = this->SetSpeed - this->ActualSpeed;
	float		incrementSpeed = this->Kp*(this->err - this->err_next) + this->Ki*this->err + this->Kd*(this->err - 2 * this->err_next + this->err_last);
	this->ActualSpeed += incrementSpeed;
	this->err_last = this->err_next;
	this->err_next = this->err;

	float index;
	if (abs(this->err) > 200)
	{
		index = 0;
	}
	else
	{
		index = 1;
		this->integral += this->err;
	}
	this->voltage = this->Kp*this->err + index * this->Ki*this->integral + this->Kd*(this->err - this->err_last); //�㷨����ʵ�ֹ���

	return this->voltage;
}
void PIDJIFENLISANTest()
{
	PIDJIFENLISAN pid;
	pid.PID_init();
	int count = 0;
	while (count < 1000)
	{
		float speed = pid.PID_realize(200.0);
		printf("%f\n", speed);
		count++;
	}
	return;
}
void PIDKangBaohejifen::PID_init()
{
	printf("PID_init begin \n");
	this->SetSpeed = 0.0;
	this->ActualSpeed = 0.0;
	this->err = 0.0;
	this->err_last = 0.0;
	this->voltage = 0.0;
	this->integral = 0.0;
	this->Kp = 0.2;
	this->Ki = 0.1; //ע�⣬���ϼ�����ȣ�����Ӵ��˻��ֻ��ڵ�ֵ
	this->Kd = 0.2;
	this->umax = 400;
	this->umin = -200;
	printf("PID_init end \n");
}
float PIDKangBaohejifen::PID_realize(float speed)
{
	int index;
	this->SetSpeed = speed;
	this->err = this->SetSpeed - this->ActualSpeed;
	if (this->ActualSpeed > this->umax) //��ɫ��ɫ��ʾ�����ֱ��͵�ʵ��
	{
		if (abs(this->err) > 200) //��ɫ��עΪ���ַ������
		{
			index = 0;
		}
		else 
		{
			index = 1;
			if (this->err < 0)
			{
				this->integral += this->err;
			}
		}
	}
	else if (this->ActualSpeed < this->umin)
	{
		if (abs(this->err) > 200) //���ַ������
		{
			index = 0;
		}
		else
		{
			index = 1;
			if (this->err > 0)
			{
				this->integral += this->err;
			}
		}
	}
	else
	{
		if (abs(this->err) > 200) //���ַ������
		{
			index = 0;
		}
		else
		{
			index = 1;
			this->integral += this->err;
		}
	}
	this->voltage = this->Kp*this->err + index * this->Ki*this->integral + this->Kd*(this->err - this->err_last);
	this->err_last = this->err;
	this->ActualSpeed = this->voltage*1.0;
	return this->ActualSpeed;
}
#endif
