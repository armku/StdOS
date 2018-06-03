#include "PID.h"
#include "stdio.h"
#include "math.h"

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
	this->Kd = 0.2; //初始化过程
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
	this->voltage = this->Kp*this->err + index * this->Ki*this->integral + this->Kd*(this->err - this->err_last); //算法具体实现过程

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
	this->Ki = 0.1; //注意，和上几次相比，这里加大了积分环节的值
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
	if (this->ActualSpeed > this->umax) //灰色底色表示抗积分饱和的实现
	{
		if (abs(this->err) > 200) //蓝色标注为积分分离过程
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
		if (abs(this->err) > 200) //积分分离过程
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
		if (abs(this->err) > 200) //积分分离过程
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
