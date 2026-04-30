#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "stm32f4xx_hal.h"

// 기본 제어
void Buzzer_Init(void);
void Buzzer_On(void);
void Buzzer_Off(void);
void Buzzer_Toggle(void);

// 간단 Beep
void Buzzer_Beep(uint16_t ms);          // 1회 beep
void Buzzer_Beep2(uint16_t ms);         // 2회 beep

#endif
