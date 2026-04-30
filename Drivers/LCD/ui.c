// ============================================================
// UI Screen Drawing Module
// ------------------------------------------------------------
// - 메인 메뉴, 슬롯 선택, 설정 화면 등 UI 구성 담당
// - 텍스트, 리스트, 선택 상태 표시
// - 실제 그래프 출력은 lcd.c에서 담당
// ============================================================

#include "ui.h"
#include "lcd_text.h"
#include "ili9341.h"
#include "fonts.h"
#include <stdio.h>
#include "settings.h"


// ============================================================
// MAIN MENU 화면
// ------------------------------------------------------------
// - 시스템 시작 시 첫 화면
// - MEASURE / SAVE / LOAD / SETTINGS 선택 가능
// - 선택된 메뉴 강조 표시
// ============================================================
void UI_DrawMainMenu(uint8_t selected)
{
    // 화면 클리어
    ILI9341_FillScreen(BLACK);

    // =========================
    // 1. 타이틀 영역
    // =========================
    LCD_DrawString(40, 20, "VIBRATION", Font_11x18, CYAN);
    LCD_DrawString(40, 40, "ANALYZER", Font_11x18, CYAN);

    // 구분선
    ILI9341_DrawHLine(20, 65, 280, GRAY);

    // =========================
    // 2. 메뉴 리스트
    // =========================
    char *menu[] = {
        "MEASURE",
        "SAVE DATA",
        "LOAD DATA",
        "SETTINGS",
        "PC MODE"
    };

    for(int i = 0; i < 5; i++)
    {
        uint16_t y = 85 + i * 25;   // 🔥 간격 줄임

        if(i == selected)
        {
            // 선택된 항목 강조
            LCD_DrawString(30, y, ">", Font_11x18, YELLOW);
            LCD_DrawString(50, y, menu[i], Font_11x18, YELLOW);

            // 강조 바
            ILI9341_DrawHLine(20, y + 20, 280, DARKGREY);
        }
        else
        {
            LCD_DrawString(50, y, menu[i], Font_11x18, WHITE);
        }
    }

    // =========================
    // 3. 하단 안내
    // =========================
    ILI9341_DrawHLine(20, 210, 280, GRAY);

    LCD_DrawString(20, 220, "UP/DOWN: MOVE", Font_7x10, GREEN);
    LCD_DrawString(180, 220, "SEL: SELECT", Font_7x10, GREEN);
}

// ============================================================
// MEASURE 슬롯 선택 화면
// ------------------------------------------------------------
// - 비교 기준이 될 FFT 데이터를 선택
// - 저장된 슬롯만 선택 가능 (활성 표시)
// - 선택된 슬롯 강조 표시
// ============================================================
void UI_DrawMeasureSlots(uint8_t selected, uint8_t *slot_used)
{
    ILI9341_FillScreen(BLACK);

    // =========================
    // 1. TITLE
    // =========================
    LCD_DrawString(70, 10, "SELECT DATA", Font_16x26, GREEN);

    // 구분선
    ILI9341_DrawHLine(0, 40, 240, DARKGREY);

    // =========================
    // 2. LIST
    // =========================
    for(int i = 0; i < 10; i++)
    {
        int y = 50 + i * 14;

        char buf[10];
        sprintf(buf, "Slot%d", i+1);

        // 선택 배경
        if(i == selected)
        {
            ILI9341_FillRect(0, y-2, 240, 14, DARKBLUE);
        }

        // 슬롯 상태 표시
        if(slot_used[i])
        {
            LCD_DrawString(10, y, buf, Font_7x10, WHITE);
        }
        else
        {
            LCD_DrawString(10, y, buf, Font_7x10, DARKGREY);
        }
    }

    // =========================
    // 3. FOOTER
    // =========================
    ILI9341_DrawHLine(0, 200, 240, DARKGREY);

    LCD_DrawString(10, 210, "UP/DOWN: MOVE", Font_7x10, GREEN);
    LCD_DrawString(10, 220, "SEL: SELECT", Font_7x10, GREEN);
    LCD_DrawString(120, 220, "MENU: BACK", Font_7x10, GREEN);
}

