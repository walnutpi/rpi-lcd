#ifndef _ST7796_H
#define _ST7796_H

#include <stdint.h>



void LCD_Init(void);
void LCD_direction(uint16_t direction);

void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd);
void LCD_FULL(uint16_t Color);
void LCD_area(uint16_t Color, int startx, int starty, int endx, int endy);
void LCD_draw_buff(uint8_t *buff, int size);

#endif

