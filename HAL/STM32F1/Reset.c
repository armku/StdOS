#include "Reset.h"

//ϵͳ��λ����
__asm void SystemReset(void)
{
 	MOV R0, #1           //; 
 	MSR FAULTMASK, R0    //; ���FAULTMASK ��ֹһ���жϲ���
 	LDR R0, =0xE000ED0C  //;
 	LDR R1, =0x05FA0004  //; 
 	STR R1, [R0]         //; ϵͳ�����λ   
 
deadloop
    B deadloop           //; ��ѭ��ʹ�������в�������Ĵ���
}

void SYS_RST(void)
{
	SystemReset();
}
