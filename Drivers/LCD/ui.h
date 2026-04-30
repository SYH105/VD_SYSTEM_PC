#ifndef UI_H
#define UI_H

#include "stm32f4xx_hal.h"

// =========================
// PC MODE 상태 정의
// =========================
typedef enum {
    PC_WAIT = 0,
    PC_CONNECTED,
    PC_MEASURING
} PCState;


void UI_DrawMainMenu(uint8_t selected);
void UI_DrawMeasureSlots(uint8_t selected, uint8_t *slot_used);
void UI_DrawSaveSlots(uint8_t selected, uint8_t *slot_used);
void UI_DrawLoadSlots(uint8_t selected, uint8_t *slot_used);
void UI_DrawDeletePopup(uint8_t slot);
void UI_DrawSettingsMenu(uint8_t selected);
void UI_DrawLegend(void);
void UI_DrawPCMode(void);
void UI_UpdatePCStatus(uint8_t state);
void UI_UpdatePCAnimation(uint8_t state);
#endif
