#include "buzzer.h"

// ===== GPIO 설정 =====
// 👉 너 회로 기준: PC4

#define BUZZER_PORT GPIOC
#define BUZZER_PIN  GPIO_PIN_4

// ===== 내부 함수 =====
static inline void BUZZER_WRITE(GPIO_PinState state)
{
    HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, state);
}

// ===== API =====

void Buzzer_Init(void)
{
    // 초기 OFF
    BUZZER_WRITE(GPIO_PIN_RESET);
}

void Buzzer_On(void)
{
    BUZZER_WRITE(GPIO_PIN_SET);
}

void Buzzer_Off(void)
{
    BUZZER_WRITE(GPIO_PIN_RESET);
}

void Buzzer_Toggle(void)
{
    HAL_GPIO_TogglePin(BUZZER_PORT, BUZZER_PIN);
}

// ===== Beep 함수 =====

void Buzzer_Beep(uint16_t ms)
{
    Buzzer_On();
    HAL_Delay(ms); //RTOS로 구조 변경 시 osDelay(ms);
    Buzzer_Off();
}

void Buzzer_Beep2(uint16_t ms)
{
    for(int i = 0; i < 2; i++)
    {
        Buzzer_On();
        HAL_Delay(ms);

        Buzzer_Off();
        HAL_Delay(ms);
    }
}
