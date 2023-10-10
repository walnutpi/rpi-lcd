#include <stdio.h>
#include <stdlib.h>
#include "hw_spi.h"
#include "common.h"
#define LCD_W 320
#define LCD_H 480

int lcd_width;
int lcd_height;

void LCD_direction(uint16_t direction)
{
    printf("rotations = %d\r\n",direction);
    switch (direction)
    {
    case 0:
        lcd_width = LCD_W;
        lcd_height = LCD_H;
        SPI_WriteReg(0x36, (1 << 3) | (1 << 6));

        break;
    case 90:
        lcd_width = LCD_H;
        lcd_height = LCD_W;
        SPI_WriteReg(0x36, (1 << 3) | (1 << 5));
        break;
    case 180:
        lcd_width = LCD_W;
        lcd_height = LCD_H;

        SPI_WriteReg(0x36, (1 << 3) | (1 << 7));
        break;
    case 270:
        lcd_width = LCD_H;
        lcd_height = LCD_W;
        SPI_WriteReg(0x36, (1 << 3) | (1 << 7) | (1 << 6) | (1 << 5));
        break;
    default:
        break;
    }
}

void LCD_Init(void)
{
    spi_init();

    // LCD_Reset(); //初始化之前复位

    //************* 7796初始化**********//
    SPI_WR_REG(0XF7);
    SPI_WR_DATA(0xA9);
    SPI_WR_DATA(0x51);
    SPI_WR_DATA(0x2C);
    SPI_WR_DATA(0x82);
    SPI_WR_REG(0xC0);
    SPI_WR_DATA(0x11);
    SPI_WR_DATA(0x09);
    SPI_WR_REG(0xC1);
    SPI_WR_DATA(0x41);
    SPI_WR_REG(0XC5);
    SPI_WR_DATA(0x00);
    SPI_WR_DATA(0x0A);
    SPI_WR_DATA(0x80);
    SPI_WR_REG(0xB1);
    SPI_WR_DATA(0xB0);
    SPI_WR_DATA(0x11);
    SPI_WR_REG(0xB4);
    SPI_WR_DATA(0x02);
    SPI_WR_REG(0xB6);
    SPI_WR_DATA(0x02);
    SPI_WR_DATA(0x42);
    SPI_WR_REG(0xB7);
    SPI_WR_DATA(0xc6);
    SPI_WR_REG(0xBE);
    SPI_WR_DATA(0x00);
    SPI_WR_DATA(0x04);
    SPI_WR_REG(0xE9);
    SPI_WR_DATA(0x00);

    SPI_WR_REG(0x3A);
    SPI_WR_DATA(0x55);
    // SPI_WR_DATA(0x66);
    SPI_WR_REG(0xE0);
    SPI_WR_DATA(0x00);
    SPI_WR_DATA(0x07);
    SPI_WR_DATA(0x10);
    SPI_WR_DATA(0x09);
    SPI_WR_DATA(0x17);
    SPI_WR_DATA(0x0B);
    SPI_WR_DATA(0x41);
    SPI_WR_DATA(0x89);
    SPI_WR_DATA(0x4B);
    SPI_WR_DATA(0x0A);
    SPI_WR_DATA(0x0C);
    SPI_WR_DATA(0x0E);
    SPI_WR_DATA(0x18);
    SPI_WR_DATA(0x1B);
    SPI_WR_DATA(0x0F);
    SPI_WR_REG(0XE1);
    SPI_WR_DATA(0x00);
    SPI_WR_DATA(0x17);
    SPI_WR_DATA(0x1A);
    SPI_WR_DATA(0x04);
    SPI_WR_DATA(0x0E);
    SPI_WR_DATA(0x06);
    SPI_WR_DATA(0x2F);
    SPI_WR_DATA(0x45);
    SPI_WR_DATA(0x43);
    SPI_WR_DATA(0x02);
    SPI_WR_DATA(0x0A);
    SPI_WR_DATA(0x09);
    SPI_WR_DATA(0x32);
    SPI_WR_DATA(0x36);
    SPI_WR_DATA(0x0F);
    SPI_WR_REG(0x11);

    SPI_WR_REG(0x29);

    // 设置LCD属性参数
    LCD_direction(LCD_ROTATIONS); // 设置LCD显示方向
}

void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd)
{
    SPI_WR_REG(0x2A);
    SPI_WR_DATA(xStar >> 8);
    SPI_WR_DATA(0x00FF & xStar);
    SPI_WR_DATA(xEnd >> 8);
    SPI_WR_DATA(0x00FF & xEnd);

    SPI_WR_REG(0x2B);
    SPI_WR_DATA(yStar >> 8);
    SPI_WR_DATA(0x00FF & yStar);
    SPI_WR_DATA(yEnd >> 8);
    SPI_WR_DATA(0x00FF & yEnd);

    SPI_WR_REG(0x2C);
}
void _LCD_SEND_DATA(uint16_t Color, int num)
{
    if (num > (LCD_H * LCD_W))
        return;

    uint16_t buf[LCD_W * LCD_H];
    for (int i = 0; i < num; i++)
    {
        buf[i] = Color;
    }

    // 颠倒字节序
    uint16_t buf2[LCD_W * LCD_H];
    // uint8_t *b1, b2;
    uint8_t *s;
    uint8_t *d;
    for (int i = 0; i < num; i++)
    {
        s = (uint8_t *)&buf[i];
        d = (uint8_t *)&buf2[i];
        *d = *(s + 1);
        *(d + 1) = *s;
    }

    // 发送
    uint8_t *p;
    p = (uint8_t *)buf2;
    int blok_size = 0;
    int p_size = num * 2;
    for (int i = 0; i < p_size; i += blok_size)
    {
        blok_size = p_size - i;

        if (blok_size > 4000)
            blok_size = 4000;

        SPI_WRITE_BUF_DATA(p + i, blok_size);
    }
}
void LCD_FULL(uint16_t Color)
{
    LCD_SetWindows(0, 0, lcd_width - 1, lcd_height - 1);
    _LCD_SEND_DATA(Color, LCD_W * LCD_H);
}

void _num_scope(int *num, int small, int big)
{
    if (*num < small)
    {
        *num = small;
    }
    if (*num > big)
    {
        *num = big;
    }
}

void LCD_area(uint16_t Color, int startx, int starty, int endx, int endy)
{
    _num_scope(&startx, 0, lcd_width);
    _num_scope(&starty, 0, lcd_height);
    _num_scope(&endx, 0, lcd_width);
    _num_scope(&endy, 0, lcd_height);
    LCD_SetWindows(startx, starty, endx, endy);
    _LCD_SEND_DATA(Color, (endx - startx + 1) * (endy - starty));
}
void LCD_draw_buff(uint8_t *buff, int size)
{
    if( size > LCD_W * LCD_H *2 )
    {
        printf("buff size error");
        exit(-1);
    }
    LCD_SetWindows(0, 0, lcd_width - 1, lcd_height - 1);
    SPI_WRITE_BUF_DATA(buff, size);
}