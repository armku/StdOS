/*
系统能自动识别常见芯片类型、主频、Flash大小、Ram大小
Sys.ID 是12字节芯片唯一标识、也就是ChipID，同一批芯片仅前面几个字节不同
毫秒级睡眠期间，系统将会安排执行其他耗时较短的任务。如果没有可用任务，系统将会进入低功耗模式，以节省能耗

 */
#include "stdio.h"
#include "Sys.h"
#include "TTime.h"
#include "stm32f10x.h"
#include "TaskScheduler.h"
#include "SmartIrq.h"
#include <string.h>
#include "Array.h"
#include "DateTime.h"
extern "C"
{
    extern uint __heap_base;
    extern uint __heap_limit;
	extern uint __initial_sp;
}
//外部注册函数
// 任务
#include "Task.h"
// 任务类
TaskScheduler *_Scheduler;

// 创建任务，返回任务编号。priority优先级，dueTime首次调度时间us，period调度间隔us，-1表示仅处理一次
uint TSys::AddTask(Action func, void *param, long dueTime, long period, const char *name)
{
    // 屏蔽中断，否则可能有线程冲突
    SmartIRQ irq;

    if (!_Scheduler)
    {
        _Scheduler = new TaskScheduler("Sys");
    }

    return _Scheduler->Add(func, param, dueTime, period, name);
}

void TSys::RemoveTask(uint taskid)
{
    #if 0
        assert_ptr(_Scheduler);
    #endif 
    _Scheduler->Remove(taskid);
}

void TSys::SetTask(uint taskid,bool onoff,long delaytime)
{
    _Scheduler->SetTask(taskid,onoff,delaytime);
}

//启动系统任务调度，该函数内部为死循环。*在此之间，添加的所有任务函数将得不到调度，所有睡眠方法无效！
void TSys::Start()
{
    if (!_Scheduler)
    {
        _Scheduler = new TaskScheduler("Sys");
    }

    #if 0		
        if (OnStart)
        {
            OnStart();
        }
        else
    #endif 
    {
        _Scheduler->Start();
    }
}

void TSys::StartInternal()
{
    _Scheduler->Start();
}

void TSys::Stop()
{
    _Scheduler->Stop();
}


void TimeSleep(uint us)
{
    // 在这段时间里面，去处理一下别的任务
    if (_Scheduler && (us >= 1000))
    {
        // 记录当前正在执行任务
        Task *task = _Scheduler->Current;

        ulong start = Time.Current();
        // 1ms一般不够调度新任务，留给硬件等待
        ulong end = start + us - 1000;
        // 如果休眠时间足够长，允许多次调度其它任务
        int cost = 0;
        while (true)
        {
            ulong start2 = Time.Current();

            _Scheduler->Execute(us);

            ulong now = Time.Current();
            cost += (int)(now - start2);

            // us=0 表示释放一下CPU
            if (!us)
            {
                return ;
            }

            if (now >= end)
            {
                break;
            }
        }

        if (task)
        {
            _Scheduler->Current = task;
            task->SleepTime += cost;
        }

        cost = (int)(Time.Current() - start);
        if (cost > 0)
        {
            return ;
        }

        us -= cost;
    }
    if (us)
    {
        Time.Sleep(us);
    }
}

//毫秒级睡眠，常用于业务层杂宁等待一定时间
void TSys::Sleep(uint ms)
{
    // 优先使用线程级睡眠
    #if 0
        if (OnSleep)
        {
            OnSleep(ms);
        }
        else
    #endif 
    {
        if (ms > 1000)
        {
            debug_printf("Sys::Sleep 设计错误，睡眠%dms太长，超过1000ms建议使用多线程Thread！", ms);
        }
        TimeSleep(ms *1000);
    }
}

//微妙级延迟，常用于高精度外设信号控制
void TSys::Delay(uint us)
{
    // 如果延迟微秒数太大，则使用线程级睡眠
    #if 0
        if (OnSleep && us >= 2000)
        {
            OnSleep((us + 500) / 1000);
        }
        else
    #endif 
    {

        if (us > 1000000)
        {
            debug_printf("Sys::Sleep 设计错误，睡眠%dus太长，超过1000ms建议使用多线程Thread！", us);
        }
        TimeSleep(us);
    }
}

