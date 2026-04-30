#include "lcd.h"
#include "ili9341.h"
#include "fonts.h"
#include "lcd_text.h"
#include <stdlib.h>
#include <stdio.h>
#include "buzzer.h"
#include "settings.h"

// =========================
// 내부 상태 변수
// =========================
#define GRID_COLOR 0x7BEF
static int prev_gauge = -1;


// ============================================================
// 내부 함수
// ============================================================

// Grid (격자) 그리기
// - FFT 그래프 배경용
// - 세로: 주파수 구간 (0~200Hz)
// - 가로: 크기 기준선
static void LCD_DrawGrid(void)
{
    // 세로 grid (25Hz → 8칸)
    for(int i = 1; i < 8; i++)
    {
        int x = GRAPH_X + (GRAPH_W * i) / 8;
        ILI9341_DrawVLine(x, GRAPH_Y, GRAPH_H, GRID_COLOR);
    }

    // 가로 grid (4등분)
    for(int i = 1; i < 4; i++)
    {
        int y = GRAPH_Y + (GRAPH_H * i) / 4;
        ILI9341_DrawHLine(GRAPH_X, y, GRAPH_W, GRID_COLOR);
    }
}


// ============================================================
// FFT 전체 그래프 (단일)
// ------------------------------------------------------------
// - SAVE / LOAD 화면에서 사용
// - GRAPH 영역 전체 사용
// - data: FFT magnitude 배열
// - size: FFT_SIZE / 2 (128)
// ============================================================
void LCD_DrawFFT(float *data, int size)
{
    // =========================
    // 1. 그래프 영역만 클리어
    // =========================
    ILI9341_FillRect(GRAPH_X+1, GRAPH_Y+1, GRAPH_W-2, GRAPH_H-2, BLACK);

    // =========================
    // 2. Grid 다시 그리기
    // =========================
    LCD_DrawGrid();

    // =========================
    // 3. FFT 그래프 그리기
    // =========================

    float max_val = g_settings.fft_scale;

    for(int i = 0; i < size; i++)
    {
        int x = GRAPH_X + (i * GRAPH_W) / size;

        // 값 스케일링
        float value = data[i];
        if(value > max_val) value = max_val;

        int bar_height = (int)((value / max_val) * GRAPH_H);

        int y = GRAPH_Y + GRAPH_H - bar_height;

        // 막대 그리기
        ILI9341_DrawVLine(x, y, bar_height, GREEN);
    }
}

// ============================================================
// FFT 영역 그래프 (분할 화면)
// ------------------------------------------------------------
// - MEASURE 화면에서 사용
// - 위: BASE (저장 데이터)
// - 아래: LIVE (실시간 데이터)
// - y_offset 기준으로 위치 결정
// - color로 그래프 구분 (BLUE / GREEN)
// ============================================================
void LCD_DrawFFT_Area(float *data, int size, int y_offset, uint16_t color)
{
    // =========================
    // 1. 그래프 내부 영역만 클리어
    // =========================
    ILI9341_FillRect(GRAPH_X + 1, y_offset + 1, GRAPH_W - 2, AREA_GRAPH_H - 2, BLACK);

    // =========================
    // 2. 세로 grid
    // =========================
    for(int i = 1; i < 8; i++)
    {
        int x = GRAPH_X + (GRAPH_W * i) / 8;
        ILI9341_DrawVLine(x, y_offset, AREA_GRAPH_H, GRID_COLOR);
    }

    // =========================
    // 3. 가로 grid
    // =========================
    for(int i = 1; i < 4; i++)
    {
        int y = y_offset + (AREA_GRAPH_H * i) / 4;
        ILI9341_DrawHLine(GRAPH_X, y, GRAPH_W, GRID_COLOR);
    }

    // =========================
    // 4. FFT 그래프 그리기
    // =========================
    float max_val = g_settings.fft_scale;

    for(int i = 0; i < size; i++)
    {
        int x = GRAPH_X + (i * GRAPH_W) / size;

        float value = data[i];
        if(value > max_val) value = max_val;

        int bar_height = (int)((value / max_val) * AREA_GRAPH_H);
        int y = y_offset + AREA_GRAPH_H - bar_height;

        ILI9341_DrawVLine(x, y, bar_height, color);
    }

    // =========================
    // 5. 테두리
    // =========================
    ILI9341_DrawRect(GRAPH_X, y_offset, GRAPH_W, AREA_GRAPH_H, WHITE);

    // =========================
    // 6. X축 Hz 표시
    // =========================
    int label_y = y_offset + AREA_GRAPH_H + 2;

    // 0Hz
    LCD_DrawString(GRAPH_X - 2, label_y, "0", Font_7x10, WHITE);

    // 50Hz
    LCD_DrawString(GRAPH_X + GRAPH_W/4 - 8, label_y, "50", Font_7x10, WHITE);

    // 100Hz
    LCD_DrawString(GRAPH_X + GRAPH_W/2 - 12, label_y, "100", Font_7x10, WHITE);

    // 150Hz
    LCD_DrawString(GRAPH_X + (GRAPH_W*3)/4 - 12, label_y, "150", Font_7x10, WHITE);

    // 200Hz
    LCD_DrawString(GRAPH_X + GRAPH_W - 28, label_y, "200Hz", Font_7x10, WHITE);
}