// ============================================================
// SAVE 슬롯 선택 화면
// ------------------------------------------------------------
// - 측정 완료된 FFT 데이터를 저장할 슬롯 선택
// - 사용 중인 슬롯은 "*" 표시
// - 선택된 슬롯 강조 표시
// ============================================================
void UI_DrawSaveSlots(uint8_t selected, uint8_t *slot_used)
{
    ILI9341_FillScreen(BLACK);

    // =========================
    // 1. TITLE
    // =========================
    LCD_DrawString(70, 10, "Save Data", Font_16x26, CYAN);

    // 구분선
    ILI9341_DrawHLine(0, 40, 240, DARKGREY);

    // =========================
    // 2. LIST
    // =========================
    for(int i = 0; i < 10; i++)
    {
        int y = 50 + i * 14;   // 🔥 간격 줄임

        char buf[12];
        sprintf(buf, "Slot %d", i+1);

        // 선택 배경
        if(i == selected)
        {
            ILI9341_FillRect(0, y-1, 240, 12, DARKBLUE);  // 🔥 높이도 축소
        }

        // 선택 표시
        if(i == selected)
        {
            LCD_DrawString(10, y, ">", Font_7x10, YELLOW);
        }

        // 텍스트 색
        uint16_t text_color = (i == selected) ? YELLOW : WHITE;

        // 슬롯 이름 (폰트 축소)
        LCD_DrawString(25, y, buf, Font_7x10, text_color);

        // 저장 여부 표시
        if(slot_used[i])
        {
            LCD_DrawString(100, y, "*", Font_7x10, GREEN);
        }
    }

    // =========================
    // 3. FOOTER
    // =========================
    ILI9341_DrawHLine(0, 200, 240, DARKGREY);

    LCD_DrawString(10, 210, "UP/DOWN: MOVE", Font_7x10, GREEN);
    LCD_DrawString(10, 220, "SEL: SAVE", Font_7x10, GREEN);
    LCD_DrawString(120, 220, "MENU: BACK", Font_7x10, GREEN);
}

// ============================================================
// LOAD 슬롯 리스트 화면
// ------------------------------------------------------------
// - 저장된 FFT 데이터를 선택하여 조회
// - 선택된 슬롯 강조 표시
// - LONG SELECT → 삭제 모드 진입
// ============================================================
void UI_DrawLoadSlots(uint8_t selected, uint8_t *slot_used)
{
    ILI9341_FillScreen(BLACK);

    // =========================
    // 1. TITLE
    // =========================
    LCD_DrawString(70, 10, "LOAD DATA", Font_16x26, GREEN);

    // 구분선
    ILI9341_DrawHLine(0, 40, 240, DARKGREY);

    // =========================
    // 2. LIST
    // =========================
    for(int i = 0; i < 10; i++)
    {
        int y = 50 + i * 14;

        char buf[10];
        sprintf(buf, "Slot%d", i+1);

        // 선택 배경
        if(i == selected)
        {
            ILI9341_FillRect(0, y-2, 240, 14, DARKBLUE);
        }

        // 슬롯 상태 표시
        if(slot_used[i])
        {
            LCD_DrawString(10, y, buf, Font_7x10, WHITE);
        }
        else
        {
            LCD_DrawString(10, y, buf, Font_7x10, DARKGREY);
        }
    }

    // =========================
    // 3. FOOTER
    // =========================
    ILI9341_DrawHLine(0, 200, 240, DARKGREY);

    LCD_DrawString(10, 210, "UP/DOWN: MOVE", Font_7x10, GREEN);
    LCD_DrawString(10, 220, "SEL: LOAD", Font_7x10, GREEN);
    LCD_DrawString(120, 210, "MENU: BACK", Font_7x10, GREEN);
    LCD_DrawString(120, 220, "HOLD SEL: DELETE", Font_7x10, RED);
}

// ============================================================
// DELETE 확인 팝업
// ------------------------------------------------------------
// - 선택한 슬롯 삭제 여부 확인
// - SEL: 삭제 수행
// - MENU: 취소 후 리스트 복귀
// ============================================================
void UI_DrawDeletePopup(uint8_t slot)
{
    // 팝업 배경 (검정 박스)
    ILI9341_FillRect(40, 80, 160, 100, BLACK);
    ILI9341_DrawRect(40, 80, 160, 100, WHITE);

    char buf[20];

    // 타이틀
    LCD_DrawString(75, 90, "DELETE?", Font_7x10, RED);

    // 슬롯 번호
    sprintf(buf, "SLOT %d", slot + 1);
    LCD_DrawString(75, 110, buf, Font_7x10, WHITE);

    // 안내
    LCD_DrawString(60, 140, "SEL: YES", Font_7x10, GREEN);
    LCD_DrawString(60, 155, "MENU: NO", Font_7x10, GREEN);
}

// ============================================================
// SETTINGS 화면
// ------------------------------------------------------------
// - FFT SCALE 설정 (그래프 크기 조절)
// - AVG COUNT 설정 (평균 횟수)
// - BUZZER ON/OFF 설정
// - 선택된 항목 강조 표시
// ============================================================
void UI_DrawSettingsMenu(uint8_t selected)
{
    ILI9341_FillScreen(BLACK);

    // =========================
    // 1. TITLE
    // =========================
    LCD_DrawString(60, 20, "SETTINGS", Font_16x26, CYAN);

    ILI9341_DrawHLine(20, 60, 280, GRAY);

    // =========================
    // 2. MENU LIST
    // =========================
    char *menu[] = {
        "FFT SCALE",
        "AVG COUNT",
        "BUZZER"
    };

    for(int i = 0; i < 3; i++)
    {
        int y = 90 + i * 30;

        char buf[20];

        // =========================
        // 메뉴별 표시
        // =========================
        if(i == 0)
        {
        	sprintf(buf, "FFT SCALE  %d", (int)g_settings.fft_scale);
        }
        else if(i == 1)
        {
            sprintf(buf, "AVG COUNT  %d", g_settings.avg_count);
        }
        else if(i == 2)
        {
            if(g_settings.buzzer_enable)
                sprintf(buf, "BUZZER  ON");
            else
                sprintf(buf, "BUZZER  OFF");
        }
        else
        {
            sprintf(buf, "%s", menu[i]);
        }

        // =========================
        // 선택 강조
        // =========================
        if(i == selected)
        {
            LCD_DrawString(30, y, ">", Font_11x18, YELLOW);
            LCD_DrawString(50, y, buf, Font_11x18, YELLOW);
            ILI9341_DrawHLine(20, y + 22, 280, DARKGREY);
        }
        else
        {
            LCD_DrawString(50, y, buf, Font_11x18, WHITE);
        }
    }

    // =========================
    // 3. FOOTER
    // =========================
    ILI9341_DrawHLine(20, 210, 280, GRAY);

    LCD_DrawString(10, 210, "UP/DOWN: MOVE", Font_7x10, GREEN);
    LCD_DrawString(10, 220, "SEL: CHANGE", Font_7x10, GREEN);
    LCD_DrawString(120, 220, "MENU: BACK", Font_7x10, GREEN);
}

