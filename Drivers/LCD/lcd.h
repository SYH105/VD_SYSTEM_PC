#ifndef LCD_LCD_H_
#define LCD_LCD_H_

#include "ili9341.h"

// =========================
// GRAPH 영역
// =========================
#define GRAPH_X   10
#define GRAPH_Y   40
#define GRAPH_W   300
#define GRAPH_H   160

// =========================
// Dual Graph (MEASURE)
// =========================
#define AREA_GRAPH_H   70
#define GRAPH_TOP_Y    40
#define GRAPH_BOTTOM_Y 130

typedef enum {
    STATUS_GOOD = 0,
    STATUS_CAUTION,
    STATUS_ERROR
} ResultStatus;

void LCD_DrawFFT(float *data, int size);
void LCD_DrawFFT_Area(float *data, int size, int y_offset, uint16_t color);

void LCD_DrawUIFrame(void);
void LCD_UpdateStatus(int16_t x, int16_t y, int16_t z, float peak_freq);
void LCD_UpdateCount(int count);
void LCD_ClearTopBar(void);
void LCD_DrawFFTProgress(int count);
void LCD_ResetFFTProgress(void);
void LCD_UpdateLoadStatus(uint8_t slot, float peak_freq);
void LCD_DrawResult(int status);
void LCD_ClearScreen(void);

#endif
