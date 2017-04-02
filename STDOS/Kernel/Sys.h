#pragma once

#ifdef DEBUG
    #include <stdio.h>
#endif 

#include "Type.h"
#include "TTime.h"
#include "Delegate.h"
#include "SString.h"
#include "..\Platform\Pin.h"

/* 串口定义 */
typedef enum
{
    AIN = 0x0,  //模拟输入 
    INPUT = 0x04,  //输入
    INPUT_PD = 0x28,  //浮空输入
    INPUT_PU = 0x48,  //推挽式输入
    OUTPUT_OD = 0x14,  //开漏输出
    OUTPUT_PP = 0x10,  //推挽式输出
    AF_OD = 0x1C,  //开漏复用输出
    AF_PP = 0x18  //推挽复用输出
} PIN_MODE;
 
#define STM32F1    //使用STM32F1系列单片机
/* 针脚 ------------------------------------------------------------------*/
#ifdef DEBUG

void assert_failed2(cstring msg, cstring file, unsigned int line);
#define assert(expr, msg) ((expr) ? (void)0 : assert_failed2(msg, (const char*)__FILE__, __LINE__))

#else

#define assert(expr, msg) ((void)0)

#endif
// 使用何种模块的宏定义
#define using(module) Sys.module.Init = T##module##_Init;

#ifdef DEBUG
    #define debug_printf printf
#else 
    inline void debug_printf(const char *format, ...){}
#endif 

class TSys: public Object
{
    public:
        TSys(uint clock = 72000000, byte messagePort = COM1);
        void Init(); //初始化
        void Start(); //启动系统任务调度，该函数内部为死循环。
        void Stop();
        void StartInternal();
        virtual void Show(bool newLine = false)const;
        void ShowInfo(); //显示系统信息
        UInt64 Ms(); //系统启动以来的毫秒数，无符号长整型8字节
        uint Seconds(); //系统绝对UTC时间，整型4字节，Unix格式，1970年以来的总秒数。
        void Delay(uint us); //微妙级延迟，常用于高精度外设信号控制
        void Sleep(uint ms); //毫秒级睡眠，常用于业务层杂宁等待一定时间
        void Reboot(uint msDelay); //异步热重启系统。延迟一定毫秒数执行。		

        uint AddTask(Action func, void *param, long dueTime, long period, const char *name = "未命名任务");
        void SetTask(uint taskid,bool onoff,long delaytime=1); //设置任务参数
        void RemoveTask(uint taskid); //删除任务

        uint Clock; //系统时钟
        UInt64 StartTime; //系统启动时间
        ushort FlashSize;
        ushort RamSize;
        byte MessagePort; // 指定printf输出的串口
        byte Inited; //串口用参数
        byte ID[12];
        uint CPUID;
        ushort RevID;
        ushort DevID;
        byte _Index; //未知
        String *CPUName;
		char * Name;//名称
        ushort Crc16(const void *buf, uint len, ushort crc = 0); //crc 计算 软件实现
        uint Crc(const void *buf, uint len); //crc 硬件实现
        uint Crc(const void *buf, uint len, uint crc);
        void ShowHex(byte *data, byte hexlength);

    private:
        void Initjs(); //计算ram、型号等

};
extern TSys Sys; //系统参数