TSys::TSys(uint clock, COM_Def messagePort)
{
    this->Clock = clock;
    this->MessagePort = messagePort;
}

void TSys::Show(bool newLine)const{

}
//系统启动以来的毫秒数，无符号长整型8字节
UInt64 TSys::Ms()
{
    return Time.Ms();
}

//系统绝对UTC时间，整型4字节，Unix格式，1970年以来的总秒数。
uint TSys::Seconds()
{
    return Time.Seconds();
}

//异步热重启系统。延迟一定毫秒数执行。
void TSys::Reboot(uint msDelay){}
//显示系统信息
void TSys::ShowInfo()
{
	uint HeapSize=0;
	uint StackSize=0;
	
	HeapSize=((uint) &__heap_limit-(uint) &__heap_base);
	StackSize=((uint) &__initial_sp-(uint) &__heap_limit);
	
    printf("STD_Embedded_Team::STD0801 Code:Demo Ver:0.0.6113 Build:%s\n", __DATE__);
    printf("STDOS::%s 72MHz Flash:%dk RAM:%dk\n", this->CPUName->GetBuffer(), this->FlashSize, this->RamSize);
    printf("DevID:0X%04X RevID:0X%04X\n", this->DevID, this->RevID);
    printf("CPUID:0X%X ARM:ARMv7-M Cortex-M3: R1p2\n", this->CPUID);
    printf("Heap :(0X%X, 0X%X) = 0X%X (%dk)\n",(uint) &__heap_base,(uint) &__heap_limit,HeapSize,HeapSize/1024);
    printf("Stack:(0X%X, 0X%X) = 0X%X (%dk)\n",(uint) &__heap_limit,(uint) &__initial_sp,StackSize,StackSize/1024);
    printf("ChipType:0x42455633 3\n");
    printf("ChipID:%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\n", ID[0], ID[1], ID[2], ID[3], ID[4], ID[5], ID[6], ID[7], ID[8], ID[9], ID[10], ID[11]);
    printf("Time : ");
    DateTime dt;
    dt.Show();

    printf("Support: http://www.armku.com\n");
}

#define delay_ostickspersec 1000			//时钟频率
byte fac_us = 0; //每个us需要的systick时钟数 

TSys Sys; //系统参数
UInt64 TicksPerms = 0;

//初始化
//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void TSys::Init()
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //选择外部时钟  HCLK/8
    fac_us = SystemCoreClock / 8000000 * 8; //为系统时钟的1/8 //非OS下,代表每个us需要的systick时钟数   
    TicksPerms = SystemCoreClock / delay_ostickspersec;
    SysTick_Config(TicksPerms); //tick is 1ms	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //关闭jtag，保留swd	


    NVIC_SetPriority(SysTick_IRQn, 0);

    #ifdef STM32F0XX
        void *p = (void*)0x1FFFF7AC;
    #else 
        void *p = (void*)0x1FFFF7E8;
    #endif 
    memcpy(ID, p, ArrayLength(ID));


    this->CPUID = SCB->CPUID;
    uint MCUID = DBGMCU->IDCODE; // MCU编码。低字设备版本，高字子版本
    this->RevID = MCUID >> 16;
    this->DevID = MCUID &0x0FFF;

    this->_Index = 0;

    #ifdef STM32F0XX
        FlashSize = *(__IO ushort*)(0x1FFFF7CC); // 容量
    #else 
        FlashSize = *(__IO ushort*)(0x1FFFF7E0); // 容量
    #endif 
    this->Initjs();
    this->Inited = 1;
}

//计算ram、型号等
void TSys::Initjs()
{
    switch (this->DevID)
    {
        case 0x0410:
            this->CPUName = new String("STM32F103C8");
            this->RamSize = 20;
            break;
        case 0X0414:
            this->CPUName = new String("STM32F103ZE");
            this->RamSize = 64;
            break;
        case 0x418:
            this->CPUName = new String("STM32F105VC");
            this->RamSize = 64;
            break;
        default:
            this->CPUName = new String("未知");
            break;
    }
}

void TSys::ShowHex(byte *data, byte hexlength)
{
	byte ba;
	ushort ba1;
	uint ba2;
    switch (hexlength)
    {
        case 1:
            ba = data[0];
            debug_printf("%u", ba);
            break;
        case 2:
            ba1 = (ushort)(*data);
            debug_printf("%u", ba1);
            break;
        case 4:
            ba2 = (uint)(*data);
            debug_printf("%u", ba2);
            break;
        default:
            break;
    }
}

