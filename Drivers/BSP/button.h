#ifndef BUTTON_H
#define BUTTON_H

#include "stm32f4xx_hal.h"

// 버튼 종류
typedef enum {
    BTN_MENU = 0,
    BTN_UP,
    BTN_DOWN,
    BTN_SELECT,
    BTN_NONE
} ButtonType;

// 이벤트 (STATE MACHINE용)
typedef enum {
    EVENT_NONE = 0,
    EVENT_MENU,
    EVENT_UP,
    EVENT_DOWN,
    EVENT_SELECT,
	EVENT_SELECT_LONG
} InputEvent;


// 이벤트 반환 (한 번만 발생)
InputEvent Button_GetEvent(void);

#endif