// ============================================================
// UI 기본 프레임 구성
// ------------------------------------------------------------
// - 화면 전체 초기화
// - 그래프 테두리 + Grid
// - X축 (Hz) 라벨 표시
// ============================================================
void LCD_DrawUIFrame(void)
{
    // 화면 클리어
    ILI9341_FillScreen(BLACK);

    // =========================
    // 1. 상단 상태 표시
    // =========================
    LCD_DrawString(10, 10, "X:0 Y:0 Z:0 MAX:0Hz", Font_7x10, WHITE);

    // =========================
    // 2. 그래프 테두리
    // =========================
    ILI9341_DrawRect(GRAPH_X, GRAPH_Y, GRAPH_W, GRAPH_H, WHITE);

    // =========================
    // 3. Grid (격자)
    // =========================
    LCD_DrawGrid();
    // =========================
    // 4. X축 라벨 (정렬 개선)
    // =========================

    int y = GRAPH_Y + GRAPH_H + 8;

    // 0Hz
    LCD_DrawString(GRAPH_X - 2, y, "0", Font_7x10, WHITE);

    // 50Hz
    LCD_DrawString(GRAPH_X + GRAPH_W/4 - 8, y, "50", Font_7x10, WHITE);

    // 100Hz
    LCD_DrawString(GRAPH_X + GRAPH_W/2 - 12, y, "100", Font_7x10, WHITE);

    // 150Hz
    LCD_DrawString(GRAPH_X + (GRAPH_W*3)/4 - 12, y, "150", Font_7x10, WHITE);

    // 200Hz
    LCD_DrawString(GRAPH_X + GRAPH_W - 28, y, "200Hz", Font_7x10, WHITE);
}

// ============================================================
// 상단 상태바 업데이트
// ------------------------------------------------------------
// 표시 항목:
// - X, Y, Z 가속도 값
// - 현재 FFT Peak 주파수
// ============================================================
void LCD_UpdateStatus(int16_t x, int16_t y, int16_t z, float peak_freq)
{
    char buf[50];
    // 상태바 영역 클리어
    ILI9341_FillRect(0, 0, 214, 20, BLACK);

    sprintf(buf, "X:%d Y:%d Z:%d MAX:%dHz", x, y, z, (int)peak_freq);
    LCD_DrawString(10, 10, buf, Font_7x10, WHITE);
}

