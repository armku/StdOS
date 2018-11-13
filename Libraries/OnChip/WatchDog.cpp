#include "WatchDog.h"
#include "Kernel\Sys.h"

#if defined(STM32F4)
#include "stm32f4xx.h"
#include "Platform\STM32F4\Pin_STM32F4.h"
#elif defined(STM32F2)
#include "stm32f2xx.h"
#elif defined(STM32F1)
#include "stm32f10x.h"
#include "Platform/STM32F1/Pin_STM32F1.h"
#elif defined(STM32F3)
#include "stm32f3xx.h"
#elif defined(STM32F0)
#include "stm32f0xx.h"
#include "Platform/STM32F0/Pin_STM32F0.h"
#elif defined(GD32F150)
#include "stm32f0xx.h"
#else
#error "����Keil��Ŀ����C/C++ҳ����оƬƽ̨����STM32F0/STM32F1/STM32F2/STM32F3/STM32F4/GD32F150"
#endif

WatchDog::WatchDog()
{
	this->Timeout = 3000;
}

WatchDog* cur;
WatchDog& WatchDog::Current()
{
	return *cur;
}
void WatchDog::FeedDogTask(void* param)
{
	WatchDog *dog = (WatchDog*)param;
	dog->Feed();
}
// �򿪿��Ź����ι��ʱ��26208ms��Ĭ��2000ms
void WatchDog::Start(uint32_t msTimeOut, uint32_t msFeed)
{
	cur = new WatchDog();
	cur->Config(msTimeOut);
	Sys.AddTask(FeedDogTask, &WatchDog::Current(), 10, msFeed, "FeedDog");
}

int OpenWatchDog(void)
{
	//  RCC_LSICmd(1);
	//  if ( RCC_GetFlagStatus(125) )
	//    RCC_ClearFlag();
	//  return IWDG_WriteAccessCmd(21845);
	return 0;
}
// ι��
void WatchDog::Feed()
{
	IWDG_ReloadCounter();
}
// ���ÿ��Ź�ι������ʱ�䣬������ʱ�佫����MCU
bool WatchDog::Config(uint32_t ms)
{
#if defined STM32F0
	if (ms == 0)
	{
		debug_printf("WatchDog msTimeout %dms must larger than 0ms\r\n", ms);
		return false;
	}
	RCC_LSICmd(ENABLE);
	/* ���ϵͳ�Ƿ��IWDG���ûָ� */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		/* ������ñ�ʶ */
		RCC_ClearFlag();
	}
	/* ��IWDG_PR��IWDG_RLR�Ĵ�����д���� */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	uint8_t pre = IWDG_Prescaler_4;
	int mul = 4;
	// ������12λ 0~0x0FFF����reload = ms/1000 / (1/(LSI/mul)) = ms * LSI / (mul*1000) = ms * 40 / mul
	// ���ǵ�reload����Ŀ��ܣ�ÿ�ַ�Ƶ���ms = reload * mul / 40 ~= 102 * mul
	int i = 0;
	/*
	#define IWDG_Prescaler_4            ((uint8_t)0x00)
	#define IWDG_Prescaler_8            ((uint8_t)0x01)
	#define IWDG_Prescaler_16           ((uint8_t)0x02)
	#define IWDG_Prescaler_32           ((uint8_t)0x03)
	#define IWDG_Prescaler_64           ((uint8_t)0x04)
	#define IWDG_Prescaler_128          ((uint8_t)0x05)
	#define IWDG_Prescaler_256          ((uint8_t)0x06)
	 */
	for (i = IWDG_Prescaler_4; i <= IWDG_Prescaler_256; i++)
	{
		pre = i;
		mul = 1 << (i + 2);

		// �ж��Ƿ��ڷ�Χ֮��
		if (ms * 40 / mul < 0x0FFF)
			break;
	}
	if (i > IWDG_Prescaler_256)
	{
		debug_printf("WatchDog msTimeout must smaller than %dms\r\n", 0x0FFF * 256 / 40);
		return false;
	}
	IWDG_SetPrescaler(pre);
	/*if(ms < (0x0FFF * 32 / 40))
	{
	// IWDG������ʱ��: LSI/32=40KHz/32=1250Hz��ÿ����0.8ms
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	}
	else
	{
	// IWDG������ʱ��: LSI/64=40KHz/64=625Hz��ÿ����0.4ms
	IWDG_SetPrescaler(IWDG_Prescaler_64);

	// ֱ�ӳ���2�����治���ظ�����
	ms >>= 2;
	}*/

	/* ���ü���������ֵΪ��ʱʱ��
	Counter Reload Value = ms / 1000 / IWDG������ʱ������
	= ms / 1000 / (1/(LSI/mul))
	= ms * LSI / (mul * 1000)
	= ms * 40k / (mul * 1000)
	= ms * 40 / mul
	 */
	IWDG_SetReload(ms * 40 / mul);

	/* ���� IWDG ������ */
	IWDG_ReloadCounter();

	/* �� IWDG (LSI����Ӳ����) */
	IWDG_Enable();

	Timeout = ms;

	return true;