/*
CRC 硬件实现
 */
//
// CRC 32 table for use under ZModem protocol, IEEE 802
// G(x) = x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x+1
//
static const uint c_CRCTable[256] = 
{
    0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005, 0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61, 0x350C9B64, 0x31CD86D3,
        0x3C8EA00A, 0x384FBDBD, 0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9, 0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75, 0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011,
        0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD, 0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039, 0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5, 0xBE2B5B58, 0xBAEA46EF,
        0xB7A96036, 0xB3687D81, 0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D, 0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49, 0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95,
        0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1, 0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D, 0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE, 0x278206AB, 0x23431B1C,
        0x2E003DC5, 0x2AC12072, 0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16, 0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA, 0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE,
        0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02, 0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066, 0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA, 0xACA5C697, 0xA864DB20,
        0xA527FDF9, 0xA1E6E04E, 0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692, 0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6, 0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A,
        0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E, 0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2, 0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686, 0xD5B88683, 0xD1799B34,
        0xDC3ABDED, 0xD8FBA05A, 0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637, 0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB, 0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F,
        0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53, 0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47, 0x36194D42, 0x32D850F5, 0x3F9B762C, 0x3B5A6B9B, 0x0315D626, 0x07D4CB91,
        0x0A97ED48, 0x0E56F0FF, 0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623, 0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7, 0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B,
        0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F, 0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3, 0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7, 0xAE3AFBA2, 0xAAFBE615,
        0xA7B8C0CC, 0xA379DD7B, 0x9B3660C6, 0x9FF77D71, 0x92B45BA8, 0x9675461F, 0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3, 0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640,
        0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C, 0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8, 0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24, 0x119B4BE9, 0x155A565E,
        0x18197087, 0x1CD86D30, 0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC, 0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088, 0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654,
        0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0, 0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C, 0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18, 0xF0A5BD1D, 0xF464A0AA,
        0xF9278673, 0xFDE69BC4, 0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0, 0x9ABC8BD5, 0x9E7D9662, 0x933EB0BB, 0x97FFAD0C, 0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668,
        0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4
};

uint TSys::Crc(const void *buf, uint len, uint crc)
{
    byte *ptr = (byte*)buf;
    while (len-- > 0)
    {
        crc = c_CRCTable[((crc >> 24) ^ (*ptr++)) &0xFF] ^ (crc << 8);
    }

    return crc;
}

uint TSys::Crc(const void *buf, uint len)
{
    #ifdef STM32F4
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
    #else 
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
    #endif 

    CRC_ResetDR();
    // STM32的初值是0xFFFFFFFF，而软Crc初值是0
    //CRC->DR = __REV(crc ^ 0xFFFFFFFF);
    //CRC->DR = 0xFFFFFFFF;
    uint *ptr = (uint*)buf;
    len >>= 2;
    while (len-- > 0)
    {
        CRC->DR = __REV(*ptr++); // 字节顺序倒过来,注意不是位序,不是用__RBIT指令
    }
    uint crc = CRC->DR;

    #ifdef STM32F4
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, DISABLE);
    #else 
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, DISABLE);
    #endif 

    return crc;
}

/*
CRC校验-软件实现
 */
static const ushort c_CRC16Table[] = 
{
    0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401, 0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400, 
};
ushort TSys::Crc16(const void *buf, uint len, ushort crc)
{
    if (!buf || !len)
        return 0;

    for (int i = 0; i < len; i++)
    {
        byte b = ((byte*)buf)[i];
        crc = (ushort)(c_CRC16Table[(b ^ crc) &0x0F] ^ (crc >> 4));
        crc = (ushort)(c_CRC16Table[((b >> 4) ^ crc) &0x0F] ^ (crc >> 4));
    }
    return crc;
}

