#include "ili9341.h"

/* ILI9341 commands */
#define ILI9341_RESET        0x01
#define ILI9341_SLEEP_OUT    0x11
#define ILI9341_DISPLAY_ON   0x29 //화면 표시 시작
#define ILI9341_COLUMN_ADDR  0x2A //x좌표 범위 설정 319 = 0x013F
/*해당 명령 이후 4개의 데이터가 따라와야 한다.
X start high → 시작 좌표 상위 8bit
X start low  → 시작 좌표 하위 8bit
X end high   → 끝 좌표 상위 8bit
X end low    → 끝 좌표 하위 8bit*/
#define ILI9341_PAGE_ADDR    0x2B //y좌표 범위 설정 239 = 0x00EF
#define ILI9341_GRAM         0x2C //LCD의 GRAM(Graphics RAM)에 픽셀 데이터를 쓰기 시작하라는 명령
#define ILI9341_PIXEL_FORMAT 0x3A //LCD가 어떤 색상 데이터 포맷으로 픽셀을 받을지 설정하는 명령, 0x55 → RGB565 (16bit)
#define ILI9341_MAC          0x36 //픽셀 메모리를 어떤 방향으로 읽고 쓸 것인지 결정

/* ---------------- Low Level IO ---------------- */

static inline void ILI9341_WriteCommand(uint16_t cmd)
{
    LCD_REG = cmd; /*LCD_REG = 0x29;>> uint16_t이기때문에 자동으로 형변환되어 *((uint16_t*)0x60000000) = 0x0029;가 된다*/
    //LCD 컨트롤러(ILI9341)는 하위 8bit만 인식하도록 설계됨
}

static inline void ILI9341_WriteData(uint16_t data)
{
    LCD_RAM = data;
}

/* Write multiple pixels (fast) */
void ILI9341_WriteColor(uint16_t color, uint32_t size) //특정 색으로 SetAddress로 정해진 영역을 덮음
{
    while(size--)
    {
        LCD_RAM = color;
    }
}

/* ---------------- Address Control ---------------- */

void ILI9341_SetAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
    ILI9341_WriteCommand(ILI9341_COLUMN_ADDR);

    ILI9341_WriteData(x1 >> 8);
    ILI9341_WriteData(x1 & 0xFF);
    ILI9341_WriteData(x2 >> 8);
    ILI9341_WriteData(x2 & 0xFF);

    ILI9341_WriteCommand(ILI9341_PAGE_ADDR);

    ILI9341_WriteData(y1 >> 8);
    ILI9341_WriteData(y1 & 0xFF);
    ILI9341_WriteData(y2 >> 8);
    ILI9341_WriteData(y2 & 0xFF);

    ILI9341_WriteCommand(ILI9341_GRAM);
}

void ILI9341_SetCursor(uint16_t x,uint16_t y)
{
    ILI9341_SetAddress(x,y,x,y);
}

/* ---------------- Basic Drawing ---------------- */

void ILI9341_DrawPixel(uint16_t x,uint16_t y,uint16_t color) //단일 픽셀을 그리는 기본함수
{
    ILI9341_SetCursor(x,y);
    ILI9341_WriteData(color);
}

void ILI9341_FillScreen(uint16_t color)
{
    ILI9341_SetAddress(0,0,ILI9341_WIDTH-1,ILI9341_HEIGHT-1);
    ILI9341_WriteColor(color, ILI9341_WIDTH * ILI9341_HEIGHT);
}

/* ---------------- Fast Primitives ---------------- */

void ILI9341_DrawHLine(uint16_t x,uint16_t y,uint16_t w,uint16_t color)
{
    ILI9341_SetAddress(x, y, x + w - 1, y);
    ILI9341_WriteColor(color, w);
}

void ILI9341_DrawVLine(uint16_t x,uint16_t y,uint16_t h,uint16_t color)
{
    ILI9341_SetAddress(x, y, x, y + h - 1);
    ILI9341_WriteColor(color, h);
}

void ILI9341_FillRect(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t color)
{
    ILI9341_SetAddress(x, y, x + w - 1, y + h - 1);
    ILI9341_WriteColor(color, w * h);
}

void ILI9341_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    ILI9341_DrawHLine(x, y, w, color);
    ILI9341_DrawHLine(x, y + h, w, color);
    ILI9341_DrawVLine(x, y, h, color);
    ILI9341_DrawVLine(x + w, y, h, color);
}

/* ---------------- Initialization ---------------- */

void ILI9341_Init(void)
{
    HAL_Delay(20);

    ILI9341_WriteCommand(ILI9341_RESET);
    HAL_Delay(10);

    /* Pixel format = RGB565 */
    ILI9341_WriteCommand(ILI9341_PIXEL_FORMAT);
    ILI9341_WriteData(0x55);

    /* Memory Access Control */
    ILI9341_WriteCommand(ILI9341_MAC);
    ILI9341_WriteData(0x28);   // rotation 0x48 0x28,0x88,0xE8,0x08

    ILI9341_WriteCommand(ILI9341_SLEEP_OUT);
    HAL_Delay(120);

    ILI9341_WriteCommand(ILI9341_DISPLAY_ON);

    ILI9341_FillScreen(BLACK);
}