#elif defined STM32F1
	if (ms == 0)
	{
		debug_printf("WatchDog msTimeout %dms must larger than 0ms\r\n", ms);
		return false;
	}
	RCC_LSICmd(ENABLE);
	/* ���ϵͳ�Ƿ��IWDG���ûָ� */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		/* ������ñ�ʶ */
		RCC_ClearFlag();
	}
	/* ��IWDG_PR��IWDG_RLR�Ĵ�����д���� */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	uint8_t pre = IWDG_Prescaler_4;
	int mul = 4;
	// ������12λ 0~0x0FFF����reload = ms/1000 / (1/(LSI/mul)) = ms * LSI / (mul*1000) = ms * 40 / mul
	// ���ǵ�reload����Ŀ��ܣ�ÿ�ַ�Ƶ���ms = reload * mul / 40 ~= 102 * mul
	int i = 0;
	/*
	#define IWDG_Prescaler_4            ((uint8_t)0x00)
	#define IWDG_Prescaler_8            ((uint8_t)0x01)
	#define IWDG_Prescaler_16           ((uint8_t)0x02)
	#define IWDG_Prescaler_32           ((uint8_t)0x03)
	#define IWDG_Prescaler_64           ((uint8_t)0x04)
	#define IWDG_Prescaler_128          ((uint8_t)0x05)
	#define IWDG_Prescaler_256          ((uint8_t)0x06)
	 */
	for (i = IWDG_Prescaler_4; i <= IWDG_Prescaler_256; i++)
	{
		pre = i;
		mul = 1 << (i + 2);

		// �ж��Ƿ��ڷ�Χ֮��
		if (ms * 40 / mul < 0x0FFF)
			break;
	}
	if (i > IWDG_Prescaler_256)
	{
		debug_printf("WatchDog msTimeout must smaller than %dms\r\n", 0x0FFF * 256 / 40);
		return false;
	}
	IWDG_SetPrescaler(pre);
	/*if(ms < (0x0FFF * 32 / 40))
	{
	// IWDG������ʱ��: LSI/32=40KHz/32=1250Hz��ÿ����0.8ms
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	}
	else
	{
	// IWDG������ʱ��: LSI/64=40KHz/64=625Hz��ÿ����0.4ms
	IWDG_SetPrescaler(IWDG_Prescaler_64);

	// ֱ�ӳ���2�����治���ظ�����
	ms >>= 2;
	}*/

	/* ���ü���������ֵΪ��ʱʱ��
	Counter Reload Value = ms / 1000 / IWDG������ʱ������
	= ms / 1000 / (1/(LSI/mul))
	= ms * LSI / (mul * 1000)
	= ms * 40k / (mul * 1000)
	= ms * 40 / mul
	 */
	IWDG_SetReload(ms * 40 / mul);

	/* ���� IWDG ������ */
	IWDG_ReloadCounter();

	/* �� IWDG (LSI����Ӳ����) */
	IWDG_Enable();

	Timeout = ms;

	return true;