// ============================================================
// MEASURE 그래프 설명 표시
// ------------------------------------------------------------
// - 상단: BASE (저장된 기준 데이터)
// - 하단: LIVE (실시간 측정 데이터)
// ============================================================
void UI_DrawLegend(void)
{
    // BASE
    ILI9341_FillRect(10, 26, 10, 10, BLUE);
    LCD_DrawString(25, 26, "BASE", Font_7x10, WHITE);

    // LIVE
    ILI9341_FillRect(100, 26, 10, 10, GREEN);
    LCD_DrawString(115, 26, "LIVE", Font_7x10, WHITE);
}

// ============================================================
// PC MODE 화면 구성
// ------------------------------------------------------------
// - PC와 UART 통신을 위한 대기 화면
// - 초기 상태: "Connect to PC..." 표시
// - 상태 영역 박스를 통해 UI 구분
// - 하단에 UART 명령 안내 및 EXIT 가이드 표시
// ============================================================
void UI_DrawPCMode(void)
{
    ILI9341_FillScreen(BLACK);

    // =========================
    // 1. TITLE
    // =========================
    LCD_DrawString(60, 20, "PC MODE", Font_16x26, CYAN);

    // 구분선
    ILI9341_DrawHLine(20, 60, 280, GRAY);


    // 초기 상태 텍스트
    LCD_DrawString(40, 120, "Connect to PC...", Font_11x18, WHITE);

    // =========================
    // 2. 설명 텍스트
    // =========================
    LCD_DrawString(30, 180, "UART CMD: HELLO / START / STOP", Font_7x10, DARKGREY);

    // =========================
    // 3. 하단 가이드
    // =========================
    ILI9341_DrawHLine(20, 210, 280, GRAY);

    LCD_DrawString(40, 220, "MENU: EXIT", Font_7x10, GREEN);
}

// ============================================================
// PC MODE 상태 텍스트 업데이트
// ------------------------------------------------------------
// - PC 연결 상태에 따라 텍스트 표시
// - PC_WAIT       → 연결 대기
// - PC_CONNECTED  → PC 연결 완료
// - PC_MEASURING  → 데이터 측정/전송 중
// - 호출 시 해당 영역을 클리어 후 텍스트 재출력
// ============================================================
void UI_UpdatePCStatus(uint8_t state)
{
    ILI9341_FillRect(0, 110, 320, 40, BLACK);

    switch(state)
    {
        case PC_WAIT:
            LCD_DrawString(30, 120, "Connect to PC...", Font_11x18, WHITE);
            break;

        case PC_CONNECTED:
            LCD_DrawString(40, 120, "Connected to PC", Font_11x18, GREEN);
            break;

        case PC_MEASURING:
            LCD_DrawString(60, 120, "Measuring...", Font_11x18, YELLOW);
            break;
    }
}

// ============================================================
// PC MODE 상태 애니메이션 업데이트
// ------------------------------------------------------------
// - 1초 주기로 텍스트 뒤에 "..." 애니메이션 적용
// - PC_WAIT / PC_MEASURING 상태에서만 동작
// - dot(0~3)을 이용해 점 개수 변경
// - 해당 영역을 주기적으로 갱신하여 동적 표현
// ============================================================
void UI_UpdatePCAnimation(uint8_t state)
{
    static uint32_t last_time = 0;
    static uint8_t dot = 0;

    uint32_t now = HAL_GetTick();

    if(now - last_time < 1000) return;

    last_time = now;

    dot++;
    if(dot > 3) dot = 0;

    char buf[30];

    ILI9341_FillRect(0, 110, 320, 40, BLACK);

    if(state == PC_WAIT)
    {
        sprintf(buf, "Connect to PC%.*s", dot, "...");
        LCD_DrawString(30, 120, buf, Font_11x18, WHITE);
    }
    else if(state == PC_MEASURING)
    {
        sprintf(buf, "Measuring%.*s", dot, "...");
        LCD_DrawString(60, 120, buf, Font_11x18, YELLOW);
    }
}
