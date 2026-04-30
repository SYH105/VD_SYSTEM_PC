#include "button.h"

// =========================
// GPIO 정의
// =========================
#define BTN_PORT GPIOC

#define BTN_MENU_PIN   GPIO_PIN_0
#define BTN_UP_PIN     GPIO_PIN_1
#define BTN_DOWN_PIN   GPIO_PIN_2
#define BTN_SELECT_PIN GPIO_PIN_3

// =========================
// 디바운스 설정
// =========================
#define DEBOUNCE_TIME 50  // ms

#define LONG_PRESS_TIME 3000   // 3초
// =========================
// 내부 변수
// =========================
static uint32_t last_time = 0;
static uint8_t prev_state[4] = {0, 0, 0, 0};

static uint32_t select_press_time = 0;
static uint8_t select_pressed = 0;

// =========================
// raw 읽기 (눌림=1)
// =========================
static uint8_t Button_ReadRaw(ButtonType btn)
{
    uint16_t pin;

    switch(btn)
    {
        case BTN_MENU:   pin = BTN_MENU_PIN; break;
        case BTN_UP:     pin = BTN_UP_PIN; break;
        case BTN_DOWN:   pin = BTN_DOWN_PIN; break;
        case BTN_SELECT: pin = BTN_SELECT_PIN; break;
        default: return 0;
    }

    // Pull-up → LOW = 눌림
    return (HAL_GPIO_ReadPin(BTN_PORT, pin) == GPIO_PIN_RESET);
}

// =========================
// 이벤트 생성 (핵심)
// =========================
InputEvent Button_GetEvent(void)
{
    uint32_t now = HAL_GetTick();

    if(now - last_time < DEBOUNCE_TIME)
        return EVENT_NONE;

    for(int i = 0; i < 4; i++)
    {
        uint8_t current = Button_ReadRaw((ButtonType)i);

        if(i == BTN_SELECT)
        {
            if(current == 1 && select_pressed == 0)
            {
                select_pressed = 1;
                select_press_time = now;
            }
            else if(current == 0 && select_pressed == 1)
            {
                uint32_t press_time = now - select_press_time;
                select_pressed = 0;
                last_time = now;

                if(press_time >= LONG_PRESS_TIME)
                    return EVENT_SELECT_LONG;
                else
                    return EVENT_SELECT;
            }

            continue;
        }

        // 상승 에지 (안눌림 → 눌림)
        if(current == 1 && prev_state[i] == 0)
        {
            prev_state[i] = 1;
            last_time = now;

            switch(i)
            {
                case BTN_MENU:   return EVENT_MENU;
                case BTN_UP:     return EVENT_UP;
                case BTN_DOWN:   return EVENT_DOWN;
            }
        }

        // 버튼 떼면 상태 초기화
        if(current == 0)
        {
            prev_state[i] = 0;
        }
    }

    return EVENT_NONE;
}
