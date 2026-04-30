#include "ili9341.h"
#include "fonts.h"

void LCD_DrawChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color)
{ //(x, y) 위치에 font 크기로 ch 문자를 color 색으로 그리는 함수
    if(ch < 32 || ch > 126) return; //ASCII기준 32~126 >> ' '(공백) ~~  '~' 까지 문자 => print 가능문자 범위밖이면 return

    uint32_t i, b, j;
    uint32_t index = (ch - 32) * font.height; //fonts.c에서 해당하는 문자의 시작 위치를 폰트 배열에서 찾아 지정함

    for(i = 0; i < font.height; i++)
    {
        b = font.data[index + i]; //현재 가로 한 줄(i)에 해당하는 픽셀 데이터(16bit)를 가져온다.

        for(j = 0; j < font.width; j++)//가로 한 줄 안에서 픽셀을 하나씩 검사
        {
            if((b << j) & 0x8000) //상위 j비트만 탐지할 것이며 한 비트씩 비트를 15번 비트로 끌어와 1인지 검사하기 위한 동작
            {
                ILI9341_DrawPixel(x + j, y + i, color);
            }
        }
    }
}

#include "ili9341.h"
#include "fonts.h"

void LCD_DrawString(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t color)
{
    uint16_t start_x = x;

    while(*str)
    {
        // 줄바꿈 처리
        if(*str == '\n')
        {
            x = start_x;
            y += font.height;
            str++;
            continue;
        }

        // 화면 오른쪽 넘어가면 자동 줄바꿈
        if(x + font.width > ILI9341_WIDTH)
        {
            x = start_x;
            y += font.height;
        }

        // 화면 아래 넘어가면 출력 중단
        if(y + font.height > ILI9341_HEIGHT)
        {
            break;
        }

        LCD_DrawChar(x, y, *str, font, color);

        x += font.width;
        str++;
    }
}