#elif defined STM32F4
	if (ms == 0)
	{
		debug_printf("WatchDog msTimeout %dms must larger than 0ms\r\n", ms);
		return false;
	}
	RCC_LSICmd(ENABLE);
	/* ���ϵͳ�Ƿ��IWDG���ûָ� */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		/* ������ñ�ʶ */
		RCC_ClearFlag();
	}
	/* ��IWDG_PR��IWDG_RLR�Ĵ�����д���� */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	uint8_t pre = IWDG_Prescaler_4;
	int mul = 4;
	// ������12λ 0~0x0FFF����reload = ms/1000 / (1/(LSI/mul)) = ms * LSI / (mul*1000) = ms * 40 / mul
	// ���ǵ�reload����Ŀ��ܣ�ÿ�ַ�Ƶ���ms = reload * mul / 40 ~= 102 * mul
	int i = 0;
	/*
	#define IWDG_Prescaler_4            ((uint8_t)0x00)
	#define IWDG_Prescaler_8            ((uint8_t)0x01)
	#define IWDG_Prescaler_16           ((uint8_t)0x02)
	#define IWDG_Prescaler_32           ((uint8_t)0x03)
	#define IWDG_Prescaler_64           ((uint8_t)0x04)
	#define IWDG_Prescaler_128          ((uint8_t)0x05)
	#define IWDG_Prescaler_256          ((uint8_t)0x06)
	 */
	for (i = IWDG_Prescaler_4; i <= IWDG_Prescaler_256; i++)
	{
		pre = i;
		mul = 1 << (i + 2);

		// �ж��Ƿ��ڷ�Χ֮��
		if (ms * 40 / mul < 0x0FFF)
			break;
	}
	if (i > IWDG_Prescaler_256)
	{
		debug_printf("WatchDog msTimeout must smaller than %dms\r\n", 0x0FFF * 256 / 40);
		return false;
	}
	IWDG_SetPrescaler(pre);
	/*if(ms < (0x0FFF * 32 / 40))
	{
	// IWDG������ʱ��: LSI/32=40KHz/32=1250Hz��ÿ����0.8ms
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	}
	else
	{
	// IWDG������ʱ��: LSI/64=40KHz/64=625Hz��ÿ����0.4ms
	IWDG_SetPrescaler(IWDG_Prescaler_64);

	// ֱ�ӳ���2�����治���ظ�����
	ms >>= 2;
	}*/

	/* ���ü���������ֵΪ��ʱʱ��
	Counter Reload Value = ms / 1000 / IWDG������ʱ������
	= ms / 1000 / (1/(LSI/mul))
	= ms * LSI / (mul * 1000)
	= ms * 40k / (mul * 1000)
	= ms * 40 / mul
	 */
	IWDG_SetReload(ms * 40 / mul);

	/* ���� IWDG ������ */
	IWDG_ReloadCounter();

	/* �� IWDG (LSI����Ӳ����) */
	IWDG_Enable();

	Timeout = ms;

	return true;
#endif
}
// ���Ź��޷��رգ�ֻ������һ�����ֵ
void WatchDog::ConfigMax()
{
#if defined STM32F0
	/* ��IWDG_PR��IWDG_RLR�Ĵ�����д���� */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	// �������Ź��޷��ر�
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	IWDG_SetReload(0x0FFF);
	IWDG_ReloadCounter();
#elif defined STM32F1
	/* ��IWDG_PR��IWDG_RLR�Ĵ�����д���� */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	// �������Ź��޷��ر�
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	IWDG_SetReload(0x0FFF);
	IWDG_ReloadCounter();
#elif defined STM32F4
	/* ��IWDG_PR��IWDG_RLR�Ĵ�����д���� */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	// �������Ź��޷��ر�
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	IWDG_SetReload(0x0FFF);
	IWDG_ReloadCounter();
#endif
}
