#ifndef __KEY_DRIVER_H__
#define __KEY_DRIVER_H__

//������Ҫ���Ͷ���ͷ�ļ����������õ���uint8_t����������
//#include "stm32l1xx_hal.h"

//����10msɨ��һ��
//����������������
#define KEY_PRESS_DB_CNT			3
//�жϰ�����������
#define KEY_LONG_PRESS_CNT		150
//�жϰ�����������
#define KEY_REPEAT_PRESS_CNT		50
//�����ͷ���������
#define KEY_RELEASE_DB_CNT		3


//����״̬����֣�8λ����lsΪ��һ��״̬����4λ����csΪ��ǰ״̬����4λ��
#define KEY_FULL_STATE(ls, cs)	(KEY_StateTypeDef)(((ls)<<4)|(cs))
//�����ĵ�ǰ״ֵ̬��ȡ����״̬����ֵĵ�4λ
#define KEY_CURT_STATE(s)			(KEY_StateTypeDef)((s)&0x0F)
//������һ��״ֵ̬��ȡ����״̬����ֵĸ�4λ
#define KEY_LAST_STATE(s)			(KEY_StateTypeDef)(((s)&0xF0)>>4)

//�����ڲ���������֣�16λ����h2���ڰ���/�ͷ�������������2λ����l14���ڳ���/������������14λ��
#define KEY_FULL_CNT(h2, l14)		((((h2)&0x0003)<<14)|((l14)&0x3FFF))
//��������/�ͷ���������ֵ
#define KEY_LOW_CNT(c)			((c)&0x3FFF)
//��������/��������ֵ
#define KEY_HIGH_CNT(c)			(((c)&0xC000)>>14)

//�����̰�����ʶ����
#define KEY_SHORT_ACTION()		KEY_FULL_STATE(KEY_SHORT_PRESSED, KEY_RELEASED)
//������������ʶ����
#define KEY_LONG_ACTION()			KEY_FULL_STATE(KEY_LONG_PRESSED, KEY_RELEASED)
//������������ʶ����
#define KEY_REPEAT_ACTION()		KEY_FULL_STATE(KEY_REPEAT_PRESS_WAIT, KEY_REPEAT_PRESSED)


//����ɨ��״̬��״̬����
typedef enum
{
	//����״̬�����������ȴ������¡�����
	KEY_FSM_WAIT_PRESS = 0,
	//����״̬�����������̰�������
	KEY_FSM_SHORT_PRESS,
	//����״̬��������������������
	KEY_FSM_LONG_PRESS,
	//����״̬��������������������
	KEY_FSM_REPEAT_PRESS,
	//����״̬�����������ͷš�����
	KEY_FSM_RELEASE
}KEY_FsmTypeDef;


//����״̬����
typedef enum
{
	//����״̬:�ͷ�״̬
	KEY_RELEASED = 0,
	//����״̬:�̰�״̬
	KEY_SHORT_PRESSED,
	//����״̬:�ȴ�����״̬
	KEY_LONG_PRESS_WAIT,
	//����״̬:����״̬
	KEY_LONG_PRESSED,
	//����״̬:�ȴ�����״̬
	KEY_REPEAT_PRESS_WAIT,
	//����״̬:һ������״̬
	KEY_REPEAT_PRESSED
}KEY_StateTypeDef;


//��������ṹ��
typedef struct
{
	uint8_t KeyId; //�������
	KEY_StateTypeDef KeyState; //����״̬�����(��4bitΪ��һ��״̬����4bitΪ��ǰ״̬)
	uint16_t KeyCnt; //�ڲ���ϼ�����(��2bit������/���������ã���14bit������/����������)
	KEY_FsmTypeDef KeyFsmState; //״̬��״̬
}KEY_HandleTypeDef;

//�����˿ڶ�ȡ������������Ҫ�û�����Ӳ���������ʵ��
uint8_t KEY_Read_Port(uint8_t KeyId);
//������ʼ����������
void KEY_Init(void);
//����ɨ�躯������
void KEY_Scan(KEY_HandleTypeDef* keyHandle);
//����״̬��ȡ��������
KEY_StateTypeDef KEY_Get_State(KEY_HandleTypeDef* keyHandle);


#endif
