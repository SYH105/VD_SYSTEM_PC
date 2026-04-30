#ifndef __LCD_TEXT_H
#define __LCD_TEXT_H

#include "fonts.h"

void LCD_DrawChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color);
void LCD_DrawString(uint16_t x, uint16_t y, char *str, FontDef font, uint16_t color);

#endif
