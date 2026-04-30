#ifndef __ILI9341_H
#define __ILI9341_H

#include "stm32f4xx_hal.h"

/* LCD resolution */
#define ILI9341_WIDTH   320
#define ILI9341_HEIGHT  240


/* Colors (RGB565) */
#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define YELLOW  0xFFE0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define GRAY    0x8410
#define DARKGREY 0x4208
#define DARKBLUE 0x0011

/* FSMC address */
#define LCD_BASE  ((uint32_t)0x60000000)

#define LCD_REG   (*((volatile uint16_t *)(LCD_BASE))) //command 전송 주소
#define LCD_RAM   (*((volatile uint16_t *)(LCD_BASE | (1 << 19))))
/* FSMC_A18 -> RS, Data전송 주소 RS = 0 : Command RS = 1 : Data */
/* 두 주소에 값을 넣는 동작 자체가 이미 Command냐 Data냐를 결정하고 동시에 A18의 값이 바뀌어 FSMC버스를 통해 LCD에 전달됨 */

/* ---------------- Initialization ---------------- */

void ILI9341_Init(void);


/* ---------------- Low Level IO ---------------- */

void ILI9341_WriteColor(uint16_t color, uint32_t size);


/* ---------------- Address Control ---------------- */

void ILI9341_SetAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void ILI9341_SetCursor(uint16_t x,uint16_t y);


/* ---------------- Basic Drawing ---------------- */

void ILI9341_DrawPixel(uint16_t x,uint16_t y,uint16_t color);
void ILI9341_FillScreen(uint16_t color);


/* ---------------- Fast Primitives ---------------- */

void ILI9341_DrawHLine(uint16_t x,uint16_t y,uint16_t w,uint16_t color);
void ILI9341_DrawVLine(uint16_t x,uint16_t y,uint16_t h,uint16_t color);
void ILI9341_FillRect(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t color);
void ILI9341_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

#endif
