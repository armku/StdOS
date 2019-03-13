/**
  * @brief  main.c--以单个按键以例，仅把与本模块相关的代码写出，其余代码未写
  * @param  
  * @retval 
  */
//包含其他头文件

//包含按键处理模块头文件
#include "key_driver.h"

//定义一个按键句柄结构体，并进行初始化
#define KEY_ID_1 1
KEY_HandleTypeDef hkey = {KEY_ID_1, KEY_FULL_STATE(KEY_RELEASED, KEY_RELEASED), 0, KEY_FSM_WAIT_PRESS};

/*若有多个按键，可以定义按键句柄结构体数组
#define KEY_ID_1 1
#define KEY_ID_2 2
#define KEY_ID_3 3
KEY_HandleTypeDef hkey[3] = {{KEY_ID_1, KEY_FULL_STATE(KEY_RELEASED, KEY_RELEASED), 0, KEY_FSM_WAIT_PRESS},
                                                         {KEY_ID_2, KEY_FULL_STATE(KEY_RELEASED, KEY_RELEASED), 0, KEY_FSM_WAIT_PRESS},
                                                         {KEY_ID_3, KEY_FULL_STATE(KEY_RELEASED, KEY_RELEASED), 0, KEY_FSM_WAIT_PRESS}};
*/


//用户根据硬件情况自行实现指定按键的IO电平读取，该函数将覆盖模块中的同名函数
uint8_t KEY_Read_Port(uint8_t KeyId)
{
        
        switch(KeyId)
        {
                case KEY_ID_1:
                        return (HAL_GPIO_ReadPin(USEKEY_GPIO_Port, USEKEY_Pin));
                        break;
                default:
                        return 0;
                        break;
        }
}

//main函数
int main(void)
{
        //按键端口配置，若其他代码已对其进行了配置，则可忽略
        KEY_Init();
        
        //其余代码
        
        while(1)
        {
                //其余代码
                
                //周期性调用按键扫描
                KEY_Scan(&hkey);
                
                //按键动作识别
                if(KEY_SHORT_ACTION() == KEY_Get_State(&hkey)) //短按，点亮led
                        HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
                if(KEY_LONG_ACTION() == KEY_Get_State(&hkey)) //长按，熄灭led
                        HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
                if(KEY_REPEAT_ACTION() == KEY_Get_State(&hkey)) //连击1次，led翻转1下
                        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
                
                //扫描间隔10ms
                HAL_Delay(10);
        }
        
        return 0;
}