#if 0
    //测试crc代码
    static const uint DataBuffer[] = 
    {
        0x00001021, 0x20423063, 0x408450a5, 0x60c670e7, 0x9129a14a, 0xb16bc18c, 0xd1ade1ce, 0xf1ef1231, 0x32732252, 0x52b54294, 0x72f762d6, 0x93398318, 0xa35ad3bd, 0xc39cf3ff,
            0xe3de2462, 0x34430420, 0x64e674c7, 0x44a45485, 0xa56ab54b, 0x85289509, 0xf5cfc5ac, 0xd58d3653, 0x26721611, 0x063076d7, 0x569546b4, 0xb75ba77a, 0x97198738, 0xf7dfe7fe,
            0xc7bc48c4, 0x58e56886, 0x78a70840, 0x18612802, 0xc9ccd9ed, 0xe98ef9af, 0x89489969, 0xa90ab92b, 0x4ad47ab7, 0x6a961a71, 0x0a503a33, 0x2a12dbfd, 0xfbbfeb9e, 0x9b798b58,
            0xbb3bab1a, 0x6ca67c87, 0x5cc52c22, 0x3c030c60, 0x1c41edae, 0xfd8fcdec, 0xad2abd0b, 0x8d689d49, 0x7e976eb6, 0x5ed54ef4, 0x2e321e51, 0x0e70ff9f, 0xefbedfdd, 0xcffcbf1b,
            0x9f598f78, 0x918881a9, 0xb1caa1eb, 0xd10cc12d, 0xe16f1080, 0x00a130c2, 0x20e35004, 0x40257046, 0x83b99398, 0xa3fbb3da, 0xc33dd31c, 0xe37ff35e, 0x129022f3, 0x32d24235,
            0x52146277, 0x7256b5ea, 0x95a88589, 0xf56ee54f, 0xd52cc50d, 0x34e224c3, 0x04817466, 0x64475424, 0x4405a7db, 0xb7fa8799, 0xe75ff77e, 0xc71dd73c, 0x26d336f2, 0x069116b0,
            0x76764615, 0x5634d94c, 0xc96df90e, 0xe92f99c8, 0xb98aa9ab, 0x58444865, 0x78066827, 0x18c008e1, 0x28a3cb7d, 0xdb5ceb3f, 0xfb1e8bf9, 0x9bd8abbb, 0x4a755a54, 0x6a377a16,
            0x0af11ad0, 0x2ab33a92, 0xed0fdd6c, 0xcd4dbdaa, 0xad8b9de8, 0x8dc97c26, 0x5c644c45, 0x3ca22c83, 0x1ce00cc1, 0xef1fff3e, 0xdf7caf9b, 0xbfba8fd9, 0x9ff86e17, 0x7e364e55,
            0x2e933eb2, 0x0ed11ef0
    };

    bool inited = false;
    void Init()
    {
        #ifdef STM32F4
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
        #else 
            RCC->AHBENR |= RCC_AHBPeriph_CRC;
        #endif 

        inited = true;
    }
    // 硬件实现的Crc
    uint HardCrc(const void *buf, int len, uint crc)
    {
        if (!inited)
            Init();

        CRC_ResetDR();
        // STM32的初值是0xFFFFFFFF，而软Crc初值是0
        CRC->DR = __REV(crc ^ 0xFFFFFFFF);
        //CRC->DR = 0xFFFFFFFF;
        uint *ptr = (uint*)buf;
        len >>= 2;
        while (len-- > 0)
        {
            CRC->DR = __REV(*ptr++); // 字节顺序倒过来,注意不是位序,不是用__RBIT指令
        }
        return CRC->DR;
        /*byte* ptr = (byte*)buf;
        while(len-- > 0)
        {
        CRC->DR = (uint)*ptr++;
        }
        return CRC->DR;*/

        /*CRC_CalcBlockCRC(ptr, len);
        return CRC_GetCRC();*/

        //UINT32 rs = CRC->DR;
        //return c_CRCTable[ ((crc >> 24) ^ rs) & 0xFF ] ^ (crc << 8);
    }

    void TestCrc()
    {
        debug_printf("\r\n\r\n");
        debug_printf("TestCrc Start......\r\n");
        debug_printf("\r\n");

        uint size = ArrayLength(DataBuffer);

        // Sys.Crc是软校验，HardCrc是硬件实现，要求硬件实现的结果跟软件实现一致
        uint data = 0x12345678;
        uint crc = Sys.Crc((byte*) &data, 4, 0);
        uint crc2 = HardCrc(&data, 4, 0);
        Sys.ShowHex((byte*) &data, 4);
        debug_printf("\r\n\tSoftCrc:0x%08x  HardCrc:0x%08x \r\n", crc, crc2);
        // 无初值时，两者一样

        uint temp = crc;
        // 试试二次计算Crc
        crc = Sys.Crc((byte*) &crc, 4, 0);
        crc2 = HardCrc(&crc2, 4, 0);
        Sys.ShowHex((byte*) &temp, 4);
        debug_printf("\r\n\t");
        debug_printf("SoftCrc:0x%08x  HardCrc:0x%08x \r\n", crc, crc2);
        // 结果相同，但都不是0

        // 连续测试。构建8字节，前面是data，后面是前面的crc
        ulong data2 = temp;
        data2 <<= 32;
        data2 += data;
        crc = Sys.Crc((byte*) &data2, 8, 0);
        crc2 = HardCrc(&data2, 8, 0);
        Sys.ShowHex((byte*) &data2, 8);
        debug_printf("\r\n\t");
        debug_printf("SoftCrc:0x%08x  HardCrc:0x%08x \r\n", crc, crc2);
        // 结果相同，但都不是0

        // 实际应用中，先计算数据的校验，然后接着附加校验码部分，跟直接连续计算效果应该一致
        // 实际上就是数字为初值，对它自身进行校验码计算
        crc = Sys.Crc((byte*) &temp, 4, data);
        crc2 = HardCrc(&temp, 4, data);
        Sys.ShowHex((byte*) &temp, 4);
        debug_printf(" <= 0x%08x\r\n\t", data);
        debug_printf("SoftCrc:0x%08x  HardCrc:0x%08x \r\n", crc, crc2);
        // 结果不同，HardCrc结果跟8字节测试相同

        crc = Sys.Crc((byte*) &temp, 4, temp);
        crc2 = HardCrc(&temp, 4, temp);
        Sys.ShowHex((byte*) &temp, 4);
        debug_printf(" <= 0x%08x\r\n\t", temp);
        debug_printf("SoftCrc:0x%08x  HardCrc:0x%08x \r\n", crc, crc2);
        // 结果不同，SoftCrc结果跟8字节测试相同

        // 对大数据块进行校验
        debug_printf("\r\n");

        crc = Sys.Crc((byte*)DataBuffer, size *4, 0);
        crc2 = HardCrc(DataBuffer, size *4, 0);
        Sys.ShowHex((byte*)DataBuffer, 0x20);
        debug_printf("\r\n\t");
        debug_printf("SoftCrc:0x%08x  HardCrc:0x%08x \r\n", crc, crc2);
        // 无初值时，两者一样

        temp = crc;

        // 实际应用中，先计算数据的校验，然后接着附加校验码部分
        crc = Sys.Crc((byte*) &temp, 4, temp);
        crc2 = HardCrc((byte*) &temp, 4, temp);
        Sys.ShowHex((byte*) &temp, 4);
        debug_printf(" <= 0x%08x\r\n\t", temp);
        debug_printf("SoftCrc:0x%08x  HardCrc:0x%08x \r\n", crc, crc2);
        // 有初值时，两者不一样

        // 增量计算CRC
        crc = Sys.Crc((byte*)DataBuffer, size *4, 0);
        temp = crc;
        crc = Sys.Crc((byte*) &crc, 4, crc);
        crc2 = HardCrc(DataBuffer, size *4, 0);
        crc2 = HardCrc((byte*) &crc2, 4, crc2);
        Sys.ShowHex((byte*)DataBuffer, 0x20);
        debug_printf(" <= 0x%08x\r\n\t", temp);
        debug_printf("SoftCrc:0x%08x  HardCrc:0x%08x \r\n", crc, crc2);

        // 测试Crc16，数据和crc部分一起计算crc16，结果为0
        debug_printf("\r\n");
        byte data16[] = 
        {
            0x01, 0x08, 0x00, 0x00
        };
        ushort crc16 = Sys.Crc16(data16, 4);
        debug_printf("Sys.Crc16(#%08x) = 0x%04x\r\n", __REV(*(uint*)data16), crc16);
        ushort crc17 = Sys.Crc16((byte*) &crc16, 2, crc16);
        debug_printf("Sys.Crc16(#%08x, 0x%04x) = 0x%04x\r\n", __REV(*(uint*)data16), crc16, crc17);

        debug_printf("\r\n");
        debug_printf("TestCrc Finish!\r\n");
    }

#endif 
