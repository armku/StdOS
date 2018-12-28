#ifndef __KEY_DRIVER_H__
#define __KEY_DRIVER_H__

//包含必要类型定义头文件，如下面用到的uint8_t等类型声明
//#include "stm32l1xx_hal.h"

//建议10ms扫描一次
//按键按下消抖计数
#define KEY_PRESS_DB_CNT			3
//判断按键长按计数
#define KEY_LONG_PRESS_CNT		150
//判断按键连击计数
#define KEY_REPEAT_PRESS_CNT		50
//按键释放消抖计数
#define KEY_RELEASE_DB_CNT		3


//按键状态组合字（8位），ls为上一个状态（高4位），cs为当前状态（低4位）
#define KEY_FULL_STATE(ls, cs)	(KEY_StateTypeDef)(((ls)<<4)|(cs))
//按键的当前状态值，取按键状态组合字的低4位
#define KEY_CURT_STATE(s)			(KEY_StateTypeDef)((s)&0x0F)
//按键上一个状态值，取按键状态组合字的高4位
#define KEY_LAST_STATE(s)			(KEY_StateTypeDef)(((s)&0xF0)>>4)

//按键内部计数组合字（16位），h2用于按下/释放消抖计数（高2位），l14用于长按/连击计数（低14位）
#define KEY_FULL_CNT(h2, l14)		((((h2)&0x0003)<<14)|((l14)&0x3FFF))
//按键按下/释放消抖计数值
#define KEY_LOW_CNT(c)			((c)&0x3FFF)
//按键长按/连击计数值
#define KEY_HIGH_CNT(c)			(((c)&0xC000)>>14)

//按键短按动作识别字
#define KEY_SHORT_ACTION()		KEY_FULL_STATE(KEY_SHORT_PRESSED, KEY_RELEASED)
//按键长按动作识别字
#define KEY_LONG_ACTION()			KEY_FULL_STATE(KEY_LONG_PRESSED, KEY_RELEASED)
//按键连击动作识别字
#define KEY_REPEAT_ACTION()		KEY_FULL_STATE(KEY_REPEAT_PRESS_WAIT, KEY_REPEAT_PRESSED)


//按键扫描状态机状态编码
typedef enum
{
	//按键状态机：“按键等待被按下”处理
	KEY_FSM_WAIT_PRESS = 0,
	//按键状态机：“按键短按”处理
	KEY_FSM_SHORT_PRESS,
	//按键状态机：“按键长按”处理
	KEY_FSM_LONG_PRESS,
	//按键状态机：“按键连击”处理
	KEY_FSM_REPEAT_PRESS,
	//按键状态机：“按键释放”处理
	KEY_FSM_RELEASE
}KEY_FsmTypeDef;


//按键状态编码
typedef enum
{
	//按键状态:释放状态
	KEY_RELEASED = 0,
	//按键状态:短按状态
	KEY_SHORT_PRESSED,
	//按键状态:等待长按状态
	KEY_LONG_PRESS_WAIT,
	//按键状态:长按状态
	KEY_LONG_PRESSED,
	//按键状态:等待连击状态
	KEY_REPEAT_PRESS_WAIT,
	//按键状态:一次连击状态
	KEY_REPEAT_PRESSED
}KEY_StateTypeDef;


//按键句柄结构体
typedef struct
{
	uint8_t KeyId; //按键编号
	KEY_StateTypeDef KeyState; //按键状态组合字(高4bit为上一个状态，低4bit为当前状态)
	uint16_t KeyCnt; //内部组合计数器(高2bit给按下/弹出计数用，低14bit给长按/连击计数用)
	KEY_FsmTypeDef KeyFsmState; //状态机状态
}KEY_HandleTypeDef;

//按键端口读取函数声明，需要用户根据硬件情况自行实现
uint8_t KEY_Read_Port(uint8_t KeyId);
//按键初始化函数声明
void KEY_Init(void);
//按键扫描函数声明
void KEY_Scan(KEY_HandleTypeDef* keyHandle);
//按键状态获取函数声明
KEY_StateTypeDef KEY_Get_State(KEY_HandleTypeDef* keyHandle);


#endif