// ============================================================
// 카운트다운 표시 (3초)
// ------------------------------------------------------------
// - MEASURE / SAVE 진입 시 사용
// - 값이 바뀔 때만 redraw (깜빡임 방지)
// ============================================================
void LCD_UpdateCount(int count)
{
    static int prev = -1;

    if(prev == count) return;   // 변화 없으면 그리지 않음
    prev = count;

    char buf[5];

    ILI9341_FillRect(250, 0, 70, 20, BLACK);
    sprintf(buf, "%d", count);
    LCD_DrawString(260, 10, buf, Font_7x10, YELLOW);
}

// ============================================================
// 상단 영역 클리어
// ------------------------------------------------------------
// - 상태바 / 카운트 영역 초기화
// ============================================================
void LCD_ClearTopBar(void)
{
    ILI9341_FillRect(0, 0, 320, 20, BLACK);
}

// ============================================================
// FFT 진행도 표시 (Progress Bar)
// ------------------------------------------------------------
// - avg_count 기준으로 진행률 계산
// - 0~10 단계 게이지
// - 증가할 때만 그려서 성능 최적화
// ============================================================
void LCD_DrawFFTProgress(int count)
{
	int gauge = (count * 10) / g_settings.avg_count;
    if(gauge > 10) gauge = 10;

    int x = 215;
    int y = 9;

    int total_w = 100;   // 전체 게이지 길이
    int h = 10;

    // 1칸당 너비
    int step_w = total_w / 10;

    // =========================
    // 1. 처음 한번만 테두리
    // =========================
    if(prev_gauge == -1)
    {
        // 전체 박스 (다운로드바 느낌)
        ILI9341_DrawRect(x, y, total_w, h, DARKGREY);
    }

    // =========================
    // 2. 변화 있을 때만 업데이트
    // =========================
    if(gauge != prev_gauge)
    {
        // 이전보다 늘어난 만큼만 채우기
        for(int i = prev_gauge; i < gauge; i++)
        {
            if(i >= 0)
            {
                ILI9341_FillRect(
                    x + i * step_w,
                    y,
                    step_w,
                    h,
                    GREEN
                );
            }
        }

        prev_gauge = gauge;
    }
}

// ============================================================
// FFT Progress 초기화
// ------------------------------------------------------------
// - 상태 진입 시 호출
// - 게이지 처음부터 다시 시작
// ============================================================
void LCD_ResetFFTProgress(void)
{
    prev_gauge = -1;
}

// ============================================================
// LOAD 화면 상태 표시
// ------------------------------------------------------------
// - 선택한 SLOT 번호
// - 해당 데이터의 Peak 주파수
// ============================================================
void LCD_UpdateLoadStatus(uint8_t slot, float peak_freq)
{
    char buf[40];

    ILI9341_FillRect(0, 0, 220, 20, BLACK);
    sprintf(buf, "SLOT%d  MAX:%dHz", slot + 1, (int)peak_freq);
    LCD_DrawString(10, 10, buf, Font_7x10, WHITE);
}

// ============================================================
// 결과 상태 표시 (하단)
// ------------------------------------------------------------
// STATUS_GOOD     → NORMAL
// STATUS_CAUTION  → WARNING
// STATUS_ERROR    → FAIL
//
// - 선택된 상태만 컬러 표시
// - 나머지는 회색 처리
// ============================================================
void LCD_DrawResult(int status)
{
    // 하단 영역 클리어
    ILI9341_FillRect(0, 220, 320, 20, BLACK);

    uint16_t normal_color  = (status == STATUS_GOOD)  ? GREEN  : DARKGREY;
    uint16_t warning_color = (status == STATUS_CAUTION) ? YELLOW : DARKGREY;
    uint16_t fail_color    = (status == STATUS_ERROR)    ? RED    : DARKGREY;

    // 좌측부터 순서대로 배치
    LCD_DrawString(5,   228, "GOOD",  Font_7x10, normal_color);
    LCD_DrawString(110, 228, "CAUTION", Font_7x10, warning_color);
    LCD_DrawString(230, 228, "ERROR",    Font_7x10, fail_color);
}

void LCD_ClearScreen(void)
{
    ILI9341_FillScreen(BLACK);
}

