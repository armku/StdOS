#ifndef __RX8025T_H
#define __RX8025T_H

#include "stdint.h"
#include "data_handler.h"

extern TIME_T rx8025Time;
extern uint8_t rx8025tIntFlag;
//外部接口
void RX8025T_Init(void);
uint8_t RX8025T_CheckDevice(void);
uint8_t RX8025T_ReadTime(void);
uint8_t RX8025T_WriteTime(void);
uint8_t RX8025T_ReadTest(void);
uint8_t RX8025T_1sOutput_Start(void);
uint8_t RX8025T_1sOutput_Stop(void);
void RX8025T_IntTask(void);

#endif
